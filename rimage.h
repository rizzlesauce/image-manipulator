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

    void equalize();
    void equalize(vector<int> &table);
    void averageWithImages(vector<RImage> &images);
    void weightedFilter(vector<vector<int> > &weights);
    void medianFilter(int neighborhoodSize);

    vector<int>& getHistogram(vector<int>& hist);
};

#endif // RIMAGE_H
