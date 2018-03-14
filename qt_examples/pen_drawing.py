import sys

import numpy
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

        self.lines = []

    def mousePressEvent(self, e: QMouseEvent):
        if e.button() == Qt.LeftButton:
            self.lines.append([e.pos()])
            self.update()
        elif e.button() == Qt.RightButton:
            if self.lines:
                self.lines.pop()
                self.update()

    def mouseReleaseEvent(self, e: QMouseEvent):
        # self.current_line = []
        self.update()

    def mouseMoveEvent(self, e: QMouseEvent):
        if self.lines and self.lines[-1][-1] != e.pos():
            self.lines[-1].append(e.pos())
        self.update()

    def paintEvent(self, e: QPaintEvent):
        super().paintEvent(e)
        painter = QPainter(self)

        def target_width(sin, cos):
            return 1.5 + abs(cos + sin) * 5

        def calc_width(current, target):
            bounds = 0.07
            return numpy.clip(target - current, -bounds, bounds)

        for line in self.lines:
            w = 4

            for a, b in zip(line, line[1:]):
                delta = sqrt((a.x() - b.x()) ** 2 + (a.y() - b.y()) ** 2)
                cos = (a.x() - b.x()) / delta
                sin = (a.y() - b.y()) / delta
                clr = QColor()
                clr.setBlueF(0.5)
                clr.setRedF(cos / 2 + 0.5)
                clr.setGreenF(sin / 2 + 0.5)

                w += calc_width(w, target_width(sin, cos)) * delta
                pen = QPen(clr)

                pen.setWidthF(w)
                painter.setPen(pen)
                painter.drawLine(a, b)


if __name__ == "__main__":
    app = QApplication(sys.argv)
    mainscreen = MyMainScreen()
    mainscreen.show()
    app.exec_()