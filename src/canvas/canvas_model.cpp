/** @file CanvasModel.cpp
 *  @brief
 *
 *  @author Viacheslav Kroilov (metopa) <slavakroilov@gmail.com>
 */

#include <QtCore/QMetaEnum>

#include "canvas_model.h"

CanvasModel::CanvasModel(QSize size) :
		images_(QMetaEnum::fromType<ImageType>().keyCount()) {
	setCanvasSize(size);
}

QImage CanvasModel::getImage(ImageType type) {
	return images_[(int) type];
}

QSize CanvasModel::getCanvasSize() const {
	return size_;
}

void CanvasModel::setCanvasSize(QSize size) {
	if (size == size_)
		return;
	size_ = size;
	//TODO Track image type users
	for (auto& img : images_)
		img = QImage(size, QImage::Format_RGB32);
	updateCanvas({{0, 0}, size}, false);
	emit canvasSizeChanged(size);
}

void CanvasModel::onMouseDown(QPoint pos) {
	if (shapes_.empty() || shapes_.back()->initialized()) {
		shapes_.push_back(createShape());
		emit startedPainting();
	}
	auto rect = shapes_.back()->onMouseDown(pos);
	updateCanvas(rect, true);
}

void CanvasModel::onMouseMove(QPoint pos) {
	if (shapes_.empty() || shapes_.back()->initialized()) {
		qDebug("Called CanvasModel::onMouseMove while in non-editing mode");
		return;
	}
	auto rect = shapes_.back()->onMouseMove(pos);
	updateCanvas(rect, true);
}

void CanvasModel::onMouseUp(QPoint pos) {
	if (shapes_.empty() || shapes_.back()->initialized()) {
		qDebug("Called CanvasModel::onMouseUp while in non-editing mode");
		return;
	}
	auto rect = shapes_.back()->onMouseUp(pos);
	updateCanvas(rect, true);
	if (shapes_.back()->initialized()) {
		emit stoppedPainting();
	}
}

void CanvasModel::updateCanvas(const QRect& clipping_region, bool emit_signal) {
	QPainter strokes(&images_[(int) ImageType::IMG_STROKES]);
	QPainter mask(&images_[(int) ImageType::IMG_MASK]);

	strokes.setRenderHint(QPainter::Antialiasing);
	mask.setRenderHint(QPainter::Antialiasing);

	strokes.fillRect(clipping_region, Qt::white);
	mask.fillRect(clipping_region, Qt::white);

	for (auto& s : shapes_) {
		if (s->rect().intersects(clipping_region)) {
			s->paint(strokes);
			s->paint(mask, QPen(Qt::black));
		}
	}

	if (emit_signal) {
		emit canvasUpdated(clipping_region);
	}
}

Shape CanvasModel::createShape() {
	//TODO Use shape factory
	return std::make_shared<Line>(QPen(QColor::fromRgb(54, 76, 85, 255)));
}

