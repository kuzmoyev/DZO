#pragma once

#include "shape.h"

class Line : public ShapeBase {
  public:
	explicit Line(const QColor& color);

  protected:
	void doOnMouseDown(QPoint pos) override;
	void doOnMouseMove(QPoint pos) override;
	void doOnMouseUp(QPoint pos) override;
	bool doInitialized() const override;

  protected:
	QRect doRect() const override;
	void doPaint(QPainter& painter, QPen& pen, ImageType role) const override;

  private:
	bool initialized_;
	QPoint start_;
	QPoint end_;
};

