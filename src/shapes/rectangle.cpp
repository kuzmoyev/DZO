#include "rectangle.h"

void Rectangle::doPaint(QPainter& painter, QPen& pen, ImageType role) const {
	Q_UNUSED(pen);
	Q_UNUSED(role);
	painter.drawRect(doRect());
}
