from message import Message
import socket
import struct


class LoadBalancerConnector:
    def __init__(self):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.message_length = 1024
        print("Socket connection opened!")

    def send(self, message):
        self.sock.connect(("127.0.0.1", 5000))
        sent = self.sock.send(message.header)
        if sent == 0:
            raise RuntimeError("Socket connection closed!")

        sent = self.sock.send(message.payload)
        if sent == 0:
            raise RuntimeError("Socket connection closed!")

    def receive(self):
        message = b''
        while len(message) < self.message_length:
            chunk = self.sock.recv(self.message_length - len(message))
            if chunk == b'':
                raise RuntimeError("Socket connection closed!")
            message = message + chunk
        return message


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
        self.current_message.payload = payload
        self.load_balancer_connector.send(self.current_message)
        return self.load_balancer_connector.receive()
