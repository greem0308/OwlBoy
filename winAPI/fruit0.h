#pragma once
#include "item.h"

class fruit0 : public item
{
public:
	bool follow; // 플레이어를 따라다니냐?
	bool cast;  //던졌냐?
	float castGravity;// 점점 증가시킬 중력.
	bool showCurve; // 커브 보여줄때 구분 불.

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

	virtual void PixelCollision(void);
	virtual void CastFunc();
	virtual void CurveLineFunc(void); // 던질떄 궤적.


	fruit0();
	~fruit0();
};

