from enum import Enum


class MessageType(Enum):
    UPLOAD_IMAGE = 0
    FIND_THUMBS = 1
    DOWNLOAD_IMAGE = 2


class UploadStatus(Enum):
    UPLOADED = 0
    DUPLICATE = 1
    INVALID = 2


class Message:
    def __init__(self):
        self.header = None
        self.payload = None
