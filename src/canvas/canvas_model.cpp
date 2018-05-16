/** @file CanvasModel.cpp
 *  @brief
 *
 *  @author Viacheslav Kroilov (metopa) <slavakroilov@gmail.com>
 */

#include <QtCore/QMetaEnum>
#include <QDebug>

#include "cuda_gauss_seidel_solver.h"
#include "gauss_seidel_solver.h"
#include "amgcl_solver.h"

#include "canvas_model.h"
#include "shapes/line.h"
#include "shapes/rectangle.h"
#include "shapes/scribble.h"

void saveImage(QImage img, QString filename) {
	img.scaled(img.size() * 2, Qt::IgnoreAspectRatio, Qt::SmoothTransformation).save(filename);
}

CanvasModel::CanvasModel(QSize size, const QColor& main, const QColor& alt) :
		images_(QMetaEnum::fromType<ImageType>().keyCount()),
		main_color_(main), alt_color_(alt),
		next_shape_(ShapeType::LINE),
		poisson_mode_(PoissonBlendingMode::OVERRIDE),
		merging_mode_(BackgroundMergingMode::PRESERVE),
		current_solver_(SolverType::AMGCL) {
	setCanvasSize(size);
	getImage(ImageType::IMG_COMPOSED) = utility::filledImage(size, Qt::white);
	connect(&solver_future_, &QFutureWatcher<QImage>::finished, this, &CanvasModel::solverFinished, Qt::QueuedConnection);
}

const QImage& CanvasModel::getImage(ImageType type) const {
	return images_[(int) type];
}

QImage& CanvasModel::getImage(ImageType type) {
	return images_[(int) type];
}

const QSize& CanvasModel::getCanvasSize() const {
	return size_;
}

const QColor& CanvasModel::getMainColor() const {
	return main_color_;
}

const QColor& CanvasModel::getAltColor() const {
	return alt_color_;
}

ShapeType CanvasModel::getNextShape() const {
	return next_shape_;
}

PoissonBlendingMode CanvasModel::getPoissonMode() const {
	return poisson_mode_;
}

BackgroundMergingMode CanvasModel::getMergingMode() const {
	return merging_mode_;
}

SolverType CanvasModel::getCurrentSolver() const {
	return current_solver_;
}

void CanvasModel::saveComposed(QString filename) {
	saveImage(getImage(ImageType::IMG_COMPOSED), filename);
}

void CanvasModel::setIterationCountExp(int value) {
	iteration_count_exp_ = value;
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

void CanvasModel::setMainColor(QColor color) {
	if (main_color_ != color) {
		main_color_ = color;
		emit colorsUpdated(main_color_, alt_color_);
	}
}
void CanvasModel::setAltColor(QColor color) {
	if (main_color_ != color) {
		alt_color_ = color;
		emit colorsUpdated(main_color_, alt_color_);
	}
}
int num = 0;
void CanvasModel::startPoisson() {
	if (poisson_mode_ != PoissonBlendingMode::OVERRIDE) {
		qDebug() << "Only OVERRIDE poisson mode is supported";
	}
	if (solver_future_.isRunning()) {
		qDebug() << "Solver still running, won't start the second one";
		return;
	}
	auto source = utility::filledImage(size_, Qt::white);
	num++;
	saveImage(source, QString::number(num) + "source.png");
	saveImage(getImage(ImageType::IMG_MASK), QString::number(num) + "mask.png");
	saveImage(getImage(ImageType::IMG_BG), QString::number(num) + "target.png");

	auto solver = getSolver();
	solver_future_.setFuture(QtConcurrent::run(solver,
					  getImage(ImageType::IMG_BG), source,
					  getImage(ImageType::IMG_MASK), 1u << iteration_count_exp_));

	emit startedSolver();
}

void CanvasModel::setNextShape(ShapeType shape) {
	next_shape_ = shape;
}

void CanvasModel::setPoissonMode(PoissonBlendingMode mode) {
	poisson_mode_ = mode;
}

void CanvasModel::setSolver(SolverType s) {
	current_solver_ = s;
}

void CanvasModel::setMergingMode(BackgroundMergingMode mode) {
	merging_mode_ = mode;
}

void CanvasModel::solverFinished() {
	getImage(ImageType::IMG_COMPOSED) = solver_future_.result();
	if (merging_mode_ == BackgroundMergingMode::REPLACE) {
		qDebug() << "Only PRESERVE merging mode is supported";
		/*getImage(ImageType::IMG_BG) = getImage(ImageType::IMG_COMPOSED);
		shapes_.clear();*/
	}
	saveImage(getImage(ImageType::IMG_COMPOSED), QString::number(num) + "result.png");
	emit stoppedSolver();
	emit canvasUpdated(getImage(ImageType::IMG_COMPOSED).rect());
}

void CanvasModel::clearImg() {
	getImage(ImageType::IMG_COMPOSED) = utility::filledImage(size_, Qt::white);
	shapes_.clear();
	updateCanvas(getImage(ImageType::IMG_COMPOSED).rect(), true);
}

void CanvasModel::updateCanvas(const QRect& clipping_region, bool emit_signal) {
	//TODO Check clipping
	QPainter strokes(&images_[(int) ImageType::IMG_STROKES]);
	QPainter mask(&images_[(int) ImageType::IMG_MASK]);
	QPainter bg(&images_[(int) ImageType::IMG_BG]);

	strokes.setRenderHint(QPainter::Antialiasing);
	bg.setRenderHint(QPainter::Antialiasing);

	strokes.setClipRect(clipping_region);
	mask.setClipRect(clipping_region);
	bg.setClipRect(clipping_region);

	strokes.fillRect(clipping_region, Qt::white);
	mask.fillRect(clipping_region, Qt::white);
	bg.fillRect(clipping_region, Qt::white);

	for (auto& s : shapes_) {
		if (s->rect().intersects(clipping_region)) {
			s->paint(strokes, ImageType::IMG_STROKES);
			s->paint(mask, ImageType::IMG_MASK);
			s->paint(bg, ImageType::IMG_BG);
		}
	}

	if (emit_signal) {
		emit canvasUpdated(clipping_region);
	}
}

Shape CanvasModel::createShape() {
	switch (next_shape_) {
		case ShapeType::LINE:
			return std::make_shared<Line>(main_color_);
		case ShapeType::RECT:
			return std::make_shared<Rectangle>(main_color_);
		case ShapeType::SCRIBBLE:
			return std::make_shared<Scribble>(main_color_);
		default:
			qDebug() << "Unexpected shape type";
			return std::make_shared<Line>(main_color_);
	}
}

CanvasModel::solver_t CanvasModel::getSolver() const {
	switch (current_solver_) {
		case SolverType::AMGCL:
			return amgcl_solver::poisson;
		case SolverType::PS_CPU:
			return gauss_seidel_solver::poisson;
		case SolverType::PS_GPU:
			return cuda_gauss_seidel_solver::poisson;
	}

	return amgcl_solver::poisson;
}
