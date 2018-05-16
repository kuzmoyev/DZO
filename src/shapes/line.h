#pragma once

#include "shape.h"

/**
 * @brief Drawn shape, represented by the line,
 * which ends are defined by start and finish positions of the cursor
 */
class Line : public ShapeBase {
  public:
	explicit Line(const QColor& color);

  protected:
	void doOnMouseDown(const QPoint& pos) override;
	void doOnMouseMove(const QPoint& pos) override;
	void doOnMouseUp(const QPoint& pos) override;
	bool doInitialized() const override;

  protected:
	QRect doRect() const override;
	void doPaint(QPainter& painter, QPen& pen, ImageType role) const override;

  private:
	bool initialized_;
	QPoint start_;
	QPoint end_;
};

