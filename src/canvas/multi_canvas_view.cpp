#include <Qt>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QSizePolicy>

#include "multi_canvas_view.h"
#include "canvas_view.h"

MultiCanvasView::MultiCanvasView(CanvasModel& model, QWidget* parent) :
		QWidget(parent), model_(model) {
	auto grid = new QGridLayout;
	grid->setSizeConstraint(QLayout::SetFixedSize);

	grid->addWidget(new QLabel("Mask"), 0, 0, Qt::AlignCenter);
	grid->addWidget(new CanvasView(model, ImageType::IMG_MASK, 1), 1, 0);

	grid->addWidget(new QLabel("Background"), 2, 0, Qt::AlignCenter);
	grid->addWidget(new CanvasView(model, ImageType::IMG_BG, 1), 3, 0);

	grid->addWidget(new QLabel("Composed"), 0, 1, 1, 1, Qt::AlignCenter);
	grid->addWidget(new CanvasView(model, ImageType::IMG_COMPOSED, 2), 1, 1, 3, 1, Qt::AlignTop);

	setLayout(grid);
}
