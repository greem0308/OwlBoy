#pragma once
#include "singletonBase.h"
#include <vector>

class action;

class actionManager : public singletonBase <actionManager>
{
private:
	typedef vector<action*> vector_action;
	typedef vector<action*>::iterator iterAction;

private:
	vector_action _vAction;
	iterAction _viAction;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	virtual void moveTo(image* img, float endX, float endY, float time);

	// 되돌아오는 함수
	virtual void moveTo(image* img, float endX, float endY, float time, bool rewind);

	// 기존의 함수에 오버로딩 해서 콜백함수를 추가했다. 
	virtual void moveTo(image* img, float endX, float endY, float time, void* cbFuntion);

	// Bezier 
	virtual void moveToBezier(image* img, float endX, float endY, float time);

	// Bezier make
	virtual void moveToBezier(image* img, float twoX, float twoY, float threeX, float threeY, float endX, float endY, float time);

	actionManager();
	~actionManager();
};

