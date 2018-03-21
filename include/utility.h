/** @file utility.h
 *  @brief
 *
 *  @author Viacheslav Kroilov (metopa) <slavakroilov@gmail.com>
 */

#ifndef GRADIENT_PAINTER_UTILITY_H
#define GRADIENT_PAINTER_UTILITY_H

#include <QtCore/QRect>
#include <QtGui/QColor>

QRect rectFrom2Points(const QPoint& a, const QPoint& b) {
	QPoint top_left(qMin(a.x(), b.x()), qMin(a.y(), b.y()));
	QPoint bottom_right(qMax(a.x(), b.x()), qMax(a.y(), b.y()));
	return {top_left, bottom_right};
}

QColor colorFromDirection(float delta_x, float delta_y) {
	float dist = std::sqrt(delta_x * delta_x + delta_y * delta_y);
	float cos = delta_x / dist;
	float sin = delta_y / dist;

	return QColor::fromRgbF((qreal) cos / 2 + 0.5f, (qreal) sin / 2 + 0.5f, 0.5f);
}

#endif //GRADIENT_PAINTER_UTILITY_H
