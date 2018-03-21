/** @file line.cpp
 *  @brief
 *
 *  @author Viacheslav Kroilov (metopa) <slavakroilov@gmail.com>
 */

#include "line.h"

#include "utility.h"

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
	switch (role) {
		case ImageType::IMG_BG:
			return;
		case ImageType::IMG_STROKES:
			painter.setPen(QPen(QPen(QColor::fromRgb(54, 76, 85, 255))));
			break;
		case ImageType::IMG_MASK:
			painter.setPen(QPen(Qt::black));
			break;
		case ImageType::IMG_COMPOSED:
			return;
	}
	painter.drawLine(start_, end_);
}

bool Line::doInitialized() const {
	return initialized_;
}
