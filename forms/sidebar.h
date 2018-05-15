#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include <src/gui/clickable_label.h>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QRadioButton>
#include "src/canvas/canvas_model.h"

namespace Ui {
	class Sidebar;
}

class Sidebar : public QWidget {
  Q_OBJECT

  public:
	explicit Sidebar(CanvasModel& model, QWidget* parent = nullptr);
	~Sidebar() override;


  signals:
	void updatedCanvasSize(QSize);
	void clickedUndo();
	void clickedRedo();
	void mainColorChanged(QColor);
	void altColorChanged(QColor);
	void nextShapeChanged(ShapeType);
	void poissonModeChanged(PoissonBlendingMode);
	void mergingModeChanged(BackgroundMergingMode);
	void solverChanged(SolverType);

  public slots:
	void setCanvasSize(QSize);
	void setUndoState(bool);
	void setRedoState(bool);
	void setRunState(bool);
	void updateColors(QColor main, QColor alt);
	void deactivateRunBtn();
	void activateRunBtn();

  private slots:
	void sizeLeChanged();
	void mainColorClicked();
	void altColorClicked();

  private:
	void initCanvasSizeLe();
	void initColorButtons();
	void initShapeSelector();
	void initSolverSelector();
	void initMergingModeSelector();

	Ui::Sidebar* ui;
	CanvasModel& model_;
	ClickableLabel* main_color_btn_;
	ClickableLabel* alt_color_btn_;
	QGroupBox* shapes_box_;
};

#endif // SIDEBAR_H
