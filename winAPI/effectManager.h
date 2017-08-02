#pragma once
#include "gameNode.h"
#include "playerEffect.h"
//#include "player.h"

class effect;

class effectManager : public gameNode
{
private:
	typedef vector<effect*> VEC_EFFECT;
	typedef vector<effect*>::iterator VECITER_EFFECT;

	VEC_EFFECT _vMinion;
	VECITER_EFFECT _viMinion;
	//player* otus;
public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void setPlayerEffect(float x,float y);

	virtual void removeMinion(int arrNum);
//	virtual void setPlayer(player* _player) { otus = _player; }

	virtual vector<effect*> getMinion(void) { return _vMinion; }

	effectManager();
	~effectManager();
};

