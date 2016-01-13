#include<iostream>
#include"opencv2/opencv.hpp"

using namespace std;
using namespace cv;

class dec_color{
  private:
    //Mat im = imread("test.jpg");
    //Mat im;
    //Mat im_dst(im.size(), im.type());
    int color;
  public:
    void dec(Mat im);
};

void dec_color::dec(Mat im){
  Mat im_dst(im.size(), im.type());
  //im.at<Vec3b>(y, x)[0]にB成分
  //im.at<Vec3b>(y, x)[1]にG成分
  //im.at<Vec3b>(y, x)[2]にR成分

  for(int y=0;y<im.rows;y++){
    for(int x=0;x<im.cols;x++){
      for(int i=0;i<im.channels();i++){
        color=im.at<Vec3b>(y,x)[i];
        //4値化
        if(0<=color && color<=63){
          im_dst.at<Vec3b>(y,x)[i] = 32;
        }else if(64<=color && color<=127){
          im_dst.at<Vec3b>(y,x)[i] = 96;
        }else if(128<=color && color<=191){
          im_dst.at<Vec3b>(y,x)[i] = 160;
        }else if(192<=color && color<=255){
          im_dst.at<Vec3b>(y,x)[i] = 224;
        }
      }
    }
  }

//Mat image = imread("test.jpg");
//for(int y = 0; y < image.rows; ++y){
//  for(int x = 0; x < image.cols; ++x){
//    // 画像のチャネル数分だけループ。白黒の場合は1回、カラーの場合は3回　　　　　
//    for(int c = 0; c < image.channels(); ++c){
//      cout << image.data[ y * image.step + x * image.elemSize() + c ] << endl;
//    }
//  }
//}

  //memcpy(src,im.data,sizeof(uchar)*256*256);
  //cout << src[100][100] << endl;
  //for(int j=0; j<256; j++){
  //      for(int i=0; i<256; i++){
  //          dst[j][i] = 255 - src[j][i];
  //      }
  //}
  //memcpy(im_dst.data,dst,sizeof(uchar)*256*256);

  imwrite("output.jpg", im_dst);
}

int main(void){
  dec_color color;
  Mat im = imread("test.jpg");
  color.dec(im);
  return 0;
}
