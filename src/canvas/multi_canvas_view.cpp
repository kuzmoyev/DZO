#include <QtWidgets/QGridLayout>

#include "multi_canvas_view.h"
#include "canvas_view.h"

MultiCanvasView::MultiCanvasView(CanvasModel& model, QWidget* parent) :
		QWidget(parent), model_(model) {
	auto* lo = new QGridLayout;
	lo->addWidget(new CanvasView(model, CanvasModel::IMG_STROKES), 0, 0);
	lo->addWidget(new CanvasView(model, CanvasModel::CanvasModel::IMG_MASK), 0, 1);
	setLayout(lo);
}
