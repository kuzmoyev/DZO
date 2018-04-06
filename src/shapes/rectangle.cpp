#include "rectangle.h"

void Rectangle::doPaint(QPainter& painter, QPen& pen, ImageType role) const {
	painter.drawRect(doRect());
}
