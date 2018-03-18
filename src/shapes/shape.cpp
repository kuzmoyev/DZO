/** @file shape.cpp
 *  @brief
 *
 *  @author Viacheslav Kroilov (metopa) <slavakroilov@gmail.com>
 */

#include "shape.h"

#include <QDebug>

ShapeBase::ShapeBase(QPen pen) : default_pen_(std::move(pen)) {}

QRect ShapeBase::onMouseDown(QPoint pos) {
	doOnMouseDown(pos);
	return rect();
}

QRect ShapeBase::onMouseMove(QPoint pos) {
	auto old_rect = rect();
	doOnMouseMove(pos);
	return rect() | old_rect;
}

QRect ShapeBase::onMouseUp(QPoint pos) {
	auto old_rect = rect();
	doOnMouseUp(pos);
	return rect() | old_rect;
}

bool ShapeBase::initialized() const {
	return doInitialized();
}

QRect ShapeBase::rect() const {
	auto r = doRect().marginsAdded(QMargins(1, 1, 1, 1));
	return r;
}

void ShapeBase::paint(QPainter& painter) const {
	paint(painter, default_pen_);
}

void ShapeBase::paint(QPainter& painter, const QPen& pen) const {
	doPaint(painter, pen);
}
