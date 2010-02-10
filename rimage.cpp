#include "rimage.h"
#include <math.h>
#include "Debugger.h"
#include <sstream>

RImage::RImage() : QImage()
{
}

RImage::RImage(QString& filename) : QImage(filename) {}

void RImage::equalize() {
    vector<int> table;
    equalize(table);
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
