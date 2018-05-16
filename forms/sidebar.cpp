#include <QtWidgets/QMessageBox>
#include <QtWidgets/QColorDialog>
#include <QDebug>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QFileDialog>

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
	initSolverSelector();
	//initMergingModeSelector();

	connect(ui->run_btn_, &QPushButton::clicked, &model_, &CanvasModel::startPoisson);

	connect(&model, &CanvasModel::startedSolver, this, &Sidebar::deactivateRunBtn);
	connect(&model, &CanvasModel::stoppedSolver, this, &Sidebar::activateRunBtn);
	connect(ui->save_canvas_btn_, &QPushButton::clicked, this, &Sidebar::saveComposed);
	model.setIterationCountExp(ui->iterations_count_->value());
	connect(ui->iterations_count_, &QSlider::valueChanged,
			&model, &CanvasModel::setIterationCountExp);

	ui->save_mask_btn_->setEnabled(true);
	ui->save_mask_btn_->setText("Clear");
	connect(ui->save_mask_btn_, &QPushButton::clicked, &model, &CanvasModel::clearImg);
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

void Sidebar::deactivateRunBtn() {
	ui->run_btn_->setEnabled(false);
	ui->run_btn_->setText("Running");
}
void Sidebar::activateRunBtn() {
	ui->run_btn_->setEnabled(true);
	ui->run_btn_->setText("Run");
}

void Sidebar::saveComposed() {
	auto filename = QFileDialog::getSaveFileName(this, "Save composed image");
	if (!filename.isEmpty())
		model_.saveComposed(filename);
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
	auto layout = new QVBoxLayout();

	auto line_shape_btn = new QRadioButton("Line");
	layout->addWidget(line_shape_btn);

	auto rect_shape_btn = new QRadioButton("Rectangle");
	layout->addWidget(rect_shape_btn);

	auto scribble_shape_btn = new QRadioButton("Scribble");
	layout->addWidget(scribble_shape_btn);

	auto group_box = new QGroupBox("Brush mode");
	group_box->setLayout(layout);

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
	}

	ui->layout_->insertWidget(ui->layout_->count() - 2, group_box);

	connect(line_shape_btn, &QRadioButton::pressed, [&] { emit nextShapeChanged(ShapeType::LINE); });
	connect(rect_shape_btn, &QRadioButton::pressed, [&] { emit nextShapeChanged(ShapeType::RECT); });
	connect(scribble_shape_btn, &QRadioButton::pressed, [&] { emit nextShapeChanged(ShapeType::SCRIBBLE); });
	connect(this, &Sidebar::nextShapeChanged, &model_, &CanvasModel::setNextShape);
}

void Sidebar::initSolverSelector() {
	auto layout = new QVBoxLayout();

	auto amgcl_btn = new QRadioButton("Amgcl");
	layout->addWidget(amgcl_btn);

	auto ps_cpu_btn = new QRadioButton("Gauss-Seidel");
	layout->addWidget(ps_cpu_btn);

	auto ps_gpu_btn = new QRadioButton("Gauss-Seidel CUDA");
	layout->addWidget(ps_gpu_btn);


	auto group_box = new QGroupBox("Solver");
	group_box->setLayout(layout);

	switch (model_.getCurrentSolver()) {
		case SolverType::AMGCL:
			amgcl_btn->toggle();
			break;
		case SolverType::PS_CPU:
		case SolverType::PS_GPU:
			ps_cpu_btn->toggle();
			break;
	}

	ui->layout_->insertWidget(ui->layout_->count() - 2, group_box);

	connect(amgcl_btn, &QRadioButton::pressed,
			[&] { emit solverChanged(SolverType::AMGCL); });
	connect(ps_cpu_btn, &QRadioButton::pressed,
			[&] { emit solverChanged(SolverType::PS_CPU); });
	connect(ps_gpu_btn, &QRadioButton::pressed,
			[&] { emit solverChanged(SolverType::PS_GPU); });

	connect(this, &Sidebar::solverChanged, &model_, &CanvasModel::setSolver);
}

void Sidebar::initMergingModeSelector() {
	auto layout = new QVBoxLayout();

	auto preserve_btn = new QRadioButton("Preserve");
	layout->addWidget(preserve_btn);

	auto replace_btn = new QRadioButton("Replace");
	layout->addWidget(replace_btn);

	auto group_box = new QGroupBox("Background merging mode");
	group_box->setLayout(layout);

	switch (model_.getMergingMode()) {
		case BackgroundMergingMode::PRESERVE:
			preserve_btn->toggle();
			break;
		case BackgroundMergingMode::REPLACE:
			replace_btn->toggle();
			break;
	}

	ui->layout_->insertWidget(ui->layout_->count() - 2, group_box);

	connect(preserve_btn, &QRadioButton::pressed,
			[&] { emit mergingModeChanged(BackgroundMergingMode::PRESERVE); });
	connect(replace_btn, &QRadioButton::pressed,
			[&] { emit mergingModeChanged(BackgroundMergingMode::REPLACE); });

	connect(this, &Sidebar::mergingModeChanged, &model_, &CanvasModel::setMergingMode);
}



