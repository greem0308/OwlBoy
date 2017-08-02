#pragma once
#include "item.h"

class fruit0 : public item
{
public:
	bool gravityOpen; // �߷� ��������.. ������ ����. 
	int holdFrame; // �Ⱦ�������. 
	int holdCount; // ��� ���� Ƚ�� ���������. 
	bool hold; // �ȷο�� ���� �ƴϰ�, ��ѳ�?

	bool follow; // �÷��̾ ����ٴϳ�?
	bool cast;  //������?
	float castGravity;// ���� ������ų �߷�.
	bool showCurve; // Ŀ�� �����ٶ� ���� ��.

	bool fruit0eat; // �Ծ���? ���� ������ ���� �ִ� ������ �� ����. ��
	int eatFrame;

	struct tagCurveLine
	{
		RECT rc;
		FLOAT angle;
		FLOAT x;
		FLOAT y;
	};
	tagCurveLine curveLine[50];

	virtual HRESULT init(float x, float y);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);
	virtual void eat(void);

	virtual void PixelCollision(void);
	virtual void CastFunc();
	virtual void CurveLineFunc(void); // ������ ����.


	fruit0();
	~fruit0();
};

