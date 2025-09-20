//
//  recognition.cpp
//  Created by Leo Xia
//  Created with VSCode on Ubuntu 22.04
//

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

Mat originalImg, grayImg, blurImg, guassianImg, medianImg, binaryImg, adaptiveImg;
Mat edgeImg, morphImg, rectImg;
vector<vector<Point>> contours;
vector<Vec4i> hierarchy;

bool angular_vaidation(RotatedRect&rect)
{
    double angle, height, width;
    angle=rect.angle;
    height=rect.size.height;
    width=rect.size.width;
    bool ind=false;
    while(angle>45.0)
    {
        angle-=90.0;
        ind^=1;
    }
    if(!(angle<=5.0&&angle>=-5.0)) return false;
    if(ind)
        return 2.65<width/height&&width/height<2.85;
    else 
        return 2.65<height/width&&height/width<2.85;
}

int main()
{
    originalImg = imread("../resources/test_image_2.jpg");
    if(originalImg.empty())
    {
        cout<<"Failed to load image!"<<endl;
        return -1;
    }
    GaussianBlur(originalImg, guassianImg, Size(5,5), 1.5);
    cvtColor(guassianImg, grayImg, COLOR_BGR2GRAY);
    threshold(grayImg, binaryImg, 192, 255, THRESH_BINARY);
    morphologyEx(binaryImg, morphImg, MORPH_OPEN, getStructuringElement(MORPH_RECT, Size(10, 10)));
    binaryImg = morphImg.clone();
    morphologyEx(binaryImg, morphImg, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(10, 10)));
    Canny(morphImg,edgeImg, 100, 200);
    findContours(edgeImg, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    rectImg=originalImg.clone();
    for(size_t i = 0; i < contours.size(); i++)
    {
        if(contourArea(contours[i]) < 1000) continue;
        RotatedRect rect = minAreaRect(contours[i]);
        cout<<rect.angle<<" "<<rect.size.height<<" "<<rect.size.width;
        if(angular_vaidation(rect))
        {
            cout<<" ok"<<endl;
            Point2f rect_points[4];
            rect.points(rect_points);
            for ( int j = 0; j < 4; j++ )
            {
                line( rectImg, rect_points[j], rect_points[(j+1)%4], Scalar(0, 0, 255),4);
            }
        }
        else cout<<" bad"<<endl;
    }
    imshow("Bounding Box", rectImg);
    waitKey(0);
    imwrite("../resources/test_image_2.result.jpg", rectImg);
    return 0;
}