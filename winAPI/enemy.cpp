#include "stdafx.h"
#include "enemy.h"


enemy::enemy()
{
}


enemy::~enemy()
{
}

HRESULT enemy::init(float x, float y)
{
	gameNode::init();


	return S_OK;
}

void enemy::release(void)
{
	gameNode::release();
}

void enemy::update(void)
{
	gameNode::update();

}

void enemy::render(void)
{
}