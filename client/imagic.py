import socket


class Imagic:
    def __init__(self, current_image, current_thumbs, image_validator, message_handler):
        self.current_image = current_image
        self.current_thumbs = current_thumbs
        self.image_validator = image_validator
        self.message_handler = message_handler

    def validate_image(self, image_file):
        valid = self.image_validator.validate(image_file)
        if valid:
            self.current_image = Image(image_file)
        return valid

    def select_category(self, category):
        self.current_image.category = category

    def upload_image(self):
        received_message = self.message_handler.send_message(b'0', self.current_image)
        return received_message.payload

    def find_thumbs(self, category):
        received_message = self.message_handler.send_message(b'1', category)
        thumbs_dict = received_message.payload
        self.current_thumbs = thumbs_dict
        return self.current_thumbs

    def show_image(self, thumb_file):
        thumb_path = self.current_thumbs[thumb_file]
        received_message = self.message_handler.send_message(b'2', thumb_path)
        image = received_message.payload
        self.current_image = image
        return self.current_image

    def download_image(self):
        return self.current_image.file


class Image:
    def __init__(self, image_file, category=None, thumb_file=None):
        self.image_file = image_file
        self.category = category
        self.thumb_file = thumb_file


class ImageValidator:
    def validate(self, image):
        pass


class MessageHandler:
    def __init__(self, message, load_balancer_connector):
        self.message = message
        self.load_balancer_connector = load_balancer_connector

    def send_message(self, message_type, payload):
        header = Header(message_type)
        message = Message(header, payload)
        self.load_balancer_connector.send(message)


class Message:
    def __init__(self, header, payload):
        self.header = header
        self.payload = payload


class Header:
    def __init__(self, message_type, source=None, destination=None):
        self.message_type = message_type
        self.message_type = source
        self.message_type = destination


class Host:
    def __init__(self, address, port):
        self.address = address
        self.port = port


class LoadBalancerConnector:
    def __init__(self, sock, source, destination):
        self.sock = sock
        self.source = source
        self.destination = destination

    def send(self, message):
        message.header.source = self.source
        message.header.destination = self.destination
        self.sock.connect(self.destination)
        self.sock.send(message)
        return self.sock.receive()


class Socket:
    def __init__(self, sock=None, message_length=1024):
        if sock is None:
            self.sock = socket.socket(
                socket.AF_INET, socket.SOCK_STREAM
            )
        else:
            self.sock = sock

        self.message_length = message_length

    def connect(self, destination):
        self.sock.connect(
            (destination.address, destination.port)
        )
        print("Socket connection opened!")

    def send(self, message):
        total_sent = 0
        while total_sent < self.message_length:
            sent = self.sock.send(message[total_sent:])
            if sent == 0:
                raise RuntimeError("Socket connection closed!")
            total_sent = total_sent + sent

    def receive(self):
        message = ''
        while len(message) < self.message_length:
            chunk = self.sock.recv(self.message_length - len(message))
            if chunk == '':
                raise RuntimeError("Socket connection closed!")
            message = message + chunk
        return message
