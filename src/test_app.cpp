#include <QApplication>
#include <QPushButton>
#include <QDebug>
#include <QtWidgets/QLabel>
#include <iostream>

#include "simple_poisson_solver/poisson.h"

using namespace std;

int main(int argc, char** argv) {
	QApplication app(argc, argv);

	int imgId = 1;

	QImage source;
	source.load("../src/imgs/source" + QString::number(imgId) + ".png");
	source = source.scaled(800, 600);

	QImage target;
	target.load("../src/imgs/target" + QString::number(imgId) + ".jpg");
	target = target.scaled(800, 600);

	QImage mask;
	mask.load("../src/imgs/mask" + QString::number(imgId) + ".png");
	mask = mask.scaled(800, 600);

	qDebug() << "opened";

	QImage result = simple_solver::poisson(target, source, mask);

	qDebug() << "poissoned";

	QLabel myLabel;
	myLabel.setPixmap(QPixmap::fromImage(result));
	myLabel.setFixedHeight(600);
	myLabel.setFixedWidth(800);

	myLabel.show();

	result.save("../src/imgs/result" + QString::number(imgId) + ".png");

	qDebug() << "showed";

	return app.exec();
}
