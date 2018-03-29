//
// Created by kuzmovych on 14.03.18.
//

#ifndef GRADIENT_PAINTER_POISSON_H
#define GRADIENT_PAINTER_POISSON_H


#include <QtCore/QString>
#include <QtGui/QBitmap>

namespace simple_solver {
    QImage poisson(const QImage background, const QImage source, const QImage mask);

}


#endif //GRADIENT_PAINTER_POISSON_H
