#include <QtWidgets/QMessageBox>
#include <QtWidgets/QColorDialog>
#include <QDebug>

#include "sidebar.h"
#include "ui_sidebar.h"

Sidebar::Sidebar(CanvasModel& model, QWidget* parent) :
		QWidget(parent),
		ui(new Ui::Sidebar),
		model_(model) {
	ui->setupUi(this);

	//TODO Refactor this monster

	ui->canvasXSizeLe->setEnabled(true);
	ui->canvasYSizeLe->setEnabled(true);
	setCanvasSize(model.getCanvasSize());
	ui->canvasXSizeLe->setValidator(new QIntValidator(20, 1024 * 8, this));
	ui->canvasYSizeLe->setValidator(new QIntValidator(20, 1024 * 8, this));
	connect(ui->canvasXSizeLe, &QLineEdit::returnPressed, this, &Sidebar::sizeLeChanged);
	connect(ui->canvasYSizeLe, &QLineEdit::returnPressed, this, &Sidebar::sizeLeChanged);

	main_color_btn_ = new ClickableLabel();
	alt_color_btn_ = new ClickableLabel();
	ui->colorBtnLayout->addWidget(main_color_btn_);
	ui->colorBtnLayout->addWidget(alt_color_btn_);
	main_color_btn_->setToolTip(tr("Set Main Color"));
	alt_color_btn_->setToolTip(tr("Set Alternative Color"));
	updateColors(model.getMainColor(), model.getAltColor());
	connect(main_color_btn_, &ClickableLabel::clicked, this, &Sidebar::mainColorClicked);
	connect(alt_color_btn_, &ClickableLabel::clicked, this, &Sidebar::altColorClicked);
	connect(this, &Sidebar::mainColorChanged, &model_, &CanvasModel::setMainColor);
	connect(this, &Sidebar::altColorChanged, &model_, &CanvasModel::setAltColor);
	connect(&model_, &CanvasModel::colorsUpdated, this, &Sidebar::updateColors);

	ui->runBtn->setEnabled(true);
	connect(ui->runBtn, &QPushButton::clicked, &model_, &CanvasModel::calculatePoisson);

	auto line_shape_btn = new QRadioButton("Line");
	auto rect_shape_btn = new QRadioButton("Rectangle");
	connect(line_shape_btn, &QRadioButton::pressed, [&] { emit nextShapeChanged(ShapeType::LINE); });
	connect(rect_shape_btn, &QRadioButton::pressed, [&] { emit nextShapeChanged(ShapeType::RECT); });
	connect(this, &Sidebar::nextShapeChanged, &model_, &CanvasModel::setNextShape);
	auto shape_box_lo = new QVBoxLayout();
	shape_box_lo->addWidget(line_shape_btn);
	shape_box_lo->addWidget(rect_shape_btn);
	auto shape_box = new QGroupBox("Brush mode");
	shape_box->setLayout(shape_box_lo);
	switch (model_.getNextShape()) {
		case ShapeType::LINE:
			line_shape_btn->toggle();
			break;
		case ShapeType::RECT:
			rect_shape_btn->toggle();
			break;
		default:
			qDebug() << "Shape type is not handled";
	}

	ui->verticalLayout->insertWidget(ui->verticalLayout->count() - 2, shape_box);
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
	main_color_btn_->setStyleSheet("QLabel {background-color:" + main.name() + "}");
	alt_color_btn_->setStyleSheet("QLabel {background-color:" + alt.name() + "}");
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
	auto color = QColorDialog::getColor(model_.getMainColor(), this,
										"Select main color",
										QColorDialog::DontUseNativeDialog);
	if (!color.isValid())
		return;
	emit mainColorChanged(color);
}

void Sidebar::altColorClicked() {
	auto color = QColorDialog::getColor(model_.getAltColor(), this,
										"Select alt color",
										QColorDialog::DontUseNativeDialog);
	if (!color.isValid())
		return;
	emit altColorChanged(color);
}

