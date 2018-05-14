#include <QDebug>

#include "utility.h"
#include "scribble.h"

Scribble::Scribble(const QColor& main_color) : ShapeBase(main_color), initialized_(false) {}

void Scribble::doOnMouseDown(const QPoint& pos) {
	path_.push_back(pos);
	bounds_.setRect(pos.x(), pos.y(), 1, 1);
}

void Scribble::doOnMouseMove(const QPoint& pos) {
	if (path_.back() != pos) {
		path_.push_back(pos);
		updateBounds(pos);
	}
}

void Scribble::doOnMouseUp(const QPoint& pos) {
	doOnMouseMove(pos);
	initialized_ = true;
}

bool Scribble::doInitialized() const {
	return initialized_;
}

QRect Scribble::doRect() const {
	return bounds_;
}

void Scribble::doPaint(QPainter& painter, QPen& pen, ImageType role) const {
	for (int i = 1; i < path_.size(); i++) {
		if (role == ImageType::IMG_STROKES) {
			auto delta = path_[i] - path_[i - 1];
			pen.setColor(utility::colorFromDirection(delta.x(), delta.y()));
			painter.setPen(pen);
		}

		painter.drawLine(path_[i], path_[i - 1]);
	}
}

void Scribble::updateBounds(const QPoint& pos) {
	bounds_ |= QRect(pos.x(), pos.y(), 1, 1);
}
