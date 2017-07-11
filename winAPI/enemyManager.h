#pragma once
#include "gameNode.h"
#include "player.h"
#include "shooter.h"
#include "eventShip.h"

class enemy;

class enemyManager : public gameNode
{
private:
	typedef vector<enemy*> VEC_ENEMY;
	typedef vector<enemy*>::iterator VECITER_ENEMY;

private:
	VEC_ENEMY _vMinion;
	VECITER_ENEMY _viMinion;

	player* otus;

public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	// enemy
	virtual void setShooter(float x, float y);
	virtual void setEventShip(float x, float y);

	virtual vector<enemy*> getMinion(void) { return _vMinion; }
	virtual void setPlayer(player* _player) { otus = _player; }
	virtual void removeMinion(int arrNum);

	enemyManager();
	~enemyManager();
};

