#include<iostream>
#include"opencv2/opencv.hpp"

using namespace std;
using namespace cv;

#define O 64

class dec_color{
  private:
    int color;
  public:
    Mat dec(Mat im);
    void create_color_histgram(Mat im,double histgram[O]);
};

Mat dec_color::dec(Mat im){
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
          im_dst.at<Vec3b>(y,x)[i] = 0;
        }else if(64<=color && color<=127){
          im_dst.at<Vec3b>(y,x)[i] = 1;
        }else if(128<=color && color<=191){
          im_dst.at<Vec3b>(y,x)[i] = 2;
        }else if(192<=color && color<=255){
          im_dst.at<Vec3b>(y,x)[i] = 3;
        }
      }
    }
  }

  return im_dst;
}

void dec_color::create_color_histgram(Mat im,double histgram[O]){
  //rgbそれぞれ入れる配列
  int color[3];
  int rows=im.rows;
  int cols=im.cols;
  int index[rows][cols];
  for(int y=0;y<rows;y++){
    for(int x=0;x<cols;x++){
      for(int c=0;c<im.channels();c++){
          color[c]=im.at<Vec3b>(y,x)[c];
      }
      index[y][x]=color[0]*16+color[1]*4+color[2];
    }
  }
  for(int y=0;y<rows;y++){
    for(int x=0;x<cols;x++){
      histgram[index[y][x]]+=1;
    }
  }
  for(int i=0;i<64;i++){
    cout<<"histgram["<<i<<"]="<<histgram[i]<<endl;
  }
}

//int main(void){
//  dec_color color;
//  double histgram[O]={0};
//  Mat im = imread("test.jpg");
//  Mat im_dst(im.size(), im.type());
//  im_dst = color.dec(im);
//  color.create_color_histgram(im_dst,histgram);
//  for(int i=0;i<O;i++){
//    cout<<i<<":"<<histgram[i]<<endl;
//  }
//  return 0;
//}
