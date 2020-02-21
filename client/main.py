from pathlib import Path
from PyQt5 import QtCore, QtGui, QtWidgets
from constants import CATEGORIES
from imagic import Imagic
from message import UploadStatus


class ClickableQLabel(QtWidgets.QLabel):
    clicked = QtCore.pyqtSignal()

    def __init__(self, parent=None):
        QtWidgets.QLabel.__init__(self, parent)

    def mousePressEvent(self, ev):
        self.clicked.emit()

    def enterEvent(self, ev):
        QtWidgets.QApplication.setOverrideCursor(QtCore.Qt.PointingHandCursor)

    def leaveEvent(self, ev):
        QtWidgets.QApplication.restoreOverrideCursor()


class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(1121, 600)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap("resources/imagic_icon.svg"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        MainWindow.setWindowIcon(icon)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.stackedWidget = QtWidgets.QStackedWidget(self.centralwidget)
        self.stackedWidget.setGeometry(QtCore.QRect(9, 9, 1101, 551))
        self.stackedWidget.setObjectName("stackedWidget")
        self.mainpage = QtWidgets.QWidget()
        self.mainpage.setObjectName("mainpage")
        self.downloadbutton = QtWidgets.QPushButton(self.mainpage)
        self.downloadbutton.setGeometry(QtCore.QRect(420, 270, 251, 41))
        font = QtGui.QFont()
        font.setPointSize(20)
        self.downloadbutton.setFont(font)
        self.downloadbutton.setObjectName("downloadbutton")
        self.uploadbutton = QtWidgets.QPushButton(self.mainpage)
        self.uploadbutton.setGeometry(QtCore.QRect(420, 330, 251, 41))
        font = QtGui.QFont()
        font.setPointSize(20)
        self.uploadbutton.setFont(font)
        self.uploadbutton.setObjectName("uploadbutton")
        self.logo = QtWidgets.QLabel(self.mainpage)
        self.logo.setGeometry(QtCore.QRect(460, 150, 157, 61))
        font = QtGui.QFont()
        font.setPointSize(40)
        self.logo.setFont(font)
        self.logo.setObjectName("logo")
        self.stackedWidget.addWidget(self.mainpage)
        self.uploadpage = QtWidgets.QWidget()
        self.uploadpage.setObjectName("uploadpage")
        self.uploadimagelabel = QtWidgets.QLabel(self.uploadpage)
        self.uploadimagelabel.setGeometry(QtCore.QRect(0, 0, 1101, 501))
        self.uploadimagelabel.setText("")
        self.uploadimagelabel.setObjectName("uploadimagelabel")
        self.uploadimagelabel.setStyleSheet("QLabel { background-color : black; }")
        self.uploadlabel = QtWidgets.QLabel(self.uploadpage)
        self.uploadlabel.setGeometry(QtCore.QRect(80, 510, 171, 28))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.uploadlabel.setFont(font)
        self.uploadlabel.setObjectName("uploadlabel")
        self.uploadimage = QtWidgets.QPushButton(self.uploadpage)
        self.uploadimage.setGeometry(QtCore.QRect(940, 510, 101, 31))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.uploadimage.setFont(font)
        self.uploadimage.setObjectName("uploadimage")
        self.uploadcancelbutton = QtWidgets.QPushButton(self.uploadpage)
        self.uploadcancelbutton.setGeometry(QtCore.QRect(840, 510, 91, 31))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.uploadcancelbutton.setFont(font)
        self.uploadcancelbutton.setObjectName("uploadcancelbutton")
        self.uploadcombobox = QtWidgets.QComboBox(self.uploadpage)
        self.uploadcombobox.setGeometry(QtCore.QRect(260, 510, 211, 31))
        self.uploadcombobox.setObjectName("uploadcombobox")
        self.uploadcombobox.addItems(CATEGORIES)
        self.stackedWidget.addWidget(self.uploadpage)
        self.findpage = QtWidgets.QWidget()
        self.findpage.setObjectName("findpage")
        self.searchlabel = QtWidgets.QLabel(self.findpage)
        self.searchlabel.setGeometry(QtCore.QRect(120, 240, 171, 28))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.searchlabel.setFont(font)
        self.searchlabel.setObjectName("searchlabel")
        self.findcancelbutton = QtWidgets.QPushButton(self.findpage)
        self.findcancelbutton.setGeometry(QtCore.QRect(850, 510, 91, 31))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.findcancelbutton.setFont(font)
        self.findcancelbutton.setObjectName("findcancelbutton")
        self.downloadlabel = QtWidgets.QLabel(self.findpage)
        self.downloadlabel.setGeometry(QtCore.QRect(120, 70, 323, 46))
        font = QtGui.QFont()
        font.setPointSize(30)
        self.downloadlabel.setFont(font)
        self.downloadlabel.setObjectName("downloadlabel")
        self.searchbutton = QtWidgets.QPushButton(self.findpage)
        self.searchbutton.setGeometry(QtCore.QRect(950, 510, 91, 31))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.searchbutton.setFont(font)
        self.searchbutton.setObjectName("searchbutton")
        self.findcombobox = QtWidgets.QComboBox(self.findpage)
        self.findcombobox.setGeometry(QtCore.QRect(300, 240, 211, 31))
        self.findcombobox.setObjectName("findcombobox")
        self.findcombobox.addItems(CATEGORIES)
        self.stackedWidget.addWidget(self.findpage)
        self.thumbspage = QtWidgets.QWidget()
        self.thumbspage.setObjectName("thumbspage")
        self.scrollArea = QtWidgets.QScrollArea(self.thumbspage)
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
        self.thumbscancelbutton = QtWidgets.QPushButton(self.thumbspage)
        self.thumbscancelbutton.setGeometry(QtCore.QRect(950, 510, 91, 31))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.thumbscancelbutton.setFont(font)
        self.thumbscancelbutton.setObjectName("thumbscancelbutton")
        self.stackedWidget.addWidget(self.thumbspage)
        self.downloadpage = QtWidgets.QWidget()
        self.downloadpage.setObjectName("downloadpage")
        self.downloadimagelabel = QtWidgets.QLabel(self.downloadpage)
        self.downloadimagelabel.setGeometry(QtCore.QRect(0, 0, 1101, 501))
        self.downloadimagelabel.setText("")
        self.downloadimagelabel.setObjectName("downloadimagelabel")
        self.downloadimagelabel.setStyleSheet("QLabel { background-color : black; }")
        self.downloadcancelbutton = QtWidgets.QPushButton(self.downloadpage)
        self.downloadcancelbutton.setGeometry(QtCore.QRect(810, 510, 91, 31))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.downloadcancelbutton.setFont(font)
        self.downloadcancelbutton.setObjectName("downloadcancelbutton")
        self.downloadimage = QtWidgets.QPushButton(self.downloadpage)
        self.downloadimage.setGeometry(QtCore.QRect(910, 510, 131, 31))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.downloadimage.setFont(font)
        self.downloadimage.setObjectName("downloadimage")
        self.stackedWidget.addWidget(self.downloadpage)
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 1121, 22))
        self.menubar.setObjectName("menubar")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)
        self.stackedWidget.setCurrentIndex(0)
        self.uploadbutton.clicked.connect(self.uploadbutton_onclick)
        self.uploadcancelbutton.clicked.connect(self.uploadcancelbutton_onclick)
        self.uploadimage.clicked.connect(self.uploadimage_onclick)
        self.downloadbutton.clicked.connect(self.downloadbutton_onclick)
        self.findcancelbutton.clicked.connect(self.findcancelbutton_onclick)
        self.searchbutton.clicked.connect(self.searchbutton_onclick)
        self.thumbscancelbutton.clicked.connect(self.thumbscancelbutton_onclick)
        self.downloadcancelbutton.clicked.connect(self.downloadcancelbutton_onclick)
        self.downloadimage.clicked.connect(self.downloadimage_onclick)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "Imagic - Upload and Download Images"))
        self.downloadbutton.setText(_translate("MainWindow", "Download images"))
        self.uploadbutton.setText(_translate("MainWindow", "Upload Images"))
        self.logo.setText(_translate("MainWindow", "Imagic"))
        self.uploadlabel.setText(_translate("MainWindow", "Insert category:"))
        self.uploadimage.setText(_translate("MainWindow", "Upload"))
        self.uploadcancelbutton.setText(_translate("MainWindow", "Cancel"))
        self.searchlabel.setText(_translate("MainWindow", "Insert category:"))
        self.findcancelbutton.setText(_translate("MainWindow", "Cancel"))
        self.downloadlabel.setText(_translate("MainWindow", "Download images"))
        self.searchbutton.setText(_translate("MainWindow", "Search"))
        self.thumbscancelbutton.setText(_translate("MainWindow", "Cancel"))
        self.downloadcancelbutton.setText(_translate("MainWindow", "Cancel"))
        self.downloadimage.setText(_translate("MainWindow", "Download"))

    def uploadbutton_onclick(self):
        file_dialog = QtWidgets.QFileDialog(self.mainpage)
        file_dialog.setFileMode(QtWidgets.QFileDialog.ExistingFile)
        file_dialog.setNameFilter("Images (*.jpg *.jpeg)")
        if file_dialog.exec_():
            file_name = file_dialog.selectedFiles()[0]
            imagic.validate_image(file_name)
            image_pixmap = QtGui.QPixmap()
            image_pixmap.loadFromData(QtCore.QByteArray(imagic.current_image.image_file))
            width = self.uploadimagelabel.width()
            height = self.uploadimagelabel.height()
            self.uploadimagelabel.setPixmap(image_pixmap.scaled(width, height, QtCore.Qt.KeepAspectRatio))
            self.uploadimagelabel.setAlignment(QtCore.Qt.AlignCenter)
            self.stackedWidget.setCurrentIndex(1)

    def uploadcancelbutton_onclick(self):
        self.stackedWidget.setCurrentIndex(0)

    def uploadimage_onclick(self):
        imagic.select_category(self.uploadcombobox.currentText())
        result = UploadStatus(imagic.upload_image())
        if result == UploadStatus.UPLOADED:
            icon = QtWidgets.QMessageBox.Information
            response = "Image successfully uploaded"
        else:
            icon = QtWidgets.QMessageBox.Warning
            if result == UploadStatus.DUPLICATE:
                response = "Image already exists in this category"
            else:  # INVALID
                response = "Image cannot be decoded"
        dialog = QtWidgets.QMessageBox(self.mainpage)
        dialog.setIcon(icon)
        dialog.setWindowTitle("Upload result")
        dialog.setText(response)
        dialog.exec_()

    def downloadbutton_onclick(self):
        self.stackedWidget.setCurrentIndex(2)

    def findcancelbutton_onclick(self):
        self.stackedWidget.setCurrentIndex(0)

    def searchbutton_onclick(self):
        imagic.find_thumbs(self.findcombobox.currentText())
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
            for thumb_file, thumb_file_name in imagic.current_thumbs.items():
                image_pixmap = QtGui.QPixmap()
                image_pixmap.loadFromData(QtCore.QByteArray(thumb_file))
                thumb_label = ClickableQLabel()
                thumb_label.setPixmap(image_pixmap)
                thumb_label.setObjectName(thumb_file_name)
                thumb_label.clicked.connect(self.thumblabel_onclick)
                self.gridLayout.addWidget(thumb_label, row, column, QtCore.Qt.AlignCenter)
                column = (column + 1) % 3
                if column == 0:
                    row += 1
        self.stackedWidget.setCurrentIndex(3)

    def thumblabel_onclick(self):
        sending_thumb = self.thumbspage.sender()
        thumb_file_name = sending_thumb.objectName()
        imagic.show_image(thumb_file_name)
        image_pixmap = QtGui.QPixmap()
        image_pixmap.loadFromData(QtCore.QByteArray(imagic.current_image.image_file))
        width = self.downloadimagelabel.width()
        height = self.downloadimagelabel.height()
        self.downloadimagelabel.setPixmap(image_pixmap.scaled(width, height, QtCore.Qt.KeepAspectRatio))
        self.downloadimagelabel.setObjectName(thumb_file_name)
        self.downloadimagelabel.setAlignment(QtCore.Qt.AlignCenter)
        self.stackedWidget.setCurrentIndex(4)

    def thumbscancelbutton_onclick(self):
        for i in reversed(range(self.gridLayout.count())):
            self.gridLayout.itemAt(i).widget().deleteLater()
        self.stackedWidget.setCurrentIndex(2)

    def downloadimage_onclick(self):
        thumb_file_name = self.downloadimagelabel.objectName()
        file_name = QtWidgets.QFileDialog.getSaveFileName(self.mainpage, "Save image", thumb_file_name.replace("_thumb", ""), "Images (*.jpg *.jpeg)")[0]
        if file_name:
            file_extension = Path(thumb_file_name).suffix
            if Path(file_name).suffix != file_extension:
                file_name += file_extension
            imagic.download_image(file_name)
            dialog = QtWidgets.QMessageBox(self.downloadpage)
            dialog.setIcon(QtWidgets.QMessageBox.Information)
            dialog.setWindowTitle("Download result")
            dialog.setText("Download successfully completed")
            dialog.exec_()

    def downloadcancelbutton_onclick(self):
        self.stackedWidget.setCurrentIndex(3)


if __name__ == "__main__":
    import sys
    imagic = Imagic()
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())
