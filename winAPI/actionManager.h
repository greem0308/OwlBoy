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

	// �ǵ��ƿ��� �Լ�
	virtual void moveTo(image* img, float endX, float endY, float time, bool rewind);

	// ������ �Լ��� �����ε� �ؼ� �ݹ��Լ��� �߰��ߴ�. 
	virtual void moveTo(image* img, float endX, float endY, float time, void* cbFuntion);

	// Bezier 
	virtual void moveToBezier(image* img, float endX, float endY, float time);

	// Bezier make
	virtual void moveToBezier(image* img, float twoX, float twoY, float threeX, float threeY, float endX, float endY, float time);

	actionManager();
	~actionManager();
};

