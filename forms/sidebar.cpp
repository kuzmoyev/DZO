#include <QtWidgets/QMessageBox>
#include "sidebar.h"
#include "ui_sidebar.h"

Sidebar::Sidebar(QSize canvas_size, QWidget* parent) :
		QWidget(parent),
		ui(new Ui::Sidebar) {
	ui->setupUi(this);

	ui->canvasXSizeLe->setEnabled(true);
	ui->canvasYSizeLe->setEnabled(true);
	setCanvasSize(canvas_size);
	ui->canvasXSizeLe->setValidator(new QIntValidator(20, 1024 * 8, this));
	ui->canvasYSizeLe->setValidator(new QIntValidator(20, 1024 * 8, this));
	connect(ui->canvasXSizeLe, &QLineEdit::returnPressed, this, &Sidebar::sizeLeChanged);
	connect(ui->canvasYSizeLe, &QLineEdit::returnPressed, this, &Sidebar::sizeLeChanged);
}

Sidebar::~Sidebar() {
	delete ui;
}

void Sidebar::setCanvasSize(QSize size) {
	ui->canvasXSizeLe->setText(QString::number(size.width()));
	ui->canvasYSizeLe->setText(QString::number(size.height()));
}

void Sidebar::setUndoState(bool on) {
	ui->undoBtn->setEnabled(on);
}

void Sidebar::setRedoState(bool on) {
	ui->redoBtn->setEnabled(on);
}

void Sidebar::setRunState(bool on) {
	ui->runBtn->setEnabled(on);
}

void Sidebar::setMainColor(QColor color) {
	ui->mainColorLabel->setStyleSheet("QLabel { background-color : " + color.name());
}

void Sidebar::setAltColor(QColor color) {
	ui->altColorLabel->setStyleSheet("QLabel { background-color : " + color.name());
}


void Sidebar::sizeLeChanged() {
	bool ok;
	auto width = ui->canvasXSizeLe->text().toInt(&ok);
	if (!ok) {
		QMessageBox msgBox;
		msgBox.setText("Invalid canvas size value: " + ui->canvasXSizeLe->text());
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.exec();
		return;
	}
	auto height = ui->canvasYSizeLe->text().toInt(&ok);
	if (!ok) {
		QMessageBox msgBox;
		msgBox.setText("Invalid canvas size value: " + ui->canvasYSizeLe->text());
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.exec();
		return;
	}

	QSize size(width, height);
	ui->canvasXSizeLe->setText(QString::number(width));
	ui->canvasYSizeLe->setText(QString::number(height));
	emit updatedCanvasSize(size);
}

