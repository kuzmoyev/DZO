#include "shape.h"

#include <QDebug>

ShapeBase::ShapeBase(const QColor& main_color) : main_color_(main_color) {}

QRect ShapeBase::onMouseDown(const QPoint& pos) {
	doOnMouseDown(pos);
	return rect();
}

QRect ShapeBase::onMouseMove(const QPoint& pos) {
	auto old_rect = rect();
	doOnMouseMove(pos);
	return rect() | old_rect;
}

QRect ShapeBase::onMouseUp(const QPoint& pos) {
	auto old_rect = rect();
	doOnMouseUp(pos);
	return rect() | old_rect;
}

bool ShapeBase::initialized() const {
	return doInitialized();
}

QRect ShapeBase::rect() const {
	return doRect() + QMargins(STROKE_WIDTH, STROKE_WIDTH, STROKE_WIDTH, STROKE_WIDTH);
}

void ShapeBase::paint(QPainter& painter, ImageType role) const {
	QPen pen;
	switch (role) {
		case ImageType::IMG_BG:
			pen.setColor(main_color_);
			pen.setWidth(STROKE_WIDTH);
			break;

		case ImageType::IMG_COMPOSED:
			return;

		case ImageType::IMG_STROKES:
			pen.setColor(Qt::darkGray);
			pen.setWidth(STROKE_WIDTH);
			break;
		case ImageType::IMG_MASK:
			pen.setColor(Qt::black);
			break;
	}

	painter.setPen(pen);
	doPaint(painter, pen, role);
}
