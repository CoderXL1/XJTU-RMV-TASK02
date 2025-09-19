//
//  main.cpp
//  Created by Leo Xia
//  Created with VSCode on Ubuntu 22.04
//

/* 最简单的 main.cpp 示例 */
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

Mat originalImg, grayImg, blurImg, guassianImg, medianImg, binaryImg, adaptiveImg;
Mat edgeImg, morphImg, contourImg, rectImg;
vector<vector<Point>> contours;
vector<Vec4i> hierarchy;

int main()
{
    originalImg = imread("example.jpg");
    if(originalImg.empty())
    {
        std::cout<<"Failed to load image!"<<std::endl;
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
    imshow("Canny", edgeImg);
    waitKey(0);
    findContours(edgeImg, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    rectImg = Mat::zeros(originalImg.size(), CV_8UC3);
    destroyAllWindows();
    for(size_t i = 0; i < contours.size(); i++)
    {
        if(contourArea(contours[i]) < 1000) continue;
        RotatedRect rect = minAreaRect(contours[i]);
        Point2f rect_points[4];
        rect.points(rect_points);
        for ( int j = 0; j < 4; j++ )
        {
            line( rectImg, rect_points[j], rect_points[(j+1)%4], Scalar(0, 0, 255));
        }
        cout<<rect.angle<<" ";
        imshow("Bounding Box", rectImg);
        waitKey(0);
        // rectangle(rectImg, rect, Scalar(0, 0, 255), 2);
        // drawContours(contourImg, contours, (int)i, Scalar(0, 255, 0), 2);
        // cout<<i<<endl;
        // imshow("Contours", contourImg);
        // waitKey(0);
        // destroyWindow("Contours");
    }
    imshow("Bounding Box", rectImg);
    waitKey(0);
    return 0;
}