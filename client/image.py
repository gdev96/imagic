from pathlib import Path
from constants import IMAGE_EXTENSIONS


class Image:
    def __init__(self, image_file):
        self.image_file = image_file
        self.category = None

    def __len__(self):
        image_length = 0
        for key in self.__dict__.keys():
            attribute = self.__getattribute__(key)
            if attribute is not None:
                image_length += len(attribute)
        return image_length


class ImageValidator:
    def validate(self, image_file_path):
        file_extension = Path(image_file_path).suffix
        if file_extension in IMAGE_EXTENSIONS:
            return self.get_image(image_file_path)
        return None

    @staticmethod
    def get_image(image_file_path):
        with open(image_file_path, "rb") as f:
            image_file = f.read()
        return image_file
