#include "rimage.h"
//#include <math.h>
#include <cmath>
#include "Debugger.h"
#include <sstream>
#include <sstream>
#include <vector>
#include <list>
using namespace std;

RImage::RImage() : QImage()
{
}

RImage::RImage(const QString& filename) : QImage(filename) {}

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

/**
 * Filters must be of odd length or undefined behavior will result.
 */
void RImage::weightedFilter(vector<vector<int> > &weights) {

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

			int sumOfWeights = 0;
			int sumOfValues = 0;
			for (int xx = x1; xx < x2; ++xx) {
				for (int yy = y1; yy < y2; ++yy) {
					int filterX = xx - x1;
					int filterY = yy - y1;

					sumOfWeights += weights[filterX][filterY];
					int value = original.pixelIndex(xx, yy) * weights[filterX][filterY];
					sumOfValues += value;
				}
			}

			int value = floor((sumOfValues / sumOfWeights) + 0.5);

			setPixel(x, y, value);
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
			for (uint i = 0; i < values.size() / 2; ++it, ++i);

			setPixel(x, y, *it);
		}
	}

}
