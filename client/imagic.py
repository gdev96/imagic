from image import Image, ImageValidator
from handlers import MessageHandler


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
        self.message_handler.send_message(b'0', self.current_image)
        received_message = self.message_handler.receive_message()
        return received_message.payload

    def find_thumbs(self, category):
        self.message_handler.send_message(b'1', category)
        received_message = self.message_handler.receive_message()
        thumbs_dict = received_message.payload
        self.current_thumbs = thumbs_dict
        return self.current_thumbs

    def show_image(self, thumb_file):
        thumb_file_path = self.current_thumbs[thumb_file]
        self.message_handler.send_message(b'2', thumb_file_path)
        received_message = self.message_handler.receive_message()
        image = received_message.payload
        self.current_image = image
        return self.current_image

    def download_image(self):
        return self.current_image.file
