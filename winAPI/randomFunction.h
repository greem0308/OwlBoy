#pragma once

#include "singletonBase.h"
#include <iostream>
#include <time.h>

class randomFunction : public singletonBase <randomFunction>
{
public:

	randomFunction(void)
	{
		//srand(time(NULL)); ���úη� ����...
		srand(GetTickCount());
		//��ǻ�� �ð�... ���������� �ð��� ���..
	}
	~randomFunction(void)
	{
	}

	//getInt
	int getInt(int num) {return rand() % num;}
	int getFromIntTo(int fromNum, int toNum)
	{
		return (rand() % (toNum - fromNum + 1)) + fromNum;
	}

	//rand �Լ��� �ִ밪 32767
	float getFloat() {return ((float)rand() / (float)RAND_MAX);}

	float getFloat(float num)
	{
		return ((float)rand() / (float)RAND_MAX) * num;
	}

	float getFromFloatTo(float fromFloat, float toFloat)
	{
		float rnd = (float)rand() / (float)RAND_MAX;
		return (rnd * (toFloat - fromFloat) + fromFloat);
	}
};


