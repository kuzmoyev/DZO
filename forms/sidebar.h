#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
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
	void clickedRun();
	void mainColorChanged(QColor);
	void altColorChanged(QColor);

  public slots:
	void setCanvasSize(QSize);
	void setUndoState(bool);
	void setRedoState(bool);
	void setRunState(bool);
	void updateColors(QColor main, QColor alt);

  private slots:
	void sizeLeChanged();
	void mainColorClicked();
	void altColorClicked();

  private:
	Ui::Sidebar* ui;
	CanvasModel& model_;
};

#endif // SIDEBAR_H
