#pragma once

#include "gamenode.h"
#include "animation.h"
#include <vector>
#include "effectManager.h"

struct tagBullet
{
	image* img;

	image* img2;
	RECT rc;
	int radius;
	float speed;
	float x, y;
	float fireX, fireY;
	float angle;
	float count;
	bool fire;
	animation* curAnimation;
};

//미리 만들어 놓고 쓰는 총알
class bullet : public gameNode
{
private:
	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;

	float _range;

public:
	virtual HRESULT init(int bulletMax, float range);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void fire(float x, float y);
	virtual void move(void);
	virtual void remove(void);
	virtual void draw(void);

	bullet(void);
	virtual ~bullet(void);
};

// enemyShip
//공용으로 쓰는 총알 (쏠때마다 만들고 삭제한다)
class bulletM1 : public gameNode
{
private:
	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;

	const char* _imageName;
	float _range;
	int _bulletMax;

public:
	virtual HRESULT init(int bulletMax, float range);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void fire(float x, float y,
		float speed,float angle);
	virtual void move(void);
	virtual void draw(void);

	void removeBullet(int arrNum);
	vector<tagBullet> getVBullet(void) { return _vBullet; }

	bulletM1(void);
	virtual ~bulletM1(void);
};

// shooter
//공용으로 쓰는 총알 (쏠때마다 만들고 삭제한다)
class bulletM2 : public gameNode
{
private:
	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;

	const char* _imageName;
	float _range;
	int _bulletMax;

public:
	virtual HRESULT init(int bulletMax, float range);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void fire(float x, float y, float angle,
		float speed);
	virtual void move(void);
	virtual void draw(void);

	void removeBullet(int arrNum);
	vector<tagBullet> getVBullet(void) { return _vBullet; }

	bulletM2(void);
	virtual ~bulletM2(void);
};


#define ANICOUNT 3.0f

//쏠때마다 만들고 삭제하는 미사일! // player
class missileM1 : public gameNode
{
private:
	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;

	float _range;
	int _bulletMax;
	int onceCount; // 한번만 들어오게. 

public:
	effectManager* _efm;
	
	virtual HRESULT init(int bulletMax, float range);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	//int NumY 는 y축 애니메이션 프레임을 얻어온다. 각도에 따라 달라짐. 
	virtual void fire(float x, float y, float angle, int Num,int NumY,float speed);
	virtual void move(void);
	virtual void draw(void);
	virtual void xMissile(int arrNum, int num, bool Check);
	virtual void effectDelete();
	void removeMissile(int arrNum);

	vector<tagBullet> getVBullet(void) { return _vBullet; }

	missileM1(void);
	virtual ~missileM1(void);
};


//쏠때마다 만들고 삭제하는 미사일! // boss
class missileM2 : public gameNode
{
private:
	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;

	float _range;
	int _bulletMax;

public:
	virtual HRESULT init(int bulletMax, float range);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void fire(float x, float y, float angle, int Num, float speed);
	virtual void move(void);
	virtual void draw(void);
	virtual void xMissile(int arrNum, int num, bool Check);

	void removeMissile(int arrNum);

	vector<tagBullet> getVBullet(void) { return _vBullet; }

	missileM2(void);
	virtual ~missileM2(void);
};

