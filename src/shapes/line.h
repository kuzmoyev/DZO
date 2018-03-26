/** @file line.h
 *  @brief
 *
 *  @author Viacheslav Kroilov (metopa) <slavakroilov@gmail.com>
 */

#ifndef GRADIENT_PAINTER_LINE_H
#define GRADIENT_PAINTER_LINE_H


#include "shape.h"

class Line : public ShapeBase {
  public:
	Line();

  protected:
	void doOnMouseDown(QPoint pos) override;
	void doOnMouseMove(QPoint pos) override;
	void doOnMouseUp(QPoint pos) override;
	bool doInitialized() const override;

  protected:
	QRect doRect() const override;
	void doPaint(QPainter& painter, ImageType role) const override;

  private:
	bool initialized_;
	QPoint start_;
	QPoint end_;
};


#endif //GRADIENT_PAINTER_LINE_H
