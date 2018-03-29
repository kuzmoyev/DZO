#include <QtWidgets/QMessageBox>
#include <QtWidgets/QColorDialog>

#include "sidebar.h"
#include "ui_sidebar.h"

Sidebar::Sidebar(CanvasModel& model, QWidget* parent) :
		QWidget(parent),
		ui(new Ui::Sidebar),
		model_(model) {
	ui->setupUi(this);

	ui->canvasXSizeLe->setEnabled(true);
	ui->canvasYSizeLe->setEnabled(true);
	setCanvasSize(model.getCanvasSize());
	ui->canvasXSizeLe->setValidator(new QIntValidator(20, 1024 * 8, this));
	ui->canvasYSizeLe->setValidator(new QIntValidator(20, 1024 * 8, this));
	connect(ui->canvasXSizeLe, &QLineEdit::returnPressed, this, &Sidebar::sizeLeChanged);
	connect(ui->canvasYSizeLe, &QLineEdit::returnPressed, this, &Sidebar::sizeLeChanged);

	ui->mainColorBtn->setEnabled(true);
	//ui->altColorBtn->setEnabled(true);
	updateColors(model.getMainColor(), model.getAltColor());
	connect(ui->mainColorBtn, &QPushButton::clicked, this, &Sidebar::mainColorClicked);
	connect(ui->altColorBtn, &QPushButton::clicked, this, &Sidebar::altColorClicked);
	connect(&model_, &CanvasModel::colorsUpdated, this, &Sidebar::updateColors);
	connect(this, &Sidebar::mainColorChanged, &model_, &CanvasModel::setMainColor);
	connect(this, &Sidebar::altColorChanged, &model_, &CanvasModel::setAltColor);
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

void Sidebar::updateColors(QColor main, QColor alt) {
	ui->mainColorLabel->setStyleSheet("QLabel {background-color:" + main.name() + "}");
	ui->altColorLabel->setStyleSheet("QLabel {background-color:" + alt.name() + "}");
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

void Sidebar::mainColorClicked() {
	auto color = QColorDialog::getColor(model_.getMainColor(), this, "Select main color");
	if (!color.isValid())
		return;
	emit mainColorChanged(color);
}

void Sidebar::altColorClicked() {
	auto color = QColorDialog::getColor(model_.getAltColor(), this, "Select alt color");
	if (!color.isValid())
		return;
	emit altColorChanged(color);
}
