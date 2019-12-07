from image import Image, ImageValidator
from handlers import MessageHandler
from constants import UPLOAD_IMAGE, FIND_THUMBS, DOWNLOAD_IMAGE


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
        received_header, received_payload = self.message_handler.send_message(UPLOAD_IMAGE, self.current_image)
        return received_payload

    def find_thumbs(self, category):
        received_header, received_payload = self.message_handler.send_message(FIND_THUMBS, category)
        thumbs_dict = received_payload
        self.current_thumbs = thumbs_dict

    def show_image(self, thumb_file):
        thumb_file_path = self.current_thumbs[thumb_file]
        received_header, received_payload = self.message_handler.send_message(DOWNLOAD_IMAGE, thumb_file_path)
        image_file = received_payload
        return image_file

    def download_image(self):
        pass  # save file to disk
