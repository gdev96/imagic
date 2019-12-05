from message import Message
from image import Image
import socket
import struct


class LoadBalancerConnector:
    def __init__(self):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        print("Socket connection opened!")

    def send(self, message):
        self.sock.connect(("127.0.0.1", 5000))
        self.sock.send(message.header)
        bytes_to_send = len(message.payload)
        times_to_send = bytes_to_send / 4096
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
        if type(payload) is Image:
            image_file_length = len(payload.image_file)
            category_length = len(payload.category)
            payload = struct.pack(
                '!I{}sI{}s'.format(image_file_length, category_length),
                image_file_length,
                payload.image_file,
                category_length,
                payload.category.encode("raw_unicode_escape")
            )
        self.current_message.payload = payload

        self.current_message.header, self.current_message.payload = \
            self.load_balancer_connector.send(self.current_message)
