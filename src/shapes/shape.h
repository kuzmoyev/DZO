#pragma once

#include <QtCore/QRect>
#include <QtGui/QColor>
#include <QtGui/QPainter>
#include <memory>

#include "canvas/enums.h"

constexpr int STROKE_WIDTH = 5;

class ShapeBase {
  public:
	ShapeBase(const QColor& main_color);

	QRect onMouseDown(QPoint pos);
	QRect onMouseMove(QPoint pos);
	QRect onMouseUp(QPoint pos);

	virtual bool initialized() const;

	QRect rect() const;
	void paint(QPainter& painter, ImageType role) const;

  protected:
	virtual void doOnMouseDown(QPoint pos) = 0;
	virtual void doOnMouseMove(QPoint pos) = 0;
	virtual void doOnMouseUp(QPoint pos) = 0;

	virtual bool doInitialized() const = 0;

	virtual QRect doRect() const = 0;
	virtual void doPaint(QPainter& painter, QPen& pen, ImageType role) const = 0;

	QColor main_color_;
};

using Shape = std::shared_ptr<ShapeBase>;

