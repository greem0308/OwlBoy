#include "stdafx.h"
#include "playerEffect.h"


playerEffect::playerEffect()
{
}


playerEffect::~playerEffect()
{
}

HRESULT playerEffect::init(float x, float y)
{
	fx.image = IMAGEMANAGER->addFrameImage("bulletEffect", "player/bulletEffect.bmp", 1000, 100, 10, 1, true, RGB(255, 0, 255));
	fx.x = x;
	fx.y = y;
	fx.frameCount = 0;
	fx.currentX = 0;
	fx.currentY = 0;
	fx.rc = RectMakeCenter(fx.x, fx.y, fx.image->getFrameWidth(), fx.image->getFrameHeight());
	fx.life = true;

	KEYANIMANAGER->addCoordinateFrameAnimation("effectStart", "bulletEffect", 0, 9, 5, false, false);
	
	fx.currentAni = KEYANIMANAGER->findAnimation("effectStart");
	fx.currentAni->start();

	frame = 0;
	return S_OK;
}

void playerEffect::release(void)
{
}

void playerEffect::update(void)
{
	//만약 애니메이션이 끝나면, 멈춰라. 
	if (fx.image->getFrameX() < fx.image->getMaxFrameX())
	{
	//	PostQuitMessage(0);
		fx.life = false;
	}

	//frame++;
	//if (frame >30)
	//{
	//	fx.life = false;
	//	frame = 0;
	//}

	if (!fx.life)
	{
		fx.rc = RectMakeCenter(fx.x + 10000, fx.y+10000, fx.image->getFrameWidth(), fx.image->getFrameHeight());
	}

	KEYANIMANAGER->update();
}

void playerEffect::render(void)
{
	if(fx.life)
	fx.image->aniRender(getMemDC(), fx.rc.left, fx.rc.top, fx.currentAni);
}
