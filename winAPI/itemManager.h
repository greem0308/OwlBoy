#pragma once
#include "gameNode.h"
#include "fruit0.h"
#include "player.h"

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

	virtual vector<item*> getItem(void) { return _vItem; }
	virtual void setPlayer(player* _player) { otus = _player; }
	virtual void removeItem(int arrNum);

public:
	itemManager();
	~itemManager();
};

