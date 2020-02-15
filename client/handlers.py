import os
import socket
import struct
from constants import *
from message import Message, MessageType


class LoadBalancerConnector:
    def __init__(self):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        host_address = os.environ["LOAD_BALANCER_ADDRESS"]
        port = int(os.environ["LOAD_BALANCER_PORT"])
        self.sock.connect((host_address, port))

    def send_bytes(self, message, message_length):
        offset = 0
        while offset < message_length:
            bytes_sent = self.sock.send(message[offset:])
            if bytes_sent == 0:
                self.sock.close()
                raise RuntimeError("Socket connection broken")
            offset += bytes_sent

    def receive_bytes(self, message_length):
        chunks = list()
        bytes_received = 0
        while bytes_received < message_length:
            chunk = self.sock.recv(min(message_length - bytes_received, CHUNK_SIZE))
            if chunk == b'':
                self.sock.close()
                raise RuntimeError("Socket connection closed")
            chunks.append(chunk)
            bytes_received += len(chunk)
        return b''.join(chunks)

    def send(self, message):
        self.send_bytes(message.header, HEADER_LENGTH)
        if isinstance(message.payload, str):
            message.payload = message.payload.encode("raw_unicode_escape")
        self.send_bytes(message.payload, len(message.payload))
        received_header = self.receive_bytes(struct.calcsize('!BII'))
        payload_length = struct.unpack('!BII', received_header)[2]
        received_payload = self.receive_bytes(payload_length)
        return received_header, received_payload


class MessageHandler:
    def __init__(self):
        self.current_message = Message()
        self.load_balancer_connector = LoadBalancerConnector()

    def send_message(self, message_type, payload):
        if message_type == MessageType.UPLOAD_IMAGE:
            payload.category = payload.category.encode("raw_unicode_escape")
            image_file_length = len(payload.image_file)
            category_length = len(payload.category)
            payload = struct.pack(
                '!I{}sI{}s'.format(image_file_length, category_length),
                image_file_length,
                payload.image_file,
                category_length,
                payload.category
            )
        else:  # FIND_THUMBS or DOWNLOAD_IMAGE
            payload = payload.encode("raw_unicode_escape")

        self.current_message.header = struct.pack(
            '!BII',
            message_type.value,
            0,
            len(payload)
        )

        self.current_message.payload = payload

        self.current_message.header, self.current_message.payload = self.load_balancer_connector.send(self.current_message)

        message_type = struct.unpack('!BII', self.current_message.header)[0]
        received_message_type = MessageType(message_type)

        if received_message_type == MessageType.UPLOAD_IMAGE:
            return self.current_message.header, self.current_message.payload.decode("raw_unicode_escape")
        if received_message_type == MessageType.FIND_THUMBS:
            thumbs_dict = dict()
            offset = 0
            while True:
                next_length = struct.unpack_from('!I', self.current_message.payload, offset)[0]
                if next_length == 0:
                    break
                offset += struct.calcsize('!I')
                thumb_file = struct.unpack_from('!{}s'.format(next_length), self.current_message.payload, offset)[0]
                offset += next_length
                next_length = struct.unpack_from('!I', self.current_message.payload, offset)[0]
                offset += struct.calcsize('!I')
                thumb_file_name = struct.unpack_from('!{}s'.format(next_length), self.current_message.payload, offset)[0]
                offset += next_length
                thumbs_dict[thumb_file] = thumb_file_name.decode("raw_unicode_escape")
            return self.current_message.header, thumbs_dict
        # DOWNLOAD_IMAGE
        return self.current_message.header, self.current_message.payload
