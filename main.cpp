#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <algorithm>
#include<stdio.h>
using namespace cv;
using namespace std;


Mat read_image(string image_name);
Mat convert_to_gray(Mat image);
Mat gradient(Mat image);
vector<Point> coins_Harris(Mat image_gray, Mat image_source);
//void ZSSD(Mat );
void display(vector<Mat> images, vector<string> image_names);


Mat read_image(string image_name){
    Mat image;
    image = imread(image_name, 1);
                if (image.empty()){
                        cout <<"impossible de charger l'image "<<endl;
                }

        return image;

}

Mat convert_to_gray(Mat image) {
    Mat src_gray_image;
    cvtColor(image, src_gray_image, CV_BGR2GRAY );
    return src_gray_image;

}

Mat gradient(Mat image) {
   Mat grad_x, grad_y;
   Mat grad_xx, grad_yy;
   Mat grad_xy, grad_xx_yy, xy;
   Mat grad_xxg, grad_yyg, grad_xyg;
   Mat trace_2;
   Mat dst;


    int ddepth = CV_16S;
    int scale = 1;
    int delta = 0;
    int k = 0.04;


       //gradient X
        Sobel(image, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
       //Filtre gaussien
        GaussianBlur(grad_x, grad_x, Size(3, 3), 0, 0, BORDER_DEFAULT);
       //gradient Y
        Sobel(image, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
       //Filtre gaussien
        GaussianBlur(grad_y, grad_y, Size(3, 3), 0, 0, BORDER_DEFAULT);

       //gradient xx
        pow(grad_x, 2.0, grad_xx);
       //gradient yy
        pow(grad_y, 2.0, grad_yy);

        grad_xy = grad_x.mul(grad_y);

       //Filtre gaussien
        GaussianBlur(grad_xx, grad_xxg, Size(3, 3), 0, 0, BORDER_DEFAULT);
        GaussianBlur(grad_yy, grad_yyg, Size(3, 3), 0, 0, BORDER_DEFAULT);
        GaussianBlur(grad_xy, grad_xyg, Size(7,7), 2.0, 2.0, BORDER_DEFAULT);

        //trace et det
        pow((grad_xxg + grad_yyg), 2.0, trace_2);
        multiply(grad_xxg, grad_yyg, grad_xx_yy);
        multiply(grad_xyg, grad_xyg, xy);
        //réponse du détecteur
        dst = (grad_xx_yy - xy) - k * trace_2;


            return dst;
    }


vector<Point> coins_Harris(Mat image_gradient, Mat image_source) {

    Mat dst_norm, dst_norm_scaled, src_gray;
    vector<Point> Tab;
    Mat image;
    Mat max;
    int pixel;

     src_gray = convert_to_gray(image_source);



    //Normalisation
    normalize(image_gradient, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
    convertScaleAbs(dst_norm, dst_norm_scaled);


    //Les coins
  for(int j = 0; j < dst_norm.rows; j++){
   for( int i = 0; i < dst_norm.cols; i++ ) {
       pixel = (int)dst_norm.at<float>(j, i);
 if(j > 0 && i > 0){
if(pixel > 233 && pixel > 0 && pixel > (int)dst_norm.at<float>(j, i + 1) && pixel > (int)dst_norm.at<float>(j, i - 1) && pixel > (int)dst_norm.at<float>(j + 1, i) && pixel > (int)dst_norm.at<float>(j + 1, i + 1) && pixel > (int)dst_norm.at<float>(j + 1, i - 1) &&  pixel > (int)dst_norm.at<float>(j - 1, i) &&  pixel > (int)dst_norm.at<float>(j - 1, i - 1) &&  pixel > (int)dst_norm.at<float>(j - 1, i + 1))
                       Tab.push_back(Point(i, j));
                   }
 if(j > 0 && i == 0){
if(pixel > 233 && pixel > 0 && pixel > (int)dst_norm.at<float>(j, i + 1) && pixel > (int)dst_norm.at<float>(j + 1, i) && pixel > (int)dst_norm.at<float>(j + 1, i + 1) &&  pixel > (int)dst_norm.at<float>(j - 1, i) &&  pixel > (int)dst_norm.at<float>(j - 1, i + 1))
                            Tab.push_back(Point(i, j));
}

 if(j == 0 && i > 0){
 if(pixel > 233 && pixel > 0 && pixel > (int)dst_norm.at<float>(j, i + 1) && pixel > (int)dst_norm.at<float>(j, i - 1) && pixel > (int)dst_norm.at<float>(j + 1, i) && pixel >= (int)dst_norm.at<float>(j + 1, i + 1) && pixel > (int)dst_norm.at<float>(j + 1, i - 1))
                Tab.push_back(Point(i, j));
}

 if(j == 0 && i == 0){
     if(pixel > 233 && pixel > 0 && pixel > (int)dst_norm.at<float>(j, i + 1) && pixel > (int)dst_norm.at<float>(j + 1, i) && pixel > (int)dst_norm.at<float>(j + 1, i + 1))
         Tab.push_back(Point(i, j));

 }

      }

    }


    return Tab;
 }





Mat imagescircles(vector<Point> tab, Mat image) {
   for(unsigned i = 0; i < tab.size(); i++) {

       circle(image, tab.at(i), 0, Scalar(0, 0, 255), 2, 8, 0);
   }
   return image;
}

void display(vector<Mat> images, vector<string> image_names) {

    for(unsigned i = 0; i < images.size(); i++) {

        namedWindow(image_names.at(i), CV_WINDOW_KEEPRATIO );
        imshow(image_names.at(i), images.at(i));
        waitKey(0);
    }
}

int main() {
    Mat image;
    vector<string> images_names, strings;
    vector<Mat> detectors, images;
    vector<Point> tab;




    string string_1 = "Corners of the image";
    string string_2 = "Corners of the item";
    strings.push_back(string_1);
    strings.push_back(string_2);
    images_names.push_back("C:\\Users\\HP\\Documents\\SDBD\\S1\\Indexation\\TP3\\image.jpg");
    images_names.push_back("C:\\Users\\HP\\Documents\\SDBD\\S1\\Indexation\\TP3\\tem0.jpg");


    for(unsigned i = 0; i < images_names.size(); i++)
        {
            image = read_image(images_names.at(i));
            image = imagescircles(coins_Harris(gradient(convert_to_gray(image)), image), image);
            images.push_back(image);
        }



                display(images, strings);






    /*int SSD = 0;
    int q = -2 , p = -2;

    for( int j = 0; j < src_gray.rows; j++ )
        { for( int i = 0; i < src_gray.cols; i++ ) {
            //if((int) dst_norm.at<float>(j,i) > 128 && && q < 4 && p < 4) {
                    int v = (int)dst_norm.at<float>(j + q, i + p) ;
                    SSD += v * v;

           circle(src_gray, Point( i, j ), 1,  Scalar(0, 0, 255), 1, 3, 0 );

                }
            }
        }*/


    return 0;


}
