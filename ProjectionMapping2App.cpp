/*camera_ctApp*/
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/Cinder.h"
#include "cinder/Capture.h"
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include "CinderOpenCv.h"
/*BasicApp*/	//water
#include "cinder/app/AppBasic.h"
#include "cinder/TriMesh.h"
#include "cinder/MayaCamUI.h"
#include "cinder/Rand.h"
#include <list>
#include <omp.h>
/*fireApp*/
#include "cinder/Perlin.h"
/*PenkiApp*/
#include "cinder/app/App.h"
#include "cinder/app/RendererDx.h"
//#include "cinder/app/RendererGl.h"
#include "cinder/svg/Svg.h"
#include <list>
#include <random>
/*TurnCubeApp*/
#include "cinder/Camera.h"
#include "cinder/CinderMath.h"
/*Shabon*/
/*window*/
#include "cinder/qtime/QuickTime.h"
#include "cinder/Surface.h"
/*soul lost*/
#include "cinder/ImageIO.h"
#include <math.h>
/*time*/
#include <time.h>

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace cv;

/*camera_ctApp*/
gl::Texture mTexture;

/*BasicApp*/
GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat mat_ambient[] = { 0.6, 0.3, 0.4, 1.0 };
GLfloat mat_diffuse[] = { 0.3, 0.5, 0.8, 1.0 };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_emission[] = { 0.0, 0.1, 0.3, 0.0 };
GLfloat mat_shininess[] = { 128.0 };
GLfloat no_shininess[] = { 0.0 };
const int BasicApp_N = 70; //100

/*fireApp*/
const int fireApp_buff = 100;
const float fireApp_a = 0.1;
const float fireApp_XY[2] = { 271, 148 };
//const float XY[2] = { 271, 148 };
//const float XY[2] = { 259, 76 };
const int fireApp_N = 170;

/*TurnCube*/
GLfloat TurnCube_no_mat[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat TurnCube_mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat TurnCube_mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat TurnCube_mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat TurnCube_mat_emission[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat TurnCube_mat_shininess[] = { 128.0 };
GLfloat TurnCube_no_shininess[] = { 0.0 };

/*Shabon*/
const int Shabon_N = 50;
const int Shabon_L = 60;

/*window*/
GLfloat window_mat_ambient[] = { 0.5, 0.5, 0.5, 1.0 }; //{ 0.6, 0.3, 0.4, 1.0 };
GLfloat window_mat_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };// { 0.3, 0.5, 0.8, 1.0 };
GLfloat window_mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat window_mat_emission[] = { 0.5, 0.5, 0.5, 1.0 };//{ 0, 0, 0, 0 };//{ 0.0, 0.1, 0.3, 0.0 };
GLfloat window_mat_shininess[] = { 128.0 };
GLfloat window_no_shininess[] = { 0.0 };

GLfloat window_index[10][3] = { { 0 - 6, 320, 240 }, { 155 - 6, 320, 5 }, { 315 - 6, 320, 240 }, { 155 - 6, 320, 475 }, { 155 - 6, 320, 240 }, { 155 - 6, 320, 240 }, { 75 - 6, 320, 240 }, { 155 - 6, 320, 120 }, { 235 - 6, 320, 240 }, { 155 - 6, 320, 360 } };
GLfloat window_size[2][3] = { { 10, 10, 480 }, { 320, 10, 10 } };
const int window_movie_size[2][2] = { { 0, 0 }, { 1200, 880 } };
const int window_N = 100;

clock_t time_start, time_end;

class ProjectionMapping2App : public AppNative {
public:
	void setup();
	void mouseDown(MouseEvent event);
	void update();
	void draw();
	void resetup(int re_sw);

	double clock_time = 0;
	double ch_time = 0;

	/*BasicApp*/
	void drawGrid(float size = 100.0f, float step = 10.0f);

	/*camera_ctApp*/
	Capture mCap;
	int cnt;
	int mouseX, mouseY;
	int px1, py1, px2, py2;
	vector< vector<Point> > contours;
	Mat hsv_image, mask_image;
	Mat erode, dilate;
	double x, y;
	int count;
	int sw;

	/*BasicApp*/
	list<ci::Vec2f>		mPoints;
	bool DIFFUSE;
	bool AMBIENT;
	bool SPECULAR;
	bool EMISSIVE;
	int wave;
	TriMesh mesh;
	int x1, y1;


	/*fireApp*/
	int fireColor[128][3];
	int fireApp_pos[fireApp_N + fireApp_buff][fireApp_N];
	int smokePos[fireApp_N + fireApp_buff][fireApp_N];
	int seedpram[fireApp_N][fireApp_N];
	int						mSeed;
	int						mOctaves;
	float					mTime;
	Perlin					mPerlin;
	float					mFrequency;
	int fireApp_x, fireApp_y;
	int fireApp_circle;

	/*PenkiApp*/
	std::random_device rnd;

	/*TurnCube*/
//	void reload();
	void fileDrop(FileDropEvent event);

	gl::Texture TurnCube_mTexture;
	Matrix44f TurnCube_mCubeRotation;
	int TurnCube_x, TurnCube_y;
	int TurnCube_f = 2;
	float TurnCube_kaiten = 0;
	float TurnCube_houkou = 0;
	float TurnCube_speed = 0;
	int TurnCube_px = 15;
	bool TurnCube_DIFFUSE;
	bool TurnCube_AMBIENT;
	bool TurnCube_SPECULAR;
	bool TurnCube_EMISSIVE;

	/*Shabon*/
	int Shabon_x, Shabon_y;
	int Shabon_z = 0;
	float Shabon_kyu[Shabon_N][3][3];
	float Shabon_a;

	/*window*/
	void loadMovieFile(const fs::path &moviePath);
	list<ci::Vec2f>		window_mPoints;
	bool window_DIFFUSE;
	bool window_AMBIENT;
	bool window_SPECULAR;
	bool window_EMISSIVE;
	double window_r;
	int window_flag;
	int window_cnt;
	qtime::MovieSurface	window_movie;
	Surface				window_mSurface;

	/*movie*/
	int avi;
	Surface movie_mSurface;
	qtime::MovieSurface movie;

	/*soul lost*/
	gl::Texture myImage, myImage2;
	int soul_x, soul_y;
	int Frag = 1;
	double soul_Speed = 1.00;
	ci::Vec2f soul_Pos, soul_input, now_xy, xyLeftUp, xyRightDown;
	int soul_PosX = 271, soul_PosY = 148;
	int soul_Count = 0;
	int soul_f = 0;
	double soul_G;
	int i = 0;
	int soul_INorOUT = 0;
	double soul_N;
	void DrawGaussian();

//protected:
	/*BasicApp*/
	float BasicApp_pos[2][BasicApp_N][BasicApp_N];
	MayaCamUI mMayaCam;
	CameraPersp cam;


//private:
	/*PenkiApp*/
	struct Waterdrop{
		ci::Vec2d PenkiApp_pos;
		float radius;
		Color color;
		float life;
		bool end;
	};
	list<Waterdrop> waterdrops;
	mt19937 mt;
	normal_distribution<float> norm;
};

void ProjectionMapping2App::setup()
{
	/*camera_ctApp*/
	cnt = 0;
	px1 = 393;	//x
	py1 = 140;	//y
	px2 = 882;	//x1
	py2 = 633;	//y1
	try{
		mCap = Capture(640, 480);
		mCap.start();
	}
	catch (...) {
		console() << "Failed to initialize capture" << std::endl;
	}

	sw = 4;		//1:BasicApp, 2:fireApp, 3:PenkiApp, 4:TurnCube, 5:Shabon, 6:window, 7:movie, 8:soul
	avi = 3;	//movie 3:openingMovie.mp4

	resetup(sw);
	//time_start = clock();

}

void ProjectionMapping2App::mouseDown( MouseEvent event )
{
	mouseX = event.getX();
	mouseY = event.getY();
	console() << mouseX << "," << mouseY << endl;
}

void ProjectionMapping2App::update()
{

	time_end = clock();
	clock_time = time_end - time_start;
	ch_time = clock_time / CLOCKS_PER_SEC;

	console() << "time:" << (double)(time_end - time_start) / CLOCKS_PER_SEC << "[sec]" << endl;

	if (ch_time > 5.0){
		sw++;
		if (sw > 8) sw = 1;
		resetup(sw);
	}
	
	/*camera_ctApp*/
	Mat input1(toOcv(mCap.getSurface()));
	cvtColor(input1, hsv_image, CV_BGR2HSV);
	inRange(hsv_image, Scalar(30, 100, 30), Scalar(40, 255, 255), mask_image);
	cv::erode(mask_image, erode, cv::Mat(), Point(-1, -1), 2);
	cv::dilate(erode, dilate, cv::Mat(), Point(-1, -1), 4);

	cv::findContours(dilate, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

#pragma omp parallel
	{
#pragma omp ss
		for (int i = 0; i < contours.size(); i++){
			count = contours.at(i).size();
			x = 0.0; y = 0.0;
			for (int j = 0; j < count; j++){
				x += contours.at(i).at(j).x;
				y += contours.at(i).at(j).y;
			}
			x /= count;
			y /= count;
		}
	}

	//time_end = clock();
	//console() << "cam:"<<(double)(time_end - time_start) / CLOCKS_PER_SEC <<"[sec]"<< endl;
	//time_start = clock();

	/*BasicApp*/
	if (x > px1 && y > py1 && x < px2 && y < py2){
		if (sw == 1){
			int x1 = x / 6;
			int y1 = y / 6;
			if (x1 > 2 && x1 < BasicApp_N - 3 && y1 > 2 && y1 < BasicApp_N - 3){
#pragma omp parallel
					{
#pragma omp ss
						for (int i = -2; i <= 2; i++){
							for (int j = -2; j <= 2; j++){
								BasicApp_pos[0][x1 + i][y1 + j] = 50 - BasicApp_N / 5;
								cout << "x:" << x1 << "y:" << y1 << endl;
							}
						}
					}
			}
		}
	}
	if (sw == 1){
		//random wave
		if (wave % 3 == 0){
			int rndX = randInt(5, BasicApp_N - 5);
			int rndY = randInt(5, BasicApp_N - 5);
#pragma omp parallel
			{
#pragma omp for
				for (int i = -5; i < 5; i++){
					for (int j = -5; j < 5; j++){
						BasicApp_pos[1][rndX + i][rndY + j] -= BasicApp_N / 500.0;
					}
				}
			}
			wave = 0;
		}
		wave++;

#pragma omp parallel
			{
#pragma omp for
				for (int i = 1; i < BasicApp_N - 1; i++){
					for (int j = 1; j < BasicApp_N - 1; j++){
						BasicApp_pos[1][i][j] = ((BasicApp_pos[0][i - 1][j] + BasicApp_pos[0][i + 1][j] + BasicApp_pos[0][i][j - 1] + BasicApp_pos[0][i][j + 1] - 200) / 4 - (BasicApp_pos[0][i][j] - 50)) + BasicApp_pos[1][i][j] * 0.9;
						if (BasicApp_pos[1][i][j]>20){
							BasicApp_pos[1][i][j] = 20;
						}
						else if (BasicApp_pos[1][i][j] < -20){
							BasicApp_pos[1][i][j] = -20;
						}
					}
				}
			}
#pragma omp parallel
			{
#pragma omp for
				for (int i = 1; i < BasicApp_N - 1; i++){
					for (int j = 1; j < BasicApp_N - 1; j++){
						BasicApp_pos[0][i][j] += BasicApp_pos[1][i][j];
						//mesh.appendVertex(Vec3f(i, pos[0][i][j], j));
					}
				}
			}
	}

	/*fireApp*/
	if (x > px1 && y > py1 && x < px2 && y < py2){
		if (sw == 2){
			if (x > 0 && x < fireApp_N - fireApp_circle && y > 0 && y < fireApp_N - fireApp_circle){
				fireApp_pos[int(y)][int(x)] = 0;
				fireApp_x = x;
				fireApp_y = y;

#pragma omp parallel
				{
#pragma omp for
					for (int i = 0; i < fireApp_circle / 2; i++){
						for (int j = 0; j < fireApp_circle - (fireApp_circle / 2 - i)*(fireApp_circle / 2 - i) * 2 / (fireApp_circle / 2); j++){
							smokePos[fireApp_y + i + fireApp_buff][fireApp_x + (fireApp_circle / 2 * 2) - fireApp_circle + ((fireApp_circle / 2 - i)*(fireApp_circle / 2 - i) / (fireApp_circle / 2)) + j] = 100;
						}
					}
				}
				//for (int i = 0; i < y; i++){
				//	for (int j = 0; j < circle; j++){
				//		pos[i][x+j] = 0;
				//	}
				//}
#pragma omp parallel
				{
#pragma omp for
					for (int i = 0; i < fireApp_circle / 2; i++){
						for (int j = 0; j < fireApp_circle - (fireApp_circle / 2 - i)*(fireApp_circle / 2 - i) * 2 / (fireApp_circle / 2); j++){
							smokePos[fireApp_y + fireApp_circle - 1 - i + fireApp_buff][fireApp_x + (fireApp_circle / 2 * 2) - fireApp_circle + ((fireApp_circle / 2 - i)*(fireApp_circle / 2 - i) / (fireApp_circle / 2)) + j] = 100;
						}
					}
				}
			}
		}
	}
	//各点(0,0)~(300,300)の火種を配置
	if (sw == 2){
#pragma omp parallel
			{
#pragma omp for
				for (int i = 0; i < fireApp_N; i++){
					for (int j = 0; j < fireApp_N; j++){
						seedpram[i][j] = (int)(127 * rand() / RAND_MAX) + i / 25;
					}
				}
			}

			//火種が発火する閾値
			int firelevel = 126;	//28

			//火種が発火するか判定
#pragma omp parallel
			{
#pragma omp for
				for (int i = 0; i < fireApp_N - 2; i++){
					for (int j = 1; j < fireApp_N - 1; j++){
						if (firelevel < seedpram[i][j])
							fireApp_pos[i + fireApp_buff][j] = 120;
						else if (seedpram[i][j] < 20)
							fireApp_pos[i + fireApp_buff][j] *= 0.3;
					}
				}
			}

			//炎の上昇を計算（周辺の平均を計算）
#pragma omp parallel
			{
#pragma omp for
				for (int i = 0; i < fireApp_N - 2 + fireApp_buff; i++){
					for (int j = 1; j < fireApp_N - 1; j++){
						fireApp_pos[i][j] = (fireApp_pos[i + 1][j] + fireApp_pos[i + 2][j] + fireApp_pos[i][j] + fireApp_pos[i + 1][j - 1] + fireApp_pos[i + 1][j + 1]) / 5;
					}
				}
			}

#pragma omp parallel
			{
#pragma omp for
				for (int i = 0; i < fireApp_N - 2 + fireApp_buff; i++){
					for (int j = 2; j < fireApp_N - 2; j++){
						smokePos[i][j] = (smokePos[i + 1][j] + smokePos[i + 2][j] + smokePos[i][j] + smokePos[i + 1][j - 1] + smokePos[i + 1][j + 1] + smokePos[i + 1][j - 2] + smokePos[i + 1][j + 2] + smokePos[i + 2][j - 1] + smokePos[i + 2][j + 1] + smokePos[i + 2][j - 2] + smokePos[i + 2][j + 2] + 7) / 12;
					}
				}
			}
	}

	/*PenkiApp*/
	if (x > px1 && y > py1 && x < px2 && y < py2){
		if (sw == 3){
			using normal = std::normal_distribution<float>;
			using uniform = std::uniform_real_distribution<float>;

			for (int i = 0; i < rand() % 10; i++){
				Waterdrop obj;
				obj.life = normal(0.4f, 0.2f)(mt);
				float dx = uniform(-3.0f, 3.0f)(mt);
				float dy = uniform(-3.0f, 3.0f)(mt);
				obj.PenkiApp_pos.x = x + dx;
				obj.PenkiApp_pos.y = y + dy;
				obj.radius = normal(10.0f, 7.0f - (abs(dx) + abs(dy)))(mt);
				float r = uniform(0.8f, 1.0f)(mt);
				float g = uniform(0.2f, 0.5f)(mt);
				float b = uniform(0.4f, 0.8f)(mt);
				obj.color = Color(r, g, b);
				obj.end = true;
				waterdrops.push_back(obj);
			}
		}
	}
	if (sw == 3){
		std::list<Waterdrop> appends;
		for (auto& obj : waterdrops){
			if (obj.end){
				obj.end = false;

				Waterdrop clone;
				clone.color = obj.color;
				clone.end = true;
				clone.PenkiApp_pos = obj.PenkiApp_pos;
				clone.radius = obj.radius;
				clone.life = obj.life * 0.95f;

				clone.radius *= 0.95f;
				clone.PenkiApp_pos.y += 2;

				appends.push_back(clone);
			}
			obj.life -= 0.01f;
		}
		waterdrops.insert(waterdrops.end(), appends.begin(), appends.end());
		waterdrops.remove_if([](Waterdrop obj) { return obj.life < 0; });
	}
	
	/*TurnCubeApp*/
	if (x > px1 && y > py1 && x < px2 && y < py2){
		if (sw == 4){
			TurnCube_x = x;
			TurnCube_y = y;
		}
	}
	if (sw == 4){
		if (TurnCube_f == 0){
			if ((TurnCube_x < 270 || TurnCube_y < 150) || (TurnCube_x > 440 || TurnCube_y > 300)){
				TurnCube_f = 2;
			}
			else{
				TurnCube_f = 0;
			}
		}
		else if (TurnCube_f == 1){
			if (TurnCube_kaiten <= TurnCube_px){
				if (TurnCube_speed < 1){
					TurnCube_mCubeRotation.rotate(ci::Vec3f(0, TurnCube_houkou, 0), TurnCube_speed / (2 * M_PI)); //回転
					TurnCube_speed += 0.01;
				}
				else{
					TurnCube_mCubeRotation.rotate(ci::Vec3f(0, TurnCube_houkou, 0), TurnCube_speed / (2 * M_PI)); //回転
					TurnCube_kaiten++;
				}
			}
			else if (TurnCube_kaiten > TurnCube_px && TurnCube_kaiten <= 100 + TurnCube_px){
				TurnCube_mCubeRotation.rotate(ci::Vec3f(0, TurnCube_houkou, 0), TurnCube_speed / (2 * M_PI)); //回転
				TurnCube_speed -= 0.01;
				TurnCube_kaiten++;
			}
			else{
				if ((TurnCube_x < 270 || TurnCube_y < 150) || (TurnCube_x > 440 || TurnCube_y > 300)){
					TurnCube_f = 2;
					TurnCube_kaiten = 0;
				}
				else{
					TurnCube_f = 1;
				}
			}
		}
		else if (TurnCube_f == 2){
			if ((TurnCube_x > 270 && TurnCube_y > 150) && (TurnCube_x < 440 / 2 && TurnCube_y < 300)){
				TurnCube_f = 1;
				TurnCube_houkou = 1;
			}
			else if ((TurnCube_x >= 440 / 2 && TurnCube_y > 150) && (TurnCube_x < 440 && TurnCube_y < 300)){
				TurnCube_f = 1;
				TurnCube_houkou = -1;
			}
		}
	}

	/*Shabon*/
	if (x > px1 && y > py1 && x < px2 && y < py2){
		if (sw == 5){
			Shabon_x = int(x);
			Shabon_y = int(y);
		}
	}
	if (sw == 5){
		for (int i = 0; i < Shabon_N; i++){
			Shabon_a = sqrt((Shabon_kyu[i][0][0] - Shabon_x)*(Shabon_kyu[i][0][0] - Shabon_x) + (Shabon_kyu[i][0][1] - Shabon_y)*(Shabon_kyu[i][0][1] - Shabon_y) + (Shabon_kyu[i][0][2] - Shabon_z)*(Shabon_kyu[i][0][2] - Shabon_z));

			if (Shabon_a < Shabon_L){
				Shabon_kyu[i][1][0] = 0.005*(Shabon_kyu[i][0][0] - Shabon_x) / Shabon_a;
				Shabon_kyu[i][1][1] = 0.005*(Shabon_kyu[i][0][1] - Shabon_y) / Shabon_a;
				Shabon_kyu[i][1][2] = (Shabon_kyu[i][0][2] - Shabon_z) / Shabon_a;
			}
			else{
				Shabon_kyu[i][1][0] = 0;
				Shabon_kyu[i][1][1] = 0;
				Shabon_kyu[i][1][2] = 0;
			}
			Shabon_kyu[i][0][0] += Shabon_kyu[i][0][0] * Shabon_kyu[i][1][0];
			Shabon_kyu[i][0][1] += Shabon_kyu[i][0][1] * Shabon_kyu[i][1][1];
			Shabon_kyu[i][0][2] += Shabon_kyu[i][0][2] * Shabon_kyu[i][1][2];
		}
	}

	/*window*/
	if (x > px1 && y > py1 && x < px2 && y < py2){
		if (sw == 6){
			window_flag = 1;
		}
	}
	if (sw == 6){
		window_r += window_flag*2;
		if (window_r > 125){
			window_cnt++;
			window_r = 125;
		}
		if (window_cnt > 50){
			window_flag *= -1;
			window_cnt = 0;
		}
		if (window_r < 0)
			window_flag = 0;

		if (window_movie)
			window_mSurface = window_movie.getSurface();
	}
	/*movie*/
	if (sw == 7){
		if (movie)
			movie_mSurface = movie.getSurface();
	}

	/*soul*/
	if (x > px1 && y > py1 && x < px2 && y < py2){
		if (sw == 8){
			soul_x = x;
			soul_y = y;
		}
	}
	if (sw == 8){
		soul_input[0] = soul_x;	soul_input[1] = soul_y;
		if (soul_PosX == 271 && soul_PosY == 148){
			soul_Count = 442 - 271 + 1;
			soul_f = 1;
		}
		else if (soul_PosX == 442 && soul_PosY == 148){
			soul_Count = 301 - 148;
			soul_f = 2;
		}
		else if (soul_PosX == 442 && soul_PosY == 301){
			soul_Count = 442 - 271;
			soul_f = 3;
		}
		else if (soul_PosX == 271 && soul_PosY == 301){
			soul_Count = 301 - 148;
			soul_f = 4;
		}

		if (soul_f == 1 && soul_Count > 0){
			soul_PosX++; soul_Count--;
		}
		else if (soul_f == 2 && soul_Count > 0){
			soul_PosY++; soul_Count--;
		}
		else if (soul_f == 3 && soul_Count > 0){
			soul_PosX--; soul_Count--;
		}
		else if (soul_f == 4 && soul_Count > 0){
			soul_PosY--; soul_Count--;
		}
		soul_Pos[0] = soul_PosX;
		soul_Pos[1] = soul_PosY;
	}
	
	//time_end = clock();
	//console() << "update:" << (double)(time_end - time_start) / CLOCKS_PER_SEC << "[sec]" << endl;

//	mTexture = gl::Texture(fromOcv(input1));
}

void ProjectionMapping2App::draw()
{

	//	time_start = clock();

	/*BasicApp*/
	if (sw == 1){
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// gray background
		gl::clear(Colorf(0.5f, 0.5f, 0.5f));
		GLfloat light_position[] = { 500, 0.0f, 100.0f, 0.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);

		// set up the camera 
		gl::pushMatrices();
		gl::setMatrices(mMayaCam.getCamera());

		// enable the depth buffer (after all, we are doing 3D)
		gl::enableDepthRead();
		gl::enableDepthWrite();

		ci::ColorA color(CM_HSV, 0.7f, 0.8f, 1.0f, 1.0f);

		/*lightset*/
		glMaterialfv(GL_FRONT, GL_DIFFUSE, color);

		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);

		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

		glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);

		// draw the grid on the floor
		drawGrid();
		gl::color(0.2f, 0.2f, 0.9f, 0.5f);
#pragma omp parallel
		{
#pragma omp for
			for (int i = 0; i < BasicApp_N - 1; i++){
				for (int j = 0; j < BasicApp_N - 1; j++){
					mesh.appendVertex(ci::Vec3f(i, BasicApp_pos[0][i][j], j));
					mesh.appendColorRgb(Color(0.3f, 0.3f, 1));
					mesh.appendVertex(ci::Vec3f(i + 1, BasicApp_pos[0][i + 1][j], j));
					mesh.appendVertex(ci::Vec3f(i, BasicApp_pos[0][i][j + 1], j + 1));
					mesh.appendVertex(ci::Vec3f(i + 1, BasicApp_pos[0][i + 1][j + 1], j + 1));
					int vIdx0 = mesh.getNumVertices() - 4;
					int vIdx1 = mesh.getNumVertices() - 3;
					int vIdx2 = mesh.getNumVertices() - 2;
					int vIdx3 = mesh.getNumVertices() - 1;
					ci::Vec3f norm = (ci::Vec3f(i + 1, BasicApp_pos[0][i + 1][j], j) - ci::Vec3f(i, BasicApp_pos[0][i][j], j)).cross(ci::Vec3f(i + 1, BasicApp_pos[0][i + 1][j], j) - ci::Vec3f(i, BasicApp_pos[0][i][j + 1], j + 1)).normalized();
					mesh.appendNormal(norm);
					mesh.appendTriangle(vIdx0, vIdx1, vIdx2);
					mesh.appendTriangle(vIdx3, vIdx1, vIdx2);
					gl::draw(mesh);
					mesh.clear();
				}
			}
		}
		gl::popMatrices();
		
		console() << "mat_ambient: " << mat_ambient[0] << endl;;

	}

	/*fireApp*/
	else if (sw == 2){
		// clear out the window with black
		gl::clear(Color(0, 0, 0));

		/*light off*/
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glDisable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//炎を描写
#pragma omp parallel
		{
#pragma omp for
			for (int i = 0; i < fireApp_N + fireApp_buff; i += 2){
				for (int j = 0; j < fireApp_N; j += 2){
					gl::color(fireColor[fireApp_pos[i][j]][0] * (1 - fireApp_a) + smokePos[i][j] * fireApp_a, fireColor[fireApp_pos[i][j]][1] * (1 - fireApp_a) + smokePos[i][j] * fireApp_a, fireColor[fireApp_pos[i][j]][2] * (1 - fireApp_a) + smokePos[i][j] * fireApp_a);
					gl::drawSolidEllipse(ci::Vec2d(fireApp_XY[0] + j, fireApp_XY[1] + i), 3.0, 3.0);
				}
			}
		}
	}


	/*PenkiApp*/
	else if (sw == 3){
		gl::clear(Color::black());

		/*light off*/
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glDisable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		for (auto& obj : waterdrops){
			gl::color(obj.color.r, obj.color.g, obj.color.b, obj.life * 0.25f);
			gl::drawSolidCircle(obj.PenkiApp_pos, obj.radius);
		}
	}

	/*TurnCube*/
	else if (sw == 4){
//		reload();
		// clear out the window with black
		gl::clear(Color(0, 0, 0));
		if (!TurnCube_mTexture)
			return;

		gl::setMatrices(mMayaCam.getCamera());

		/*light on*/
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_BLEND);

		// enable the depth buffer (after all, we are doing 3D)
		gl::enableDepthRead();
		gl::enableDepthWrite();

		/*light set*/
		ci::ColorA color(CM_HSV, 0.7f, 0.8f, 1.0f, 1.0f);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, color);

		glMaterialfv(GL_FRONT, GL_AMBIENT, TurnCube_mat_ambient);

		glMaterialfv(GL_FRONT, GL_SPECULAR, TurnCube_mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, TurnCube_mat_shininess);

		glMaterialfv(GL_FRONT, GL_EMISSION, TurnCube_mat_emission);

		TurnCube_mTexture.bind();
		glPushMatrix();
		//if (TurnCube_speed != 0){
		//	gl::multModelView(TurnCube_mCubeRotation);
		//}

		gl::drawCube(ci::Vec3f(0, 0, 0), ci::Vec3f(120 * 0.454, 100 * 0.454, 0.05f));
		glPopMatrix();
	}

	/*Shabon*/
	else if (sw == 5){
		// clear out the window with black
		gl::clear(Color(0, 0, 0));

		gl::color(1, 1, 1);

		/*light off*/
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glDisable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		for (int i = 0; i < Shabon_N; i++){
			if (Shabon_kyu[i][0][0] >= 271 && Shabon_kyu[i][0][0] <= 422 && Shabon_kyu[i][0][1] >= 148 && Shabon_kyu[i][0][1] <= 301){
				gl::color(Color(CM_HSV,Shabon_kyu[i][2][0], Shabon_kyu[i][2][1], Shabon_kyu[i][2][2]));
				//gl::color(Shabon_kyu[i][2][0], Shabon_kyu[i][2][1], Shabon_kyu[i][2][2]);
				gl::drawSolidCircle(ci::Vec2f(Shabon_kyu[i][0][0], Shabon_kyu[i][0][1]), 8, -1);
			}
			else{
				//座標
				Shabon_kyu[i][0][0] = randFloat(271, 422);
				Shabon_kyu[i][0][1] = randFloat(148, 301);
				//色
				Shabon_kyu[i][2][0] = randFloat(0, 1);
				Shabon_kyu[i][2][1] = randFloat(0.4, 1);
			}
		}
	}

	/*window*/
	else if (sw == 6){
		/*light on*/
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// gray background
		gl::clear(Colorf(0, 0, 0));
		GLfloat light_position[] = { 500, 0.0f, 100.0f, 0.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);

		// set up the camera 
		gl::pushMatrices();
		gl::setMatrices(mMayaCam.getCamera());

		// enable the depth buffer (after all, we are doing 3D)
		gl::enableDepthRead();
		gl::enableDepthWrite();

		/*light set*/
		ci::ColorA color(CM_HSV, 0.7f, 0.8f, 1.0f, 1.0f);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, color);

		glMaterialfv(GL_FRONT, GL_AMBIENT, window_mat_ambient);

		glMaterialfv(GL_FRONT, GL_SPECULAR, window_mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, window_mat_shininess);

		glMaterialfv(GL_FRONT, GL_EMISSION, window_mat_emission);

		Rectf bounds(window_movie_size[0][0], window_movie_size[0][1], window_movie_size[1][0], window_movie_size[1][1]); //movie size
		gl::enableAlphaBlending(true);

		if ((!window_movie) || (!window_mSurface))
			return;

		// We are using OpenGL to draw the frames here, so we'll make a texture out of the surface
		glPushMatrix();
		glTranslated(-270, 10, -200);
		glRotated(90, 1, 0, 0);
		gl::draw(gl::Texture(window_mSurface), bounds);
		glPopMatrix();
		gl::color(109 / 256.0f, 60 / 256.0f, 50 / 256.0f, 0.5f);

#pragma omp parallel
		{
#pragma omp for
			for (int i = 0; i < 10; i++){
				glPushMatrix();
				glTranslated(0, 320, 240);
				glRotated(-window_r, 0, 0, 1);
				glTranslated(0, -320, -240);
				if (i < 6)
					gl::drawCube(ci::Vec3f(window_index[i]), ci::Vec3f(window_size[i % 2][0] + 5, window_size[i % 2][1] + 2, window_size[i % 2][2] + 5));
				else gl::drawCube(ci::Vec3f(window_index[i]), ci::Vec3f(window_size[i % 2]));

				glPopMatrix();
			}
		}

#pragma omp parallel
		{
#pragma omp for
			for (int i = 0; i < 10; i++){
				glPushMatrix();
				glTranslated(640 + 5, 320, 240);
				glRotated(window_r, 0, 0, 1);
				glTranslated(-640 - 5, -320, -240);
				if (i < 6)
					gl::drawCube(ci::Vec3f(640 - window_index[i][0], window_index[i][1], window_index[i][2]), ci::Vec3f(window_size[i % 2][0] + 5, window_size[i % 2][1] + 2, window_size[i % 2][2] + 5));
				gl::drawCube(ci::Vec3f(640 - window_index[i][0], window_index[i][1], window_index[i][2]), ci::Vec3f(window_size[i % 2]));
				glPopMatrix();
			}
		}
	}

	/*movie*/
	else if (sw == 7){
		//gl::pushMatrices();
		gl::setMatrices(mMayaCam.getCamera());

		Rectf bounds(window_movie_size[0][0], window_movie_size[0][1], window_movie_size[1][0], window_movie_size[1][1]);
		gl::enableAlphaBlending(true);

		if ((!movie) || (!movie_mSurface))
			return;

		glPushMatrix();
		glTranslated(-270, 10, -200);
		glRotated(90, 1, 0, 0);
		gl::draw(gl::Texture(movie_mSurface), bounds);
		glPopMatrix();
		gl::color(109 / 256.0f, 60 / 256.0f, 50 / 256.0f, 0.5f);
	}

	/*soul*/
	else if (sw == 8){
		gl::clear(Color(0, 0, 0));

		/*light off*/
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glDisable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (soul_input[0]>xyLeftUp[0] && soul_input[0]<xyRightDown[0] && soul_input[1]>xyLeftUp[1] && soul_input[1]<xyRightDown[1]){	//マウスカーソルが窓内にあるとき
			soul_INorOUT = 1;
			soul_f = 0;

			soul_N = abs(soul_input[1] - soul_Pos[1]) / abs(soul_input[0] - soul_Pos[0]);

			if (soul_N <= 1){
				soul_N = (soul_input[1] - soul_Pos[1]) / (soul_input[0] - soul_Pos[0]);
				soul_PosX += (soul_input[0] - soul_Pos[0]) / abs(soul_input[0] - soul_Pos[0]);
				soul_PosY = (int)(soul_PosY + soul_N + 0.5);
			}
			else{
				soul_N = (soul_input[0] - soul_Pos[0]) / (soul_input[1] - soul_Pos[1]);
				soul_PosX = (int)(soul_PosX + soul_N + 0.5);
				soul_PosY += (soul_input[1] - soul_Pos[1]) / abs(soul_input[1] - soul_Pos[1]);
			}

			DrawGaussian();

			//i = 0;
		}
		else{
			if (soul_INorOUT == 1){ soul_PosX = 271; soul_PosY = 148; }
			soul_INorOUT = 2;
			DrawGaussian();
		}
	}
	//time_end = clock();
	//console() << "draw:" << (double)(time_end - time_start) / CLOCKS_PER_SEC << "[sec]" << endl;

}

void ProjectionMapping2App::drawGrid(float size, float step)
{
	gl::color(Colorf(0.2f, 0.2f, 0.2f));
	for (float i = -size; i <= size; i += step) {
		gl::drawLine(ci::Vec3f(i, 0.0f, -size), ci::Vec3f(i, 0.0f, size));
		gl::drawLine(ci::Vec3f(-size, 0.0f, i), ci::Vec3f(size, 0.0f, i));
	}
}

//void ProjectionMapping2App::reload()
//{
//	// now tell our Camera that the window aspect ratio has changed
//	cam.setPerspective(60, getWindowAspectRatio(), 1, 1000);
//
//	// and in turn, let OpenGL know we have a new camera
//	gl::setMatrices(cam);
//}

void ProjectionMapping2App::fileDrop(FileDropEvent event)
{
	try {
		TurnCube_mTexture = gl::Texture(loadImage(event.getFile(0)));
	}
	catch (...) {
		console() << "unable to load the texture file!" << std::endl;
	};
}

void ProjectionMapping2App::loadMovieFile(const fs::path &moviePath)
{
	try {
		window_movie = qtime::MovieSurface(moviePath);

		console() << "Dimensions:" << window_movie.getWidth() << " x " << window_movie.getHeight() << std::endl;
		console() << "Duration:  " << window_movie.getDuration() << " seconds" << std::endl;
		console() << "Frames:    " << window_movie.getNumFrames() << std::endl;
		console() << "Framerate: " << window_movie.getFramerate() << std::endl;
		console() << "Alpha channel: " << window_movie.hasAlpha() << std::endl;
		console() << "Has audio: " << window_movie.hasAudio() << " Has visuals: " << window_movie.hasVisuals() << std::endl;
		window_movie.setLoop(true, true);
		window_movie.seekToStart();
		window_movie.play();
	}
	catch (...) {
		console() << "Unable to load the movie." << std::endl;
	}
}

void ProjectionMapping2App::DrawGaussian(){

	Color color[5] = { { 255, 255, 255 }, { 200, 200, 200 }, { 150, 150, 150 }, { 100, 100, 100 }, { 20, 20, 20 } };

	ci::Vec2f soul_Pos_xy;
	double soul_D = 0;

	for (int i = -3; i <= 3; i++){
		for (int j = -3; j <= 3; j++){
			soul_D = sqrt((i*i) + (j*j));

			if (soul_D < sqrt(13)){
				soul_D = ceil(soul_D);
				soul_Pos_xy[0] = soul_Pos[0] + i; soul_Pos_xy[1] = soul_Pos[1] + j;
				gl::color(color[(int)soul_D]);
				gl::drawSolidCircle(soul_Pos_xy, 1.5);
			}
		}
	}
}

void ProjectionMapping2App::resetup(int re_sw){
	/*BasicApp*/
	if (re_sw == 1){
		//set up the camera
		cam.setEyePoint(ci::Vec3f(BasicApp_N / 2, 200, BasicApp_N / 2));
		cam.setCenterOfInterestPoint(ci::Vec3f(BasicApp_N / 2, 0.0f, BasicApp_N / 2));
		cam.setPerspective(60.0f, getWindowAspectRatio(), 1.0f, 200.0f);
		mMayaCam.setCurrentCam(cam);

		//set window size
		setWindowSize(1000, 800);

		DIFFUSE = true;
		AMBIENT = true;
		SPECULAR = true;
		EMISSIVE = true;

		//init
		for (int i = 0; i < 1; i++){
			for (int j = 0; j < BasicApp_N; j++){
				for (int k = 0; k < BasicApp_N; k++){
					BasicApp_pos[i][j][k] = 50;
				}
			}
		}
		for (int j = 0; j < BasicApp_N; j++){
			for (int k = 0; k < BasicApp_N; k++){
				BasicApp_pos[1][j][k] = 0;
			}
		}
	}

	/*fireApp*/
	else if (re_sw == 2){
		fireApp_x = fireApp_y = 0;
		for (int i = 0; i < 32; i++){
			fireColor[i][0] = i * 8;
			fireColor[i][1] = 0;
			fireColor[i][2] = 0;
		}
		for (int i = 32; i < 64; i++){
			fireColor[i][0] = 255;
			fireColor[i][1] = (i - 32) * 8;
			fireColor[i][2] = 0;
		}
		for (int i = 64; i < 128; i++){
			fireColor[i][0] = 255;
			fireColor[i][1] = 255;
			fireColor[i][2] = (i - 64) * 4;
		}
//		setWindowSize(1000, 800);
		mSeed = clock() & 65535;
		mOctaves = 4;
		mTime = 0.0f;
		mFrequency = 1 * 50 / 200.0f;
#pragma omp parallel
		{
#pragma omp for
			for (int i = 0; i < fireApp_N + fireApp_buff; i++){
				for (int j = 0; j < fireApp_N; j++){
					fireApp_pos[i][j] = 0;
					smokePos[i][j] = 0;
				}
			}
		}
	}

	/*PenkiApp*/
	else if (re_sw == 3){
		mt.seed(rnd());
	}

	/*TurnCubeApp*/
	else if (re_sw == 4){
		//画像読み込み
		//fs::path path = getOpenFilePath("movie_snap.png", ImageIo::getLoadExtensions());
		TurnCube_mTexture = gl::Texture(loadImage(loadAsset("movie_snap.png")));

		TurnCube_DIFFUSE = true;
		TurnCube_AMBIENT = true;
		TurnCube_SPECULAR = true;
		TurnCube_EMISSIVE = true;

		//視点設定
		cam.setEyePoint(ci::Vec3f(85.5, -47.5, 200));
		cam.setCenterOfInterestPoint(ci::Vec3f(85.5, -47.5, 0));
		cam.setPerspective(60.0f, getWindowAspectRatio(), 1.0f, 200.0f);
		mMayaCam.setCurrentCam(cam);

		setWindowSize(1000, 800);
		glEnable(GL_TEXTURE_2D);
		gl::enableDepthRead();
		gl::enableDepthWrite();
	}

	/*Shabon*/
	else if (re_sw == 5){
		setWindowSize(1000, 800);
		//座標の設定
		for (int i = 0; i < Shabon_N; i++){
			Shabon_kyu[i][0][0] = randFloat(272, 421);
			Shabon_kyu[i][0][1] = randFloat(149, 300);
			Shabon_kyu[i][0][2] = 0;
			//初期速度と色情報
			for (int j = 0; j < 3; j++){
				Shabon_kyu[i][1][j] = 0;
			}
			Shabon_kyu[i][2][0] = randFloat(0, 1);
			Shabon_kyu[i][2][1] = randFloat(0.4, 1);
			Shabon_kyu[i][2][2] = randFloat(1, 1);
		}
	}

	/*window*/
	else if (re_sw == 6){
		cam.setEyePoint(ci::Vec3f(320, 700, 240));
		cam.setCenterOfInterestPoint(ci::Vec3f(320, 0, 240));
		cam.setPerspective(80.0f, getWindowAspectRatio(), 1.0f, 700.0f);
		mMayaCam.setCurrentCam(cam);

		fs::path a_moviePath("C:\\cinder_0.8.6_vc2013\\project\\ProjectionMapping2\\resources\\MeditationVideo.mp4");
		if (!a_moviePath.empty())
			loadMovieFile(a_moviePath);

		//set window size
		setWindowSize(1280, 800);

		window_DIFFUSE = true;
		window_AMBIENT = true;
		window_SPECULAR = true;
		window_EMISSIVE = true;

		window_r = 0;
		window_flag = 0;
		window_cnt = 0;
	}

	/*movie*/
	else if (re_sw == 7){
		//CameraPersp cam;
		cam.setEyePoint(ci::Vec3f(320, 700, 240));
		cam.setCenterOfInterestPoint(ci::Vec3f(320, 0, 240));
		cam.setPerspective(80.0f, getWindowAspectRatio(), 1.0f, 700.0f);
		mMayaCam.setCurrentCam(cam);

		if (avi == 1){
			fs::path avi_moviePath("C:\\cinder_0.8.6_vc2013\\project\\ProjectionMapping2\\resources\\fire_water.avi");
			if (!avi_moviePath.empty())
				loadMovieFile(avi_moviePath);
		}
		else if (avi == 2){
			fs::path avi_moviePath("C:\\cinder_0.8.6_vc2013\\project\\ProjectionMapping2\\resources\\water_window.avi");
			if (!avi_moviePath.empty())
				loadMovieFile(avi_moviePath);
		}
		else if (avi == 3){
			fs::path avi_moviePath("C:\\cinder_0.8.6_vc2013\\project\\ProjectionMapping2\\resources\\openingMovie.mp4");
			if (!avi_moviePath.empty())
				loadMovieFile(avi_moviePath);
		}
	}

	/*soul*/
	else if (re_sw == 8){
		gl::clear(Color(0, 0, 0));
		soul_Pos[0] = 271;		soul_Pos[1] = 148;
		xyLeftUp[0] = 271;		xyLeftUp[1] = 148;
		xyRightDown[0] = 442;	xyRightDown[1] = 301;
	}

	time_start = clock();

}

CINDER_APP_NATIVE( ProjectionMapping2App, RendererGl )