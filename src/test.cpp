//
//  test.cpp
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
    {
        return 2.65<width/height&&width/height<2.85;
    }
    else 
    {
        return 2.65<height/width&&height/width<2.85;
    }
}

int main()
{
    originalImg = imread("../resources/example.jpg");
    if(originalImg.empty())
    {
        cout<<"Failed to load image!"<<endl;
        return -1;
    }
    // imshow("Image", originalImg);
    // waitKey(0);
    GaussianBlur(originalImg, guassianImg, Size(5,5), 1.5);
    // imshow("GuassianBlur", guassianImg);
    // waitKey(0);
    cvtColor(guassianImg, grayImg, COLOR_BGR2GRAY);
    // imshow("Gray", grayImg);
    // waitKey(0);
    // blur(originalImg, blurImg, Size(5,5));
    // imshow("Blur", blurImg);
    // waitKey(0);
    // medianBlur(originalImg, medianImg, 5);
    // imshow("MedianBlur", medianImg);
    // waitKey(0);
    threshold(grayImg, binaryImg, 192, 255, THRESH_BINARY);
    // imshow("Binary", binaryImg);
    // waitKey(0);
    morphologyEx(binaryImg, morphImg, MORPH_OPEN, getStructuringElement(MORPH_RECT, Size(10, 10)));
    // imshow("Morph1", morphImg);
    // waitKey(0);
    binaryImg = morphImg.clone();
    morphologyEx(binaryImg, morphImg, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(10, 10)));
    // imshow("Morph2", morphImg);
    // waitKey(0);
    // return 0;
    // adaptiveThreshold(grayImg, adaptiveImg, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 11, 2);
    // imshow("Adaptive", adaptiveImg);
    // waitKey(0);
    Canny(morphImg,edgeImg, 100, 200);
    // imshow("Canny", edgeImg);
    // waitKey(0);
    findContours(edgeImg, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    // rectImg = Mat::zeros(originalImg.size(), CV_8UC3);
    rectImg=originalImg.clone();
    destroyAllWindows();
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
        // imshow("Bounding Box", rectImg);
        // waitKey(0);
        // rectangle(rectImg, rect, Scalar(0, 0, 255), 2);
        // drawContours(contourImg, contours, (int)i, Scalar(0, 255, 0), 2);
        // cout<<i<<endl;
        // imshow("Contours", contourImg);
        // waitKey(0);
        // destroyWindow("Contours");
    }
    imshow("Bounding Box", rectImg);
    waitKey(0);
    imwrite("../resources/result.jpg", rectImg);
    return 0;
}