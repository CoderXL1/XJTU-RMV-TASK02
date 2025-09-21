//
//  main.cpp
//  Created by Leo Xia
//  Created with VSCode on Ubuntu 22.04
//

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

Mat originalImg, grayImg, hsvImg, blurImg, guassianImg, binaryImg;
Mat edgeImg, morphImg, redParts, canvasImg, rotatedImg, croppedImg;
Mat lightParts, lightParts_gray, lightParts_binary, lightParts_dilate, lightParts_erode, lightParts_floodfill;

int sumArea;
vector<vector<Point>> contours;
vector<Vec4i> hierarchy;
vector<Rect> boundRect;

void convert()
{
    cvtColor(originalImg, grayImg, COLOR_BGR2GRAY);
    cvtColor(originalImg, hsvImg, COLOR_BGR2HSV);
}
void blur()
{
    blur(originalImg, blurImg, Size(5,5));
    GaussianBlur(originalImg, guassianImg, Size(5,5), 1.5);
}
void extract()
{
    Mat mask1,mask2;
    Mat kernal=getStructuringElement(MORPH_RECT, Size(5, 5));
    inRange(hsvImg, Scalar(0,0,0), Scalar(10,255,255), mask1);
    inRange(hsvImg, Scalar(110,0,0), Scalar(180,255,255), mask2);
    binaryImg = mask1 | mask2;
    morphologyEx(binaryImg, binaryImg, MORPH_OPEN, kernal);
    bitwise_and(originalImg, originalImg, redParts, binaryImg);
    // imshow("Red Parts", redParts);
    // waitKey(0);
    Canny(binaryImg, edgeImg, 100, 200);
    morphologyEx(edgeImg, morphImg, MORPH_CLOSE, kernal);
    findContours(morphImg, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    for(size_t i = 0; i < contours.size(); i++)
    {
        boundRect.push_back(boundingRect(contours[i]));
    }
    for(size_t i = 0; i < contours.size(); i++)
    {
        sumArea+=contourArea(contours[i]);
    }
    inRange(hsvImg, Scalar(0,0,180), Scalar(180,255,255), mask1);
    bitwise_and(originalImg, originalImg, lightParts, mask1);
    cvtColor(lightParts, lightParts_gray, COLOR_BGR2GRAY);
    threshold(lightParts_gray, lightParts_binary, 100, 255, THRESH_BINARY);
    dilate(lightParts_binary, lightParts_dilate, kernal);
    erode(lightParts_dilate, lightParts_erode, kernal);
    lightParts_floodfill=lightParts_erode.clone();
    cvtColor(lightParts_floodfill, lightParts_floodfill, COLOR_GRAY2BGR);
    floodFill(lightParts_floodfill, Point(0,0), Scalar(0,0,255));
    // imshow("Light Parts", lightParts);
    // waitKey(0);
    // imshow("Light Parts", lightParts_gray);
    // waitKey(0);
    // imshow("Light Parts", lightParts_binary);
    // waitKey(0);
    // imshow("Light Parts", lightParts_erode);
    // waitKey(0);
    // imshow("Light Parts", lightParts_floodfill);
    // waitKey(0);
    // imshow("Light Parts", lightParts);
    // waitKey(0);
    
}

void draw()
{
    canvasImg=originalImg.clone();
    rectangle(canvasImg, Rect(10,10,100,100), Scalar(0,255,0),2);
    circle(canvasImg, Point(200,200), 50, Scalar(255,0,0), -1);
    putText(canvasImg, "Test", Point(300,300), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255),2);
    // imshow("Canvas", canvasImg);
    // waitKey(0);
    for(size_t i = 0; i < contours.size(); i++)
    {
        drawContours(canvasImg, contours, (int)i, Scalar(255,0,0), 2);
        rectangle(canvasImg, boundingRect(contours[i]), Scalar(0,255,0),2);
    }
    // imshow("Canvas", canvasImg);
    // waitKey(0);
}
void transform()
{
    Mat M= getRotationMatrix2D(Point(originalImg.cols/2, originalImg.rows/2), 35, 1);
    warpAffine(originalImg, rotatedImg, M, originalImg.size());
    // imshow("Rotated", rotatedImg);
    // waitKey(0);
    croppedImg=originalImg(Rect(0,0,originalImg.cols/2, originalImg.rows/2));
    // imshow("Cropped", croppedImg);
    // waitKey(0);
    // Mat scaledImg;
    // resize(originalImg, scaledImg, Size(), 0.5, 0.5);
    // imshow("Scaled", scaledImg);
    // waitKey(0);
    // Mat translatedImg;
    // Mat T = (Mat_<double>(2,3) << 1, 0, 100, 0, 1, 50);
    // warpAffine(originalImg, translatedImg, T, originalImg.size());
    // imshow("Translated", translatedImg);
    // waitKey(0);
}
void saveImg()
{
    //save all Mat images to corresponding files, format: test_image.[name].jpg
    imwrite("../outputs/test_image.gray.jpg", grayImg);
    imwrite("../outputs/test_image.hsv.jpg", hsvImg);
    imwrite("../outputs/test_image.blur.jpg", blurImg);
    imwrite("../outputs/test_image.gaussian.jpg", guassianImg);
    imwrite("../outputs/test_image.binary.jpg", binaryImg);
    imwrite("../outputs/test_image.edge.jpg", edgeImg);
    imwrite("../outputs/test_image.morph.jpg", morphImg);
    imwrite("../outputs/test_image.redparts.jpg", redParts);
    imwrite("../outputs/test_image.canvas.jpg", canvasImg);
    imwrite("../outputs/test_image.rotated.jpg", rotatedImg);
    imwrite("../outputs/test_image.cropped.jpg", croppedImg);
    imwrite("../outputs/test_image.lightparts.jpg", lightParts);
    imwrite("../outputs/test_image.lightparts.gray.jpg", lightParts_gray);
    imwrite("../outputs/test_image.lightparts.binary.jpg", lightParts_binary);
    imwrite("../outputs/test_image.lightparts.dilate.jpg", lightParts_dilate);
    imwrite("../outputs/test_image.lightparts.erode.jpg", lightParts_erode);
    imwrite("../outputs/test_image.lightparts.floodfill.jpg", lightParts_floodfill);
    cout<<"Contour Area Sum: "<<sumArea<<endl;
}
int main()
{
    originalImg = imread("../resources/test_image.jpg");
    if(originalImg.empty())
    {
        cout<<"Failed to load image!"<<endl;
        return -1;
    }
    imshow("Original", originalImg);
    waitKey(0);
    convert();
    blur();
    extract();
    draw();
    transform();
    saveImg();
    // cvtColor(guassianImg, grayImg, COLOR_BGR2GRAY);
    // // imshow("Gray", grayImg);
    // // waitKey(0);
    // // blur(originalImg, blurImg, Size(5,5));
    // // imshow("Blur", blurImg);
    // // waitKey(0);
    // // medianBlur(originalImg, medianImg, 5);
    // // imshow("MedianBlur", medianImg);
    // // waitKey(0);
    // threshold(grayImg, binaryImg, 192, 255, THRESH_BINARY);
    // // imshow("Binary", binaryImg);
    // // waitKey(0);
    // morphologyEx(binaryImg, morphImg, MORPH_OPEN, getStructuringElement(MORPH_RECT, Size(10, 10)));
    // // imshow("Morph1", morphImg);
    // // waitKey(0);
    // binaryImg = morphImg.clone();
    // morphologyEx(binaryImg, morphImg, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(10, 10)));
    // // imshow("Morph2", morphImg);
    // // waitKey(0);
    // // return 0;
    // // adaptiveThreshold(grayImg, adaptiveImg, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 11, 2);
    // // imshow("Adaptive", adaptiveImg);
    // // waitKey(0);
    // Canny(morphImg,edgeImg, 100, 200);
    // // imshow("Canny", edgeImg);
    // // waitKey(0);
    // findContours(edgeImg, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    // // rectImg = Mat::zeros(originalImg.size(), CV_8UC3);
    // rectImg=originalImg.clone();
    // destroyAllWindows();
    // for(size_t i = 0; i < contours.size(); i++)
    // {
    //     if(contourArea(contours[i]) < 1000) continue;
    //     RotatedRect rect = minAreaRect(contours[i]);
    //     cout<<rect.angle<<" "<<rect.size.height<<" "<<rect.size.width;
    //     if(angular_vaidation(rect))
    //     {
    //         cout<<" ok"<<endl;
    //         Point2f rect_points[4];
    //         rect.points(rect_points);
    //         for ( int j = 0; j < 4; j++ )
    //         {
    //             line( rectImg, rect_points[j], rect_points[(j+1)%4], Scalar(0, 0, 255),4);
    //         }
    //     }
    //     else cout<<" bad"<<endl;
    //     // imshow("Bounding Box", rectImg);
    //     // waitKey(0);
    //     // rectangle(rectImg, rect, Scalar(0, 0, 255), 2);
    //     // drawContours(contourImg, contours, (int)i, Scalar(0, 255, 0), 2);
    //     // cout<<i<<endl;
    //     // imshow("Contours", contourImg);
    //     // waitKey(0);
    //     // destroyWindow("Contours");
    // }
    // imshow("Bounding Box", rectImg);
    // waitKey(0);
    // imwrite("../outputs/test_image.result.jpg", rectImg);
    return 0;
}