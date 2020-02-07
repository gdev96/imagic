from PyQt5 import QtCore, QtGui, QtWidgets
from imagic import Imagic


class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(1121, 600)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.stackedWidget = QtWidgets.QStackedWidget(self.centralwidget)
        self.stackedWidget.setGeometry(QtCore.QRect(9, 9, 1101, 551))
        self.stackedWidget.setObjectName("stackedWidget")
        self.mainpage = QtWidgets.QWidget()
        self.mainpage.setObjectName("mainpage")
        self.downloadbutton = QtWidgets.QPushButton(self.mainpage)
        self.downloadbutton.setGeometry(QtCore.QRect(420, 270, 251, 51))
        font = QtGui.QFont()
        font.setPointSize(20)
        self.downloadbutton.setFont(font)
        self.downloadbutton.setObjectName("downloadbutton")
        self.uploadbutton = QtWidgets.QPushButton(self.mainpage)
        self.uploadbutton.setGeometry(QtCore.QRect(420, 340, 251, 51))
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
        self.uploadsearchtext = QtWidgets.QLineEdit(self.uploadpage)
        self.uploadsearchtext.setGeometry(QtCore.QRect(260, 500, 211, 31))
        self.uploadsearchtext.setObjectName("uploadsearchtext")
        self.imagelabel = QtWidgets.QLabel(self.uploadpage)
        self.imagelabel.setGeometry(QtCore.QRect(20, 20, 1051, 451))
        self.imagelabel.setText("")
        self.imagelabel.setObjectName("imagelabel")
        self.uploadlabel = QtWidgets.QLabel(self.uploadpage)
        self.uploadlabel.setGeometry(QtCore.QRect(80, 500, 171, 28))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.uploadlabel.setFont(font)
        self.uploadlabel.setObjectName("uploadlabel")
        self.uploadimage = QtWidgets.QPushButton(self.uploadpage)
        self.uploadimage.setGeometry(QtCore.QRect(910, 490, 101, 41))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.uploadimage.setFont(font)
        self.uploadimage.setObjectName("uploadimage")
        self.uploadcancelbutton = QtWidgets.QPushButton(self.uploadpage)
        self.uploadcancelbutton.setGeometry(QtCore.QRect(810, 490, 91, 41))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.uploadcancelbutton.setFont(font)
        self.uploadcancelbutton.setObjectName("uploadcancelbutton")
        self.stackedWidget.addWidget(self.uploadpage)
        self.downloadpage = QtWidgets.QWidget()
        self.downloadpage.setObjectName("downloadpage")
        self.searchlabel = QtWidgets.QLabel(self.downloadpage)
        self.searchlabel.setGeometry(QtCore.QRect(150, 240, 171, 28))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.searchlabel.setFont(font)
        self.searchlabel.setObjectName("searchlabel")
        self.downloadcancelbutton = QtWidgets.QPushButton(self.downloadpage)
        self.downloadcancelbutton.setGeometry(QtCore.QRect(810, 490, 91, 41))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.downloadcancelbutton.setFont(font)
        self.downloadcancelbutton.setObjectName("downloadcancelbutton")
        self.downloadlabel = QtWidgets.QLabel(self.downloadpage)
        self.downloadlabel.setGeometry(QtCore.QRect(150, 70, 323, 46))
        font = QtGui.QFont()
        font.setPointSize(30)
        self.downloadlabel.setFont(font)
        self.downloadlabel.setObjectName("downloadlabel")
        self.searchtext = QtWidgets.QLineEdit(self.downloadpage)
        self.searchtext.setGeometry(QtCore.QRect(150, 290, 211, 31))
        self.searchtext.setObjectName("searchtext")
        self.searchbutton = QtWidgets.QPushButton(self.downloadpage)
        self.searchbutton.setGeometry(QtCore.QRect(910, 490, 91, 41))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.searchbutton.setFont(font)
        self.searchbutton.setObjectName("searchbutton")
        self.stackedWidget.addWidget(self.downloadpage)
        self.thumbspage = QtWidgets.QWidget()
        self.thumbspage.setObjectName("thumbspage")
        self.scrollArea = QtWidgets.QScrollArea(self.thumbspage)
        self.scrollArea.setGeometry(QtCore.QRect(0, 0, 1101, 481))
        self.scrollArea.setWidgetResizable(True)
        self.scrollArea.setObjectName("scrollArea")
        self.scrollAreaWidgetContents = QtWidgets.QWidget()
        self.scrollAreaWidgetContents.setGeometry(QtCore.QRect(0, 0, 1099, 479))
        self.scrollAreaWidgetContents.setObjectName("scrollAreaWidgetContents")
        self.gridLayout = QtWidgets.QGridLayout(self.scrollAreaWidgetContents)
        self.gridLayout.setContentsMargins(0, 0, 0, 0)
        self.gridLayout.setObjectName("gridLayout")
        self.scrollArea.setWidget(self.scrollAreaWidgetContents)
        self.thumbscancelbutton = QtWidgets.QPushButton(self.thumbspage)
        self.thumbscancelbutton.setGeometry(QtCore.QRect(910, 490, 91, 41))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.thumbscancelbutton.setFont(font)
        self.thumbscancelbutton.setObjectName("thumbscancelbutton")
        self.stackedWidget.addWidget(self.thumbspage)
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
        self.downloadcancelbutton.clicked.connect(self.downloadcancelbutton_onclick)
        self.searchbutton.clicked.connect(self.searchbutton_onclick)
        self.thumbscancelbutton.clicked.connect(self.thumbscancelbutton_onclick)
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
        self.downloadcancelbutton.setText(_translate("MainWindow", "Cancel"))
        self.downloadlabel.setText(_translate("MainWindow", "Download images"))
        self.searchbutton.setText(_translate("MainWindow", "Search"))
        self.thumbscancelbutton.setText(_translate("MainWindow", "Cancel"))

    def uploadbutton_onclick(self):
        file_dialog = QtWidgets.QFileDialog(self.mainpage)
        file_dialog.setFileMode(QtWidgets.QFileDialog.ExistingFile)
        file_dialog.setNameFilter("Images (*.jpg *.jpeg)")
        if file_dialog.exec_():
            file_name = file_dialog.selectedFiles()[0]
            imagic.validate_image(file_name)
            image_pixmap = QtGui.QPixmap()
            image_pixmap.loadFromData(QtCore.QByteArray(imagic.current_image.image_file))
            width = self.imagelabel.width()
            height = self.imagelabel.height()
            self.imagelabel.setPixmap(image_pixmap.scaled(width, height, QtCore.Qt.KeepAspectRatio))
            self.imagelabel.setAlignment(QtCore.Qt.AlignCenter)
            self.stackedWidget.setCurrentIndex(1)

    def uploadcancelbutton_onclick(self):
        self.stackedWidget.setCurrentIndex(0)

    def uploadimage_onclick(self):
        imagic.select_category(self.uploadsearchtext.text())
        result = imagic.upload_image()
        if result is True:
            response = "Image successfully uploaded"
            icon = QtWidgets.QMessageBox.Information
        else:
            response = "Error while uploading image"
            icon = QtWidgets.QMessageBox.Warning
        dialog = QtWidgets.QMessageBox(self.mainpage)
        dialog.setIcon(icon)
        dialog.setWindowTitle("Upload result")
        dialog.setText(response)
        dialog.exec_()

    def downloadbutton_onclick(self):
        self.stackedWidget.setCurrentIndex(2)

    def downloadcancelbutton_onclick(self):
        self.stackedWidget.setCurrentIndex(0)

    def searchbutton_onclick(self):
        imagic.find_thumbs(self.searchtext.text())
        row = 0
        column = 0
        if not imagic.current_thumbs:
            thumb_label = QtWidgets.QLabel()
            thumb_label.setText("No image found")
            self.gridLayout.addWidget(thumb_label, row, column)
        else:
            for thumb_file, thumb_path in imagic.current_thumbs.items():
                image_pixmap = QtGui.QPixmap()
                image_pixmap.loadFromData(QtCore.QByteArray(thumb_file))
                thumb_label = QtWidgets.QLabel()
                thumb_label.setObjectName(thumb_path)
                thumb_label.setPixmap(image_pixmap)
                thumb_label.setAlignment(QtCore.Qt.AlignCenter)
                # thumb_label.clicked.connect(self.thumblabel_onclick)
                # thumb_label.hovered.connect(self.thumblabel_onhover)
                self.gridLayout.addWidget(thumb_label, row, column)
                column = (column + 1) % 3
                if column == 0:
                    row += 1
        self.stackedWidget.setCurrentIndex(3)

    def thumblabel_onclick(self):
        sending_thumb = self.thumbspage.sender()
        imagic.show_image(sending_thumb.objectName())

    def thumblabel_onhover(self):
        sending_thumb = self.thumbspage.sender()
        sending_thumb.setCursor(QtGui.QCursor(QtCore.Qt.PointingHandCursor))

    def thumbscancelbutton_onclick(self):
        for i in reversed(range(self.gridLayout.count())):
            self.gridLayout.itemAt(i).widget().deleteLater()
        self.stackedWidget.setCurrentIndex(2)


if __name__ == "__main__":
    import sys
    imagic = Imagic()
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())
