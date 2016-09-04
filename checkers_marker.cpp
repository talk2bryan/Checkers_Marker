#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <stdio.h>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
    Mat image = imread("./images/threshold.png");
    Mat new_frame = image.clone();
    Mat  roi_frame = cv::Mat(200,200,CV_8U); //this will contain our roi

	//points describing the edges from observation
	//for debug purposes
 	cv::line(new_frame,Point(118,140),Point(118,220),CV_RGB(0,255,0),1,8);
 	cv::line(new_frame,Point(118,140),Point(220,140),CV_RGB(0,255,0),1,8);
 	cv::line(new_frame,Point(118,220),Point(210,224),CV_RGB(0,255,0),1,8);
 	cv::line(new_frame,Point(220,140),Point(210,224),CV_RGB(0,255,0),1,8);

 	vector<cv::Point> edges;
 	//place points in anti-clockwise order from top left
 	edges.push_back(Point(118,140));
 	edges.push_back(Point(118,220));
 	edges.push_back(Point(210,224));
 	edges.push_back(Point(220,140));
 	
	cv::Point2f dst_vertices[4]; 
    // define dimensions of new ROI in same order
    dst_vertices[0] = cv::Point(0,0);
    dst_vertices[1] = cv::Point(0,199);
    dst_vertices[2] = cv::Point(199,199);	
    dst_vertices[3] = cv::Point(199,0);

    //place points in an array to perform 
    //transformation
    Point2f src_vertices[4];
    src_vertices[0] = edges[0];
    src_vertices[1] = edges[1];
    src_vertices[2] = edges[2];
    src_vertices[3] = edges[3];

    //perform transformation
    cv::Mat warpAffineMatrix = cv::getPerspectiveTransform(src_vertices,dst_vertices);

    cv::Size warp_size(200,200);
    cv::warpPerspective(new_frame,roi_frame,warpAffineMatrix,warp_size,cv::INTER_LINEAR,cv::BORDER_CONSTANT);


    imwrite("./images/marked_frame.png",new_frame);
    imwrite("./images/roi.png",roi_frame);


    // Store the set of points in the ROI image
    vector<cv::Point> points;
    Mat_<float>::iterator it = new_frame.begin<float>();
    Mat_<float>::iterator end = new_frame.end<float>();
    for (; it != end; ++it)
    {
        if (*it) 
        {
            points.push_back(it.pos());
            cout<<it.pos();
        }
    }
    int x,y;

    for (int i = 0; i < points.size(); ++i)
    {
        x = points[i].x;
        y = points[i].y;
        printf("x:%d y:%d %d\n",x,y,roi_frame.at<uchar>(x,y));
    }

    // while(1)
    // {
    // 	imshow("mat",image);
    // 	// imshow("box", new_frame);
    // 	if((waitKey(30)) == 10) break;
    // }
    return 0;
}