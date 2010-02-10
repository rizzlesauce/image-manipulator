#ifndef RIMAGE_H
#define RIMAGE_H

#include <QImage>
#include <vector>

using namespace std;

class RImage : public QImage
{

public:
    RImage();
    RImage(QString&);

    void equalize();
    void equalize(vector<int> &table);

    vector<int>& getHistogram(vector<int>& hist);
};

#endif // RIMAGE_H
