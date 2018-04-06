#pragma once

#include "line.h"

class Rectangle : public Line {
  public:
	using Line::Line;

  protected:
	void doPaint(QPainter& painter, QPen& pen, ImageType role) const override;
};

