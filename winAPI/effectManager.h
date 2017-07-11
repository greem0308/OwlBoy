#pragma once
#include "gameNode.h"
#include "playerEffect.h"

class effectManager : public gameNode
{
private:
	typedef vector<effect*> VEC_EFFECT;
	typedef vector<effect*>::iterator VECITER_EFFECT;

public:
	VEC_EFFECT _vMinion;
	VECITER_EFFECT _viMinion;

	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void removeMinion(int arrNum);
	virtual void setPlayerEffect(float x, float y);

	virtual vector<effect*> getMinion(void) { return _vMinion; }

	effectManager();
	~effectManager();
};

