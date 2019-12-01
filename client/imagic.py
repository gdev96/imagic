from pathlib import Path
import socket


class Image:
    def __init__(self, image_file):
        self.image_file = image_file
        self.category = None
        self.thumb_file = None


class ImageValidator:
    def __init__(self):
        self.image_extensions = self.load_image_extensions()

    @staticmethod
    def load_image_extensions():
        return '.jpg', '.png', '.gif', '.tif'

    def validate(self, image_file_path):
        file_extension = Path(image_file_path).suffix
        if file_extension in self.image_extensions:
            return self.get_image(image_file_path)
        return None

    @staticmethod
    def get_image(image_file_path):
        with open(image_file_path, "rb") as f:
            image_file = f.read()
            f.close()
            return image_file


class Host:
    def __init__(self, address, port):
        self.address = address
        self.port = port


class Header:
    def __init__(self, message_type=None):
        self.message_type = message_type
        self.source = None
        self.destination = None
        self.payload_length = None


class Message:
    def __init__(self):
        self.header = Header()
        self.payload = None


class LoadBalancerConnector:
    def __init__(self):
        self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.destination = Host("localhost", 5000)
        self.client_socket.connect((self.destination.address, self.destination.port))
        self.source = self.client_socket.getsockname()
        self.message_length = 1024
        print("Socket connection opened!")
        print("Source address:", self.source)
        print("Destination address:", self.destination)

    def send(self, message):
        message.header.source = self.source
        message.header.destination = self.destination

        total_sent = 0
        while total_sent < self.message_length:
            sent = self.client_socket.send(message[total_sent:])
            if sent == 0:
                raise RuntimeError("Socket connection closed!")
            total_sent = total_sent + sent

    def receive(self):
        message = b''
        while len(message) < self.message_length:
            chunk = self.client_socket.recv(self.message_length - len(message))
            if chunk == b'':
                raise RuntimeError("Socket connection closed!")
            message = message + chunk
        return message


class MessageHandler:
    def __init__(self):
        self.current_message = Message()
        self.load_balancer_connector = LoadBalancerConnector()

    def send_message(self, message_type, payload):
        self.current_message.header.message_type = message_type
        self.current_message.payload = payload
        self.load_balancer_connector.send(self.current_message)

    def receive_message(self):
        return self.load_balancer_connector.receive()


class Imagic:
    def __init__(self):
        self.current_image = None
        self.current_thumbs = None
        self.image_validator = ImageValidator()
        self.message_handler = MessageHandler()

    def validate_image(self, image_file_path):
        image_file = self.image_validator.validate(image_file_path)
        if image_file:
            self.current_image = Image(image_file)
        return image_file

    def select_category(self, category):
        self.current_image.category = category

    def upload_image(self):
        self.message_handler.send_message(0, self.current_image)
        received_message = self.message_handler.receive_message()
        return received_message.payload

    def find_thumbs(self, category):
        self.message_handler.send_message(1, category)
        received_message = self.message_handler.receive_message()
        thumbs_dict = received_message.payload
        self.current_thumbs = thumbs_dict
        return self.current_thumbs

    def show_image(self, thumb_file):
        thumb_file_path = self.current_thumbs[thumb_file]
        self.message_handler.send_message(2, thumb_file_path)
        received_message = self.message_handler.receive_message()
        image = received_message.payload
        self.current_image = image
        return self.current_image

    def download_image(self):
        return self.current_image.file
