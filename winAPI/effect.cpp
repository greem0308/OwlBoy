#include "stdafx.h"
#include "effect.h"


effect::effect()
{
}


effect::~effect()
{
}

HRESULT effect::init(float x, float y)
{
	gameNode::init();

	return S_OK;
}

void effect::release(void)
{
	gameNode::release();
}

void effect::update(void)
{
	gameNode::update();

	fx.rc = RectMakeCenter(fx.x, fx.y, 50, fx.image->getFrameHeight());

	KEYANIMANAGER->update();
}

void effect::render(void)
{

}
