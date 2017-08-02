#include "stdafx.h"
#include "pixelTestScene.h"


pixelTestScene::pixelTestScene()
{
}


pixelTestScene::~pixelTestScene()
{
}


HRESULT pixelTestScene::init(void)
{
	gameNode::init();
	IMAGEMANAGER->addImage("pinkPixel", "test/pinkPixel.bmp", 1280, 720, false);
	IMAGEMANAGER->addImage("bluePixel", "test/bluePixel.bmp", 1280, 720, false);
	IMAGEMANAGER->addImage("greenPixel", "test/greenPixel.bmp", 1280, 720, false);
	IMAGEMANAGER->addImage("yellowPixel", "test/yellowPixel.bmp", 1280, 720, false);

	_player = new player;
	_player->init(400, 200);

	return S_OK;
}

void pixelTestScene::release(void)
{
	gameNode::release();
	//SAFE_DELETE(_player);
}

void pixelTestScene::update(void)
{
	gameNode::update();
	_player->update();

}

void pixelTestScene::render(void)
{
	// 뒤에 픽셀배경들 안되게끔.
	IMAGEMANAGER->findImage("mapImage")->render(getMemDC()); // 원래
	IMAGEMANAGER->findImage("mapImage")->render(getPixel()); // 핑크
	IMAGEMANAGER->findImage("mapImage")->render(getPixelBlue()); // 블루
	IMAGEMANAGER->findImage("mapImage")->render(getPixelGreen()); // 그린
	IMAGEMANAGER->findImage("mapImage")->render(getPixelYellow()); // 옐로우

	IMAGEMANAGER->findImage("pinkPixel")->render(getPixel());
	IMAGEMANAGER->findImage("bluePixel")->render(getPixelBlue());
	IMAGEMANAGER->findImage("greenPixel")->render(getPixelGreen());
	IMAGEMANAGER->findImage("yellowPixel")->render(getPixelYellow());

	_player->render();
}
