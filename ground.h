#pragma once

#ifdef GPGROUNDDLL_EXPORTS
#define DLL_ __declspec(dllexport)
#else
#define DLL_ __declspec(dllimport)//导入
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
	ground(point 点[16], int 门_1, int 门_2);
	ground(std::vector<point>& 点, int 门_1, int 门_2);

	seg get_door(int i = 0) const;

	float area() const;

#ifndef no_opencv
	void print(cv::InputOutputArray 图像, float 比例, const cv::Scalar& 颜色, int 粗细 = 1) const;
#endif // no_opencv

	void data(std::vector<float>& 数据);
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
	building(point 点[16], int 门_1, int 门_2, int 类型, float 目标大小);
	building(std::vector<point>& 点, int 门_1, int 门_2, int 类型, float 目标大小);
	void move(vector 移动, int index);
	void move(vector 移动[16]);
	void move(std::vector<vector>& 移动, std::vector<building>& b, ground a,float& n);
	void change(point 点, int index);
	void change(point 点[16]);

	void data(std::vector<float>& 数据);
};


//void 建筑更改(std::vector<building>& 建筑, std::vector<vector>& 移动);

DLL_ building 停车场设置(building 分拣区);

extern float
面积_权重,
平直角_权重,
距离_权重,
门_权重,
周长_权重,
非法_权重;

DLL_ float 奖励函数(ground 场地, std::vector<building>& 建筑, bool& reset);

DLL_ void 仓库面积_计算(std::vector<float>& 仓库面积, std::vector<float>& 补货点_, std::vector<float>& 订货批量_, std::vector<char>& 库存类型, std::vector<float>& 仓库限高);

DLL_ void 面积设定(std::vector<building>& 建筑, float 总需求, std::vector<float>& 仓库面积);



DLL_ float norm_quantile(float a);

DLL_ float 订货批量(float 年需求量, float 订货成本, float 持有成本);

DLL_ float 补货点(float 年需求量, float 需求方差, float 提前期, float 提前期方差, float 服务水平);

DLL_ ground 场地设定(float 面积);

DLL_ void 权重调整_();