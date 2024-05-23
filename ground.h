#pragma once

#ifdef GPGROUNDDLL_EXPORTS
#define DLL_ __declspec(dllexport)
#else
#define DLL_ __declspec(dllimport)//����
#endif


#define _USE_MATH_DEFINES 

#ifndef geometry
#define geometry
#include "geometry.h"
#endif



#include <cmath>
#include <vector>

//#define no_opencv 1

#ifndef no_opencv
#include <opencv.hpp>
#endif // no_opencv




class DLL_ ground
{
public:
	int door[2];
	poly site;
	ground();
	ground(point ��[16], int ��_1, int ��_2);
	ground(std::vector<point>& ��, int ��_1, int ��_2);

	seg get_door(int i = 0) const;

	float area() const;

#ifndef no_opencv
	void print(cv::InputOutputArray ͼ��, float ����, const cv::Scalar& ��ɫ, int ��ϸ = 1) const;
#endif // no_opencv

	void data(std::vector<float>& ����);
};

const int
fun_port = 0,
fun_sort = 1,
fun_ware = 2,
fun_cold = 3,
fun_freezing = 4,
fun_adm = 5,
fun_live = 6,
fun_port_2 = 7;


class DLL_ building :public ground
{
public:
	int fun;
	float target_area;
	building();
	building(point ��[16], int ��_1, int ��_2, int ����, float Ŀ���С);
	building(std::vector<point>& ��, int ��_1, int ��_2, int ����, float Ŀ���С);
	void move(vector �ƶ�, int index);
	void move(vector �ƶ�[16]);
	void move(std::vector<vector>& �ƶ�, std::vector<building>& b, ground a,float& n);
	void change(point ��, int index);
	void change(point ��[16]);

	void data(std::vector<float>& ����);
};


//void ��������(std::vector<building>& ����, std::vector<vector>& �ƶ�);

DLL_ building ͣ��������(building �ּ���);

extern float
���_Ȩ��,
ƽֱ��_Ȩ��,
����_Ȩ��,
��_Ȩ��,
�ܳ�_Ȩ��,
�Ƿ�_Ȩ��;

DLL_ float ��������(ground ����, std::vector<building>& ����, bool& reset);

DLL_ void �ֿ����_����(std::vector<float>& �ֿ����, std::vector<float>& ������_, std::vector<float>& ��������_, std::vector<char>& �������, std::vector<float>& �ֿ��޸�);

DLL_ void ����趨(std::vector<building>& ����, float ������, std::vector<float>& �ֿ����);



DLL_ float norm_quantile(float a);

DLL_ float ��������(float ��������, float �����ɱ�, float ���гɱ�);

DLL_ float ������(float ��������, float ���󷽲�, float ��ǰ��, float ��ǰ�ڷ���, float ����ˮƽ);

DLL_ ground �����趨(float ���);

DLL_ void Ȩ�ص���_();