/** @file image_type.h
 *  @brief
 *
 *  @author Viacheslav Kroilov (metopa) <slavakroilov@gmail.com>
 */

#ifndef GRADIENT_PAINTER_IMAGE_TYPE_H
#define GRADIENT_PAINTER_IMAGE_TYPE_H

#include <QObject>

class Enums : public QObject {
  Q_OBJECT

  public:

	enum class ImageType {
		IMG_BG, IMG_STROKES, IMG_MASK, IMG_COMPOSED
	};

	Q_ENUM(ImageType);

	enum class ShapeType {
		LINE, RECT
	};

	Q_ENUM(ShapeType)
};

using ImageType = Enums::ImageType;
using ShapeType = Enums::ShapeType;

#endif //GRADIENT_PAINTER_IMAGE_TYPE_H