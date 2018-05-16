/** @file canvas_view.h
 *  @brief
 *
 *  @author Viacheslav Kroilov (metopa) <slavakroilov@gmail.com>
 */

#ifndef GRADIENT_PAINTER_CANVAS_VIEW_H
#define GRADIENT_PAINTER_CANVAS_VIEW_H


#include <QWidget>

#include "canvas_model.h"

//TODO Use QGraphicsView approach
class CanvasView : public QWidget {
  Q_OBJECT
  public:
	CanvasView(CanvasModel& model, ImageType role, int scale, QWidget* parent = nullptr);

  protected slots:
	void updateCanvas(QRect);
	void changeCanvasSize(QSize);
	void blockPainting();
	void unblockPainting();

  protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void paintEvent(QPaintEvent* event) override;

  private:
	enum PaintingMode {
		DISABLED, ENABLED, ACTIVE, BLOCKED
	};
	CanvasModel& model_;
	ImageType role_;
	PaintingMode painting_mode_;
	int scale_;
};


#endif //GRADIENT_PAINTER_CANVAS_VIEW_H
