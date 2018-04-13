#pragma once

#include <QDebug>
#include <QtGui/QBitmap>


const QRgb WHITE = 0xffffffff;

template <typename T>
T bound(T x, T min = 0, T max = 255) {
    return std::max(std::min(x, max), min);
}
