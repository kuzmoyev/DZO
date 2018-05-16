#include <QApplication>
#include <QPushButton>
#include <QDebug>
#include <QtWidgets/QLabel>
#include <iostream>

#include "include/poisson.h"

using namespace std;

int main(int argc, char** argv) {
	QApplication app(argc, argv);

	int imgId = 1;

	QImage source;
	source.load("../resources/imgs/source" + QString::number(imgId) + ".png");
	source = source.scaled(800, 600);

	QImage target;
	target.load("../resources/imgs/target" + QString::number(imgId) + ".jpg");
	target = target.scaled(800, 600);

	QImage mask;
	mask.load("../resources/imgs/mask" + QString::number(imgId) + ".png");
	mask = mask.scaled(800, 600);

	qDebug() << "opened";

	QImage result = simple_solver::poisson(target, source, mask);

	qDebug() << "poissoned";

	QLabel myLabel;
	myLabel.setPixmap(QPixmap::fromImage(result));
	myLabel.setFixedHeight(600);
	myLabel.setFixedWidth(800);

	myLabel.show();

	result.save("../resources/imgs/result" + QString::number(imgId) + ".png");

	qDebug() << "showed";

	return app.exec();
}
