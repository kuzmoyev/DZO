#include "line.h"

#include "src/utility.h"

Line::Line(const QColor& color) : ShapeBase(color), initialized_(false) {}

void Line::doOnMouseDown(const QPoint& pos) {
	start_ = pos;
	end_ = pos;
}

void Line::doOnMouseMove(const QPoint& pos) {
	end_ = pos;
}

void Line::doOnMouseUp(const QPoint& pos) {
	doOnMouseMove(pos);
	initialized_ = true;
}

QRect Line::doRect() const {
	return rectFrom2Points(start_, end_);
}

void Line::doPaint(QPainter& painter, QPen& pen, ImageType role) const {
	if (role == ImageType::IMG_STROKES) {
		auto delta = end_ - start_;
		pen.setColor(colorFromDirection(delta.x(), delta.y()));
		painter.setPen(pen);
	}

	painter.drawLine(start_, end_);
}

bool Line::doInitialized() const {
	return initialized_;
}
