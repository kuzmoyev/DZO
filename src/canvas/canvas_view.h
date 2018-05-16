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
	/**
	 * @brief View ctor
	 * @param model - main app model
	 * @param role - type of canvas to display
	 * @param scale - image display size scaling
	 * @param parent - QWidget parent
	 */
	CanvasView(CanvasModel& model, ImageType role, int scale, QWidget* parent = nullptr);

  protected slots:
	/**
	 * @brief Redraw part of canvas
	 */
	void updateCanvas(QRect);
	/**
	 * @brief Change size of canvas
	 */
	void changeCanvasSize(QSize);
	/**
	 * @brief Stops reacting to mouse event
	 *
	 * This slot is activated when used starts drawing on another view
	 */
	void blockPainting();
	/**
	 * @brief Reenable reaction to mouse event
	 *
	 * This slot is activated when used stops drawing on another view
	 */
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
