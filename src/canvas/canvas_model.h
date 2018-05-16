/** @file CanvasModel.h
 *  @brief
 *
 *  @author Viacheslav Kroilov (metopa) <slavakroilov@gmail.com>
 */

#ifndef GRADIENT_PAINTER_CANVASMODEL_H
#define GRADIENT_PAINTER_CANVASMODEL_H


#include <memory>

#include <QtConcurrent/QtConcurrent>
#include <QtCore/QObject>
#include <QtCore/QRect>
#include <QtGui/QImage>
#include <QtGui/QBitmap>

#include "shapes/shape.h"
#include "enums.h"

/**
 * @brief Main class that implements whole application logic
 *
 * This class is responsible for maintaining and adding new shapes,
 * updating different kinds of canvas images,
 * launching poisson solvers (in a separate threads) and accepting calculated result,
 * and storing application configuration (like current drawing color or shape type).
 *
 * This class also fulfills responsibilities of application event bus.
 * Signals of the class are emitted on global events.
 * Each object that want to react on changes of
 * the global state of application
 * should subscribe to these signals.
 */
class CanvasModel : public QObject {
  Q_OBJECT
	using solver_t = QImage(*)(const QImage&, const QImage&, const QImage&, unsigned int);
  public:

	/**
	 * @brief Model ctor.
	 * Initial values of some configuration variables are accepted as parameters.
	 * @param size - initial size of canvas
	 * @param main - initial drawing color
	 * @param alt - reserved for future implementations
	 */
	explicit CanvasModel(QSize size, const QColor& main, const QColor& alt);

	const QImage& getImage(ImageType type) const;
	QImage& getImage(ImageType type);
	const QSize& getCanvasSize() const;
	const QColor& getMainColor() const;
	const QColor& getAltColor() const;
	/**
	 * @brief Construct an instance of currently selected drawing shape
	 */
	ShapeType getNextShape() const;
	/**
	 * @brief Reserved for future features
	 */
	PoissonBlendingMode getPoissonMode() const;
	/**
	 * @brief Reserved for future features
	 */
	BackgroundMergingMode getMergingMode() const;
	/**
	 * @brief Return type of currently selected poisson solver
	 */
	SolverType getCurrentSolver() const;
	/**
	 * @brief Save calculated image to file
	 * @param filename - path to filename.
	 *                      Filename extension should define
	 *                      the extension the file will be stored under.
	 */
	void saveComposed(QString filename);

  signals:
	/**
	 * @brief Emitted when any part of canvases is changed.
	 * View objects should react on this signal
	 */
	void canvasUpdated(QRect bounds);
	/**
	 * @brief Emitted when canvas size is changed.
	 * View objects should react on this signal
	 */
	void canvasSizeChanged(QSize size);
	/**
	 * @brief Emitted when user starts to draw on any of the canvases.
	 */
	void startedPainting();
	/**
	 * @brief Emitted when user stops to draw on any of the canvases.
	 */
	void stoppedPainting();
	/**
	 * @brief Emitted when user selects new drawing color.
	 */
	void colorsUpdated(QColor main, QColor alt);
	/**
	 * @brief Emits when solver thread has been started.
	 */
	void startedSolver();
	/**
	 * @brief Emits when solver thread finished computations.
	 */
	void stoppedSolver();

  public slots:
	/**
	 * @brief Set Gauss-Seidel quality.
	 * Number of iterations equals 2^iterations_count_exp
	 */
	void setIterationCountExp(int);
	/**
	 * @brief Set canvas size
	 * canvasSizeChanged signal is emitted
	 */
	void setCanvasSize(QSize size);
	/**
	 * @brief Create new drawing shape and start passing
	 * cursor position information until shape is initialized
	 */
	void onMouseDown(QPoint pos);
	/**
	 * @brief Pass moved cursor position to the currently drawing shape and update canvases
	 */
	void onMouseMove(QPoint pos);
	/**
	 * @brief Pass moved cursor position to the currently drawing shape and update canvases.
	 * ASk if shape has been initialized. Stop updating the shape if so.
	 */
	void onMouseUp(QPoint pos);
	/**
	 * @brief Set drawing color
	 */
	void setMainColor(QColor);
	/**
	 * @brief Reserved for future features
	 */
	void setAltColor(QColor);
	/**
	 * @brief Starts poisson solver in the separate thread
	 * startedSolver signal is emitted
	 */
	void startPoisson();
	/**
	 * @brief This slot is activated by solver thread when it finished calculation.
	 * Update composed canvas and emit canvasUpdated and solverFinished signals
	 */
	void solverFinished();
	/**
	 * @brief Change next shape to be drawn
	 */
	void setNextShape(ShapeType);
	/**
	 * @brief Reserved for future features
	 */
	void setPoissonMode(PoissonBlendingMode);
	/**
	 * @brief Set solver to be used for calculations
	 */
	void setSolver(SolverType);
	/**
	 * @brief Reserved for future features
	 */
	void setMergingMode(BackgroundMergingMode);
	/**
	 * @brief Reset all canvases and shape list
	 */
	void clearImg();

  private:
	void updateCanvas(const QRect& clipping_region, bool emit_signal);
	Shape createShape();
	solver_t getSolver() const;

	QVector<QImage> images_;
	QVector<Shape> shapes_;

	QFutureWatcher<QImage> solver_future_;

	QSize size_;
	QColor main_color_;
	QColor alt_color_;
	ShapeType next_shape_;
	int iteration_count_exp_;
	PoissonBlendingMode poisson_mode_;
	BackgroundMergingMode merging_mode_;
	SolverType current_solver_;
};


#endif //GRADIENT_PAINTER_CANVASMODEL_H
