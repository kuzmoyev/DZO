#pragma once

#include <QtCore/QRect>
#include <QtGui/QColor>
#include <QtGui/QPainter>
#include <memory>

#include "canvas/enums.h"

constexpr int STROKE_WIDTH = 5;

/**
 * @brief Base class for each drawn shape
 */
class ShapeBase {
  public:
	explicit ShapeBase(const QColor& main_color);

	QRect onMouseDown(const QPoint& pos);
	QRect onMouseMove(const QPoint& pos);
	QRect onMouseUp(const QPoint& pos);

	virtual bool initialized() const;

	QRect rect() const;
	void paint(QPainter& painter, ImageType role) const;

  protected:
	virtual void doOnMouseDown(const QPoint& pos) = 0;
	virtual void doOnMouseMove(const QPoint& pos) = 0;
	virtual void doOnMouseUp(const QPoint& pos) = 0;

	virtual bool doInitialized() const = 0;

	virtual QRect doRect() const = 0;
	virtual void doPaint(QPainter& painter, QPen& pen, ImageType role) const = 0;

	QColor main_color_;
};

using Shape = std::shared_ptr<ShapeBase>;

