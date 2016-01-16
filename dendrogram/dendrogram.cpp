#include<iostream>
#include<math.h>
#include<vector>
#include"dec_color.cpp"
#include"opencv2/opencv.hpp"
using namespace std;

//#define N 18
#define N 6
#define M 64
//#define M 1
//Nはファイル数
//Mは次元

double dist(double x1[M],double x2[M]);
void center(double x1[M],double x2[M],double pos[N]);
int parent(int c,double CL[N*2-1][4],double D[N][M]);
void dendrogram(double CL[N*2-1][4],double pos[N*2][M],double min_dist);
void cut(int C[N],double CL[N*2-1][4],double D[N][M]);

int main(void){
  dec_color dec_color_hist;
  double D[N][M];
  double histgram[M];
  Mat im[N];
  Mat im_dst(im[1].size(), im[1].type());
  //画像読み込み
  //string pic[N]={"./apple/56501XV-1243_B1.jpg","./apple/shinanored-1.jpg","./apple/151021_3.jpg","./orange/373_fit512.jpg","./orange/56501XV-1266.jpg","./orange/56501XV-1326_B2.jpg"};
  //string pic[N]={"./color/r1.jpg","./color/r2.jpg","./color/r3.jpg","./color/b1.jpg","./color/b2.jpg","./color/b3.jpg"};
  //string pic[N]={"./color_ringo/r1.jpg","./color_ringo/r2.jpg","./color_ringo/r3.jpg","./color_ringo/b1.png","./color_ringo/b2.jpg","./color_ringo/b3.png"};
  string pic[N]={"./color_zaku/r1.jpg","./color_zaku/r2.jpg","./color_zaku/r3.jpg","./color_zaku/b1.jpg","./color_zaku/b2.jpg","./color_zaku/b3.jpg"};
  //string pic[N]={"./color_ringo_sttr/r1.jpg","./color_ringo_sttr/r2.jpg","./color_ringo_sttr/r3.jpg","./color_ringo_sttr/b1.jpg","./color_ringo_sttr/b2.jpg","./color_ringo_sttr/b3.jpg"};
  //string pic[N]={"./color_bekki/r1.jpg","./color_bekki/r2.jpg","./color_bekki/r3.jpg","./color_bekki/b1.jpg","./color_bekki/b2.jpg","./color_bekki/b3.jpg"};
  
  for(int i=0;i<N;i++){
    im[i]=imread(pic[i]);
    for(int j=0;j<M;j++){
        histgram[j]=0;
    }
      im_dst=dec_color_hist.dec(im[i]);
      dec_color_hist.create_color_histgram(im_dst,histgram);
      for(int k=0;k<M;k++){
        D[i][k]=histgram[k];
      }
  }
  for(int i=0;i<N;i++){
    for(int j=0;j<M;j++){
      //D[i][j]=histgram[j];
      cout<<"D["<<i<<"]["<<j<<"]:"<<D[i][j]<<endl;
    }
  }

  //int class1[];
  //int class2[];
  //int D[N]={3,1,8,13,5,12};
  //int D[N]={3,1,6,13,20,12};
  //double D[N]={3,1,6,13,20,12};
  //double D[N][M]={{3,2,5},{1,13,7},{3,6,13},{1,13,7},{3,2,5},{5,12,6}};
  //double D[N]={3,1,6,13,19,12};
  //double D[N][M]={3,1,6,13,19,12};
  //int D[N]={1,2,3,4,5,6};
  //int D[N]={3,7,12,100,23,35};
  //int D[N]={100,85,5,87,81,82,4,1,3,6,7,8,3,1,2,20,4,100};
  //全クラスタを格納する配列CL
  double CL[N*2-1][4];
  double min_dist=100000000;
  int tmp_i,tmp_j,tmp;

  //CL[i][0]:左側
  //CL[i][1]:右側
  //CL[i][2]:座標
  //CL[i][3]:削除フラグ、0は有効、1は削除、-1はクラスタ未作成
  //初期化。CL[0]~CL[N-1]にD[0]=D[N-1]を入れる
  //CL[i][2]にはそのノードの座標を入れる
  //n次元拡張ふええ
  //CL[i][2]を別変数にする
  double pos[N*2-1][M];
  for(int i=0;i<N;i++){
    for(int j=0;j<M;j++){
      pos[i][j]=D[i][j];
    }
  }

  //初期化。フラグを未使用にする
  //葉の次の要素を-1に設定する(無効にする)
  for(int i=0;i<N;i++){
    CL[i][3]=0;
    CL[i+N][3]=-1;
    CL[i][0]=CL[i][1]=-1;
  }


  //min_distの設定
  //十分大きい数値にすればいい
  //CLの最大値にする？
  //これいらない疑惑
  //if(CL[i][2]<min_dist)min_dist=CL[i][2];
  //最短距離のノードを求める
  //min_dist=1000;

  cout<<"dist(pos[0],pos[1])"<<dist(D[0],D[1])<<endl;

  //デンドログラムの作成
  dendrogram(CL,pos,min_dist);

  
  //表示
  cout << "新しく生成したクラスタとそのクラスタの子クラスタの番号" << endl;
  for(int i=0;i<N*2-1;i++){
    cout << i  << ":"  << CL[i][0] << ","  << CL[i][1] << endl;
  }

  int C[N]={0};
  cut(C,CL,D);

  for(int i=0;i<N;i++){
    cout <<"C[" << i << "]" << C[i] << endl;
  }

  return 0;
}


//距離
//double dist(double x1[M],double x2[M]){
//  double sum=0;
//  for(int i=0;i<M;i++){
//    sum+=pow((x1[i]-x2[i]),2);
//  }
//  cout << "dist=" << sqrt(sum) << endl;
//  return sqrt(sum);
//}

//cosine類似度方向余弦
//double dist(double x1[M],double x2[M]){
//  double inner=0;
//  double norm_inner[2]={0};
//  for(int i=0;i<M;i++){
//    inner+=x1[i]*x2[i];
//  }
//
//  for(int i=0;i<M;i++){
//    norm_inner[0]+=x1[i]*x1[i];
//    norm_inner[1]+=x2[i]*x2[i];
//  }
//  return inner/(sqrt(norm_inner[0])*sqrt(norm_inner[1]));
//}

//double dist(double x1[M],double x2[M]){
//  for(int i=0;i<M;i++){
//    x1[i]+=1;
//  }
//  double ave_x1,ave_x2;
//  double sum1=0;
//  double sum2=0;
//  double deno=0;
//  double nume_x=0;
//  double nume_y=0;
//  for(int i=0;i<M;i++){
//    sum1+=x1[i];
//    sum2+=x2[i];
//  }
//  ave_x1=sum1/M;
//  ave_x2=sum2/M;
//
//  for(int i=0;i<M;i++){
//    deno+=(x1[i]-ave_x1)*(x2[i]-ave_x2);
//    nume_x+=pow(x1[i]-ave_x1,2);
//    nume_y+=pow(x2[i]-ave_x2,2);
//  }
//
//  return deno/(sqrt(nume_x)*sqrt(nume_y));
//}

//Histogram Intersection法
double dist(double x1[M],double x2[M]){
  double sum=0;
  double nume_sum=0;
  for(int i=0;i<M;i++){
    if(x1[i]<=x2[i]){
      sum+=x1[i];
    }else{
      sum+=x2[i];
    }
  }
  for(int i=0;i<M;i++){
    nume_sum+=x1[i];
  }
  return -sum;
}

//新しいクラスタの中央値を決める
//グループ平均法の実装したい
//ここ考える
void center(double x1[M],double x2[M],double pos[M]){
  for(int i=0;i<M;i++){
    pos[i]=fabs(x1[i]+x2[i])/2.0;
  }
}

//引数で指定されたノードの親を求める
int parent(int c,double CL[N*2-1][4],double D[N][M]){
  int c1=CL[N*2-2][1];
  int c2=CL[N*2-2][0];
  int p;
    for(int i=N;i<N*2-1;i++){
      for(int j=0;j<2;j++){
        if(CL[i][j]==c){
          p=i;
        }
      }
    }
  return p;

}

void dendrogram(double CL[N*2-1][4],double pos[N*2][M], double min_dist){
  int tmp_i,tmp_j;
//デンドログラムを求める
  //形式は以下
  //配列CL[i]の[0][1]は自分の子要素を表し、
  //[2]はそのノードが持つ値
  //[3]はフラグ
  //CL[N]~CL[N*2-1]が求めるノード
  //最小の距離の差とその組み合わせを求める
  //削除はどうしよう。削除されたやつはスキップする？フラグがいる？
  //同じ距離の場合は右側にあるものが採用される
  //ああ違う。飛ばされるCLがある。
  //ああ、ああ、ああ、ああ、ああ、ああ、ああ
  //配列操作で木構造をよしなに
  //デンドログラムの作成はだいたい成功してる
  for(int k=N;k<N*2-1;k++){
  for(int i=0;i<N*2-1;i++){
    for(int j=i+1;j<N*2;j++){
      if(j==N*2-1){
      }
      if(k!=i && k!= j && CL[i][3]==0 && CL[j][3]==0 && dist(pos[i],pos[j])<=min_dist){
        //min_dist=dist(CL[i][2],CL[j][2]);
        //for(int c=0;c<M;c++){
        //cout<<"pos["<<k<<"]["<<i<<"]="<<pos[k][c]<<endl;
        //}
        min_dist=dist(pos[i],pos[j]);
        //新しいクラスタ候補の作成
        CL[k][0]=i;
        CL[k][1]=j;
        CL[k][3]=0;
        tmp_i=i;
        tmp_j=j;
      }
    }
  }
  //最小が確定したところでフラグを立てる
  //ここでエラー
  CL[tmp_i][3]=1;
  CL[tmp_j][3]=1;
  //ノードに値の代入
  center(pos[tmp_i],pos[tmp_j],pos[k]);
  cout<<"tmp_i,tmp_j"<<tmp_i<<","<<tmp_j<<",k_min_dist"<<min_dist<<endl;
  cout<<"dist[0][1]"<<dist(pos[0],pos[1])<<endl;
  cout<<"dist[1][2]"<<dist(pos[1],pos[2])<<endl;
  min_dist=100000000;
  }
}


void cut(int C[N],double CL[N*2-1][4],double D[N][M]){
  //切り出し部分
  //こっからN以下の数値を取り出す
  //contain判定
  //モジュール化したい
  //うまく実装できてねえじゃねえか
  //初期化
  //C[N]={0};
  int p;

  //切り出し
  //親をたどって根の子要素にあたるか根にあたるかしたら抜ける
  vector<int> index;
  for(int i=0;i<N;i++){
    p=parent(i,CL,D);
    while(true){
      if(p==CL[N*2-2][0]){
        index.push_back(i);
        break;
      }else if(p==CL[N*2-2][1]){
        break;
      }else if(p==N*2-2){
        index.push_back(i);
        cout<<p<<endl;
        break;
      }
      p=parent(p,CL,D);
    }
  }
  for(int i=0;i<index.size();i++){
    C[index[i]]=1;
  }
  //切り出しここまで
}

