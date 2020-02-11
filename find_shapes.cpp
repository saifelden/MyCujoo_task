#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <exception>

using namespace cv;
using namespace std;

vector< vector<int> > ht;
vector< pair<int,int> > curr_sol;
void dfs(Mat& white_img,int i,int j)
{
    vector< pair<int,int> > sol1,sol2,sol3,sol4;
    if(i+1 <white_img.rows)
    {

	    Scalar colour = white_img.at<uchar>(Point(j,i+1));
        if(colour.val[0]>=170 && colour.val[0]<=255 && colour.val[1]>=0 && colour.val[1]<10 && colour.val[2]>=0 && colour.val[2]< 255&& ht[i+1][j]==0)
        {
            ht[i+1][j]=1;
            dfs(white_img,i+1,j);
            curr_sol.push_back(make_pair(i+1,j));
        }

    }
    if(j+1 <white_img.cols)
    {
        Scalar colour = white_img.at<uchar>(Point(j+1,i));
        if(colour.val[0]>=170 && colour.val[0]<=255 && colour.val[1]>=0 && colour.val[1]<10 && colour.val[2]>=0 && colour.val[2]< 255 && ht[i][j+1]==0)
        {
            ht[i][j+1]=1;
            dfs(white_img,i,j+1);
            curr_sol.push_back(make_pair(i,j+1));
        }

    }
    if(j-1 >=0)
    {
        Scalar colour = white_img.at<uchar>(Point(j-1,i));
        if(colour.val[0]>=170 && colour.val[0]<=255 && colour.val[1]>=0 && colour.val[1]<10 && colour.val[2]>=0 && colour.val[2]< 255 && ht[i][j-1]==0)
        {
            ht[i][j-1]=1;
            dfs(white_img,i,j-1);
            curr_sol.push_back(make_pair(i,j-1));
        }

    }
    if(i-1 >=0)
    {
    	Scalar colour = white_img.at<uchar>(Point(j,i-1));
        if(colour.val[0]>=170 && colour.val[0]<=255 && colour.val[1]>=0 && colour.val[1]<10 && colour.val[2]>=0 && colour.val[2]< 255 && ht[i-1][j]==0)
        {
            ht[i-1][j]=1;
            dfs(white_img,i-1,j);
            curr_sol.push_back(make_pair(i-1,j));
        }

    }

}

vector< pair<int,int> > traverse_white(Mat white_img)
{
    for(int i=0;i<white_img.rows;i++)
    {
        vector<int> tmp(white_img.cols,0);
        ht.push_back(tmp);
    }
    vector< pair<int,int> > sol,max_sol;
    cout<<white_img.rows<<endl;
    cout<<white_img.cols<<endl;
    for (int i=0;i<white_img.rows;i++)
    {
    	for(int j=0;j<white_img.cols;j++)
    	{

    	Scalar colour = white_img.at<uchar>(Point(j, i));

    	if(colour.val[0]>=170 && colour.val[0]<=255 && colour.val[1]>=0 && colour.val[1]<10 && colour.val[2]>=0 && colour.val[2]< 255){
        if(ht[i][j]==0)
        {
        	ht[i][j]=1;
        	curr_sol.clear();
        	curr_sol.push_back(make_pair(i,j));
            dfs(white_img,i,j);
            if(curr_sol.size()>max_sol.size())
            {
                max_sol = curr_sol;
            }
        }
    }
    }
    }

    return max_sol;

}

int main(int argc, char** argv)
{
 // Read the image file
 Mat image = imread(argv[1]);
 Mat bgr[3];
 Mat hsv,red_threshold, blue_threshold, white_threshold,dilated;
 cvtColor(image, hsv, COLOR_BGR2HSV);
 split(image,bgr);
 String windowName = "the original image"; //Name of the window
 std::vector<std::vector<Point> > red_contours,blue_contours,white_contours;
 int low_threshold = 30;
 inRange(hsv ,Scalar(0, 200, 200), Scalar(10, 255, 255), red_threshold);
 inRange(hsv ,Scalar(110, 200, 200), Scalar(130, 255, 255), blue_threshold);
 inRange(hsv ,Scalar(0, 0, 200), Scalar(180, 0, 255), white_threshold);
 cv::findContours(blue_threshold, blue_contours, CV_RETR_TREE, CHAIN_APPROX_SIMPLE);
 cv::findContours(red_threshold, red_contours, CV_RETR_TREE, CHAIN_APPROX_SIMPLE);
  /// Apply the specified morphology operation
 int morph_size = 7;
 Mat element = getStructuringElement( MORPH_RECT, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
 morphologyEx( white_threshold, white_threshold, MORPH_TOPHAT, element, Point(-1,-1),10);
 vector< pair<int,int> > white_contour = traverse_white(white_threshold);
 int min_sum = 2000,max_sum=0;
 pair<int,int> min_red,min_blue,max_red,max_blue,min_white,max_white;
 for(int i=0;i<white_contour.size();i++)
{
    if(white_contour[i].first+white_contour[i].second<min_sum)
    {
        min_white = make_pair( white_contour[i].first,white_contour[i].second);
        min_sum = white_contour[i].first+white_contour[i].second;
    }
    if(white_contour[i].first+white_contour[i].second>max_sum)
    {
        max_white = make_pair( white_contour[i].first,white_contour[i].second);
        max_sum = white_contour[i].first+white_contour[i].second;
    }
}
  Scalar color( rand()&255, rand()&255, rand()&255 );

 cout<<min_white.first<<" "<<min_white.second<<" "<<max_white.first<<" "<<max_white.second<<endl;
 rectangle(image, Point(min_white.second,min_white.first), Point(max_white.second,max_white.first), Scalar(0, 255, 0), 5, 8, 0);
 drawContours( image, red_contours, 0, color,5);
 Scalar color2( rand()&255, rand()&255, rand()&255 );
 drawContours( image, blue_contours, 0, color2,5);
 Rect red_rect = boundingRect(red_contours[0]);
 putText(image, "rectangle_1", Point(((red_rect.x+red_rect.width)/2)-10,((red_rect.y+red_rect.height)/2)+5),2,0.5, color);
 Rect blue_rect = boundingRect(blue_contours[0]);
 putText(image, "circle", Point((blue_rect.x+(blue_rect.width)/2)-5,(blue_rect.y+(blue_rect.height)/2)),2,0.5, color2);
 putText(image, "rectangle_2", Point(min_white.second+5,min_white.first+20),2,0.5, color2);
 namedWindow(windowName); // Create a window
 imshow(windowName,image); // Show our image inside the created window.
 waitKey(0); // Wait for any keystroke in the window
 destroyWindow(windowName); //destroy the created window
 return 0;
}