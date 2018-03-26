#include <QApplication>
#include <QPushButton>
#include <QDebug>
#include <include/simple_poisson_solver/poisson.h>
#include <QtWidgets/QLabel>
#include <iostream>

#include "forms/mainwindow.h"

using namespace std;

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    QImage source;
    source.load("/home/kuzmovych/ClionProjects/DZO/src/source.png");
    source = source.scaled(800, 600);

    QImage target;
    target.load("/home/kuzmovych/ClionProjects/DZO/src/target.jpg");
    target = target.scaled(800, 600);

    QImage mask;
    mask.load("/home/kuzmovych/ClionProjects/DZO/src/mask.png");
    mask = mask.scaled(800, 600);

    cout << "opened" << endl;

    QImage result = simple_solver::poisson(target, source, mask);

    cout << "poissoned" << endl;

    QLabel myLabel;
    myLabel.setPixmap(QPixmap::fromImage(result));
    myLabel.setFixedHeight(600);
    myLabel.setFixedWidth(800);

    myLabel.show();

    cout << "Showed" << endl;

    return app.exec();
}
