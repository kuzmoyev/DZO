#pragma once

#include <QtCore/QRect>
#include <QtGui/QColor>

QRect rectFrom2Points(const QPoint& a, const QPoint& b);

QColor colorFromDirection(float delta_x, float delta_y);

