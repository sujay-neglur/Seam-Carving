#include <opencv2/opencv.hpp>
#include "sc.h"

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    if(argc!=5){
        cout<<"Usage: ../sc input_image new_width new_height output_image"<<endl;
        return -1;
    }

    // Load the input image
    // the image should be a 3 channel image by default but we will double check that in teh seam_carving
    Mat in_image;
    in_image = imread(argv[1]/*, CV_LOAD_IMAGE_COLOR*/);

    if(!in_image.data)
    {
        cout<<"Could not load input image!!!"<<endl;
        return -1;
    }

    if(in_image.channels()!=3){
        cout<<"Image does not have 3 channels!!! "<<in_image.depth()<<endl;
        return -1;
    }

    // get the new dimensions from the argument list
    int newRows = atoi(argv[3]);
    int newCols = atoi(argv[2]);

    if (newCols > in_image.cols) {
        cout << "Invalid request!!! new_width has to be smaller than the current size!" << endl;
        return -1;
    }

    if (newRows > in_image.rows) {
        cout << "Invalid request!!! ne_height has to be smaller than the current size!" << endl;
        return -1;
    }

    if (newCols <= 0) {
        cout << "Invalid request!!! new_width has to be positive!" << endl;
        return -1;
    }

    if (newRows <= 0) {
        cout << "Invalid request!!! new_height has to be positive!" << endl;
        return -1;
    }
    // the output image
    Mat out_image=seamCarving(in_image,newRows,newCols);
    // write it on disk
    imwrite( argv[4], out_image);

    // also display them both

    namedWindow( "Original image", WINDOW_AUTOSIZE );
    namedWindow( "Seam Carved Image", WINDOW_AUTOSIZE );
    imshow( "Original image", in_image );
    imshow( "Seam Carved Image", out_image );
    waitKey(0);
    return 0;
}
