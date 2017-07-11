#include "stdafx.h"
#include "item.h"


item::item()
{
}


item::~item()
{
}


HRESULT item::init(float x, float y)
{
	gameNode::init();

	return S_OK;
}

void item::release(void)
{
	gameNode::release();
}

void item::update(void)
{
	gameNode::update();
}

void item::render(void)
{

}