/** @file shape.h
 *  @brief
 *
 *  @author Viacheslav Kroilov (metopa) <slavakroilov@gmail.com>
 */

#ifndef GRADIENT_PAINTER_SHAPE_H
#define GRADIENT_PAINTER_SHAPE_H


#include <QtCore/QRect>
#include <QtGui/QColor>
#include <QtGui/QPainter>
#include <memory>

class ShapeBase {
  public:
	explicit ShapeBase(QPen pen);

	QRect onMouseDown(QPoint pos);
	QRect onMouseMove(QPoint pos);
	QRect onMouseUp(QPoint pos);

	virtual bool initialized() const;

	QRect rect() const;
	void paint(QPainter& painter) const;
	void paint(QPainter& painter, const QPen& pen) const;

  protected:
	virtual void doOnMouseDown(QPoint pos) = 0;
	virtual void doOnMouseMove(QPoint pos) = 0;
	virtual void doOnMouseUp(QPoint pos) = 0;

	virtual bool doInitialized() const = 0;

	virtual QRect doRect() const = 0;
	virtual void doPaint(QPainter& painter, const QPen& pen) const = 0;

  private:
	QPen default_pen_;
};

using Shape = std::shared_ptr<ShapeBase>;


#endif //GRADIENT_PAINTER_SHAPE_H
