#include "stdafx.h"
#include "test.h"


test::test()
{
}


test::~test()
{
}


HRESULT test::init(void)
{
	gameNode::init();

	return S_OK;
}

void test::release(void)
{
	gameNode::release();
}

void test::update(void)
{
	gameNode::update();

}

void test::render(void)
{
	IMAGEMANAGER->findImage("mapImage")->render(getMemDC());

}
