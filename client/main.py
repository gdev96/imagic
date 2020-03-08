from pathlib import Path
from PyQt5 import QtCore, QtGui, QtWidgets
from constants import CATEGORIES
from imagic import Imagic
from message import UploadStatus


class Worker(QtCore.QObject):
    finished = QtCore.pyqtSignal()

    def setArgument(self, argument):
        self.argument = argument

    @QtCore.pyqtSlot()
    def uploadImage(self):
        self.result = UploadStatus(imagic.upload_image())
        self.finished.emit()

    @QtCore.pyqtSlot()
    def findThumbs(self):
        imagic.find_thumbs(self.argument)
        self.finished.emit()

    @QtCore.pyqtSlot()
    def downloadImage(self):
        imagic.download_image(self.argument)
        self.finished.emit()


class ClickableQLabel(QtWidgets.QLabel):
    clicked = QtCore.pyqtSignal()

    def __init__(self, parent=None):
        super().__init__(parent)

    def mousePressEvent(self, ev):
        self.clicked.emit()

    def enterEvent(self, ev):
        QtWidgets.QApplication.setOverrideCursor(QtCore.Qt.PointingHandCursor)

    def leaveEvent(self, ev):
        QtWidgets.QApplication.restoreOverrideCursor()


class Ui_MainWindow:
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.setFixedSize(1121, 600)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap("resources/imagic_icon.svg"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        MainWindow.setWindowIcon(icon)
        self.centralWidget = QtWidgets.QWidget(MainWindow)
        self.centralWidget.setObjectName("centralWidget")
        self.stackedWidget = QtWidgets.QStackedWidget(self.centralWidget)
        self.stackedWidget.setGeometry(QtCore.QRect(9, 9, 1101, 551))
        self.stackedWidget.setObjectName("stackedWidget")
        self.mainPage = QtWidgets.QWidget()
        self.mainPage.setObjectName("mainPage")
        self.logo = QtWidgets.QLabel(self.mainPage)
        self.logo.setGeometry(QtCore.QRect(460, 150, 157, 61))
        font = QtGui.QFont()
        font.setPointSize(40)
        self.logo.setFont(font)
        self.logo.setObjectName("logo")
        self.downloadButton = QtWidgets.QPushButton(self.mainPage)
        self.downloadButton.setGeometry(QtCore.QRect(420, 270, 251, 41))
        font = QtGui.QFont()
        font.setPointSize(20)
        self.downloadButton.setFont(font)
        self.downloadButton.setObjectName("downloadButton")
        self.uploadButton = QtWidgets.QPushButton(self.mainPage)
        self.uploadButton.setGeometry(QtCore.QRect(420, 330, 251, 41))
        font = QtGui.QFont()
        font.setPointSize(20)
        self.uploadButton.setFont(font)
        self.uploadButton.setObjectName("uploadButton")
        self.stackedWidget.addWidget(self.mainPage)
        self.uploadPage = QtWidgets.QWidget()
        self.uploadPage.setObjectName("uploadPage")
        self.uploadImageLabel = QtWidgets.QLabel(self.uploadPage)
        self.uploadImageLabel.setGeometry(QtCore.QRect(0, 0, 1101, 501))
        self.uploadImageLabel.setObjectName("uploadImageLabel")
        self.uploadImageLabel.setStyleSheet("background-color: black;")
        self.uploadCategoryLabel = QtWidgets.QLabel(self.uploadPage)
        self.uploadCategoryLabel.setGeometry(QtCore.QRect(80, 510, 171, 28))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.uploadCategoryLabel.setFont(font)
        self.uploadCategoryLabel.setObjectName("uploadCategoryLabel")
        self.uploadComboBox = QtWidgets.QComboBox(self.uploadPage)
        self.uploadComboBox.setGeometry(QtCore.QRect(260, 510, 211, 31))
        self.uploadComboBox.setObjectName("uploadComboBox")
        self.uploadComboBox.addItems(CATEGORIES)
        self.uploadCancelButton = QtWidgets.QPushButton(self.uploadPage)
        self.uploadCancelButton.setGeometry(QtCore.QRect(850, 510, 91, 31))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.uploadCancelButton.setFont(font)
        self.uploadCancelButton.setObjectName("uploadCancelButton")
        self.uploadImage = QtWidgets.QPushButton(self.uploadPage)
        self.uploadImage.setGeometry(QtCore.QRect(950, 510, 101, 31))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.uploadImage.setFont(font)
        self.uploadImage.setObjectName("uploadImage")
        self.stackedWidget.addWidget(self.uploadPage)
        self.searchPage = QtWidgets.QWidget()
        self.searchPage.setObjectName("searchPage")
        self.downloadLabel = QtWidgets.QLabel(self.searchPage)
        self.downloadLabel.setGeometry(QtCore.QRect(120, 70, 323, 46))
        font = QtGui.QFont()
        font.setPointSize(30)
        self.downloadLabel.setFont(font)
        self.downloadLabel.setObjectName("downloadLabel")
        self.searchCategoryLabel = QtWidgets.QLabel(self.searchPage)
        self.searchCategoryLabel.setGeometry(QtCore.QRect(120, 240, 171, 28))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.searchCategoryLabel.setFont(font)
        self.searchCategoryLabel.setObjectName("searchCategoryLabel")
        self.searchComboBox = QtWidgets.QComboBox(self.searchPage)
        self.searchComboBox.setGeometry(QtCore.QRect(300, 240, 211, 31))
        self.searchComboBox.setObjectName("searchComboBox")
        self.searchComboBox.addItems(CATEGORIES)
        self.searchCancelButton = QtWidgets.QPushButton(self.searchPage)
        self.searchCancelButton.setGeometry(QtCore.QRect(850, 510, 91, 31))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.searchCancelButton.setFont(font)
        self.searchCancelButton.setObjectName("searchCancelButton")
        self.searchButton = QtWidgets.QPushButton(self.searchPage)
        self.searchButton.setGeometry(QtCore.QRect(950, 510, 91, 31))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.searchButton.setFont(font)
        self.searchButton.setObjectName("searchButton")
        self.stackedWidget.addWidget(self.searchPage)
        self.thumbsPage = QtWidgets.QWidget()
        self.thumbsPage.setObjectName("thumbsPage")
        self.scrollArea = QtWidgets.QScrollArea(self.thumbsPage)
        self.scrollArea.setGeometry(QtCore.QRect(0, 0, 1101, 501))
        self.scrollArea.setWidgetResizable(True)
        self.scrollArea.setObjectName("scrollArea")
        self.scrollAreaWidgetContents = QtWidgets.QWidget()
        self.scrollAreaWidgetContents.setGeometry(QtCore.QRect(0, 0, 1099, 499))
        self.scrollAreaWidgetContents.setObjectName("scrollAreaWidgetContents")
        self.gridLayout = QtWidgets.QGridLayout(self.scrollAreaWidgetContents)
        self.gridLayout.setContentsMargins(0, 0, 0, 0)
        self.gridLayout.setObjectName("gridLayout")
        self.scrollArea.setWidget(self.scrollAreaWidgetContents)
        self.downloadCancelButton = QtWidgets.QPushButton(self.thumbsPage)
        self.downloadCancelButton.setGeometry(QtCore.QRect(950, 510, 91, 31))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.downloadCancelButton.setFont(font)
        self.downloadCancelButton.setObjectName("downloadCancelButton")
        self.stackedWidget.addWidget(self.thumbsPage)
        self.downloadPage = QtWidgets.QWidget()
        self.downloadPage.setObjectName("downloadPage")
        self.downloadImageLabel = QtWidgets.QLabel(self.downloadPage)
        self.downloadImageLabel.setGeometry(QtCore.QRect(0, 0, 1101, 501))
        self.downloadImageLabel.setObjectName("downloadImageLabel")
        self.downloadImageLabel.setStyleSheet("background-color: black;")
        self.saveCancelButton = QtWidgets.QPushButton(self.downloadPage)
        self.saveCancelButton.setGeometry(QtCore.QRect(850, 510, 91, 31))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.saveCancelButton.setFont(font)
        self.saveCancelButton.setObjectName("saveCancelButton")
        self.saveButton = QtWidgets.QPushButton(self.downloadPage)
        self.saveButton.setGeometry(QtCore.QRect(950, 510, 91, 31))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.saveButton.setFont(font)
        self.saveButton.setObjectName("saveButton")
        self.stackedWidget.addWidget(self.downloadPage)
        MainWindow.setCentralWidget(self.centralWidget)
        self.menuBar = QtWidgets.QMenuBar(MainWindow)
        self.menuBar.setGeometry(QtCore.QRect(0, 0, 1121, 22))
        self.menuBar.setObjectName("menuBar")
        MainWindow.setMenuBar(self.menuBar)
        self.statusBar = QtWidgets.QStatusBar(MainWindow)
        self.statusBar.setSizeGripEnabled(False)
        self.statusBar.setObjectName("statusBar")
        MainWindow.setStatusBar(self.statusBar)

        self.retranslateUi(MainWindow)
        self.stackedWidget.setCurrentIndex(0)

        worker.finished.connect(thread.quit)
        worker.finished.connect(loop.quit)

        self.uploadButton.clicked.connect(self.uploadButton_onclick)
        self.uploadCancelButton.clicked.connect(self.uploadCancelButton_onclick)
        self.uploadImage.clicked.connect(self.uploadImage_onclick)
        self.downloadButton.clicked.connect(self.downloadButton_onclick)
        self.searchCancelButton.clicked.connect(self.searchCancelButton_onclick)
        self.searchButton.clicked.connect(self.searchButton_onclick)
        self.downloadCancelButton.clicked.connect(self.downloadCancelButton_onclick)
        self.saveCancelButton.clicked.connect(self.saveCancelButton_onclick)
        self.saveButton.clicked.connect(self.saveButton_onclick)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "Imagic - Upload and Download Images"))
        self.logo.setText(_translate("MainWindow", "Imagic"))
        self.downloadButton.setText(_translate("MainWindow", "Download image"))
        self.uploadButton.setText(_translate("MainWindow", "Upload Image"))
        self.uploadCategoryLabel.setText(_translate("MainWindow", "Insert category:"))
        self.uploadCancelButton.setText(_translate("MainWindow", "Cancel"))
        self.uploadImage.setText(_translate("MainWindow", "Upload"))
        self.downloadLabel.setText(_translate("MainWindow", "Download images"))
        self.searchCategoryLabel.setText(_translate("MainWindow", "Insert category:"))
        self.searchCancelButton.setText(_translate("MainWindow", "Cancel"))
        self.searchButton.setText(_translate("MainWindow", "Search"))
        self.downloadCancelButton.setText(_translate("MainWindow", "Back"))
        self.saveCancelButton.setText(_translate("MainWindow", "Cancel"))
        self.saveButton.setText(_translate("MainWindow", "Save"))

    def uploadButton_onclick(self):
        image_file_name = QtWidgets.QFileDialog.getOpenFileName(self.mainPage, "Select image to upload", "", "Images (*.jpg *.jpeg *.png *.tif *.tiff *.gif)")[0]
        if image_file_name:
            if imagic.validate_image(image_file_name):
                width = self.uploadImageLabel.width()
                height = self.uploadImageLabel.height()
                if Path(image_file_name).suffix != ".gif":
                    image_pixmap = QtGui.QPixmap()
                    image_pixmap.loadFromData(QtCore.QByteArray(imagic.current_image.image_file))
                    if not image_pixmap.isNull():
                        self.uploadImageLabel.setPixmap(image_pixmap.scaled(width, height, QtCore.Qt.KeepAspectRatio))
                        self.uploadImageLabel.setAlignment(QtCore.Qt.AlignCenter)
                        self.stackedWidget.setCurrentIndex(1)
                    else:
                        QtWidgets.QMessageBox.warning(self.uploadPage, "Error", "Unable to load image")
                else:
                    self.image_data = QtCore.QByteArray(imagic.current_image.image_file)
                    self.image_buffer = QtCore.QBuffer(self.image_data)
                    self.image_buffer.open(QtCore.QIODevice.ReadOnly)
                    movie = QtGui.QMovie(self.image_buffer, b'gif')
                    if movie.isValid():
                        movie.start()
                        frame_rect = movie.frameRect()
                        movie.setScaledSize(frame_rect.size().scaled(width, height, QtCore.Qt.KeepAspectRatio))
                        self.uploadImageLabel.setMovie(movie)
                        self.uploadImageLabel.setAlignment(QtCore.Qt.AlignCenter)
                        self.stackedWidget.setCurrentIndex(1)
                    else:
                        QtWidgets.QMessageBox.warning(self.uploadPage, "Error", "Unable to load GIF")
            else:
                QtWidgets.QMessageBox.warning(self.uploadPage, "Error", "Illegal image format")

    def uploadCancelButton_onclick(self):
        if hasattr(self, "image_buffer") and self.image_buffer.isOpen():
            self.image_buffer.close()
        self.stackedWidget.setCurrentIndex(0)

    def uploadImage_onclick(self):
        imagic.select_category(self.uploadComboBox.currentText())

        # Block signals during image upload
        self.uploadImage.blockSignals(True)
        self.uploadCancelButton.blockSignals(True)

        thread.started.connect(worker.uploadImage)
        thread.start()

        # Wait for thread to finish
        loop.exec()

        thread.started.disconnect(worker.uploadImage)

        # Restore signals
        self.uploadImage.blockSignals(False)
        self.uploadCancelButton.blockSignals(False)

        if worker.result == UploadStatus.UPLOADED:
            QtWidgets.QMessageBox.information(self.uploadPage, "Upload result", "Image successfully uploaded")
        else:
            if worker.result == UploadStatus.DUPLICATE:
                response = "Image already exists in this category"
            else:  # INVALID
                response = "Image cannot be decoded"
            QtWidgets.QMessageBox.warning(self.uploadPage, "Upload result", response)

    def downloadButton_onclick(self):
        self.stackedWidget.setCurrentIndex(2)

    def searchCancelButton_onclick(self):
        self.stackedWidget.setCurrentIndex(0)

    def searchButton_onclick(self):
        worker.setArgument(self.searchComboBox.currentText())

        # Block signals during thumbs retrieval
        self.searchButton.blockSignals(True)
        self.searchCancelButton.blockSignals(True)

        thread.started.connect(worker.findThumbs)
        thread.start()

        # Wait for thread to finish
        loop.exec()

        thread.started.disconnect(worker.findThumbs)

        # Restore signals
        self.searchButton.blockSignals(False)
        self.searchCancelButton.blockSignals(False)

        row = 0
        column = 0
        if not imagic.current_thumbs:
            thumb_label = QtWidgets.QLabel()
            font = QtGui.QFont()
            font.setPointSize(20)
            thumb_label.setFont(font)
            thumb_label.setText("No image found")
            self.gridLayout.addWidget(thumb_label, row, column, QtCore.Qt.AlignCenter)
        else:
            for thumb_file_name, thumb_file in imagic.current_thumbs.items():
                image_pixmap = QtGui.QPixmap()
                image_pixmap.loadFromData(QtCore.QByteArray(thumb_file))
                thumb_label = ClickableQLabel()
                thumb_label.setPixmap(image_pixmap)
                thumb_label.setObjectName(thumb_file_name)
                thumb_label.clicked.connect(self.thumbLabel_onclick)
                self.gridLayout.addWidget(thumb_label, row, column, QtCore.Qt.AlignCenter)
                column = (column + 1) % 3
                if column == 0:
                    row += 1
        self.stackedWidget.setCurrentIndex(3)

    def downloadCancelButton_onclick(self):
        for i in reversed(range(self.gridLayout.count())):
            self.gridLayout.itemAt(i).widget().deleteLater()
        self.stackedWidget.setCurrentIndex(2)

    def thumbLabel_onclick(self):
        sending_thumb = self.thumbsPage.sender()
        thumb_file_name = sending_thumb.objectName()
        worker.setArgument(thumb_file_name)

        # Block signals during image download
        for i in range(self.gridLayout.count()):
            self.gridLayout.itemAt(i).widget().blockSignals(True)
        self.downloadCancelButton.blockSignals(True)

        thread.started.connect(worker.downloadImage)
        thread.start()

        # Wait for thread to finish
        loop.exec()

        thread.started.disconnect(worker.downloadImage)

        # Restore signals
        for i in range(self.gridLayout.count()):
            self.gridLayout.itemAt(i).widget().blockSignals(False)
        self.downloadCancelButton.blockSignals(False)

        width = self.downloadImageLabel.width()
        height = self.downloadImageLabel.height()
        if Path(thumb_file_name).suffix != ".gif":
            image_pixmap = QtGui.QPixmap()
            image_pixmap.loadFromData(QtCore.QByteArray(imagic.current_image.image_file))
            if not image_pixmap.isNull():
                self.downloadImageLabel.setPixmap(image_pixmap.scaled(width, height, QtCore.Qt.KeepAspectRatio))
                self.downloadImageLabel.setObjectName(thumb_file_name)
                self.downloadImageLabel.setAlignment(QtCore.Qt.AlignCenter)
                self.stackedWidget.setCurrentIndex(4)
            else:
                QtWidgets.QMessageBox.warning(self.uploadPage, "Error", "Unable to load image")
        else:
            self.image_data = QtCore.QByteArray(imagic.current_image.image_file)
            self.image_buffer = QtCore.QBuffer(self.image_data)
            self.image_buffer.open(QtCore.QIODevice.ReadOnly)
            movie = QtGui.QMovie(self.image_buffer, b'gif')
            if movie.isValid():
                movie.start()
                frame_rect = movie.frameRect()
                movie.setScaledSize(frame_rect.size().scaled(width, height, QtCore.Qt.KeepAspectRatio))
                self.downloadImageLabel.setMovie(movie)
                self.downloadImageLabel.setObjectName(thumb_file_name)
                self.downloadImageLabel.setAlignment(QtCore.Qt.AlignCenter)
                self.stackedWidget.setCurrentIndex(4)
            else:
                QtWidgets.QMessageBox.warning(self.uploadPage, "Error", "Unable to load GIF")

    def saveCancelButton_onclick(self):
        if hasattr(self, "image_buffer") and self.image_buffer.isOpen():
            self.image_buffer.close()
        self.stackedWidget.setCurrentIndex(3)

    def saveButton_onclick(self):
        thumb_file_name = self.downloadImageLabel.objectName()
        image_file_name = QtWidgets.QFileDialog.getSaveFileName(self.downloadPage, "Save image", thumb_file_name.replace("_thumb", ""), "Images (*.jpg *.jpeg *.png *.tif *.tiff *.gif)")[0]
        if image_file_name:
            file_extension = Path(thumb_file_name).suffix
            if Path(image_file_name).suffix != file_extension:
                image_file_name += file_extension
            imagic.save_image(image_file_name)
            QtWidgets.QMessageBox.information(self.downloadPage, "Download result", "Image successfully saved")


if __name__ == "__main__":
    import sys

    imagic = Imagic()

    thread = QtCore.QThread()
    worker = Worker()
    worker.moveToThread(thread)

    app = QtWidgets.QApplication(sys.argv)
    loop = QtCore.QEventLoop()
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())
