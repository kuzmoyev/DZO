#include <QtCore/QString>
#include <QtGui/QBitmap>
#include <vector>
#include <cmath>
#include <QtGui/QImage>

using namespace std;

namespace blur {
    vector<vector<double>> getGaussianKernel(size_t size);


    QImage gaussBlur(const QImage& img, int kernel=15);
}


