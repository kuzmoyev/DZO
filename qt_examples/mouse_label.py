import sys

from PyQt5.QtCore import QRect, QMetaObject, Qt, QRectF
from PyQt5.QtGui import QPainter, QPen, QMouseEvent, QPaintEvent, QBrush, QColor
from PyQt5.QtWidgets import QApplication, QHBoxLayout, QWidget, QMenuBar, QStatusBar, QMainWindow, QLabel
from math import sqrt


class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.resize(500, 500)
        self.centralwidget = QWidget(MainWindow)
        self.horizontalLayout = QHBoxLayout(self.centralwidget)
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QMenuBar(MainWindow)
        self.menubar.setGeometry(QRect(0, 0, 500, 22))
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QStatusBar(MainWindow)
        MainWindow.setStatusBar(self.statusbar)
        QMetaObject.connectSlotsByName(MainWindow)


class MyMainScreen(QMainWindow):
    def __init__(self, parent=None):
        QMainWindow.__init__(self, parent)
        self.ui = Ui_MainWindow()  # This is from a python export from QtDesigner
        self.ui.setupUi(self)
        self.setMouseTracking(True)

        self.last_label = None
        self.labels = []
        self.current_line = []

    def mousePressEvent(self, e: QMouseEvent):
        if e.button() == Qt.LeftButton:
            self.last_label = QLabel(self.ui.centralwidget)
            self.labels.append(self.last_label)
            self.last_label.show()
            self.current_line = [e.pos()]
            self.update()
        elif e.button() == Qt.RightButton:
            if self.current_line:
                self.current_line = []
                self.update()
            elif self.labels:
                self.labels.pop().deleteLater()

    def mouseReleaseEvent(self, e: QMouseEvent):
        self.last_label = None
        # self.current_line = []
        self.update()

    def mouseMoveEvent(self, e: QMouseEvent):
        if not self.last_label:
            return

        self.last_label.setText(f'({e.pos().x()}, {e.pos().y()})')
        self.last_label.move(e.pos())
        if not self.current_line or self.current_line[-1] != e.pos():
            self.current_line.append(e.pos())

    def paintEvent(self, e: QPaintEvent):
        super().paintEvent(e)
        painter = QPainter(self)
        for a, b in zip(self.current_line, self.current_line[1:]):
            delta = sqrt((a.x() - b.x())**2 + (a.y() - b.y())**2)
            cos = (a.x() - b.x()) / delta
            sin = (a.y() - b.y()) / delta
            clr = QColor()
            clr.setBlueF(0.5)
            clr.setRedF(cos / 2 + 0.5)
            clr.setGreenF(sin / 2 + 0.5)
            pen = QPen(clr)
            pen.setWidthF(2)
            painter.setPen(pen)
            painter.drawLine(a, b)


if __name__ == "__main__":
    app = QApplication(sys.argv)
    mainscreen = MyMainScreen()
    mainscreen.show()
    app.exec_()