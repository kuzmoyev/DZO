/** @file utility.h
 *  @brief
 *
 *  @author Viacheslav Kroilov (metopa) <slavakroilov@gmail.com>
 */

#ifndef GRADIENT_PAINTER_UTILITY_H
#define GRADIENT_PAINTER_UTILITY_H

#include <QtCore/QRect>

QRect rectFrom2Points(const QPoint& a, const QPoint& b) {
	QPoint top_left(qMin(a.x(), b.x()), qMin(a.y(), b.y()));
	QPoint bottom_right(qMax(a.x(), b.x()), qMax(a.y(), b.y()));
	return {top_left, bottom_right};
}

#endif //GRADIENT_PAINTER_UTILITY_H
