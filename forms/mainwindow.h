#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>

#include "src/canvas/canvas_model.h"

#include "sidebar.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

  public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow() override;


  private:
	Ui::MainWindow* ui;
	Sidebar* sidebar_;
	QColor main_color_;
	QColor alt_color_;
	//QThread compute_thread_;
	CanvasModel model_;
};

#endif // MAINWINDOW_H
