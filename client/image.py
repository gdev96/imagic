from pathlib import Path
from constants import IMAGE_EXTENSIONS


class Image:
    def __init__(self, image_file):
        self.image_file = image_file
        self.category = None
        self.thumb_file = None

    def __len__(self):
        image_length = 0
        for key in self.__dict__.keys():
            attr = self.__getattribute__(key)
            if attr is not None:
                image_length += len(attr)
        return image_length


class ImageValidator:
    def __init__(self):
        self.image_extensions = IMAGE_EXTENSIONS

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
