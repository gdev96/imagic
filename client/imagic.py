from image import Image, ImageValidator
from handlers import MessageHandler
from message import MessageType


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

    def select_category(self, category):
        self.current_image.category = category

    def upload_image(self):
        received_payload = self.message_handler.send_message(MessageType.UPLOAD_IMAGE, self.current_image)[1]
        return received_payload

    def find_thumbs(self, category):
        received_payload = self.message_handler.send_message(MessageType.FIND_THUMBS, category)[1]
        self.current_thumbs = received_payload

    def show_image(self, thumb_file_name):
        received_payload = self.message_handler.send_message(MessageType.DOWNLOAD_IMAGE, thumb_file_name)[1]
        self.current_image = Image(received_payload)

    def download_image(self, image_path):
        with open(image_path, "wb") as f:
            f.write(self.current_image.image_file)
            f.close()
