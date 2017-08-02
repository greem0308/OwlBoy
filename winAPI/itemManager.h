#pragma once
#include "gameNode.h"
#include "player.h"
#include "fruit0.h"
#include "fruit1.h"
#include "fruit2.h"
#include "fruit3.h"
#include "glassObject.h"
#include "glassObject2.h"

class item;

class itemManager : public gameNode
{
private:
	typedef vector<item*> VEC_ITEM;
	typedef vector<item*>::iterator VECITER_ITEM;

private:
	VEC_ITEM _vItem;
	VECITER_ITEM _viItem;

	player* otus;

public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	// enemy
	virtual void setFruit0(float x, float y);
	virtual void setFruit1(float x, float y);
	virtual void setFruit2(float x, float y);
	virtual void setFruit3(float x, float y);
	virtual void setGlassObject(float x, float y);
	virtual void setGlassObject2(float x, float y);

	virtual vector<item*> getItem(void) { return _vItem; }
	virtual void setPlayer(player* _player) { otus = _player; }
	virtual void removeItem(int arrNum);

public:
	itemManager();
	~itemManager();
};

