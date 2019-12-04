from pathlib import Path


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
