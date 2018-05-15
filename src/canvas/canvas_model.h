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
#include "enums.h"

//TODO Start in a worker thread
class CanvasModel : public QObject {
  Q_OBJECT

  	using solver_t = QImage(*)(const QImage&, const QImage&, const QImage&);
  public:

	explicit CanvasModel(QSize size, const QColor& main, const QColor& alt);

  const QImage& getImage(ImageType type) const;
	QImage& getImage(ImageType type);
	const QSize& getCanvasSize() const;
	const QColor& getMainColor() const;
	const QColor& getAltColor() const;
	ShapeType getNextShape() const;
	PoissonBlendingMode getPoissonMode() const;
	BackgroundMergingMode getMergingMode() const;
	SolverType getCurrentSolver() const;

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
	void calculatePoisson();
	void setNextShape(ShapeType);
	void setPoissonMode(PoissonBlendingMode);
	void setSolver(SolverType);
	void setMergingMode(BackgroundMergingMode);


  private:
	void updateCanvas(const QRect& clipping_region, bool emit_signal);
	Shape createShape();
	solver_t getSolver() const;

	QVector<QImage> images_;
	QVector<Shape> shapes_;

	QSize size_;
	QColor main_color_;
	QColor alt_color_;
	ShapeType next_shape_;
	PoissonBlendingMode poisson_mode_;
	BackgroundMergingMode merging_mode_;
	SolverType current_solver_;
};


#endif //GRADIENT_PAINTER_CANVASMODEL_H
