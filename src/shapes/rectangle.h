#pragma once

#include "line.h"
/**
 * @brief Drawn shape, represented by the rectangle,
 * which corners are defined by start and finish positions of the cursor
 */
class Rectangle : public Line {
  public:
	using Line::Line;

  protected:
	void doPaint(QPainter& painter, QPen& pen, ImageType role) const override;
};

