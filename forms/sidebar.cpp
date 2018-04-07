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

	initCanvasSizeLe();
	initColorButtons();
	initShapeSelector();

	connect(ui->run_btn_, &QPushButton::clicked, &model_, &CanvasModel::calculatePoisson);
}

Sidebar::~Sidebar() {
	delete ui;
}

void Sidebar::setCanvasSize(QSize size) {
	ui->canvas_size_x_le_->setText(QString::number(size.width()));
	ui->canvas_size_y_le_->setText(QString::number(size.height()));
}

void Sidebar::setUndoState(bool on) {
	ui->undo_btn_->setEnabled(on);
}

void Sidebar::setRedoState(bool on) {
	ui->redo_btn_->setEnabled(on);
}

void Sidebar::setRunState(bool on) {
	ui->run_btn_->setEnabled(on);
}

void Sidebar::updateColors(QColor main, QColor alt) {
	main_color_btn_->setStyleSheet("QLabel {background-color:" + main.name() + "}");
	alt_color_btn_->setStyleSheet("QLabel {background-color:" + alt.name() + "}");
}


void Sidebar::sizeLeChanged() {
	bool ok;
	auto width = ui->canvas_size_x_le_->text().toInt(&ok);
	if (!ok) {
		QMessageBox msgBox;
		msgBox.setText("Invalid canvas size value: " + ui->canvas_size_x_le_->text());
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.exec();
		return;
	}
	auto height = ui->canvas_size_y_le_->text().toInt(&ok);
	if (!ok) {
		QMessageBox msgBox;
		msgBox.setText("Invalid canvas size value: " + ui->canvas_size_y_le_->text());
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.exec();
		return;
	}

	QSize size(width, height);
	ui->canvas_size_x_le_->setText(QString::number(width));
	ui->canvas_size_y_le_->setText(QString::number(height));
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

void Sidebar::initCanvasSizeLe() {
	ui->canvas_size_x_le_->setValidator(new QIntValidator(20, 1024 * 8, this));
	ui->canvas_size_y_le_->setValidator(new QIntValidator(20, 1024 * 8, this));

	connect(ui->canvas_size_x_le_, &QLineEdit::returnPressed, this, &Sidebar::sizeLeChanged);
	connect(ui->canvas_size_y_le_, &QLineEdit::returnPressed, this, &Sidebar::sizeLeChanged);
	setCanvasSize(model_.getCanvasSize());
}

void Sidebar::initColorButtons() {
	main_color_btn_ = new ClickableLabel();
	main_color_btn_->setToolTip(tr("Set Main Color"));
	ui->color_btn_lo_->addWidget(main_color_btn_);

	alt_color_btn_ = new ClickableLabel();
	alt_color_btn_->setToolTip(tr("Set Alternative Color"));
	ui->color_btn_lo_->addWidget(alt_color_btn_);

	connect(main_color_btn_, &ClickableLabel::clicked, this, &Sidebar::mainColorClicked);
	connect(this, &Sidebar::mainColorChanged, &model_, &CanvasModel::setMainColor);

	connect(alt_color_btn_, &ClickableLabel::clicked, this, &Sidebar::altColorClicked);
	connect(this, &Sidebar::altColorChanged, &model_, &CanvasModel::setAltColor);

	connect(&model_, &CanvasModel::colorsUpdated, this, &Sidebar::updateColors);
	updateColors(model_.getMainColor(), model_.getAltColor());
}

void Sidebar::initShapeSelector() {
	auto shape_box_lo = new QVBoxLayout();

	auto line_shape_btn = new QRadioButton("Line");
	shape_box_lo->addWidget(line_shape_btn);

	auto rect_shape_btn = new QRadioButton("Rectangle");
	shape_box_lo->addWidget(rect_shape_btn);

	auto scribble_shape_btn = new QRadioButton("Scribble");
	shape_box_lo->addWidget(scribble_shape_btn);

	auto shape_box = new QGroupBox("Brush mode");
	shape_box->setLayout(shape_box_lo);

	switch (model_.getNextShape()) {
		case ShapeType::LINE:
			line_shape_btn->toggle();
			break;
		case ShapeType::RECT:
			rect_shape_btn->toggle();
			break;
		case ShapeType::SCRIBBLE:
			scribble_shape_btn->toggle();
			break;
		default:
			qDebug() << "Shape type is not handled";
	}

	ui->layout_->insertWidget(ui->layout_->count() - 2, shape_box);

	connect(line_shape_btn, &QRadioButton::pressed, [&] { emit nextShapeChanged(ShapeType::LINE); });
	connect(rect_shape_btn, &QRadioButton::pressed, [&] { emit nextShapeChanged(ShapeType::RECT); });
	connect(scribble_shape_btn, &QRadioButton::pressed, [&] { emit nextShapeChanged(ShapeType::SCRIBBLE); });
	connect(this, &Sidebar::nextShapeChanged, &model_, &CanvasModel::setNextShape);
}

