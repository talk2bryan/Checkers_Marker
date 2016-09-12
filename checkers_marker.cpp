#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <stdio.h>

using namespace cv;
using namespace std;

static float get_2D_distance(const Point& pt1, const Point& pt2)
{//based on the Euclidean plane
    float diffX = pt1.x - pt2.x;
    float diffY = pt1.y - pt2.y;
    return sqrt( (diffX * diffX) + (diffY * diffY) );
}

int main(int argc, char* argv[])
{
    Mat image = imread("./images/threshold.png");
    Mat new_frame = image.clone();
    Mat  roi_frame = Mat(200,200,CV_8U); //this will contain our roi

	//points describing the edges from observation
	//for debug purposes
 	line(new_frame,Point(118,140),Point(118,220),CV_RGB(0,255,0),1,8);
 	line(new_frame,Point(118,140),Point(220,140),CV_RGB(0,255,0),1,8);
 	line(new_frame,Point(118,220),Point(210,224),CV_RGB(0,255,0),1,8);
 	line(new_frame,Point(220,140),Point(210,224),CV_RGB(0,255,0),1,8);


 	vector<Point> edges;
 	//place points in anti-clockwise order from top left
 	edges.push_back(Point(118,140));
 	edges.push_back(Point(118,220));
 	edges.push_back(Point(210,224));
 	edges.push_back(Point(220,140));
 	
	Point2f dst_vertices[4]; 
    // define dimensions of new ROI in same order
    dst_vertices[0] = Point(0,0);
    dst_vertices[1] = Point(0,199);
    dst_vertices[2] = Point(199,199);	
    dst_vertices[3] = Point(199,0);

    //place points in an array to perform 
    //transformation
    Point2f src_vertices[4];
    src_vertices[0] = edges[0];
    src_vertices[1] = edges[1];
    src_vertices[2] = edges[2];
    src_vertices[3] = edges[3];

    //perform transformation
    Mat warpAffineMatrix = getPerspectiveTransform(src_vertices,dst_vertices);

    Size warp_size(200,200);
    warpPerspective(new_frame,roi_frame,warpAffineMatrix,warp_size,INTER_LINEAR,BORDER_CONSTANT);

    // Store the set of points in the ROI image
    vector<Point> points;
    Mat_<float>::iterator it = roi_frame.begin<float>();
    Mat_<float>::iterator end = roi_frame.end<float>();
    for (; it != end; ++it)
    {
        if (*it) 
        {
            points.push_back(it.pos());
        }
    }
    int x,y;

    for (int i = 0; i < points.size(); ++i)
    {
        x = points[i].x;
        y = points[i].y;
        printf("x:%d y:%d %d\n",x,y,roi_frame.at<uchar>(x,y));
    }
    //152,149 to 181,149
    //32, 147 to 57,147

    x = (get_2D_distance(Point(0,0), Point(199,0)) ) /2;
    y = (get_2D_distance(Point(0,0),Point(0,199)) ) /2;

    line(roi_frame,Point(110,110),Point(120,110),CV_RGB(255,0,0),1,1); //middle

    
    line(roi_frame,Point(40,160),Point(50,160),CV_RGB(255,0,0),1,8); //BL
    line(roi_frame,Point(50,57),Point(60,57),CV_RGB(255,0,0),1,8); //TL
    line(roi_frame,Point(160,59),Point(170,59),CV_RGB(255,0,0),1,1); //TR
    line(roi_frame,Point(170,159),Point(180,159),CV_RGB(255,0,0),1,1); //BR
    

    // line(roi_frame,Point(32,149),Point(74,149),CV_RGB(255,0,0),1,8);


    /*
        int count_black = 0;
        int count_white = 0;
        for( int y = 0; y < gray.rows; y++ ) {
          for( int x = 0; x < gray.cols; x++ ) {
            if ( mask.at<uchar>(y,x) != 0 ) {
              if ( gray.at<uchar>(y,x) == 255 ) {
                count_white++;
              } 
              else {
                count_black++;
              } 
            }
}
    */

    // while(1)
    // {
    // 	imshow("mat",image);
    // 	// imshow("box", new_frame);
    // 	if((waitKey(30)) == 10) break;
    // }
    imwrite("./images/marked_frame.png",new_frame);
    imwrite("./images/roi.png",roi_frame);

    return 0;
}