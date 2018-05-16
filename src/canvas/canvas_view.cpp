/** @file canvas_view.cpp
 *  @brief
 *
 *  @author Viacheslav Kroilov (metopa) <slavakroilov@gmail.com>
 */

#include "canvas_view.h"

#include "QMouseEvent"

CanvasView::CanvasView(CanvasModel& model, ImageType role, int scale, QWidget* parent) :
		QWidget(parent), model_(model), role_(role), painting_mode_(ENABLED), scale_(scale) {
	setFixedSize(model.getCanvasSize() * scale_);
	connect(&model_, &CanvasModel::canvasUpdated, this, &CanvasView::updateCanvas);
	connect(&model_, &CanvasModel::canvasSizeChanged, this, &CanvasView::changeCanvasSize);
	connect(&model_, &CanvasModel::startedPainting, this, &CanvasView::blockPainting);
	connect(&model_, &CanvasModel::stoppedPainting, this, &CanvasView::unblockPainting);
}

void CanvasView::updateCanvas(QRect clip_area) {
	update(QRect(clip_area.topLeft() * scale_, (clip_area.bottomRight() +QPoint(1, 1)) * scale_));
}

void CanvasView::changeCanvasSize(QSize size) {
	setFixedSize(size * scale_);
	update(); //TODO Check if needed
}

void CanvasView::blockPainting() {
	if (painting_mode_ == ACTIVE) {
		painting_mode_ = BLOCKED;
	}
}

void CanvasView::unblockPainting() {
	if (painting_mode_ == BLOCKED || painting_mode_ == ACTIVE) {
		painting_mode_ = ENABLED;
	}
}

void CanvasView::mousePressEvent(QMouseEvent* event) {
	if (painting_mode_ == ENABLED) {
		model_.onMouseDown(event->pos() / scale_);
		painting_mode_ = ACTIVE;
	}
}

void CanvasView::mouseReleaseEvent(QMouseEvent* event) {
	if (painting_mode_ == ACTIVE) {
		model_.onMouseUp(event->pos() / scale_);
	}
}

void CanvasView::mouseMoveEvent(QMouseEvent* event) {
	if (painting_mode_ == ACTIVE) {
		model_.onMouseMove(event->pos() / scale_);
	}
}

void CanvasView::paintEvent(QPaintEvent* event) {
	Q_UNUSED(event);
	//TODO check painting is clipped
	QPainter p(this);
	auto img = model_.getImage(role_);
	p.drawImage(QPoint(0, 0), img.scaled(img.size() * scale_));
}
