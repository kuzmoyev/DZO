/** @file line.cpp
 *  @brief
 *
 *  @author Viacheslav Kroilov (metopa) <slavakroilov@gmail.com>
 */

#include "line.h"

#include "include/utility.h"

Line::Line() : initialized_(false) {}

void Line::doOnMouseDown(QPoint pos) {
	start_ = pos;
	end_ = pos;
}

void Line::doOnMouseMove(QPoint pos) {
	end_ = pos;
}

void Line::doOnMouseUp(QPoint pos) {
	end_ = pos;
	initialized_ = true;
}

QRect Line::doRect() const {
	return rectFrom2Points(start_, end_);
}

void Line::doPaint(QPainter& painter, ImageType role) const {
	auto delta = end_ - start_;
	QPen pen;
	switch (role) {
		case ImageType::IMG_BG:
			return;
		case ImageType::IMG_COMPOSED:
			return;
		case ImageType::IMG_STROKES:
			pen.setColor(colorFromDirection(delta.x(), delta.y()));
			pen.setWidth(3);
			break;
		case ImageType::IMG_MASK:
			pen.setColor(Qt::black);
			break;
	}
	painter.setPen(pen);
	painter.drawLine(start_, end_);
}

bool Line::doInitialized() const {
	return initialized_;
}
