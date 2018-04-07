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
		LINE, RECT, SCRIBBLE
	};
	Q_ENUM(ShapeType)

	enum class PoissonBlendingMode {
		OVERRIDE, BLEND
	};
	Q_ENUM(PoissonBlendingMode)

	enum class BackgroundMergingMode {
		PRESERVE, REPLACE
	};
	Q_ENUM(BackgroundMergingMode)
};

using ImageType = Enums::ImageType;
using ShapeType = Enums::ShapeType;
using PoissonBlendingMode = Enums::PoissonBlendingMode;
using BackgroundMergingMode = Enums::BackgroundMergingMode;

#endif //GRADIENT_PAINTER_IMAGE_TYPE_H
