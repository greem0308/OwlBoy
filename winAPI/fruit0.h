#pragma once
#include "item.h"

class fruit0 : public item
{
public:
	bool gravityOpen; // 중력 줄지말지.. 잡으면 안줌. 
	int holdFrame; // 안쓰고있음. 
	int holdCount; // 잡아 당기는 횟수 계산을위한. 
	bool hold; // 팔로우는 아직 아니고, 잡앗냐?

	bool follow; // 플레이어를 따라다니냐?
	bool cast;  //던졌냐?
	float castGravity;// 점점 증가시킬 중력.
	bool showCurve; // 커브 보여줄때 구분 불.

	bool fruit0eat; // 먹었냐? 과일 먹을때 각도 주는 프레임 을 위한. 불
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
	virtual void CurveLineFunc(void); // 던질떄 궤적.


	fruit0();
	~fruit0();
};

