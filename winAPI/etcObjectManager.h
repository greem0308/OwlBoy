#pragma once
#include "gameNode.h"
#include "player.h"

class etcObject;

class etcObjectManager : public gameNode
{
private:
	typedef vector<etcObject*> VEC_OBJ;
	typedef vector<etcObject*>::iterator VECITER_OBJ;

private:
	VEC_OBJ _vObj;
	VECITER_OBJ _viObj;

	player* otus;

public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	// Obj
//	virtual void setObj1(float x, float y);
	
	virtual vector<etcObject*> getItem(void) { return _vObj; }
	virtual void setPlayer(player* _player) { otus = _player; }
	virtual void removeObj(int arrNum);

public:
	etcObjectManager();
	~etcObjectManager();
};

