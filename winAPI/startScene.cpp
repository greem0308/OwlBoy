#include "stdafx.h"
#include "startScene.h"


startScene::startScene()
{
}


startScene::~startScene()
{
}

HRESULT startScene::init(void)
{
	gameNode::init();

	return S_OK;
}

void startScene::release(void)
{
	gameNode::release();
}

void startScene::update(void)
{
	gameNode::update();

}

void startScene::render(void)
{

}
