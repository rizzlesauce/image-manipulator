#ifndef RIMAGE_H
#define RIMAGE_H

#include <QImage>
#include <vector>

using namespace std;

class RImage : public QImage
{

public:
    RImage();
    RImage(const QString&);
    RImage(int width, int height, Format format);

    void setGrayColorTable();
    void equalize();
    void equalize(vector<int> &table);
    void averageWithImages(vector<RImage> &images);
    void weightedFilter(vector<vector<int> > &weights);
    void spatialFilter(vector<vector<int> > &weights, vector<vector<int> > &sumsOfValues);
    void spatialFilterClip(vector<vector<int> > &weights, int divisor = 1);
    void spatialFilterScale(vector<vector<int> > &weights, int divisor = 1);
    void sobelX();
    void sobelY();
    void laplacian();
    void medianFilter(int neighborhoodSize);
    void unsharpMask(int neighborhoodSize, int sharpeningLevel);
    void gradientMagnitude();
    void sobelXWeights(vector<vector<int> > &weights);
    void sobelYWeights(vector<vector<int> > &weights);
    void predictiveEncode();
    void predictiveDecode();
    RImage magnify(double factor);
    RImage shrink(double factor);
    RImage rotate(double degrees);
    static double rotateX(double x, double y, double xc, double yc, double radians);
    static double rotateY(double x, double y, double xc, double yc, double radians);
    double bilinearInterpolateValue(double x, double y);
    static int getPredictedValue(vector<vector<int> > &valuesSeenSoFar, int w, int h, int x, int y);
    static int getCorrection(int, int);
    static int correctPredicted(int, int);

    vector<int>& getHistogram(vector<int>& hist);
};

#endif // RIMAGE_H
