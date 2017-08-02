#include "stdafx.h"
#include "glassObject.h"


glassObject::glassObject()
{
}


glassObject::~glassObject()
{
}

HRESULT glassObject::init(float x, float y)
{
	tem.image = IMAGEMANAGER->addFrameImage("grass", "item/grass.bmp", 768*1.5,70*1.5, 16,2, true, RGB(255, 0, 255));
	tem.rc = RectMakeCenter(tem.x, tem.y, tem.image->getFrameWidth(), tem.image->getFrameHeight());

	tem.x = x;
	tem.y = y;
	tem.coinUp = 0;
	tem.hpUp = 0; // 먹으면 플레이어 hp += 5 
	tem.life = true;
	tem.temState = STAY;
	tem.angle = PI;
	tem.gravity = 30.0f;
	tem.groundgrv = 0;
	tem.radius = tem.image->getFrameWidth();
	tem.ground = false;
	tem.angle = PI; // 포물선으로 던지기위한 앵글변수. 
	tem.speed = 12; // 던지면 날라가는 속도. 속도를 높일수록 castGravity값도 높여야한다.
	tem.hitCheck = false;

	x = 0;
	y = 0; 
	tem.rc = RectMakeCenter(x,y,50,50);
	frameCount=0;
	currentX = 0;
	glassState=glass_IDLE;
	hitCount = 0;

	return S_OK;
}

void glassObject::release(void)
{
}

void glassObject::update(void)
{
	x = tem.x;
	y = tem.y; 
	tem.rc = RectMakeCenter(x, y, 50, 50);

	rc = RectMakeCenter(x, y, 50, 50);

	// 조건
	//만약 player한테 맞으면,
	if (tem.hitCheck)
	{
		hitCount++;
		if (hitCount % 20 == 0)
		{
			//PostQuitMessage(0);
			glassState = glass_MOVE;
			tem.hitCheck = false;
		}
	}

	// 애니메이션
	frameCount++;
	switch (glassState)
	{
	case glass_MOVE:
		if (frameCount > 3)
		{
			frameCount = 0;
			currentX++;
			if (currentX > 15)
			{
				currentX = 0;
				glassState = glass_IDLE;
			}
		}
		break;

	case glass_IDLE:
		if (frameCount > 10)
		{
			frameCount = 0;
			currentX++;
			if (currentX > 1)
			{
				currentX = 0;
			}
		}
		break;
	default:
		break;
	}
}

void glassObject::render(void)
{
	//Rectangle(getMemDC(), tem.rc.left, tem.rc.top, tem.rc.right, tem.rc.bottom);
	tem.image->frameRender(getMemDC(), tem.rc.left-10, tem.rc.top-5, currentX,glassState);
}

