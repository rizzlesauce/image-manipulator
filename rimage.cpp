#include "rimage.h"
//#include <math.h>
#include <cmath>
#include "Debugger.h"
#include <sstream>
#include <sstream>
#include <vector>
#include <list>
#include <cmath>
#include <QVector>
#include <QRgb>
#include <QColor>
#include "SampledTone.h"
using namespace std;

RImage::RImage() : QImage()
{
}

RImage::RImage(const QString& filename) : QImage(filename) {}

RImage::RImage(int width, int height, Format format) : QImage(width, height, format) {}

void RImage::setGrayColorTable() {
    QVector<QRgb> table(256);

    for (int i = 0; i < table.size(); ++i) {
        table[i] = qRgb(i, i, i);
    }

    setColorTable(table);
}

void RImage::equalize() {
    vector<int> table;
    equalize(table);
}

void RImage::averageWithImages(vector<RImage> &images) {
    int value;

    for (int x = 0; x < width(); ++x) {
        for (int y = 0; y < height(); ++y) {
            value = pixelIndex(x, y);
            for (uint i = 0; i < images.size(); ++i) {
                value += images[i].pixelIndex(x, y);
            }

            value = floor(value / (images.size() + 1) + 0.5);

            /*
            stringstream ss;
            ss << "value: " << value;
            Debugger::getInstance().print(ss.str());
            */

            setPixel(x, y, value);
        }
    }
}

void RImage::unsharpMask(int neighborhoodSize, int sharpeningLevel) {

    //int negativeA = sharpeningLevel * -1;
    vector<vector<int> > mask(neighborhoodSize, vector<int>(neighborhoodSize, -1));//negativeA));

    int midIndex = neighborhoodSize / 2;

    mask[midIndex][midIndex] = sharpeningLevel + (neighborhoodSize * 2) - 1;//sharpeningLevel * neighborhoodSize;
    //mask[midIndex][midIndex] = neighborhoodSize * 2 - 1;

    weightedFilter(mask);
}

/**
 * Filters must be of odd length or undefined behavior will result.
 */
void RImage::weightedFilter(vector<vector<int> > &weights) {

    int sumOfWeights = 0;
    for (uint x = 0; x < weights.size(); ++x) {
        for (uint y = 0; y < weights.size(); ++y) {
            sumOfWeights += weights[x][y];
        }
    }

    spatialFilterClip(weights, sumOfWeights);
}

void RImage::sobelX() {
    vector<vector<int> > weights;
    sobelXWeights(weights);
    spatialFilterClip(weights, 8);
}

void RImage::sobelY() {
    vector<vector<int> > weights;
    sobelYWeights(weights);
    spatialFilterClip(weights, 8);
}

void RImage::laplacian() {
    vector<vector<int> > weights(3, vector<int>(3, 0));

    weights[1][0] = 1;
    weights[0][1] = 1;
    weights[1][1] = -4;
    weights[2][1] = 1;
    weights[1][2] = 1;

    spatialFilterScale(weights);
}

/**
 * Filters must be of odd length or undefined behavior will result.
 */
void RImage::spatialFilterClip(vector<vector<int> > &weights, int divisor) {

    vector<vector<int> > sumsOfValues(width(), vector<int>(height(), 0));
    spatialFilter(weights, sumsOfValues);

    for (int x = 0; x < width(); ++x) {
        for (int y = 0; y < height(); ++y) {
            int value = sumsOfValues[x][y] / divisor;

            if (value < 0) {
                value = 0;
            } else if (value > 255) {
                value = 255;
            }

            setPixel(x, y, value);
        }
    }
}

/**
 * Filters must be of odd length or undefined behavior will result.
 */
void RImage::spatialFilterScale(vector<vector<int> > &weights, int divisor) {

    /*
    int filterSize = weights.size();

    int highestValue = 255;

    // Get scaling info from weights and divisor
    int positiveSum = 0;
    int negativeSum = 0;
    for (int x = 0; x < filterSize; ++x) {
        for (int y = 0; y < filterSize; ++y) {
            int weight = weights[x][y];
            if (weight > 0) {
                positiveSum += weight;
            } else {
                negativeSum += weight;
            }
        }
    }
    int highest = positiveSum * highestValue / divisor;
    int lowest = negativeSum * highestValue / divisor;
    int range = highest - lowest;

    vector<vector<int> > sumsOfValues(width(), vector<int>(height(), 0));
    spatialFilter(weights, sumsOfValues);

    for (int x = 0; x < width(); ++x) {
        for (int y = 0; y < height(); ++y) {
            int value = (((sumsOfValues[x][y] / divisor) - lowest) / range) * highestValue;

            setPixel(x, y, value);
        }
    }
    */

    vector<vector<int> > sumsOfValues(width(), vector<int>(height(), 0));
    spatialFilter(weights, sumsOfValues);

    int highestValue = 255;
    int highest = -50000000;
    int lowest = 500000000;

    for (int x = 0; x < width(); ++x) {
        for (int y = 0; y < height(); ++y) {
            int value = sumsOfValues[x][y] / divisor;
            if (value > highest) {
                highest = value;
            }
            if (value < lowest) {
                lowest = value;
            }
        }
    }
    int range = highest - lowest;

    for (int x = 0; x < width(); ++x) {
        for (int y = 0; y < height(); ++y) {
            int value = (((sumsOfValues[x][y] / divisor) - lowest) / range) * highestValue;
            setPixel(x, y, value);
        }
    }
}

/**
 * Filters must be of odd length or undefined behavior will result.
 */
void RImage::spatialFilter(vector<vector<int> > &weights, vector<vector<int> > &sumsOfValues) {

    RImage original(*this);

    int filterSize = weights.size();

    int sidesLength = filterSize / 2;
    for (int x = 0; x < width(); ++x) {
        for (int y = 0; y < height(); ++y) {
            // walk around the neighborhood
            int x1 = x - sidesLength;
            int x2 = x + sidesLength;
            int y1 = y - sidesLength;
            int y2 = y + sidesLength;

            int sumOfValues = 0;
            for (int xx = x1; xx <= x2; ++xx) {
                int imgX = xx;

                if (imgX < 0) {
                    imgX = 0;
                } else if (imgX >= width()) {
                    imgX = width() - 1;
                }

                for (int yy = y1; yy <= y2; ++yy) {
                    int filterX = xx - x1;
                    int filterY = yy - y1;

                    int imgY = yy;

                    if (imgY < 0) {
                        imgY = 0;
                    } else if (imgY >= height()) {
                        imgY = height() - 1;
                    }

                    int value = original.pixelIndex(imgX, imgY) * weights[filterX][filterY];
                    sumOfValues += value;
                }
            }

            sumsOfValues[x][y] = sumOfValues;
        }
    }
}


void RImage::equalize(vector<int> &table) {
    // equalize the histogram of the image

    // create the conversion table

    vector<int> hist;
    getHistogram(hist);

    int numValues = hist.size();
    int area = width() * height();

    table.resize(numValues, 0);

    int count = 0;

    for (int i = 0; i < numValues; ++i) {
        count += hist[i];

        table[i] = floor(((float)count / area) * (numValues - 1));
        /*
        stringstream ss;
        ss << "value at " << i << ": " << table[i] << endl;
        ss << "count: " << count << endl;
        ss << "hist[i]: " << hist[i] << endl;
        ss << "count/area: " << ((float)count / area) << endl;
        ss << endl;

        Debugger::getInstance().print(ss.str());
        */
    }
    //this->fromData(

    // convert the values
    for (int x = 0; x < width(); ++x) {
        for (int y = 0; y < height(); ++y) {
            setPixel(x, y, table[pixelIndex(x, y)]);
        }
    }
}

vector<int>& RImage::getHistogram(vector<int>& hist) {
    int numValues = numColors();
    /*
    if (image->numColors() != 0 && image->numColors() != INT_MAX) {
        numValues = image->numColors();
    }
    */

    hist.resize(numValues, 0);

    for (int x = 0; x < width(); ++x)
    {
        for (int y = 0; y < height(); ++y)
        {
            hist[pixelIndex(x, y)]++;
        }
    }

    return hist;
}

void RImage::medianFilter(int neighborhoodSize) {
    RImage original(*this);

    int sidesLength = neighborhoodSize / 2;

    for (int x = 0; x < width(); ++x) {
        for (int y = 0; y < height(); ++y) {
            // walk around the neighborhood
            int x1 = x - sidesLength;
            int x2 = x + sidesLength;
            int y1 = y - sidesLength;
            int y2 = y + sidesLength;

            if (x1 < 0) {
                x1 = 0;
            }
            if (x2 > width()) {
                x2 = width();
            }
            if (y1 < 0) {
                y1 = 0;
            }
            if (y2 > height()) {
                y2 = height();
            }

            list<int> values;
            for (int xx = x1; xx < x2; ++xx) {
                for (int yy = y1; yy < y2; ++yy) {
                    values.push_back(original.pixelIndex(xx, yy));
                }
            }
            values.sort();
            list<int>::iterator it = values.begin();
            for (uint i = 0; i < values.size() / 2; ++it, ++i) {
                setPixel(x, y, *it);
            }
        }
    }

}

void RImage::sobelXWeights(vector<vector<int> > &weights) {
    weights.resize(3, vector<int>(3, 0));

    weights[0][0] = -1;
    weights[0][1] = -2;
    weights[0][2] = -1;
    weights[2][0] = 1;
    weights[2][1] = 2;
    weights[2][2] = 1;
}

void RImage::sobelYWeights(vector<vector<int> > &weights) {
    weights.resize(3, vector<int>(3, 0));

    weights[0][0] = -1;
    weights[1][0] = -2;
    weights[2][0] = -1;
    weights[0][2] = 1;
    weights[1][2] = 2;
    weights[2][2] = 1;
}

void RImage::gradientMagnitude() {
    vector<vector<int> > xChanges(width(), vector<int>(height()));
    vector<vector<int> > yChanges(width(), vector<int>(height()));

    vector<vector<int> > xWeights;
    vector<vector<int> > yWeights;
    sobelXWeights(xWeights);
    sobelYWeights(yWeights);

    spatialFilter(xWeights, xChanges);
    spatialFilter(yWeights, yChanges);

    vector<vector<double> > squareRoots(width(), vector<double>(height()));

    double lowest = 5000000;
    double highest = 0;
    for (int x = 0; x < width(); ++x) {
        for (int y = 0; y < height(); ++y) {
            double sqroot = sqrt(pow((float)xChanges[x][y], 2) + pow((float)yChanges[x][y], 2));
            squareRoots[x][y] = sqroot;

            if (sqroot < lowest) {
                lowest = sqroot;
            }
            if (sqroot > highest) {
                highest = sqroot;
            }
        }
    }
    double range = highest - lowest;
    int highestValue = 255;

    for (int x = 0; x < width(); ++x) {
        for (int y = 0; y < height(); ++y) {
            int value = ((squareRoots[x][y] - lowest) / range) * highestValue;

            setPixel(x, y, value);
        }
    }
}

void RImage::predictiveEncode() {
    //RImage encodedImage(width(), height(), QImage::Format_Indexed8);
    //encodedImage.setGrayColorTable();

    vector<vector<int> > valuesSeenSoFar(width(), vector<int>(height(), 0));

    int predictedValue;
    int actualValue;
    int correction;

    // for each pixel...
    for (int y = 0; y < height(); ++y) {
        for (int x = 0; x < width(); ++x) {

            predictedValue = RImage::getPredictedValue(valuesSeenSoFar, width(), height(), x, y);

            actualValue = pixelIndex(x, y);

            // get the correction value
            correction = RImage::getCorrection(predictedValue, actualValue);

            // update values seen so far
            valuesSeenSoFar[x][y] = actualValue;

            // encode the value in this image
            setPixel(x, y, correction);

        }
    }
}

void RImage::predictiveDecode() {

    vector<vector<int> > valuesSeenSoFar(width(), vector<int>(height(), 0));

    int predictedValue;
    int actualValue;
    int correction;

    // for each pixel...
    for (int y = 0; y < height(); ++y) {
        for (int x = 0; x < width(); ++x) {

            predictedValue = RImage::getPredictedValue(valuesSeenSoFar, width(), height(), x, y);

            correction = pixelIndex(x, y);

            actualValue = RImage::correctPredicted(predictedValue, correction);

            // update values seen so far
            valuesSeenSoFar[x][y] = actualValue;

            // encode the value in this image
            setPixel(x, y, actualValue);
        }
    }
}




int RImage::getCorrection(int predictedValue, int trueValue) {
    int numLevels = 256;

    int correction = predictedValue - trueValue;
    if (correction < 0) {
        correction += numLevels;
    }

    return correction;
}

int RImage::correctPredicted(int predictedValue, int correction) {
    int numLevels = 256;

    int trueValue = predictedValue - correction;
    if (trueValue < 0) {
        trueValue += numLevels;
    }

    return trueValue;
}

RImage RImage::magnify(double factor) {
    int newWidth = width() * factor;
    int newHeight = height() * factor;

    double backX, backY, value;

    RImage newImage(newWidth, newHeight, QImage::Format_Indexed8);
    newImage.setGrayColorTable();

    for (int x = 0; x < newWidth; ++x) {
        for (int y = 0; y < newHeight; ++y) {
            // get the backward mapped point
            backX = (double)x / factor;
            backY = (double)y / factor;
            value = bilinearInterpolateValue(backX, backY);
            newImage.setPixel(x, y, (int)value);
        }
    }

    return newImage;
}

RImage RImage::shrink(double factor) {
    int newWidth = floor(((double)width() / factor) + 0.5);
    int newHeight = floor(((double)height() / factor) + 0.5);

    double backX, backY, value;

    RImage newImage(newWidth, newHeight, QImage::Format_Indexed8);
    newImage.setGrayColorTable();

    for (int x = 0; x < newWidth; ++x) {
        for (int y = 0; y < newHeight; ++y) {
            // get the backward mapped point
            backX = (double)x * factor;
            backY = (double)y * factor;

            if (backX >= width()) {
                backX = width() - 1;
            }
            if (backY >= height()) {
                backY = height() - 1;
            }

            value = (double)pixelIndex(backX, backY);
            newImage.setPixel(x, y, (int)value);
        }
    }

    return newImage;
}

double RImage::bilinearInterpolateValue(double x, double y) {
    // find the square this fits inside
    int x0 = floor(x);
    int y0 = floor(y);
    int x1 = x0 + 1;
    int y1 = y0 + 1;
    if (x1 >= width()) {
        x1 = width() - 1;
    }
    if (y1 >= height()) {
        y1 = height() - 1;
    }

    double value;

    x = x - (double)x0;
    y = y - (double)y0;

    double v1, v2, v3, v4;

    v1 = (double)pixelIndex(x0, y0);
    v2 = (double)pixelIndex(x1, y0);
    v3 = (double)pixelIndex(x0, y1);
    v4 = (double)pixelIndex(x1, y1);

    value = (v2 - v1) * x + (v3 - v1) * y + (v1 - v2 - v3 +v4) * x * y + v1;

    return value;
}

int RImage::getPredictedValue(vector<vector<int> > &valuesSeenSoFar, int w, int h, int x, int y) {

    //int w = valuesSeenSoFar.size();
    //int h = valuesSeenSoFar[0].size();

    int numLevels = 256;

    int neighborX[] = { -1, -1, 0, 1 };
    int neighborY[] = { 0, -1, -1, -1 };
    int neighborhoodSize = 4;

    int tmpX, tmpY;
    int averageValue;
    int predictedValue;

    vector<int> neighborValues;

    // get the neighboring pixels
    for (int i = 0; i < neighborhoodSize; ++i) {

        tmpX = x + neighborX[i];
        tmpY = y + neighborY[i];

        if (tmpX < 0 || tmpX >= w) {
                // out of range, don't use it
        } else if (tmpY < 0 || tmpY >= h) {
                // out of range, don't use it
        } else {
            neighborValues.push_back(valuesSeenSoFar[tmpX][tmpY]);
        }
    }

    // predict the value

    // get the average value from neighbors
    averageValue = 0;
    for (unsigned int i = 0; i < neighborValues.size(); ++i) {
        averageValue += neighborValues[i];
    }

    if (neighborValues.size() == 0) {
        // no neighbors, so use half the value range
        predictedValue = (numLevels - 1) / 2;
    } else {
        predictedValue = averageValue / neighborValues.size();
    }

    return predictedValue;
}

double RImage::rotateX(double x, double y, double xc, double yc, double radians) {
    return ((double)x - xc) * cos(radians) - ((double)y - yc) * sin(radians) + xc;
}
double RImage::rotateY(double x, double y, double xc, double yc, double radians) {
    return ((double)x - xc) * sin(radians) + ((double)y - yc) * cos(radians) + yc;
}

RImage RImage::rotate(double degrees) {
    double t = (degrees / 180.0) * (SAMPLEDTONE_PI);
    double radians = t * -1.0;

    // get the new dimensions
    double xs[4];
    double ys[4];
    double xc = (double)width() / 2.0;
    double yc = (double)height() / 2.0;

    xs[0] = RImage::rotateX(0, 0, xc, yc, radians);
    ys[0] = RImage::rotateY(0, 0, xc, yc, radians);
    xs[1] = RImage::rotateX(0, height() - 1, xc, yc, radians);
    ys[1] = RImage::rotateY(0, height() - 1, xc, yc, radians);
    xs[2] = RImage::rotateX(width() - 1, height() - 1, xc, yc, radians);
    ys[2] = RImage::rotateY(width() - 1, height() - 1, xc, yc, radians);
    xs[3] = RImage::rotateX(width() - 1, 0, xc, yc, radians);
    ys[3] = RImage::rotateY(width() - 1, 0, xc, yc, radians);

    // get the mins and maxes
    bool first = true;
    double minX, maxX, minY, maxY;
    for (int i = 0; i < 4; ++i) {
        if (first) {
            minX = xs[i];
            maxX = xs[i];
            minY = ys[i];
            maxY = ys[i];
            first = false;
        } else {
            if (xs[i] < minX) {
                minX = xs[i];
            }
            if (xs[i] > maxX) {
                maxX = xs[i];
            }
            if (ys[i] < minY) {
                minY = ys[i];
            }
            if (ys[i] > maxY) {
                maxY = ys[i];
            }
        }
    }

    RImage newImage(floor(maxX - minX + 0.5), floor(maxY - minY + 0.5), QImage::Format_Indexed8);
    newImage.setGrayColorTable();

    double backX, backY, value;
    double xc2 = (double)newImage.width() / 2.0;
    double yc2 = (double)newImage.height() / 2.0;

    double xcDiff = xc2 - xc;
    double ycDiff = yc2 - yc;

    for (int x = 0; x < newImage.width(); ++x) {
        for (int y = 0; y < newImage.height(); ++y) {
            // get the backward mapped point
            backX = RImage::rotateX(x, y, xc2, yc2, t) - xcDiff;
            backY = RImage::rotateY(x, y, xc2, yc2, t) - ycDiff;

            if (backX < 0 || backX >= width() || backY < 0 || backY >= height()) {
                    // don't know where it came from
                value = 0.0;
            } else {
                value = bilinearInterpolateValue(backX, backY);
            }
            newImage.setPixel(x, y, (int)value);
        }
    }

    int top, bottom, left, right;
    // find the top
    bool found = false;
    for (int y = 0; y < newImage.height() && !found; ++y) {
        for (int x = 0; x < newImage.width() && !found; ++x) {
            if (newImage.pixelIndex(x, y) != 0) {
                top = y;
                found = true;
            }
        }
    }

    // find the bottom
    found = false;
    for (int y = newImage.height() - 1; y >= 0 && !found; --y) {
        for (int x = newImage.width() - 1; x >= 0 && !found; --x) {
            if (newImage.pixelIndex(x, y) != 0) {
                bottom = y;
                found = true;
            }
        }
    }

    // find the left
    found = false;
    for (int x = 0; x < newImage.width() && !found; ++x) {
        for (int y = 0; y < newImage.height() && !found; ++y) {
            if (newImage.pixelIndex(x, y) != 0) {
                left = x;
                found = true;
            }
        }
    }

    // find the right
    found = false;
    for (int x = newImage.width() - 1; x >= 0 && !found; --x) {
        for (int y = newImage.height() - 1; y >= 0 && !found; --y) {
            if (newImage.pixelIndex(x, y) != 0) {
                right = x;
                found = true;
            }
        }
    }

    int pieceWidth = right - left + 1;
    int pieceHeight = bottom - top + 1;

    QImage cropped = newImage.copy(left, top, pieceWidth, pieceHeight);
    newImage = RImage(cropped.width(), cropped.height(), QImage::Format_Indexed8);
    newImage.setGrayColorTable();
    for (int x = 0; x < newImage.width(); ++x) {
        for (int y = 0; y < newImage.height(); ++y) {
            newImage.setPixel(x, y, cropped.pixelIndex(x, y));
        }
    }

    return newImage;
}
