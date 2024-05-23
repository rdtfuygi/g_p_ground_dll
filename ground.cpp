#include"ground.h"

ground::ground() :site(), door() {}

ground::ground(point ��[16], int ��_1, int ��_2) : site(��)
{
	door[0] = ��_1;
	door[1] = ��_2;
}

ground::ground(std::vector<point>& ��, int ��_1, int ��_2) : site(��)
{
	door[0] = ��_1;
	door[1] = ��_2;
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
void ground::print(cv::InputOutputArray ͼ��, float ����, const cv::Scalar& ��ɫ, int ��ϸ) const
{
	site.print(ͼ��, ����, ��ɫ, ��ϸ);
	get_door(0).print(ͼ��, ����, ��ɫ, ��ϸ * 2);
	get_door(1).print(ͼ��, ����, ��ɫ, ��ϸ * 2);
	//site.center().print(ͼ��, ����, ��ɫ, ��ϸ * 2);
	//site.fast_center().print(ͼ��, ����, cv::Scalar(0, 0, 255), ��ϸ * 2);

}
#endif

void ground::data(std::vector<float>& ����)
{
	���� = std::vector<float>();
	����.reserve(90);
	float �ܳ� = 0;
	for (int i = 0; i < 16; i++)
	{
		����.push_back(site[i].origin[0] / 512);
		����.push_back(site[i].origin[1] / 512);
		����.push_back(site[i].dir[0]);
		����.push_back(site[i].dir[1]);
		����.push_back(site[i].dist / 512);

		�ܳ� += site[i].dist;
	}
	point ���� = site.fast_center();
	����.push_back(����[0] / 512);
	����.push_back(����[1] / 512);
	����.push_back(float(door[0]) / 32);
	����.push_back(float(door[1]) / 32);
	����.push_back(area() / 1024 / 1024);
	����.push_back(area() / 1024 / 1024);
	����.push_back(fmaxf(�ܳ� / 4 / sqrt(area()), 1) - 1);
	����.push_back(0.0f);
	����.push_back(0.0f);
	����.push_back(0.0f);
}


building::building() :ground(), fun(0), target_area(0) {}

building::building(point ��[16], int ��_1, int ��_2, int ����, float Ŀ���С) :ground(��, ��_1, ��_2), fun(����), target_area(Ŀ���С) {}

building::building(std::vector<point>& ��, int ��_1, int ��_2, int ����, float Ŀ���С) :ground(��, ��_1, ��_2), fun(����), target_area(Ŀ���С) {}

void building::move(vector �ƶ�, int index)
{
	site[index].origin = point(vector(site[index].origin) + �ƶ�);
}

void building::move(vector �ƶ�[16])
{
	for (int i = 0; i < 16; i++)
	{
		move(�ƶ�[i], i);
	}
	site.reset_seg();
}

void building::move(std::vector<vector>& �ƶ�, std::vector<building>& b, ground a, float& n)
{
	for (int i = 0; i < 16; i++)
	{
		poly temp = site;
		move(�ƶ�[i], i);

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
			n += �ƶ�[i].length();
			site = temp;
		}
	}

}

void building::change(point ��, int index)
{
	site[index].origin = ��;
}

void building::change(point ��[16])
{
	for (int i = 0; i < 16; i++)
	{
		change(��[i], i);
	}
	site.reset_seg();
}

void building::data(std::vector<float>& ����)
{
	���� = std::vector<float>();
	����.reserve(90);

	float �ܳ� = 0;
	for (int i = 0; i < 16; i++)
	{
		����.push_back(site[i].origin[0] / 512);
		����.push_back(site[i].origin[1] / 512);
		����.push_back(site[i].dir[0]);
		����.push_back(site[i].dir[1]);
		����.push_back(site[i].dist / 512);

		�ܳ� += site[i].dist;
	}
	point ���� = site.fast_center();
	����.push_back(����[0] / 512);
	����.push_back(����[1] / 512);
	����.push_back(float(door[0]) / 32);
	����.push_back(float(door[1]) / 32);
	����.push_back(area() / 1024 / 1024);
	����.push_back(target_area / 1024 / 1024);
	����.push_back(fmaxf(�ܳ� / 4 / sqrt(area()), 1) - 1);	
	����.push_back(0.0f);
	����.push_back(0.0f);
	����.push_back(0.0f);
}


building ͣ��������(building �ּ���)
{
	seg ƽ��[5];
	float ��ת = 90;
	if (�ּ���.site.dir_area() > 0)
	{
		��ת = -90;
	}

	for (int i = 0; i < 5; i++)
	{
		ƽ��[i].origin = rotate(�ּ���.site[i].origin, �ּ���.site[i].point_get(27), ��ת);
		ƽ��[i].dir = �ּ���.site[i].dir;
		ƽ��[i].dist = �ּ���.site[i].dist;
	}



	point ��[16];
	for (int i = 0; i < 5; i++)
	{
		��[i] = �ּ���.site[i].origin;
	}
	��[5] = ƽ��[4].end();
	for (int i = 1; i < 4; i++)
	{
		��[i + 5] = cross(line(ƽ��[5 - i]), line(ƽ��[5 - i - 1]));
	}
	��[9] = ƽ��[0].origin;

	for (int i = 8; i < 16; i++)
	{
		��[i] = ��[0];
	}


	return building(��, 0, 4, fun_port, 0);
}



const char ������[8][8] =
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
���_Ȩ�� = 0,
ƽֱ��_Ȩ�� = 0,
����_Ȩ�� = 0,
��_Ȩ�� = 0,
�ܳ�_Ȩ�� = 0,
�Ƿ�_Ȩ�� = 0;

float ��������(ground ����, std::vector<building>& ����, bool& reset)
{
	float ���� = 0;




	for (int i = 0; i < ����.size(); i++)
	{
		float ��� = ����[i].area();

		for (int j = 0; j < i; j++)
		{
			���� += -dist(����[i].site, ����[j].site) * ������[����[i].fun][����[j].fun] * ����_Ȩ��;

			���� += -fminf(dist(����[j].site, ����[i].get_door(0)), dist(����[j].site, ����[i].get_door(1))) * ������[����[i].fun][����[j].fun] * ��_Ȩ��;
		}

		���� += (1 - powf(1 - ��� / ����[i].target_area, 2) + fminf(1 - ��� / ����[i].target_area, 0)) * ���_Ȩ��;
		printf("%.3f,", ��� / ����[i].target_area);

		float �ܳ� = 0;
		for (int j = 0; j < 16; j++)
		{
			float a = fmaxf(fmaxf((����[i].site[j].dir * ����[i].site[(j + 1) % 16].dir), 0), abs(����[i].site[j].dir ^ ����[i].site[(j + 1) % 16].dir));
			���� += (a + powf(a, 16)) / 2 / 16 * ƽֱ��_Ȩ��;

			�ܳ� += ����[i].site[j].dist;


			for (int k = 0; k < i; k++)
			{
				for (int l = 0; l < 16; l++)
				{
					���� += -exp(-����[k].site[l].point_dist(����[i].site[j].origin) * 4) * �Ƿ�_Ȩ��;
				}
			}

			for (int k = 0; k < 16; k++)
			{
				if ((k == j) && (k == ((j + 15) % 16)))
				{
					continue;
				}
				���� += -exp(-����[i].site[k].point_dist(����[i].site[j].origin) / 4) * �Ƿ�_Ȩ��;
			}

			for (int k = 0; k < 16; k++)
			{
				���� += -exp(-����.site[k].point_dist(����[i].site[j].origin) / 4) * �Ƿ�_Ȩ��;
			}
		}

		���� += (1 - fmaxf(�ܳ� / 4 / sqrt(���), 1)) * �ܳ�_Ȩ��;

	}
	printf("\n");
	return ���� / 8;
}

void �ֿ����_����(std::vector<float>& �ֿ����, std::vector<float>& ������_, std::vector<float>& ��������_, std::vector<char>& �������, std::vector<float>& �ֿ��޸�)
{
	�ֿ���� = { 0,0,0 };
	for (int i = 0; i < ������_.size(); i++)
	{
		�ֿ����[�������[i]] += (������_[i] + ��������_[i]) / �ֿ��޸�[�������[i]];
	}
}

void ����趨(std::vector<building>& ����, float ������, std::vector<float>& �ֿ����)
{
	���� = std::vector<building>(8);

	����[0].target_area = ������ / 30 / 365 / 24 * 3 * 35;
	����[1].target_area = ������ / 30 / 365 / 24 * 3 * 20;
	����[2].target_area = �ֿ����[0] * 1.7f;
	����[3].target_area = �ֿ����[1] * 1.7f;
	����[4].target_area = �ֿ����[2] * 1.7f;
	����[5].target_area = 5000;
	����[6].target_area = ������ / 30 / 365 / 24 * (3 + 2) * 10;
	����[7].target_area = (500 * 0.7f + ������ / 30 / 365 / 24 * 3 * 0.3f) * 40;

	for (int i = 0; i < ����.size(); i++)
	{
		����[i].fun = i;
	}
}
