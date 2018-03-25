#ifndef SEAMCARVINGCOMP665156
#define SEAMCARVINGCOMP665156

#include <vector>
#include <opencv2/opencv.hpp>
using namespace std;

// the function you need to implement - by defaiult it calls seam_carving_trivial
cv::Mat seamCarving(cv::Mat &in_image, int newRows, int newCols);

cv::Mat calculateEnergy(cv::Mat &image);
cv::Mat removeVerticalSeam(vector<uint> &verticalSeam, cv::Mat &in_image_clone, cv::Mat &temp_image);

vector<int> findVerticalSeam(cv::Mat &image);
#endif