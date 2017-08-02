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
		bullet.img2 = new image;
		bullet.img2->init("bullet.bmp", 12, 12, true, RGB(255, 0, 255));
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


///////////////////////////////////////////////////////////////////////////////////////////////////////enemyShip
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

	bullets.img = IMAGEMANAGER->addFrameImage("cannonBall", "Scene/event/CannonBall.bmp", 640*2, 656*2, 16, 16, true, RGB(255, 0, 255));
	bullets.speed = speed;
	bullets.angle = angle;
	bullets.radius = 30;
	bullets.x = bullets.fireX = x;
	bullets.y = bullets.fireY = y;
	bullets.rc = RectMakeCenter(bullets.x, bullets.y,
	60,60);

	KEYANIMANAGER->addCoordinateFrameAnimation("bulletFire", "cannonBall", 0, 60, 15, false, true);
	KEYANIMANAGER->addCoordinateFrameAnimation("bulletDie", "cannonBall", 4, 50, 30, false, true);

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

		_viBullet->rc = RectMakeCenter(_viBullet->x+30, _viBullet->y+30,
			_viBullet->img->getFrameWidth()/1.5, _viBullet->img->getFrameHeight()/ 1.5);

		//사거리 밖으로 나가면...
		if (_range - 10 < getDistance(_viBullet->fireX,
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
		_viBullet->img->aniRender(getMemDC(), _viBullet->rc.left-10, _viBullet->rc.top-10, _viBullet->curAnimation);
		//Rectangle(getMemDC(),_viBullet->rc.left, _viBullet->rc.top, _viBullet->rc.right, _viBullet->rc.bottom);
	}
}

void bulletM1::removeBullet(int arrNum)
{
	_vBullet.erase(_vBullet.begin() + arrNum);
}




///////////////////////////////////////////////////////////////////////////////////////////////////////shooter
bulletM2::bulletM2(void)
{
}
bulletM2::~bulletM2(void)
{
}

//공용으로 쓰는 총알 (쏠때마다 만들고 삭제한다)
HRESULT bulletM2::init(int bulletMax, float range)
{
	gameNode::init();

	_bulletMax = bulletMax;
	_range = range;

	return S_OK;
}

void bulletM2::release(void)
{
	gameNode::release();

	_vBullet.clear();
}

void bulletM2::update(void)
{
	gameNode::update();

	move();
}

void bulletM2::render(void)
{
	draw();
}

void bulletM2::fire(float x, float y, float angle,
	float speed)
{
	//발사 갯수 제한 한다
	if (_bulletMax < _vBullet.size()) return;

	tagBullet bullets;
	ZeroMemory(&bullets, sizeof(tagBullet));

	bullets.img = IMAGEMANAGER->addFrameImage("rock", "dungeon/rock.bmp", 65/1.4,65/1.4, 1, 1, true, RGB(255, 0, 255));
	bullets.speed = speed;
	bullets.angle = angle;
	bullets.radius = 30;
	bullets.x = bullets.fireX = x;
	bullets.y = bullets.fireY = y;
	bullets.rc = RectMakeCenter(bullets.x, bullets.y,
		60, 60);

	//KEYANIMANAGER->addCoordinateFrameAnimation("rockFire", "rock", 0, 1, 15, false, true);
	//KEYANIMANAGER->addCoordinateFrameAnimation("rockDie", "rock", 2, 15, 30, false, true);
	//
	//bullets.curAnimation = KEYANIMANAGER->findAnimation("rockFire");
	//bullets.curAnimation->start();
	_vBullet.push_back(bullets);
}


void bulletM2::move(void)
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end();)
	{
		_viBullet->x += cosf(_viBullet->angle) * _viBullet->speed;
		_viBullet->y += -sinf(_viBullet->angle) * _viBullet->speed;

		_viBullet->rc = RectMakeCenter(_viBullet->x + 30, _viBullet->y + 30,
			_viBullet->img->getFrameWidth() / 1.5, _viBullet->img->getFrameHeight() / 1.5);

		//사거리 밖으로 나가면...
		if (_range - 30 < getDistance(_viBullet->fireX,
			_viBullet->fireY, _viBullet->x, _viBullet->y))
		{
		//	_viBullet->curAnimation = KEYANIMANAGER->findAnimation("rockDie");
		//	_viBullet->curAnimation->start();
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

void bulletM2::draw(void)
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		//_viBullet->img->aniRender(getMemDC(), _viBullet->rc.left, _viBullet->rc.top, _viBullet->curAnimation);
		_viBullet->img->frameRender(getMemDC(), _viBullet->rc.left-5, _viBullet->rc.top-5,0,0);
		
		//Rectangle(getMemDC(),_viBullet->rc.left, _viBullet->rc.top, _viBullet->rc.right, _viBullet->rc.bottom);
	}
}

void bulletM2::removeBullet(int arrNum)
{
	_vBullet.erase(_vBullet.begin() + arrNum);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////player

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

//	_efm = new effectManager;
	//_efm->init();
	//_efm->setPlayer(_player);
	onceCount = 0;

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
	//_efm->update();
	move();
}

void missileM1::render(void)
{
	draw();
	//_efm->render();
}

//뽜이야~~~~!!
void missileM1::fire(float x, float y, float angle, int Num,int NumY,float speed)
{
	//최대 발사 갯수 제한
	if (_bulletMax <= _vBullet.size()) return;

	tagBullet bullet;
	ZeroMemory(&bullet, sizeof(tagBullet));

	bullet.img = new image;
	bullet.img->init("player/playerBullet1.bmp",900,1600, 9,16, true, RGB(255, 0, 255));
	bullet.speed = speed;
	bullet.angle = angle;
	bullet.x = bullet.fireX = x + cosf(bullet.angle);
	bullet.y = bullet.fireY = y - sinf(bullet.angle);
	bullet.rc = RectMakeCenter(x, y, bullet.img->getFrameWidth(),
		bullet.img->getFrameHeight());
	bullet.img->setFrameX(Num);
	bullet.img->setFrameY(NumY); // 16방향 Y축.
	_vBullet.push_back(bullet);
}

void missileM1::move(void)
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end();)
	{
		_viBullet->x += cosf(_viBullet->angle) * _viBullet->speed;
		_viBullet->y += -sinf(_viBullet->angle) * _viBullet->speed;
		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y,
			_viBullet->img->getFrameWidth()/3,
			_viBullet->img->getFrameHeight()/3);
		//onceCount++;
		//if (onceCount % 2 == 0) // 피가 한번만 닳게끔. 
		//{
		//_efm->setPlayerEffect(_viBullet->x, _viBullet->y);
		//onceCount = 0;
		//}
		if (_range - 32 < getDistance(_viBullet->fireX, _viBullet->fireY, _viBullet->x, _viBullet->y))
		{
			_viBullet->img->setFrameX(1); 
		}
		if (_range - 28 < getDistance(_viBullet->fireX, _viBullet->fireY, _viBullet->x, _viBullet->y))
		{
			_viBullet->img->setFrameX(2);
		}if (_range - 24 < getDistance(_viBullet->fireX, _viBullet->fireY, _viBullet->x, _viBullet->y))
		{
			_viBullet->img->setFrameX(3);
		}if (_range - 20 < getDistance(_viBullet->fireX, _viBullet->fireY, _viBullet->x, _viBullet->y))
		{
			_viBullet->img->setFrameX(4);
		}if (_range - 16 < getDistance(_viBullet->fireX, _viBullet->fireY, _viBullet->x, _viBullet->y))
		{
			_viBullet->img->setFrameX(5);
		}if (_range - 12 < getDistance(_viBullet->fireX, _viBullet->fireY, _viBullet->x, _viBullet->y))
		{
			_viBullet->img->setFrameX(6);
		}
		if (_range - 8 < getDistance(_viBullet->fireX, _viBullet->fireY, _viBullet->x, _viBullet->y))
		{
			_viBullet->img->setFrameX(7);
		}
		if (_range - 4 < getDistance(_viBullet->fireX, _viBullet->fireY, _viBullet->x, _viBullet->y))
		{
			_viBullet->img->setFrameX(8);
		}

		if (_range < getDistance(_viBullet->fireX, _viBullet->fireY, _viBullet->x, _viBullet->y))
		{
			SAFE_DELETE(_viBullet->img);
			_viBullet = _vBullet.erase(_viBullet);
			//for (int i = 0; i < _efm->getMinion().size(); ++i)
			//{
			//	if (_efm->getMinion()[i]->fx.life)s
			//	{
			//		_efm->getMinion()[i]->fx.life = false;
			//	}
			//}
		}
		
		else ++_viBullet;
	}
}

void missileM1::draw(void)
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		//Rectangle(getMemDC(), _viBullet->rc.left, _viBullet->rc.top, _viBullet->rc.right, _viBullet->rc.bottom);
		_viBullet->img->frameRender(getMemDC(), _viBullet->rc.left-15, _viBullet->rc.top-20,
			_viBullet->img->getFrameX(), _viBullet->img->getFrameY());
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


void missileM1::effectDelete(void)
{
	//for (int i = 0; i < _efm->getMinion().size(); ++i)
	//{
	//	if (!_efm->getMinion()[i]->fx.life)
	//	{
	//
	//
	//		_efm->removeMinion(i);
	//	}
	//}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////  boss

missileM2::missileM2(void)
{
}

missileM2::~missileM2(void)
{
}

HRESULT missileM2::init(int bulletMax, float range)
{
	gameNode::init();

	//사거리
	_range = range;

	//무기 최대 갯수
	_bulletMax = bulletMax;

	return S_OK;
}

void missileM2::release(void)
{
	gameNode::release();

	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end();)
	{
		SAFE_DELETE(_viBullet->img);
	}

	_vBullet.clear();
}

void missileM2::update(void)
{
	gameNode::update();

	move();
}

void missileM2::render(void)
{
	draw();
}

//뽜이야~~~~!!
void missileM2::fire(float x, float y, float angle, int Num, float speed)
{
	//최대 발사 갯수 제한
	if (_bulletMax <= _vBullet.size()) return;

	tagBullet bullet;
	ZeroMemory(&bullet, sizeof(tagBullet));

	bullet.img = new image;
	bullet.img->init("boss/bossBullet.bmp", 800,200, 16,4, true, RGB(255, 0, 255));
	bullet.speed = speed;
	bullet.angle = angle;
	bullet.x = bullet.fireX = x + cosf(bullet.angle);
	bullet.y = bullet.fireY = y - sinf(bullet.angle);
	bullet.rc = RectMakeCenter(x, y, bullet.img->getFrameWidth()/2,
		bullet.img->getFrameHeight()/2);
	bullet.img->setFrameX(Num);
	_vBullet.push_back(bullet);
}

void missileM2::move(void)
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end();)
	{
		_viBullet->x += cosf(_viBullet->angle) * _viBullet->speed;
		_viBullet->y += -sinf(_viBullet->angle) * _viBullet->speed;
		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y,
			_viBullet->img->getFrameWidth()/2,
			_viBullet->img->getFrameHeight()/2);

		if (_range - 50 < getDistance(_viBullet->fireX, _viBullet->fireY, _viBullet->x, _viBullet->y))
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

void missileM2::draw(void)
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		//Rectangle(getMemDC(), _viBullet->rc.left, _viBullet->rc.top, _viBullet->rc.right, _viBullet->rc.bottom);

		_viBullet->img->frameRender(getMemDC(), _viBullet->rc.left-20, _viBullet->rc.top-20,
			_viBullet->img->getFrameX(), 0);
	}
}

void missileM2::removeMissile(int arrNum)
{
	_vBullet.erase(_vBullet.begin() + arrNum);
}

void missileM2::xMissile(int arrNum, int num, bool Check)
{
	if (Check)_vBullet[arrNum].x += num;
	else _vBullet[arrNum].x -= num;
}