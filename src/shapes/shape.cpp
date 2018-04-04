/** @file shape.cpp
 *  @brief
 *
 *  @author Viacheslav Kroilov (metopa) <slavakroilov@gmail.com>
 */

#include "shape.h"

#include <QDebug>

ShapeBase::ShapeBase(const QColor& main_color) : main_color_(main_color) {}

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
	auto r = doRect().marginsAdded(QMargins(STROKE_WIDTH, STROKE_WIDTH, STROKE_WIDTH, STROKE_WIDTH));
	return r;
}

void ShapeBase::paint(QPainter& painter, ImageType role) const {
	doPaint(painter, role);
}
