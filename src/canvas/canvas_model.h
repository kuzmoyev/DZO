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

//TODO Start in a worker thread
class CanvasModel : public QObject {
  Q_OBJECT

  public:
	enum ImageType {
		IMG_BG, IMG_STROKES, IMG_MASK, IMG_COMPOSED
	};

	Q_ENUM(ImageType)

	explicit CanvasModel(QSize size);

	QImage getImage(ImageType type);
	QSize getCanvasSize() const;

  signals:
	void canvasUpdated(QRect bounds);
	void canvasSizeChanged(QSize size);
	void startedPainting();
	void stoppedPainting();

  public slots:
	// void setShapeFactory();
	void setCanvasSize(QSize size);
	void onMouseDown(QPoint pos);
	void onMouseMove(QPoint pos);
	void onMouseUp(QPoint pos);

  private:
	void updateCanvas(const QRect& clipping_region, bool emit_signal);
	Shape createShape();

	QVector<QImage> images_;
	QVector<Shape> shapes_;
	QSize size_;
};


#endif //GRADIENT_PAINTER_CANVASMODEL_H
