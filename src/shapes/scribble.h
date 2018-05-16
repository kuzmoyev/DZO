#pragma once

#include "shape.h"
/**
 * @brief Drawn shape, defined by the cursor path
 */
class Scribble : public ShapeBase {
  public:
	Scribble(const QColor& main_color);

  protected:
	void doOnMouseDown(const QPoint& pos) override;
	void doOnMouseMove(const QPoint& pos) override;
	void doOnMouseUp(const QPoint& pos) override;

	bool doInitialized() const override;

	QRect doRect() const override;
	void doPaint(QPainter& painter, QPen& pen, ImageType role) const override;

  private:
	void updateBounds(const QPoint& pos);

	bool initialized_;
	QRect bounds_;
	QVector<QPoint> path_;
};
