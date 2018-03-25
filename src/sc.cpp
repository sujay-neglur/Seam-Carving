
#include "sc.h"

using namespace cv;
using namespace std;

Mat calculateEnergy(Mat &image) {
    Mat energy(image.rows, image.cols, CV_8UC1);
    Mat grdX, grdY;
    Sobel(image, grdX, CV_64F, 1, 0, 1);
    convertScaleAbs(grdX, grdX);
    Sobel(image, grdY, CV_64F, 0, 1, 1);
    convertScaleAbs(grdY, grdY);
    addWeighted(grdX, 0.5, grdY, 0.5, 0, energy);
    return energy;

}

vector<int> findVerticalSeam(Mat &image) {
    int rows = image.rows;
    int cols = image.cols;
    int i, j, pos;

    vector <vector<int> > cumulativeEnergy(rows, vector<int>(cols));

    for (int j = 0; j < cols; j++) {
        cumulativeEnergy[0][j] = (int) image.at<uchar>(0, j);
    }

    for (int i = 1; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (j == 0) {
                cumulativeEnergy[i][j] = min(cumulativeEnergy[i - 1][j + 1],
                                             cumulativeEnergy[i - 1][j]);
            } else if (j == cols - 1) {
                cumulativeEnergy[i][j] = min(cumulativeEnergy[i - 1][j - 1],
                                             cumulativeEnergy[i - 1][j]);
            } else
                cumulativeEnergy[i][j] = min(
                        min(cumulativeEnergy[i - 1][j - 1], cumulativeEnergy[i - 1][j]),
                        cumulativeEnergy[i - 1][j + 1]);
            cumulativeEnergy[i][j] += (int) image.at<uchar>(i, j);
        }
    }

    int minimum = cumulativeEnergy[rows - 1][0];
    pos = 0;

    for (i = rows - 1, j = 0; j < cols; j++) {
        if (cumulativeEnergy[i][j] < minimum) {
            minimum = cumulativeEnergy[i][j];
            pos = j;
        }
    }
    vector<int> seam(image.rows);
    int least;
    i = rows - 1;
    j = pos;
    seam[i] = j;
    for (i = rows - 1; i >= 1; i--) {
        least = cumulativeEnergy[i][j] - (int) image.at<uchar>(i, j);
        if (j == 0) {
            if (least = cumulativeEnergy[i - 1][j + 1]) {
                seam[i] = j + 1;
                j++;
            } else {
                seam[i] = j;
            }
        } else {
            if (j == cols - 1) {
                if (least == cumulativeEnergy[i - 1][j - 1]) {
                    seam[i] = j - 1;
                    j--;
                } else {
                    seam[i] = j;
                }
            } else {
                if (least == cumulativeEnergy[i - 1][j - 1]) {
                    seam[i] = j - 1;
                    j--;
                } else if (least == cumulativeEnergy[i - 1][j + 1]) {
                    seam[i] = j + 1;
                    j++;
                } else {
                    seam[i] = j;
                }
            }
        }
    }
    return seam;
}

Mat removeVerticalSeam(vector<int> &verticalSeam, Mat &in_image_clone, Mat &temp_image) {
    int i, j;
    int rows = in_image_clone.rows;
    int cols = in_image_clone.cols - 1;//1000
    Mat seamRemovedImage(rows, cols, CV_8UC3);
    for (i = 0; i < rows; i++) {
        int k = 0;
        for (j = 0; j <= cols; j++) {
            if (j == verticalSeam[i]) {
                continue;
            } else {
                seamRemovedImage.at<Vec3b>(i, k) = in_image_clone.at<Vec3b>(i, j);
                k++;
            }
        }

    }
    return seamRemovedImage;
}

Mat seamCarving(Mat &in_image, int newRows, int newCols) {

    char *windowName = "Scharr edge";
    int scale = 1;
    int delta = 0;
    int depth = CV_8UC3;
    int i, j;
    Mat grdX, grdY, out_image, temp_image, ii_image, image_transpose;
    ii_image = in_image.clone();
    temp_image = ii_image.clone();
    while (ii_image.rows != newRows || ii_image.cols != newCols) {
        if (ii_image.cols > newCols) {
            cv::cvtColor(ii_image, temp_image, CV_BGR2GRAY);
            temp_image = calculateEnergy(temp_image);
            vector<int> verticalseam;
            verticalseam = findVerticalSeam(temp_image);
            out_image = removeVerticalSeam(verticalseam, ii_image, temp_image);
            ii_image = out_image.clone();
        }
        if (ii_image.rows > newRows) {
            transpose(ii_image,image_transpose);
            cv::cvtColor(image_transpose, temp_image, CV_BGR2GRAY);
            temp_image = calculateEnergy(temp_image);
            vector<int> verticalseam;
            verticalseam = findVerticalSeam(temp_image);
            out_image = removeVerticalSeam(verticalseam, image_transpose, temp_image);
            image_transpose = out_image.clone();
            transpose(image_transpose,ii_image);
        }
    }

    return ii_image;

}
