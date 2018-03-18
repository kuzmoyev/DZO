/** @file line.cpp
 *  @brief
 *
 *  @author Viacheslav Kroilov (metopa) <slavakroilov@gmail.com>
 */

#include "line.h"

#include "utility.h"

Line::Line(const QPen& pen) : ShapeBase(pen), initialized_(false) {}

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

void Line::doPaint(QPainter& painter, const QPen& pen) const {
	painter.setPen(pen);
	painter.drawLine(start_, end_);
}

bool Line::doInitialized() const {
	return initialized_;
}
