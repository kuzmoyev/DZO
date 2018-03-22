#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>

namespace Ui {
	class Sidebar;
}

class Sidebar : public QWidget {
  Q_OBJECT

  public:
	explicit Sidebar(QSize canvas_size, QWidget* parent = nullptr);
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
	void setMainColor(QColor);
	void setAltColor(QColor);

  private slots:
	void sizeLeChanged();

  private:
	Ui::Sidebar* ui;
};

#endif // SIDEBAR_H
