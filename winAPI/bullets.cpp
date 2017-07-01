#include "StdAfx.h"
#include "bullets.h"


bullet::bullet(void)
{
}


bullet::~bullet(void)
{
}

HRESULT bullet::init(int bulletMax, float range)
{
	gameNode::init();

	_range = range;

	for (int i = 0; i < bulletMax; i++)
	{
		tagBullet bullet;
		ZeroMemory(&bullet, sizeof(tagBullet));
		bullet.img = new image;
		bullet.img->init("bullet.bmp", 12, 12, true, RGB(255, 0, 255));
		bullet.speed = 1.1f;
		bullet.fire = false;
		_vBullet.push_back(bullet);
	}

	return S_OK;
}

void bullet::release(void)
{
	gameNode::release();

	remove();
}

void bullet::remove(void)
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		SAFE_DELETE(_viBullet->img);
	}

	_vBullet.clear();
}

void bullet::update(void)
{
	gameNode::update();

	move();
}

void bullet::render(void)
{
	draw();
}

void bullet::fire(float x, float y)
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		if (_viBullet->fire) continue;
		_viBullet->fire = true;
		_viBullet->x = _viBullet->fireX = x;
		_viBullet->y = _viBullet->fireY = y;
		_viBullet->rc = RectMakeCenter(x, y,
			_viBullet->img->getWidth(),
			_viBullet->img->getHeight());
		break;
	}
}

//총알 이동
void bullet::move(void)
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		if (!_viBullet->fire) continue;
		_viBullet->y -= _viBullet->speed;
		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y,
			_viBullet->img->getWidth(),
			_viBullet->img->getHeight());

		//사거리 밖으로 나감.!
		if (_range < getDistance(_viBullet->fireX, _viBullet->fireY, _viBullet->x, _viBullet->y))
		{
			_viBullet->fire = false;
		}
	}
}

//총알 그리기
void bullet::draw(void)
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		if (!_viBullet->fire) continue;

		_viBullet->img->render(getMemDC(), _viBullet->rc.left, _viBullet->rc.top);
	}
}

bulletM1::bulletM1(void)
{
}

bulletM1::~bulletM1(void)
{
}

//공용으로 쓰는 총알 (쏠때마다 만들고 삭제한다)
HRESULT bulletM1::init(int bulletMax, float range)
{
	gameNode::init();

	_bulletMax = bulletMax;
	_range = range;

	return S_OK;
}

void bulletM1::release(void)
{
	gameNode::release();

	_vBullet.clear();
}

void bulletM1::update(void)
{
	gameNode::update();

	move();
}

void bulletM1::render(void)
{
	draw();
}

void bulletM1::fire(float x, float y, float angle,
	float speed)
{
	//발사 갯수 제한 한다
	if (_bulletMax < _vBullet.size()) return;

	tagBullet bullets;
	ZeroMemory(&bullets, sizeof(tagBullet));
	
	bullets.img = IMAGEMANAGER->addFrameImage("bomb","bomb.bmp", 96*2, 16*2, 6, 1, true, RGB(255, 0, 255));
	bullets.speed = speed;
	bullets.angle = angle;
	bullets.radius = bullets.img->getWidth() / 2;
	bullets.x = bullets.fireX = x;
	bullets.y = bullets.fireY = y;
	bullets.rc = RectMakeCenter(bullets.x, bullets.y,
		bullets.img->getFrameWidth(), bullets.img->getFrameHeight());

	KEYANIMANAGER->addCoordinateFrameAnimation("bulletFire", "bomb", 0, 3, 15, false, true);
	KEYANIMANAGER->addCoordinateFrameAnimation("bulletDie", "bomb", 4, 5, 30, false, true);
	
	bullets.curAnimation = KEYANIMANAGER->findAnimation("bulletFire");
	bullets.curAnimation->start();
	_vBullet.push_back(bullets);
}


void bulletM1::move(void)
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end();)
	{
		
		_viBullet->x += cosf(_viBullet->angle) * _viBullet->speed;
		_viBullet->y += -sinf(_viBullet->angle) * _viBullet->speed;

		
		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y,
		_viBullet->img->getWidth(), _viBullet->img->getFrameHeight());

		//사거리 밖으로 나가면...
		if (_range-30 < getDistance(_viBullet->fireX,
			_viBullet->fireY, _viBullet->x, _viBullet->y))
		{
			_viBullet->curAnimation = KEYANIMANAGER->findAnimation("bulletDie");
			_viBullet->curAnimation->start();
		}
	
		//사거리 밖으로 나가면...
		if (_range < getDistance(_viBullet->fireX,
			_viBullet->fireY, _viBullet->x, _viBullet->y))
		{
			_viBullet = _vBullet.erase(_viBullet++);
		}
		else ++_viBullet;
	}
}

void bulletM1::draw(void)
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		_viBullet->img->aniRender(getMemDC(), _viBullet->rc.left, _viBullet->rc.top, _viBullet->curAnimation);
	}
}

void bulletM1::removeBullet(int arrNum)
{
	_vBullet.erase(_vBullet.begin() + arrNum);
}

missileM1::missileM1(void)
{
}


missileM1::~missileM1(void)
{
}

HRESULT missileM1::init(int bulletMax, float range)
{
	gameNode::init();

	//사거리
	_range = range;

	//무기 최대 갯수
	_bulletMax = bulletMax;

	return S_OK;
}

void missileM1::release(void)
{
	gameNode::release();

	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end();)
	{
		SAFE_DELETE(_viBullet->img);
	}

	_vBullet.clear();
}

void missileM1::update(void)
{
	gameNode::update();

	move();
}

void missileM1::render(void)
{
	draw();
}

//뽜이야~~~~!!
void missileM1::fire(float x, float y,float angle,int Num)
{
	//최대 발사 갯수 제한
	if (_bulletMax <= _vBullet.size()) return;

	tagBullet bullet;
	ZeroMemory(&bullet, sizeof(tagBullet));

	bullet.img = new image;
	bullet.img->init("player/bullet.bmp", 80, 80, 1, 1, true, RGB(255, 0, 255));
	bullet.speed = 5.0f;
	bullet.angle = angle;
	bullet.x = bullet.fireX = x + cosf(bullet.angle)*30;
	bullet.y = bullet.fireY = y - sinf(bullet.angle) * 30;
	bullet.rc = RectMakeCenter(x, y, bullet.img->getFrameWidth(),
		bullet.img->getFrameHeight());
	bullet.img->setFrameX(Num);
	_vBullet.push_back(bullet);
}

void missileM1::move(void)
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end();)
	{
		_viBullet->x += cosf(_viBullet->angle) * _viBullet->speed;
		_viBullet->y += -sinf(_viBullet->angle) * _viBullet->speed;
		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y, 
			_viBullet->img->getFrameWidth(),
		_viBullet->img->getFrameHeight());

		if (_range - 10 < getDistance(_viBullet->fireX, _viBullet->fireY, _viBullet->x, _viBullet->y))
		{
			_viBullet->img->setFrameX(1);
		}

		if (_range < getDistance(_viBullet->fireX, _viBullet->fireY, _viBullet->x, _viBullet->y))
		{
			SAFE_DELETE(_viBullet->img);
			_viBullet = _vBullet.erase(_viBullet);
		}
		else ++_viBullet;
	}
}

void missileM1::draw(void)
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		_viBullet->img->frameRender(getMemDC(), _viBullet->rc.left, _viBullet->rc.top,
			_viBullet->img->getFrameX(), 0);
	}
}

void missileM1::removeMissile(int arrNum)
{
	_vBullet.erase(_vBullet.begin() + arrNum);
}

void missileM1::xMissile(int arrNum, int num, bool Check)
{
	if (Check)_vBullet[arrNum].x += num;
	else _vBullet[arrNum].x -= num;
}