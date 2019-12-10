from message import Message
import socket
import struct
from constants import *


class LoadBalancerConnector:
    def __init__(self):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def send(self, message):
        self.sock.connect((HOST_ADDRESS, PORT))
        self.sock.send(message.header)
        if isinstance(message.payload, str):
            message.payload = message.payload.encode("raw_unicode_escape")
        bytes_to_send = len(message.payload)
        times_to_send = bytes_to_send / PACKET_LENGTH
        while times_to_send >= 0:
            self.sock.send(message.payload)
            times_to_send -= 1

        received_header = self.sock.recv(struct.calcsize('!BII'))
        payload_length = struct.unpack('!BII', received_header)[2]
        received_payload = self.sock.recv(payload_length)
        self.sock.close()
        return received_header, received_payload


class MessageHandler:
    def __init__(self):
        self.current_message = Message()
        self.load_balancer_connector = LoadBalancerConnector()

    def send_message(self, message_type, payload):
        self.current_message.header = struct.pack(
            '!BII',
            message_type,
            0,
            len(payload)
        )
        if message_type == UPLOAD_IMAGE:
            image_file_length = len(payload.image_file)
            category_length = len(payload.category)
            payload = struct.pack(
                '!I{}sI{}s'.format(image_file_length, category_length),
                image_file_length,
                payload.image_file,
                category_length,
                payload.category.encode("raw_unicode_escape")
            )
        else:  # FIND_THUMBS or DOWNLOAD_IMAGE
            payload = payload.encode("raw_unicode_escape")

        self.current_message.payload = payload

        self.current_message.header, self.current_message.payload = \
            self.load_balancer_connector.send(self.current_message)

        message_type = struct.unpack('!BII', self.current_message.header)[0]

        if message_type == FIND_THUMBS:
            thumbs_dict = dict()
            offset = 0
            while True:
                next_length = struct.unpack_from('!I', self.current_message.payload, offset)[0]
                if next_length == 0:
                    break
                offset += struct.calcsize('!I')
                thumb_file = struct.unpack_from('!{}s'.format(next_length), self.current_message.payload, offset)[0]
                offset += struct.calcsize('!{}s'.format(next_length))
                next_length = struct.unpack_from('!I', self.current_message.payload, offset)[0]
                offset += struct.calcsize('!I')
                thumb_path = struct.unpack_from('!{}s'.format(next_length), self.current_message.payload, offset)[0]
                offset += struct.calcsize('!{}s'.format(next_length))
                thumbs_dict[thumb_file] = thumb_path.decode("raw_unicode_escape")
            return self.current_message.header, thumbs_dict
        return self.current_message.header, self.current_message.payload
