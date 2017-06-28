#pragma once

#include "gamenode.h"
#include "animation.h"
#include <vector>

struct tagBullet
{
	image* img;
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

//�̸� ����� ���� ���� �Ѿ�
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

//�������� ���� �Ѿ� (�򶧸��� ����� �����Ѵ�)
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

	virtual void fire(float x, float y, float angle,
		float speed);
	virtual void move(void);
	virtual void draw(void);

	void removeBullet(int arrNum);
	vector<tagBullet> getVBullet(void) { return _vBullet; }

	bulletM1(void);
	virtual ~bulletM1(void);
};

#define ANICOUNT 3.0f

//�򶧸��� ����� �����ϴ� �̻���!
class missileM1 : public gameNode
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

	virtual void fire(float x, float y, float angle,int Num);
	virtual void move(void);
	virtual void draw(void);
	virtual void xMissile(int arrNum, int num, bool Check);

	void removeMissile(int arrNum);

	vector<tagBullet> getVBullet(void) {return _vBullet;}

	missileM1(void);
	virtual ~missileM1(void);
};

