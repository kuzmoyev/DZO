#include "utility.h"
#include <cmath>

using namespace std;

namespace utility {
	vector<vector<double>> getGaussianKernel(size_t size) {
		double sigma = 1;
		vector<vector<double>> kernel(size, vector<double>(size));
		double mean = size / 2;
		double sum = 0.0; // For accumulating the kernel values
		for (size_t x = 0; x < size; ++x)
			for (size_t y = 0; y < size; ++y) {
				kernel[x][y] = exp(-0.5 * (pow((x - mean) / sigma, 2.0) + pow((y - mean) / sigma, 2.0))) /
							   (2 * M_PI * sigma * sigma);

				sum += kernel[x][y];
			}

		for (size_t x = 0; x < size; ++x)
			for (size_t y = 0; y < size; ++y)
				kernel[x][y] /= sum;
		return kernel;
	}


	QImage gaussBlur(const QImage& img, int kernelSize) {
		QImage result = img.copy();
		auto kernel = getGaussianKernel(kernelSize);
		int kernelHalfSize = kernelSize / 2;

		for (int y = 0; y < img.height(); y++) {
			for (int x = 0; x < img.width(); x++) {

				double rt = 0, gt = 0, bt = 0;
				double p = 0;
				for (int i = 0, dy = -kernelHalfSize; i < kernelSize; i++, dy++) {
					for (int j = 0, dx = -kernelHalfSize; j < kernelSize; j++, dx++) {
						if (x + dx >= 0 && x + dx < img.width() && y + dy >= 0 && y + dy < img.height()) {
							QColor s(img.pixel(x + dx, y + dy));
							bt += s.blue() * kernel[i][j];
							gt += s.green() * kernel[i][j];
							rt += s.red() * kernel[i][j];
							p += kernel[i][j];
						}
					}
				}

				QColor color = QColor(int(rt / p), int(gt / p), int(bt / p));
				result.setPixel(x, y, color.rgb());
			}
		}
		return result;
	}
}
