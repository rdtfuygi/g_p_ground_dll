#include"ground.h"

ground::ground() :site(), door() {}

ground::ground(point 点[16], int 门_1, int 门_2) : site(点)
{
	door[0] = 门_1;
	door[1] = 门_2;
}

ground::ground(std::vector<point>& 点, int 门_1, int 门_2) : site(点)
{
	door[0] = 门_1;
	door[1] = 门_2;
}

seg ground::get_door(int i) const
{
	return site[door[i]];
}

float ground::area() const
{
	return abs(site.dir_area());
}

#ifndef no_opencv
void ground::print(cv::InputOutputArray 图像, float 比例, const cv::Scalar& 颜色, int 粗细) const
{
	site.print(图像, 比例, 颜色, 粗细);
	get_door(0).print(图像, 比例, 颜色, 粗细 * 2);
	get_door(1).print(图像, 比例, 颜色, 粗细 * 2);
	//site.center().print(图像, 比例, 颜色, 粗细 * 2);
	//site.fast_center().print(图像, 比例, cv::Scalar(0, 0, 255), 粗细 * 2);

}
#endif

void ground::data(std::vector<float>& 数据)
{
	数据 = std::vector<float>();
	数据.reserve(90);
	float 周长 = 0;
	for (int i = 0; i < 16; i++)
	{
		数据.push_back(site[i].origin[0] / 512);
		数据.push_back(site[i].origin[1] / 512);
		数据.push_back(site[i].dir[0]);
		数据.push_back(site[i].dir[1]);
		数据.push_back(site[i].dist / 512);

		周长 += site[i].dist;
	}
	point 重心 = site.fast_center();
	数据.push_back(重心[0] / 512);
	数据.push_back(重心[1] / 512);
	数据.push_back(float(door[0]) / 32);
	数据.push_back(float(door[1]) / 32);
	数据.push_back(area() / 1024 / 1024);
	数据.push_back(area() / 1024 / 1024);
	数据.push_back(fmaxf(周长 / 4 / sqrt(area()), 1) - 1);
	数据.push_back(0.0f);
	数据.push_back(0.0f);
	数据.push_back(0.0f);
}


building::building() :ground(), fun(0), target_area(0) {}

building::building(point 点[16], int 门_1, int 门_2, int 类型, float 目标大小) :ground(点, 门_1, 门_2), fun(类型), target_area(目标大小) {}

building::building(std::vector<point>& 点, int 门_1, int 门_2, int 类型, float 目标大小) :ground(点, 门_1, 门_2), fun(类型), target_area(目标大小) {}

void building::move(vector 移动, int index)
{
	site[index].origin = point(vector(site[index].origin) + 移动);
}

void building::move(vector 移动[16])
{
	for (int i = 0; i < 16; i++)
	{
		move(移动[i], i);
	}
	site.reset_seg();
}

void building::move(std::vector<vector>& 移动, std::vector<building>& b, ground a, float& n)
{
	for (int i = 0; i < 16; i++)
	{
		poly temp = site;
		move(移动[i], i);

		site.reset_seg(i);
		site.reset_seg((i + 15) % 16);

		bool m = true;

		//if ((200 < site[(i + 1) % 16].dist) || (200 < site[(i + 15) % 16].dist))
		//{
		//	m = false;
		//}
		if ((inc_angle_cos(site[(i + 1) % 16], site[i % 16]) < -float(M_SQRT1_2)) || (inc_angle_cos(site[i], site[(i + 15) % 16]) < -float(M_SQRT1_2)) || (inc_angle_cos(site[(i + 15) % 16], site[(i + 14) % 16]) < -float(M_SQRT1_2)))
		{
			m = false;
		}
		else if (!site.legal())
		{
			m = false;
		}
		else if (!a.site.full_overlap(site))
		{
			m = false;
		}
		else
		{
			for (int j = 0; j < 8; j++)
			{
				if(j!=fun)
				{
					if (site.is_overlap(b[j].site))
					{
						m = false;
						break;
					}
				}
			}
		}



		if (!m)
		{
			n += 移动[i].length();
			site = temp;
		}
	}

}

void building::change(point 点, int index)
{
	site[index].origin = 点;
}

void building::change(point 点[16])
{
	for (int i = 0; i < 16; i++)
	{
		change(点[i], i);
	}
	site.reset_seg();
}

void building::data(std::vector<float>& 数据)
{
	数据 = std::vector<float>();
	数据.reserve(90);

	float 周长 = 0;
	for (int i = 0; i < 16; i++)
	{
		数据.push_back(site[i].origin[0] / 512);
		数据.push_back(site[i].origin[1] / 512);
		数据.push_back(site[i].dir[0]);
		数据.push_back(site[i].dir[1]);
		数据.push_back(site[i].dist / 512);

		周长 += site[i].dist;
	}
	point 重心 = site.fast_center();
	数据.push_back(重心[0] / 512);
	数据.push_back(重心[1] / 512);
	数据.push_back(float(door[0]) / 32);
	数据.push_back(float(door[1]) / 32);
	数据.push_back(area() / 1024 / 1024);
	数据.push_back(target_area / 1024 / 1024);
	数据.push_back(fmaxf(周长 / 4 / sqrt(area()), 1) - 1);	
	数据.push_back(0.0f);
	数据.push_back(0.0f);
	数据.push_back(0.0f);
}


building 停车场设置(building 分拣区)
{
	seg 平行[5];
	float 旋转 = 90;
	if (分拣区.site.dir_area() > 0)
	{
		旋转 = -90;
	}

	for (int i = 0; i < 5; i++)
	{
		平行[i].origin = rotate(分拣区.site[i].origin, 分拣区.site[i].point_get(27), 旋转);
		平行[i].dir = 分拣区.site[i].dir;
		平行[i].dist = 分拣区.site[i].dist;
	}



	point 点[16];
	for (int i = 0; i < 5; i++)
	{
		点[i] = 分拣区.site[i].origin;
	}
	点[5] = 平行[4].end();
	for (int i = 1; i < 4; i++)
	{
		点[i + 5] = cross(line(平行[5 - i]), line(平行[5 - i - 1]));
	}
	点[9] = 平行[0].origin;

	for (int i = 8; i < 16; i++)
	{
		点[i] = 点[0];
	}


	return building(点, 0, 4, fun_port, 0);
}



const char 关联表[8][8] =
{
	{0, 4, 0, 0, 0, 0, 3, 0},
	{4, 0, 4, 3, 3, 0, 0, 0},
	{0, 4, 0,-1,-1, 0, 0, 0},
	{0, 3,-1, 0, 1,-1,-1, 0},
	{0, 3,-1, 1, 0,-1,-1, 0},
	{0, 0, 0,-1,-1, 0, 3, 3},
	{3, 0, 0,-1,-1, 3, 0, 1},
	{0, 0, 0, 0, 0, 3, 1, 0}
};

float
面积_权重 = 0,
平直角_权重 = 0,
距离_权重 = 0,
门_权重 = 0,
周长_权重 = 0,
非法_权重 = 0;

float 奖励函数(ground 场地, std::vector<building>& 建筑, bool& reset)
{
	float 分数 = 0;




	for (int i = 0; i < 建筑.size(); i++)
	{
		float 面积 = 建筑[i].area();

		for (int j = 0; j < i; j++)
		{
			分数 += -dist(建筑[i].site, 建筑[j].site) * 关联表[建筑[i].fun][建筑[j].fun] * 距离_权重;

			分数 += -fminf(dist(建筑[j].site, 建筑[i].get_door(0)), dist(建筑[j].site, 建筑[i].get_door(1))) * 关联表[建筑[i].fun][建筑[j].fun] * 门_权重;
		}

		分数 += (1 - powf(1 - 面积 / 建筑[i].target_area, 2) + fminf(1 - 面积 / 建筑[i].target_area, 0)) * 面积_权重;
		printf("%.3f,", 面积 / 建筑[i].target_area);

		float 周长 = 0;
		for (int j = 0; j < 16; j++)
		{
			float a = fmaxf(fmaxf((建筑[i].site[j].dir * 建筑[i].site[(j + 1) % 16].dir), 0), abs(建筑[i].site[j].dir ^ 建筑[i].site[(j + 1) % 16].dir));
			分数 += (a + powf(a, 16)) / 2 / 16 * 平直角_权重;

			周长 += 建筑[i].site[j].dist;


			for (int k = 0; k < i; k++)
			{
				for (int l = 0; l < 16; l++)
				{
					分数 += -exp(-建筑[k].site[l].point_dist(建筑[i].site[j].origin) * 4) * 非法_权重;
				}
			}

			for (int k = 0; k < 16; k++)
			{
				if ((k == j) && (k == ((j + 15) % 16)))
				{
					continue;
				}
				分数 += -exp(-建筑[i].site[k].point_dist(建筑[i].site[j].origin) / 4) * 非法_权重;
			}

			for (int k = 0; k < 16; k++)
			{
				分数 += -exp(-场地.site[k].point_dist(建筑[i].site[j].origin) / 4) * 非法_权重;
			}
		}

		分数 += (1 - fmaxf(周长 / 4 / sqrt(面积), 1)) * 周长_权重;

	}
	printf("\n");
	return 分数 / 8;
}

void 仓库面积_计算(std::vector<float>& 仓库面积, std::vector<float>& 补货点_, std::vector<float>& 订货批量_, std::vector<char>& 库存类型, std::vector<float>& 仓库限高)
{
	仓库面积 = { 0,0,0 };
	for (int i = 0; i < 补货点_.size(); i++)
	{
		仓库面积[库存类型[i]] += (补货点_[i] + 订货批量_[i]) / 仓库限高[库存类型[i]];
	}
}

void 面积设定(std::vector<building>& 建筑, float 总需求, std::vector<float>& 仓库面积)
{
	建筑 = std::vector<building>(8);

	建筑[0].target_area = 总需求 / 30 / 365 / 24 * 3 * 35;
	建筑[1].target_area = 总需求 / 30 / 365 / 24 * 3 * 20;
	建筑[2].target_area = 仓库面积[0] * 1.7f;
	建筑[3].target_area = 仓库面积[1] * 1.7f;
	建筑[4].target_area = 仓库面积[2] * 1.7f;
	建筑[5].target_area = 5000;
	建筑[6].target_area = 总需求 / 30 / 365 / 24 * (3 + 2) * 10;
	建筑[7].target_area = (500 * 0.7f + 总需求 / 30 / 365 / 24 * 3 * 0.3f) * 40;

	for (int i = 0; i < 建筑.size(); i++)
	{
		建筑[i].fun = i;
	}
}
