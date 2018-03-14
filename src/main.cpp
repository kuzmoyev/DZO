#include <QApplication>
#include <QPushButton>
#include <QDebug>


int main(int argc, char** argv) {
	QApplication app(argc, argv);

	qDebug() << QT_VERSION_STR;

	QPushButton button;
	button.setText(QT_VERSION_STR);
	button.setToolTip("A tooltip");
	button.move(500, 400);
	button.show();

	QObject::connect(&button, &QPushButton::clicked, &app, QApplication::quit);


	return app.exec();
}
