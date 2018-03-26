#include <src/canvas/multi_canvas_view.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sidebar.h"

MainWindow::MainWindow(QWidget* parent) :
		QMainWindow(parent),
		ui(new Ui::MainWindow),
		model_({300, 300}) {
	sidebar_ = new Sidebar(model_.getCanvasSize(), this);
	ui->setupUi(this);
	ui->centralwidget->layout()->addWidget(sidebar_);
	ui->centralwidget->layout()->addWidget(new MultiCanvasView(model_, this));

	connect(sidebar_, &Sidebar::updatedCanvasSize, &model_, &CanvasModel::setCanvasSize);


	setWindowTitle("Gradient painter [MI-DZO]");
}

MainWindow::~MainWindow() {
	delete ui;
}
