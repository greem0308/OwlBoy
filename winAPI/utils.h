#pragma once

#include <math.h>

#define DEG_TO_RAD 0.017453f
#define PI 3.141592654f
#define PI2 6.283185f
#define PI_2 float(PI / 2.0f)
#define PI_4 float(PI / 4.0f)
#define PI_8 float(PI / 8.0f)
#define PI_16 float(PI / 16.0f)
#define PI_32 float(PI / 32.0f)
#define PI_64 float(PI / 64.0f)
#define PI_128 float(PI / 128.0f)
#define PI_256 float(PI / 256.0f)

#define FLOAT_EPSILON 0.001f

//�Ǽ��� ���������� ��ȯ�Ѵ�..!!
#define FLOAT_TO_INT(f1) static_cast<int>(f1 + FLOAT_EPSILON)

//�� �Ǽ��� ������ Ȯ���Ѵ�!!
#define FLOAT_EQUAL(f1, f2) (fabs(f1 - f2) <= FLOAT_EPSILON)
//fabs�� �Ҽ����� �� ���밪�� ��ȯ�ϴ� �Լ��̴�.

namespace MY_UTIL
{
	//�Ÿ� ���Ѵ�
	float getDistance(float startX, float startY, float endX, float endY);

	//���������� �� �������� ���� �������� ���Ѵ�
	float getAngle(float startX, float startY, float endX, float endY);
}