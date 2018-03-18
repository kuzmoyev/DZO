#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "src/canvas/canvas_model.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

  public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow() override;

  public slots:

  private:
	Ui::MainWindow* ui;
	CanvasModel model_;
};

#endif // MAINWINDOW_H
