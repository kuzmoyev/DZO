#include <src/canvas/multi_canvas_view.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sidebar.h"

MainWindow::MainWindow(QWidget* parent) :
		QMainWindow(parent),
		ui(new Ui::MainWindow),
		model_({300, 300}) {
	ui->setupUi(this);
	ui->centralwidget->layout()->addWidget(new Sidebar());
	ui->centralwidget->layout()->addWidget(new MultiCanvasView(model_));

	setWindowTitle("Gradient painter [MI-DZO]");
}

MainWindow::~MainWindow() {
	delete ui;
}
