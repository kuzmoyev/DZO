#include <Qt>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>

#include "multi_canvas_view.h"
#include "canvas_view.h"

MultiCanvasView::MultiCanvasView(CanvasModel& model, QWidget* parent) :
		QWidget(parent), model_(model) {
	auto grid = new QGridLayout;
	grid->addWidget(new QLabel("Strokes"), 0, 0, Qt::AlignCenter);
	grid->addWidget(new CanvasView(model, CanvasModel::IMG_STROKES), 1, 0);

	grid->addWidget(new QLabel("Mask"), 0, 1, Qt::AlignCenter);
	grid->addWidget(new CanvasView(model, CanvasModel::IMG_MASK), 1, 1);

	grid->addWidget(new QLabel("Background"), 2, 0, Qt::AlignCenter);
	grid->addWidget(new CanvasView(model, CanvasModel::IMG_BG), 3, 0);

	grid->addWidget(new QLabel("Composed"), 2, 1, Qt::AlignCenter);
	grid->addWidget(new CanvasView(model, CanvasModel::IMG_COMPOSED), 3, 1);

	setLayout(grid);
}
