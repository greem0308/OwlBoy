#pragma once
#include "gameNode.h"
#include "player.h"
#include "animation.h"
#include "bullets.h"

enum ENEMYDIRECTION
{
    ERIGHT,ELEFT
};

class enemy : public gameNode
{
public:
	struct ENEMY
	{
		float x, y,angle;
		float fireX, fireY;
		bool hitCheck;
		bool life;
		bool AI;
		int Count;
		int HP;
		RECT HPbar;
		RECT rc;
		bool stoned;
		image* image;
		bulletM1* _bullet; // enemyShip
		bulletM2* _bullet2; // shooter
		ENEMYDIRECTION direction;
	};
	ENEMY _enemy;
	animation* curAnimation;
	player* otus;

	bool Start;
	int Count;

	virtual HRESULT init(float x, float y);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void setPlayer(player* _player) { otus = _player; }

	enemy();
	~enemy();
};

