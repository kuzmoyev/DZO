/** @file CanvasModel.h
 *  @brief
 *
 *  @author Viacheslav Kroilov (metopa) <slavakroilov@gmail.com>
 */

#ifndef GRADIENT_PAINTER_CANVASMODEL_H
#define GRADIENT_PAINTER_CANVASMODEL_H


#include <memory>

#include <QtCore/QObject>
#include <QtCore/QRect>
#include <QtGui/QImage>
#include <QtGui/QBitmap>

#include "shapes/shape.h"
#include "shapes/line.h"
#include "image_type.h"

//TODO Start in a worker thread
class CanvasModel : public QObject {
  Q_OBJECT

  public:
	explicit CanvasModel(QSize size, const QColor& main, const QColor& alt);

	QImage getImage(ImageType type);
	QSize getCanvasSize() const;
	const QColor& getMainColor() const ;
	const QColor& getAltColor() const ;

  signals:
	void canvasUpdated(QRect bounds);
	void canvasSizeChanged(QSize size);
	void startedPainting();
	void stoppedPainting();
	void colorsUpdated(QColor main, QColor alt);

  public slots:
	// void setShapeFactory();
	void setCanvasSize(QSize size);
	void onMouseDown(QPoint pos);
	void onMouseMove(QPoint pos);
	void onMouseUp(QPoint pos);
	void setMainColor(QColor);
	void setAltColor(QColor);

  private:
	void updateCanvas(const QRect& clipping_region, bool emit_signal);
	Shape createShape();

	QVector<QImage> images_;
	QVector<Shape> shapes_;

	QSize size_;
	QColor main_color_;
	QColor alt_color_;
};


#endif //GRADIENT_PAINTER_CANVASMODEL_H