/** @file canvas_view.cpp
 *  @brief
 *
 *  @author Viacheslav Kroilov (metopa) <slavakroilov@gmail.com>
 */

#include "canvas_view.h"

#include "QMouseEvent"

CanvasView::CanvasView(CanvasModel& model,
					   CanvasModel::ImageType role,
					   QWidget* parent) : QWidget(parent),
										  model_(model),
										  role_(role),
										  painting_mode_(ENABLED) {
	setFixedSize(model.getCanvasSize());
	connect(&model_, &CanvasModel::canvasUpdated, this, &CanvasView::updateCanvas);
	connect(&model_, &CanvasModel::canvasSizeChanged, this, &CanvasView::changeCanvasSize);
	connect(&model_, &CanvasModel::startedPainting, this, &CanvasView::blockPainting);
	connect(&model_, &CanvasModel::stoppedPainting, this, &CanvasView::unblockPainting);
}

void CanvasView::updateCanvas(QRect clip_area) {
	update(clip_area);
}

void CanvasView::changeCanvasSize(QSize size) {
	setFixedSize(size);
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
		model_.onMouseDown(event->pos());
		painting_mode_ = ACTIVE;
	}
}

void CanvasView::mouseReleaseEvent(QMouseEvent* event) {
	if (painting_mode_ == ACTIVE) {
		model_.onMouseUp(event->pos());
	}
}

void CanvasView::mouseMoveEvent(QMouseEvent* event) {
	if (painting_mode_ == ACTIVE) {
		model_.onMouseMove(event->pos());
	}
}

void CanvasView::paintEvent(QPaintEvent* event) {
	//TODO check painting is clipped
	QPainter p(this);
	QWidget::paintEvent(event);
	p.drawImage(QPoint(0, 0), model_.getImage(role_));
}
