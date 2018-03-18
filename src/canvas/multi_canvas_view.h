#ifndef MULTI_CANVAS_VIEW_H
#define MULTI_CANVAS_VIEW_H

#include <QWidget>

#include "canvas_model.h"


class MultiCanvasView : public QWidget {
  Q_OBJECT

  public:
	explicit MultiCanvasView(CanvasModel& model, QWidget* parent = nullptr);

  private:
	CanvasModel& model_;
};

#endif // MULTI_CANVAS_VIEW_H
