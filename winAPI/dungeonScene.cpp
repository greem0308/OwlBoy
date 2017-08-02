#include "stdafx.h"
#include "dungeonScene.h"


dungeonScene::dungeonScene()
{
}


dungeonScene::~dungeonScene()
{
}

HRESULT dungeonScene::init(void)
{
	gameNode::init();
	skyLayer1 = IMAGEMANAGER->addImage("skyLayer1", "dungeon/skyLayer1.bmp", 1280* 1.3, 720 * 1.3, true, RGB(255, 0, 255));
	skyLayer2 = IMAGEMANAGER->addImage("skyLayer2", "dungeon/skyLayer2.bmp", 1280 * 1.3, 720 * 1.3, true, RGB(255, 0, 255));
	skyLayer3 = IMAGEMANAGER->addImage("skyLayer3", "dungeon/skyLayer3.bmp", 3840,2880, true, RGB(255, 0, 255));

	layer1_offsetX=0;
	layer2_offsetX=0;

	IMAGEMANAGER->addImage("layer1BG", "dungeon/layer1BG.bmp",3840,2880, true,RGB(255,0,255));
	IMAGEMANAGER->addImage("layer2BG", "dungeon/layer2BG.bmp", 3840, 2880, true, RGB(255, 0, 255));

	// pixel
	IMAGEMANAGER->addImage("dungeonPixelPink", "dungeon/pixelPink.bmp", 3840, 2880, false);
	IMAGEMANAGER->addImage("dungeonBlue", "dungeon/dungeonBlue.bmp", 3840, 2880, false);
	IMAGEMANAGER->addImage("dungeonGreen", "dungeon/dungeonGreen.bmp", 3840, 2880, false);
	IMAGEMANAGER->addImage("dungeonYellow", "dungeon/dungeonYellow.bmp", 3840, 2880, false);
	
	// cloudWater
	IMAGEMANAGER->addFrameImage("cloudWater", "dungeon/cloudWater.bmp", 360,1000,6,1, true, RGB(255, 0, 255));
	// puzzleRock
	IMAGEMANAGER->addImage("puzzleRock", "dungeon/puzzleRock.bmp", 190,215, true, RGB(255, 0, 255));
	// 키블록
	IMAGEMANAGER->addFrameImage("keyBlock","dungeon/keyBlock.bmp",760 * 1.5,86 * 1.5,10,2,true,RGB(255,0,255));
	// door
	IMAGEMANAGER->addFrameImage("dungeonDoor", "dungeon/door.bmp",1008 * 1.4,197 * 1.4,9,1, true, RGB(255, 0, 255));
	// door2
	IMAGEMANAGER->addFrameImage("dungeonDoor2", "dungeon/door2.bmp", 1008 * 1.4, 197 * 1.4, 9, 1, true, RGB(255, 0, 255));
	// cloud
	IMAGEMANAGER->addFrameImage("cloud", "dungeon/cloud.bmp", 570 * 1.5,54 * 1.5, 6, 1, true, RGB(255, 0, 255));
	// lamp
	IMAGEMANAGER->addFrameImage("lamp", "dungeon/lamp.bmp",408 * 1.5,140 * 1.5,2,8, true, RGB(255, 0, 255));
	// splash
	IMAGEMANAGER->addFrameImage("splash", "dungeon/splash.bmp",612 * 1.5,51 * 1.5, 6, 1, true, RGB(255, 0, 255));
	// wind
	IMAGEMANAGER->addFrameImage("wind", "dungeon/wind.bmp", 300 * 1.5,92 * 1.5, 6, 1, true, RGB(255, 0, 255));
	// teamBomb
	IMAGEMANAGER->addFrameImage("teamBomb", "dungeon/teamBomb.bmp", 488 * 1.5,276 * 1.5, 8, 4, true, RGB(255, 0, 255));

	// container
	IMAGEMANAGER->addFrameImage("container","dungeon/container.bmp", 4410,750, 30,3, true, RGB(255, 0, 255));

	// bossRock 
	IMAGEMANAGER->addImage("bossRock", "dungeon/bossRock.bmp", 190,274, true, RGB(255, 0, 255));

	_player = new player;
	_player->init(3100,1900);
	_player->playNum = 5;
	_player->geddy.x = 3100;
	_player->geddy.y = 1900;

	// 영상으 ㄹ위해
	//_player->init(948,852);
	//_player->playNum = 5;
	//_player->geddy.x = 948;
	//_player->geddy.y = 852;


	//_player->init(900, 100);

	cameraX = 0;
	cameraY = 0;
	//rc = RectMake(WINSIZEX / 2 - 300, WINSIZEY / 2, 50, 50);

	_em = new enemyManager;
	_em->init();
	_em->setPlayer(_player);
	enemyShowFrame = 0;

	hitCount = 0;
	hurt = false;

	puzzleRock.x = 828;
	puzzleRock.y = 2245;
	puzzleRock.radius = 175;
	puzzleRock.Break = false;
	puzzleRock.rc = RectMake(0, 0, 200, 200);

	camera = true;

	doorInit();
	teamBombInit();
	cloudInit();
	ringInit();
	WringInit();

	// 던전으로 가는 문. 
	bossDoor.x = 1990;
	bossDoor.y = 0;
	bossDoor.rx = 1990;
	bossDoor.ry = 100;
	bossDoor.push = false;

	fx1Init();
	fx2Init();

	return S_OK;
}

void dungeonScene::release(void)
{
	gameNode::release();
	//SAFE_DELETE(_player);
}

void dungeonScene::update(void)
{
	gameNode::update();
	_player->update();
	_em->update();
	fx1Update();
	fx2Update();

	// 보스로 가는 문, 돌맹이 하나 막힌것도 그린담에 카메라 그리자. 
	RECT rcTemp;
	bossDoor.toBossRC = RectMake(bossDoor.x,bossDoor.y,200,150);
	bossDoor.bossRock = RectMake(bossDoor.rx, bossDoor.ry, 190,274);
	if (IntersectRect(&rcTemp,&_player->_player.rc,&bossDoor.toBossRC))
	{
		DATABASE->getElementData("player")->currentHP = _player->_player.currentHP;
		DATABASE->getElementData("player")->coin = _player->_player.coin;
		SCENEMANAGER->changeScene("bossBridge");
	}

	// 만약 돌맹이 위로 갈려면 못감 
	if (_player->_player.rc.left > bossDoor.bossRock.left - 50 && _player->_player.x < bossDoor.bossRock.right + 50
		&& _player->_player.rc.top < bossDoor.bossRock.bottom)
	{
		_player->_player.rc.top = bossDoor.bossRock.bottom;
		_player->_player.rc.bottom = bossDoor.bossRock.bottom + 50;
		if (_player->_player.y < 355)
		{
			_player->_player.y = 360;
		}
	}
	// 구름으로 물주면 밀림. 
	if (bossDoor.push) bossDoor.ry -= 0.4f;

	updateRC();

	Create(1);
	Delete();

	collisionFunc();

	if (camera) // 문에 부딧치면 카메라 작동 안하게.
	{
		dungeonCameraMove();
	} 

	toolFrameFunc();

	// 0번쨰 문 함수
	doorFunc0();

	teamBombUpdate();
	toolCollisionFunc();
	cloudUpdate();
	ringUpdate();
	WringUpdate();

	layer1_offsetX += 0.3f;
	layer2_offsetX -= 0.35f;
}

void dungeonScene::render(void)
{
	// 뒤에 픽셀배경들 안되게끔.
	IMAGEMANAGER->findImage("mapImage")->render(getMemDC()); // 원래
	IMAGEMANAGER->findImage("mapImage")->render(getPixel()); // 핑크
	IMAGEMANAGER->findImage("mapImage")->render(getPixelBlue()); // 블루
	IMAGEMANAGER->findImage("mapImage")->render(getPixelGreen()); // 그린
	IMAGEMANAGER->findImage("mapImage")->render(getPixelYellow()); // 옐로우
	
	IMAGEMANAGER->findImage("dungeonPixelPink")->render(getPixel(),cameraX,cameraY);
	IMAGEMANAGER->findImage("dungeonBlue")->render(getPixelBlue(), cameraX, cameraY);
	IMAGEMANAGER->findImage("dungeonGreen")->render(getPixelGreen(), cameraX, cameraY);
	IMAGEMANAGER->findImage("dungeonYellow")->render(getPixelYellow(), cameraX, cameraY);

	//구름 배경 
	RECT loopRC = RectMake(_player->_player.x - WINSIZEX / 2, _player->_player.y - WINSIZEY / 2 - 50, WINSIZEX, WINSIZEY + 80);
	
	skyLayer3->render(getMemDC());
	skyLayer2->loopRender(getMemDC(), &loopRC, layer2_offsetX, 0);
	skyLayer1->loopRender(getMemDC(), &loopRC, layer1_offsetX, 0);

	// layer2
	IMAGEMANAGER->findImage("layer2BG")->render(getMemDC(),cameraX, cameraY);
	
	// 물은 벽돌 뒤에 보이게 렌더한다.
	for (int i = 0; i < 2; i++)
	{
		if (cloud[i].life)
		{
			if (cloud[i].waterFall)
			{
				IMAGEMANAGER->findImage("cloudWater")->frameRender(getMemDC(), cloud[i].waterRC.left, cloud[i].waterRC.top
					, cloud[i].waterCurrentX, 0);
			}
		}
	}

	//우물 렌더. 
	for (int i = 0; i < 2; i++)
	{
		IMAGEMANAGER->findImage("container")->frameRender(getMemDC(), cloud[i].waterBowl.left - 30, cloud[i].waterBowl.top - 80, cloud[i].wfCurrentX, cloud[i].waterState);
	}

	// layer1
	IMAGEMANAGER->findImage("layer1BG")->render(getMemDC(), cameraX, cameraY);

	// 도구들 이미지 렌더. 
	for (int i = 0; i < 3; i++)
	{
		IMAGEMANAGER->findImage("keyBlock")->frameRender(getMemDC(), keyBlock[i].rc.left-15, keyBlock[i].rc.top-5,
			keyBlock[i].currentX,keyBlock[i].kbState);
	}

	IMAGEMANAGER->findImage("dungeonDoor")->frameRender(getMemDC(), door[0].rc.left - 45, door[0].rc.top,
		door[0].currentX, 0);
	IMAGEMANAGER->findImage("dungeonDoor")->frameRender(getMemDC(), door[1].rc.left - 45, door[1].rc.top, 
		door[1].currentX, 0);

	IMAGEMANAGER->findImage("dungeonDoor2")->frameRender(getMemDC(), door[2].rc.left - 10, door[2].rc.top, 
		door[2].currentX, 0);

	if (!puzzleRock.Break)
	{
		IMAGEMANAGER->findImage("puzzleRock")->render(getMemDC(), puzzleRock.rc.left, puzzleRock.rc.top);
	}

	CurveLineRender();

	//showRect();

	cloudRender();
	
	ringRenderBehind();
	WringRenderBehind();
	
	_em->render();
	fx1Render();

	_player->render();

	ringRender();
	WringRender();

	if (teamBomb.life)
	{
		IMAGEMANAGER->findImage("teamBomb")->frameRender(getMemDC(), teamBomb.rc.left, teamBomb.rc.top,
			teamBomb.currentX, teamBomb.tbState);
	}

	IMAGEMANAGER->findImage("bossRock")->render(getMemDC(), bossDoor.bossRock.left, bossDoor.bossRock.top);
	fx2Render();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
/////////////////////////////////////////////////////////////////////////////////////////////////////////////


//던전 배경 크기 3840,2880 ///////////////////////////////////////////////////////////////////////////////////camera,showRect
void dungeonScene::dungeonCameraMove()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//만약 플레이어가 오른쪽에 있고, 배경x축이 - (배경이미지-화면사이즈)보다 크다면 /////////////////////////////////////
	if (_player->_player.x > WINSIZEX / 2 && cameraX > -(3840 - WINSIZEX)) // 오른쪽
	{
		cameraX -= _player->_player.x - WINSIZEX / 2;
		_player->geddy.x -= _player->_player.x - WINSIZEX / 2;

		//cloudX -= _player->_player.x - WINSIZEX / 2;

		//// item	
		//for (int i = 0; i < _im->getItem().size(); ++i)
		//{
		//	_im->getItem()[i]->tem.x -= _player->_player.x - WINSIZEX / 2;
		//}

		// enemy
		for (int i = 0; i < _em->getMinion().size(); ++i)
		{
			_em->getMinion()[i]->_enemy.x -= _player->_player.x - WINSIZEX / 2;
		}

		// shooter Bullet
		for (int i = 0; i < _em->getMinion().size(); ++i)
		{
			for (int j = 0; j < _em->getMinion()[i]->_enemy._bullet2->getVBullet().size(); j++)
			{
				_em->getMinion()[i]->_enemy._bullet2->getVBullet()[j].x -= _player->_player.x - WINSIZEX / 2;
			}
		}

		//player 총알
		for (int i = 0; i < _player->_player._fire->getVBullet().size(); i++)
		{
			_player->_player._fire->getVBullet()[i].x -= _player->_player.x - WINSIZEX / 2;
		}

		// 도구 
		for (int i = 0; i < 3; i++)
		{
			keyBlock[i].x -= _player->_player.x - WINSIZEX / 2;
			keyBlock[i].cx -= _player->_player.x - WINSIZEX / 2;
			door[i].x -= _player->_player.x - WINSIZEX / 2;
			door[i].cx -= _player->_player.x - WINSIZEX / 2;
		}
		teamBomb.x -= _player->_player.x - WINSIZEX / 2;
		
		//구름
		for (int i = 0; i < 2; i++)
		{
			cloud[i].x -= _player->_player.x - WINSIZEX / 2;
			//우물 
			cloud[i].wbX -= _player->_player.x - WINSIZEX / 2;
		}

		puzzleRock.x -= _player->_player.x - WINSIZEX / 2;

		// 링
		for (int i = 0; i < RINGMAX; i++)
 		{
			ring[i].x -= _player->_player.x - WINSIZEX / 2;
			wring[i].x -= _player->_player.x - WINSIZEX / 2;
		}

		bossDoor.x -= _player->_player.x - WINSIZEX / 2;
		bossDoor.rx -= _player->_player.x - WINSIZEX / 2;

		_player->_player.x = WINSIZEX / 2;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (_player->_player.x  < WINSIZEX / 2 && cameraX < 0) // 왼쪽에있고, 카메라X가 0보다 크다면,/////////////////////////////
	{
		cameraX += WINSIZEX / 2 - _player->_player.x;
		_player->geddy.x += WINSIZEX / 2 - _player->_player.x;

		//cloudX += WINSIZEX / 2 - _player->_player.x;

		//// item	
		//for (int i = 0; i < _im->getItem().size(); ++i)
		//{
		//	_im->getItem()[i]->tem.x += WINSIZEX / 2 - _player->_player.x;
		//}

		// enemy
		for (int i = 0; i < _em->getMinion().size(); ++i)
		{
			_em->getMinion()[i]->_enemy.x += WINSIZEX / 2 - _player->_player.x;
		}

		// shooter Bullet
		for (int i = 0; i < _em->getMinion().size(); ++i)
		{
			for (int j = 0; j < _em->getMinion()[i]->_enemy._bullet2->getVBullet().size(); j++)
			{
				_em->getMinion()[i]->_enemy._bullet2->getVBullet()[j].x += WINSIZEX / 2 - _player->_player.x;
			}
		}

		//player 총알
		for (int i = 0; i < _player->_player._fire->getVBullet().size(); i++)
		{
			_player->_player._fire->getVBullet()[i].x += WINSIZEX / 2 - _player->_player.x;
		}

		// 도구 
		for (int i = 0; i < 3; i++)
		{
			keyBlock[i].x += WINSIZEX / 2 - _player->_player.x;
			keyBlock[i].cx += WINSIZEX / 2 - _player->_player.x;
			door[i].x += WINSIZEX / 2 - _player->_player.x;
			door[i].cx += WINSIZEX / 2 - _player->_player.x;
		}
		//구름
		for (int i = 0; i < 2; i++)
		{
			cloud[i].x += WINSIZEX / 2 - _player->_player.x;
			//우물 
			cloud[i].wbX += WINSIZEX / 2 - _player->_player.x;
		}

		teamBomb.x += WINSIZEX / 2 - _player->_player.x;
		puzzleRock.x += WINSIZEX / 2 - _player->_player.x;

		// 링
		for (int i = 0; i < RINGMAX; i++)
		{
			ring[i].x += WINSIZEX / 2 - _player->_player.x;
			wring[i].x += WINSIZEX / 2 - _player->_player.x;
		}

		bossDoor.x += WINSIZEX / 2 - _player->_player.x;
		bossDoor.rx += WINSIZEX / 2 - _player->_player.x;

		_player->_player.x = WINSIZEX / 2;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (_player->_player.y > WINSIZEY / 2 && cameraY > -(2880 - WINSIZEY)) // 오른쪽 ////////////////////////////////////////
	{
		cameraY -= _player->_player.y - WINSIZEY / 2;
		_player->geddy.y -= _player->_player.y - WINSIZEY / 2;

		//cloudY -= _player->_player.y - WINSIZEY / 2;

		//// item	
		//for (int i = 0; i < _im->getItem().size(); ++i)
		//{
		//	_im->getItem()[i]->tem.y -= _player->_player.y - WINSIZEY / 2;
		//}

		// enemy
		for (int i = 0; i < _em->getMinion().size(); ++i)
		{
			_em->getMinion()[i]->_enemy.y -= _player->_player.y - WINSIZEY / 2;
		}

		// shooter Bullet
		for (int i = 0; i < _em->getMinion().size(); ++i)
		{
			for (int j = 0; j < _em->getMinion()[i]->_enemy._bullet2->getVBullet().size(); j++)
			{
				_em->getMinion()[i]->_enemy._bullet2->getVBullet()[j].y -= _player->_player.y - WINSIZEY / 2;
			}
		}

		//player 총알
		for (int i = 0; i < _player->_player._fire->getVBullet().size(); i++)
		{
			_player->_player._fire->getVBullet()[i].x -= _player->_player.y - WINSIZEY / 2;
		}

		// 도구 
		for (int i = 0; i < 3; i++)
		{
			keyBlock[i].y -= _player->_player.y - WINSIZEY / 2;
			keyBlock[i].cy -= _player->_player.y - WINSIZEY / 2;
			door[i].y -= _player->_player.y - WINSIZEY / 2;
			door[i].cy -= _player->_player.y - WINSIZEY / 2;
		}
		//구름
		for (int i = 0; i < 2; i++)
		{
			cloud[i].y -= _player->_player.y - WINSIZEY / 2;
			//우물 
			cloud[i].wbY -= _player->_player.y - WINSIZEY / 2;
		}
		teamBomb.y -= _player->_player.y - WINSIZEY / 2;
		puzzleRock.y -= _player->_player.y - WINSIZEY / 2;

		// 링
		for (int i = 0; i < RINGMAX; i++)
		{
			ring[i].y -= _player->_player.y - WINSIZEY / 2;
			wring[i].y -= _player->_player.y - WINSIZEY / 2;
		}

		bossDoor.y -= _player->_player.y - WINSIZEY / 2;
		bossDoor.ry -= _player->_player.y - WINSIZEY / 2;

		_player->_player.y = WINSIZEY / 2;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (_player->_player.y  < WINSIZEY / 2 && cameraY < 0) // 왼쪽에있고, 카메라X가 0보다 크다면,////////////////////////////
	{
		cameraY += WINSIZEY / 2 - _player->_player.y;
		_player->geddy.y += WINSIZEY / 2 - _player->_player.y;

		//cloudY += WINSIZEY / 2 - _player->_player.y;

		//// item	
		//for (int i = 0; i < _im->getItem().size(); ++i)
		//{
		//	_im->getItem()[i]->tem.y += WINSIZEY / 2 - _player->_player.y;
		//}
		
		// enemy
		for (int i = 0; i < _em->getMinion().size(); ++i)
		{
			_em->getMinion()[i]->_enemy.y += WINSIZEY / 2 - _player->_player.y;
		}

		// shooter Bullet
		for (int i = 0; i < _em->getMinion().size(); ++i)
		{
			for (int j = 0; j < _em->getMinion()[i]->_enemy._bullet2->getVBullet().size(); j++)
			{
				_em->getMinion()[i]->_enemy._bullet2->getVBullet()[j].y += WINSIZEY / 2 - _player->_player.y;
			}
		}

		//player 총알
		for (int i = 0; i < _player->_player._fire->getVBullet().size(); i++)
		{
			_player->_player._fire->getVBullet()[i].x += WINSIZEY / 2 - _player->_player.y;
		}

		// 도구 
		for (int i = 0; i < 3; i++)
		{
			keyBlock[i].y += WINSIZEY / 2 - _player->_player.y;
			keyBlock[i].cy += WINSIZEY / 2 - _player->_player.y;
			door[i].y += WINSIZEY / 2 - _player->_player.y;
			door[i].cy += WINSIZEY / 2 - _player->_player.y;
		}

		//구름
		for (int i = 0; i < 2; i++)
		{
			cloud[i].y += WINSIZEY / 2 - _player->_player.y;
			//우물 
			cloud[i].wbY += WINSIZEY / 2 - _player->_player.y;
		}
		teamBomb.y += WINSIZEY / 2 - _player->_player.y;
		puzzleRock.y += WINSIZEY / 2 - _player->_player.y;

		// 링
		for (int i = 0; i < RINGMAX; i++)
		{
			ring[i].y += WINSIZEY / 2 - _player->_player.y;
			wring[i].y += WINSIZEY / 2 - _player->_player.y;
		}

		bossDoor.y += WINSIZEY / 2 - _player->_player.y;
		bossDoor.ry += WINSIZEY / 2 - _player->_player.y;

		_player->_player.y = WINSIZEY / 2;
	}

	// 이미 위에서 카메라 화면제한이 되어있어서 플레이어x,y는 항상 WINSIZEX/2,WINSIZEY/2로 되어있고
	// 화면제한에 다까이오게되면 중점을 벗어나므로 밑에와같이 플레이어 화면제한을 걸어줄 수 있다.
	if (_player->_player.x < 50) _player->_player.x = 50;
	if (_player->_player.x >WINSIZEX - 50) _player->_player.x = WINSIZEX - 50;
	if (_player->_player.y < 50) _player->_player.y = 50;
	if (_player->_player.y >WINSIZEY - 50) _player->_player.y = WINSIZEY - 50;
}

void dungeonScene::showRect()
{
	//char str[256];
	//SetTextColor(getMemDC(), RGB(100, 100, 255));
	//HFONT myFont = CreateFont(28, 0, 0, 0, 1, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, TEXT("System"));
	//SelectObject(getMemDC(), myFont);
	//
	////enemy position
	//for (int i = 0; i < _em->getMinion().size(); i++)
	//{
	//	sprintf(str, "%0.2f", _em->getMinion()[i]->_enemy.x);
	//	sprintf(str, "%0.2f", _em->getMinion()[i]->_enemy.y);
	//}
	//TextOut(getMemDC(), 100, 650, str, strlen(str));
	//TextOut(getMemDC(), 200, 650, str, strlen(str));
	//
	//sprintf(str, "door : %0.2f", door[0].x);
	//TextOut(getMemDC(), 30, 450, str, strlen(str));
	//
	//sprintf(str, "bombX : %0.2f", teamBomb.x);
	//TextOut(getMemDC(), 30, 550, str, strlen(str));
	//sprintf(str, "bombY : %0.2f", teamBomb.y);
	//TextOut(getMemDC(), 30, 600, str, strlen(str));
	//
	//sprintf(str, "ring[0].x : %0.2f", ring[0].x);
	//TextOut(getMemDC(), 900, 600, str, strlen(str));
	//sprintf(str, "ring[0].y : %0.2f", ring[0].y);
	//TextOut(getMemDC(), 1100, 600, str, strlen(str));

	// bossRock
	//Rectangle(getMemDC(),bossDoor.toBossRC.left, bossDoor.toBossRC.top, bossDoor.toBossRC.right, bossDoor.toBossRC.bottom);
	
	// enemy
	//for (int i = 0; i < _em->getMinion().size(); ++i)
	//{
	//	Rectangle(getMemDC(), _em->getMinion()[i]->_enemy.rc.left, _em->getMinion()[i]->_enemy.rc.top,
	//		_em->getMinion()[i]->_enemy.rc.right, _em->getMinion()[i]->_enemy.rc.bottom);
	//}
	//Rectangle(getMemDC(), rc.left, rc.top, rc.right, rc.bottom);


	// 도구들 rc ///////////////////////////////////////////////////////////////////////////////////////////////////////
	// 문, 키블록
	//for (int i = 0; i<3; i++)
	//{
	//	//  Rectangle(getMemDC(),keyBlock[i].rc.left, keyBlock[i].rc.top, keyBlock[i].rc.right, keyBlock[i].rc.bottom);
	//	//	Rectangle(getMemDC(), keyBlock[i].collisionRC.left, keyBlock[i].collisionRC.top,
	//	//		keyBlock[i].collisionRC.right, keyBlock[i].collisionRC.bottom);
	//	// Rectangle(getMemDC(), door[i].rc.left, door[i].rc.top, door[i].rc.right, door[i].rc.bottom);
	//	//if (!door[i].open)
	//	//{
	//	//	Rectangle(getMemDC(), door[i].collisionRC.left, door[i].collisionRC.top, door[i].collisionRC.right, door////[i].collisionRC.bottom);
	//	//}
	//}
	//
	//시한 폭탄
	//Rectangle(getMemDC(),teamBomb.rc.left, teamBomb.rc.top, teamBomb.rc.right, teamBomb.rc.bottom);

	// 구름
	//for (int i = 0; i < 2; i++)
	//{
	//	if (cloud[i].life)
	//	{
	//		Rectangle(getMemDC(), cloud[i].rc.left, cloud[i].rc.top, cloud[i].rc.right, cloud[i].rc.bottom);
	//	}
	//}

	// 터뜨릴 블록
	//if (!puzzleRock.Break)
	//{
	//	Rectangle(getMemDC(), puzzleRock.rc.left, puzzleRock.rc.top, puzzleRock.rc.right, puzzleRock.rc.bottom);
	//}

	////시한 폭탄 터지는렉트
	//if (teamBomb.timeOver)
	//{
	//	Rectangle(getMemDC(), teamBomb.hitRC.left, teamBomb.hitRC.top, teamBomb.hitRC.right, teamBomb.hitRC.bottom);
	//}
}

void dungeonScene::updateRC()
{
	// 움직이는 도구들 rc 
	for (int i = 0; i<3; i++)
	{
		keyBlock[i].collisionRC = RectMake(keyBlock[i].cx, keyBlock[i].cy, 90, 50);
		keyBlock[i].rc = RectMake(keyBlock[i].x, keyBlock[i].y, 90, 50);

		// 콜리젼 문만 설정. 
		if (!door[i].open)
		{
			door[i].collisionRC = RectMake(door[i].cx, door[i].cy, 80, 300);
		}
		// 이미지를 위한 문.. 
		door[i].rc = RectMake(door[i].x, door[i].y, 80, 300);
	}

	// 구름 렉트. 
	for (int i = 0; i < 2; i++)
	{
		if (cloud[i].life)
		{
			cloud[i].rc = RectMakeCenter(cloud[i].x, cloud[i].y, 140, 75);
		}
	}

	// 퍼즐 렉트
	if (!puzzleRock.Break)
	{
		puzzleRock.rc = RectMake(puzzleRock.x, puzzleRock.y, puzzleRock.radius, puzzleRock.radius + 20);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////cteate,delete
void dungeonScene::Create(int Num)
{
	enemyShowFrame++;
	if (enemyShowFrame == 1)
	{
		// shooter
		_em->setShooter(588,1530);
		_em->setShooter(1024, 1260);

		 //reviver
		_em->setReviver(1450,2350, 380);
		_em->setReviver(1450, 2350, 300);
		_em->setReviver(1450, 2350, 220);

		//fireLion
		//_em->setFireLion(2800, 360); // 400

		// follower
		// 첫번째 문
		//_em->setFollower(2500,2320);
		//_em->setFollower(2600, 2320);
		// 굴
		//_em->setFollower(500,2600);
		//_em->setFollower(400, 2600);
		
		// 두번째 문
		_em->setFollower(400, 915);
		_em->setFollower(550, 915);
		//_em->setFollower(650, 915);

		_em->setFollower(1090,160);
		_em->setFollower(890, 160);
	}
}

// 여긴되는데 
void dungeonScene::Delete(void)
{
	for (int i = 0; i < _em->getMinion().size(); ++i)
	{
		if (!_em->getMinion()[i]->_enemy.life) _em->removeMinion(i);
	}
}

// 적들과의 충돌. ////////////////////////////////////////////////////////////////////////////////////////////////충돌 함수들
void dungeonScene::collisionFunc(void)
{
	RECT tempRC;
	// shooter 총알과 플레이어가 충돌 _____________________________________________________________________________________
	for (int i = 0; i < _em->getMinion().size(); ++i)
	{
		for (int j = 0; j < _em->getMinion()[i]->_enemy._bullet2->getVBullet().size(); j++)
		{
			if (IntersectRect(&tempRC, &_player->_player.rc, &_em->getMinion()[i]->_enemy._bullet2->getVBullet()[j].rc))
			{
				hurt = true;
				break;
			}
		}
	}

	if (hurt)
	{
		hitCount++;
		if (hitCount % 20 == 0) // 피가 한번만 닳게끔. 
		{
			_player->_player.state = HURT;
			_player->_player.x -= 20;
			_player->geddy.cast = true;
			_player->geddy.follow = false;
			DATABASE->getElementData("player")->currentHP -= 2;
			_player->se7 = true;
			hurt = false;
			hitCount = 0;
		}
	}


	// 플레이어 총알과 적이 충돌 _____________________________________________________________________________________
	for (int i = 0; i < _em->getMinion().size(); ++i)
	{
		for (int j = 0; j < _player->_player._fire->getVBullet().size(); j++)
		{
			if (IntersectRect(&tempRC, &_player->_player._fire->getVBullet()[j].rc, &_em->getMinion()[i]->_enemy.rc))
			{
				_em->getMinion()[i]->_enemy.hitCheck = true;
				_player->removeMissile(j);
			}
		}
	}

	// 적들 몸체랑 플레이어랑 충돌. 
	for (int i = 0; i < _em->getMinion().size(); ++i)
	{
		if (IntersectRect(&tempRC, &_player->_player.rc, &_em->getMinion()[i]->_enemy.rc))
		{
			hurt = true;
			break;
		}
	}

	// 시한폭탄이랑 충돌.
	if (IntersectRect(&tempRC, &_player->_player.rc, &teamBomb.hitRC))
	{
		hurt = true;
	}
     
	// 불뿜는애랑 물이랑.
	
		for (int j = 0; j < _em->getMinion().size(); ++j)
		{
			for (int i = 0; i < 2; i++)
			{
			 if (IntersectRect(&tempRC, &_em->getMinion()[i]->_enemy.rc, &cloud[i].waterRC))
			 {
				_em->getMinion()[i]->_enemy.stoned = true;
				break;
			 }
		    }
	    }
}

void dungeonScene::toolCollisionFunc()
{
	RECT rcTemp;
	// 시한폭탄하고 퍼즐락 충돌하면 
	if (IntersectRect(&rcTemp, &teamBomb.rc, &puzzleRock.rc))
	{
		fx2.fire = true;
		_player->se7 = true;
		puzzleRock.Break = true;
		teamBomb.life = false;
	}

	// 플레이어 총알과 문 렉트가 충돌 _____________________________________________________________________________________
	for (int i = 0; i<3; ++i)
	{
		for (int j = 0; j < _player->_player._fire->getVBullet().size(); j++)
		{
			if (IntersectRect(&rcTemp, &_player->_player._fire->getVBullet()[j].rc, &door[i].rc))
			{
				_player->removeMissile(j);
			}
		}
	}
}

// 도구 애니메이션.
void dungeonScene::toolFrameFunc()
{
	// 구름 프레임. 
	for (int i = 0; i < 2; i++)
	{
		cloud[i].frameCount++;
		if (cloud[i].frameCount > 15)
		{
			cloud[i].frameCount = 0;
			cloud[i].currentX++;
			if (cloud[i].currentX > 5)
			{
				cloud[i].currentX = 0;
			}
		}
	}
	
	// 구름 물 프레임. 
	for (int i = 0; i < 2; i++)
	{
		cloud[i].waterFrameCount++;
		if (cloud[i].waterFrameCount > 10)
		{
			cloud[i].waterFrameCount = 0;
			cloud[i].waterCurrentX++;
			if (cloud[i].waterCurrentX > 5)
			{
				cloud[i].waterCurrentX = 0;
			}
		}
	}

	// 우물 프레임. 
	for (int i = 0; i < 2; i++)
	{
		cloud[i].wfFrameCount++;
		switch (cloud[i].waterState)
		{
		case WF_IDLE:
			cloud[i].wfCurrentX = 0;
			break;
		case WF_LAMPON:
			if (cloud[i].wfFrameCount > 15)
			{
				cloud[i].wfFrameCount = 0;
				cloud[i].wfCurrentX++;
				if (cloud[i].wfCurrentX > 1)
				{
					cloud[i].wfCurrentX = 0;
				}
			}
			break;
		case WF_FALL:
			if (cloud[i].wfFrameCount > 15)
			{
				cloud[i].wfFrameCount = 0;
				cloud[i].wfCurrentX++;
				if (cloud[i].wfCurrentX > 27)
				{
					cloud[i].wfCurrentX = 27;
				}
			}
			break;

		default:
			break;
		}
	}

	//door 프레임. 
	for (int i = 0; i < 3; i++)
	{
		door[i].frameCount++;
		switch (door[i].doorState)
		{
		case D_IDLE:
			door[i].currentX = 0;
			break;

		case D_OPEN:
			if (door[i].frameCount > 12)
			{
				door[i].frameCount = 0;
				door[i].currentX++;
				if (door[i].currentX >8)
				{
					door[i].currentX = 8;
				}
			}
			break;
		default:
			break;
		}
	}
	
	// keyBlock
	for (int i = 0; i < 3; i++)
	{
		keyBlock[i].frameCount++;
		switch (keyBlock[i].kbState)
		{
		case KB_IDLE:
			keyBlock[i].currentX = 0;
			break;
		case KB_DOWN:
			if (keyBlock[i].frameCount > 10)
			{
				keyBlock[i].frameCount = 0;
				keyBlock[i].currentX++;
				if (keyBlock[i].currentX > 9)
				{
					keyBlock[i].currentX = 9;
				}
			}
			break;
		default:
			break;
		}
	}

	// teamBomb
	teamBomb.frameCount++;
	switch (teamBomb.tbState)
	{
	case TB_IDLE: 
		if (teamBomb.frameCount > 15)
		{
			teamBomb.frameCount = 0;
			teamBomb.currentX++;
			if (teamBomb.currentX > 3)
			{
				teamBomb.currentX = 0;
			}
		}
		break;

	case TB_DANGER1:
		if (teamBomb.frameCount > 15)
		{
			teamBomb.frameCount = 0;
			teamBomb.currentX++;
			if (teamBomb.currentX > 4)
			{
				teamBomb.currentX = 0;
			}
		}
		break; 

	case TB_DANGER2:
		if (teamBomb.frameCount > 15)
		{
			teamBomb.frameCount = 0;
			teamBomb.currentX++;
			if (teamBomb.currentX > 4)
			{
				teamBomb.currentX = 0;
			}
		}
		break; 

	case TB_WALK:
		if (teamBomb.frameCount > 15)
		{
			teamBomb.frameCount = 0;
			teamBomb.currentX++;
			if (teamBomb.currentX > 7)
			{
				teamBomb.currentX = 0;
			}
		}
		break; 
	default:
		break;
	}
}

// 키블록 콜리젼.
void dungeonScene::keyBlockCollision()
{
	RECT rcTemp;
	for (int i = 0; i<2; i++)
	{
		if (IntersectRect(&rcTemp, &_player->_player.rc, &keyBlock[i].collisionRC))
		{
			door[i].open = true; // 해당 배열 넘버 문이 열림. 
			_player->_player.ground = true; // 땅이라고 알려줌. 
			_player->_player.y += _player->_player.groundgrv; // 플레이어y += 땅 그래피티 
			_player->miniY += _player->_player.groundgrv;

			_player->_player.groundgrv = 0;
			_player->_player.jumpCount = 0;
			if (_player->_player.fly)
			{
				_player->_player.fly = false;
				_player->_player.state = IDLE;
			}
			if (_player->_player.jump)
			{
				_player->_player.jump = false;
				_player->_player.state = IDLE;
			}
			if (_player->_player.fall)
			{
				_player->_player.fall = false;
				_player->_player.state = IDLE;
			}
		}
	}

}

// 문 함수 ______________________________________________________________________________________________________ 문
void dungeonScene::doorFunc0()
{
	// 만약 키블록이랑 충돌하면, 문이 오픈.
	RECT rcTemp;
	for (int i = 0; i < 2; i++)
	{
		if (IntersectRect(&rcTemp, &_player->_player.rc, &keyBlock[i].collisionRC))
		{
			door[i].open = true; // 해당 배열 넘버 문이 열림. 

			keyBlock[i].down = true; // 해당 키블록 애니메이션이 진행된다. 
			keyBlock[i].kbState = KB_DOWN;
			door[i].doorState = D_OPEN;

			keyBlock[i].frameCount0++;
			if (keyBlock[i].frameCount0 > 55) // 블록내려가는 애니메이션 된 후 렉트 내려가게.
			{
				if (keyBlock[i].frameCount0 == 56)
				{
					_player->se5 = true;
				}
				// 내려가..
				if (i == 0)
				{
					keyBlock[0].cy = keyBlock[0].cy + 30;
				}
				if (i == 1)
				{
					keyBlock[1].cy = keyBlock[1].cy + 30;
				}
				if (keyBlock[i].frameCount0 == 80)
				{
					_player->se6 = true;
				}
			}
		}
	}

	for (int i = 0; i < 3; i++)
	{
		if (door[i].open)
		{
			door[i].doorState = D_OPEN;
			break;
		}
	}

	// 키블록 충돌.
	keyBlockCollision();

	//만약 문충돌렉트랑 부딧치면 못가게 조정, 
	if (!door[0].open)
	{
		//isCollision_notinter(door[i].collisionRC, _player->_player.rc);
		if (_player->_player.rc.left < door[0].collisionRC.right && door[0].collisionRC.top < _player->_player.rc.top)
		{
			camera = false; //일단 동작함.
			_player->_player.rc.left = door[0].collisionRC.right;
			_player->_player.rc.right = _player->_player.rc.left + 50;

			// 계속 벽이랑 부딧치면 player.x만 이동해서, 나중에 뒤로갈때 붙잡혀서 빼줌. 
			if(_player->_player.x < 630)(_player->_player.x = 640);
		}
		else
		{
			camera = true;
		}
	}

	if (door[0].open && !door[1].open)
	{
		if (_player->_player.rc.left < door[1].collisionRC.right && door[1].collisionRC.top < _player->_player.rc.top
			&& _player->_player.rc.bottom <= door[1].collisionRC.bottom)
		{
			camera = false; //일단 동작함.
			_player->_player.rc.left = door[1].collisionRC.right;
			_player->_player.rc.right = _player->_player.rc.left + 50;

			if (_player->_player.x < 630)(_player->_player.x = 640);
		}
		else
		{
			camera = true;
		}
	}

	if (door[0].open && door[1].open && !door[2].open)
	{
		if (_player->_player.rc.right > door[2].collisionRC.left
			&& door[2].collisionRC.bottom >= _player->_player.rc.bottom)
		{
			camera = false; //일단 동작함.
			_player->_player.rc.right = door[2].collisionRC.left;
			_player->_player.rc.left = _player->_player.rc.right - 50;

			if (_player->_player.x > 645)(_player->_player.x = 640);
		}
		else
		{
			camera = true;
		}
	}
}

void dungeonScene::doorInit()
{
	door[0].x = 1900;
	door[0].y = 2340;
	door[1].x = 870;
	door[1].y = 1900;
	door[2].x = 1350;
	door[2].y = 488;
	door[0].cx = 1900;
	door[0].cy = 2340;
	door[1].cx = 870;
	door[1].cy = 1900;
	door[2].cx = 1350;
	door[2].cy = 488;

	keyBlock[0].x = 2110;
	keyBlock[0].y = 2520;
	keyBlock[1].x = 900;
	keyBlock[1].y = 2395;
	keyBlock[0].cx = 2110;
	keyBlock[0].cy = 2515;
	keyBlock[1].cx = 900;
	keyBlock[1].cy = 2390;

	// 불,카운트 초기화. 
	for (int i = 0; i < 3; i++)
	{
		door[i].doorState = D_IDLE;
		door[i].open = false;
		door[i].frameCount = 0;
		door[i].currentX = 0; // 닫혀있는게 8번째 프레임임.

		keyBlock[i].down = false;
		keyBlock[i].colliRCdown = false;
		keyBlock[i].kbState = KB_IDLE;
		keyBlock[i].frameCount = 0;
		keyBlock[i].currentX = 0;
		keyBlock[i].frameCount0 = 0;
		keyBlock[i].currentX0 = 0;
		keyBlock[i].frameCount1 = 0;
		keyBlock[i].currentX1 = 0;
	}

}

// 시한폭탄 함수 ________________________________________________________________________________________________시한 폭탄
void dungeonScene::teamBombInit()
{
	teamBomb.life = true;
	teamBomb.timeOver = false;
	teamBomb.frameCount = 0;
	teamBomb.currentX = 0;
	
	teamBomb.follow = false;
	teamBomb.cast = false;
	teamBomb.showCurve = false;
	teamBomb.castGravity = 0;
	teamBomb.speed = 14.0f; // 던져지는 속도.
	teamBomb.angle = PI;

	//x,y, 초기화
	teamBomb.x = 820;
	teamBomb.y = 2730;
	teamBomb.tbState = TB_WALK;

	teamBomb.gravity = 30.0f;
	teamBomb.groundgrv = 0;
	teamBomb.ground = false;

	teamBomb.downLength = 18;

	teamBomb.timeframeCount = 0;
	teamBomb.timecurrentX = 0;
	teamBomb.gravityOpen = false;

	teamBomb.moveFrame = 0;
	teamBomb.moveUp = true;
	teamBomb.moveDown = false;

	teamBomb.timeOver = false;
}

void dungeonScene::teamBombUpdate()
{
	//RECT tempRC;
	//if (IntersectRect(&tempRC, &_player->_player.rc, &teamBomb.rc))
	//{
		if (KEYMANAGER->isOnceKeyDown('8'))
		{
			teamBomb.follow = true;
		}
	//}

		if (teamBomb.follow)
		{
			_player->_player.state = FLYHOLD;
			teamBomb.x = _player->_player.x - 20;
			teamBomb.y = _player->_player.y + teamBomb.downLength;
			teamBomb.ground = false;
			teamBomb.gravityOpen = true;
		}

	//시한 폭탄 
	teamBomb.rc = RectMakeCenter(teamBomb.x, teamBomb.y, 50, 50);

	bombPixelCollision();
	CastFunc();
	CurveLineFunc();

	// 든 상태에서 30초가 지나면 TB_DANGER1
	if (teamBomb.follow)
	{
		teamBomb.timeframeCount++;
		if (teamBomb.timeframeCount > 120)
		{
			teamBomb.tbState = TB_DANGER1;
		}
		if (teamBomb.timeframeCount > 300)
		{
			teamBomb.tbState = TB_DANGER2;
		}
		if (teamBomb.timeframeCount > 420)
		{
			teamBomb.timeOver = true;
			if (teamBomb.timeOver)
			{
				teamBomb.hitRC = RectMakeCenter(teamBomb.x, teamBomb.y, 80, 80);
				fx1.fire = true;
			}
			teamBomb.life = false;
			_player->_player.state = HURT;
		}
	}

	if (teamBomb.timeframeCount > 421 )
	{
		teamBomb.hitRC = RectMakeCenter(teamBomb.x, teamBomb.y, 1, 1);
		teamBomb.timeOver = false;
	}

	if (!teamBomb.life)
	{
		if (teamBomb.timeframeCount > 500)
		{
			teamBomb.timeframeCount = 0;
			teamBomb.x = 640;
			teamBomb.y = 550;
			teamBomb.follow = false;
			teamBomb.gravityOpen = false;
			teamBomb.tbState = TB_WALK;
			teamBomb.timeOver = false;
			teamBomb.life = true;
		}
	}

	if(!teamBomb.follow)bombMove();
}

void dungeonScene::bombMove()
{
	if (teamBomb.moveUp && !teamBomb.moveDown)
	{
		teamBomb.x -= 0.6f;
	}
	if (teamBomb.moveDown && !teamBomb.moveUp)
	{
		teamBomb.x += 0.6f;
	}

	teamBomb.moveFrame++;
	if (teamBomb.moveFrame > 300)
	{
		teamBomb.moveUp = false;
		teamBomb.moveDown = true;
	}
	if (teamBomb.moveFrame < 300)
	{
		teamBomb.moveUp = true;
		teamBomb.moveDown = false;
	}
	if (teamBomb.moveFrame > 600)
	{
		teamBomb.moveFrame = 0;
	}
}

void dungeonScene::CastFunc()
{
	if (teamBomb.follow)
	{
		if (KEYMANAGER->isOnceKeyUp(MK_RBUTTON))
		{
			teamBomb.cast = true;
			teamBomb.showCurve = false;
			teamBomb.follow = false;
			teamBomb.castGravity = 0;
			_player->_player.state = FLYIDLE;
		}
	}

	if (teamBomb.cast)
	{
		teamBomb.castGravity += 0.5f;
		teamBomb.gravity = 0;
		teamBomb.groundgrv = 0;
		teamBomb.x += cosf(teamBomb.angle) *  teamBomb.speed;
		teamBomb.y += -sinf(teamBomb.angle) * teamBomb.speed + teamBomb.castGravity;
	}
	if (_player->_player.ground)
	{
		teamBomb.cast = false;
		teamBomb.castGravity = 0;
	}
}

//궤적 만들기 
void dungeonScene::CurveLineFunc()
{
	if (teamBomb.follow)
	{
		if (KEYMANAGER->isStayKeyDown(MK_RBUTTON))
		{
			teamBomb.showCurve = true;
			teamBomb.angle = getAngle(teamBomb.x, teamBomb.y, _ptMouse.x, _ptMouse.y);
			curveLine[0].x = teamBomb.x;
			curveLine[0].y = teamBomb.y;
			curveLine[0].rc = RectMakeCenter(curveLine[0].x, curveLine[0].y, CURVE_CIRCLE_SIZE / 3, CURVE_CIRCLE_SIZE / 3);
			curveLine[0].angle = teamBomb.angle;

			for (int i = 0, j = 1; j < CURVE_CIRCLE_LINE; i++, j++)
			{
				if (j == 1) teamBomb.castGravity = 0.05f;
				else if (j == 2) teamBomb.castGravity = 2.0f;
				else if (j == 3) teamBomb.castGravity = 4.0f;
				else if (j == 4) teamBomb.castGravity = 6.0f;
				else if (j == 5) teamBomb.castGravity = 8.0f;
				else if (j == 6) teamBomb.castGravity = 12.0f;
				else if (j == 7) teamBomb.castGravity = 17.0f;
				else if (j == 8) teamBomb.castGravity = 22.0f;
				else if (j == 9) teamBomb.castGravity = 30.0f;

				curveLine[j].angle = curveLine[i].angle;
				curveLine[j].x = cosf(curveLine[i].angle) * CURVE_CIRCLE_SIZE;
				curveLine[j].y = -sinf(curveLine[i].angle) * CURVE_CIRCLE_SIZE + teamBomb.castGravity;

				curveLine[j].x = curveLine[j].x + curveLine[i].x;
				curveLine[j].y = curveLine[j].y + curveLine[i].y;

				curveLine[j].rc = RectMakeCenter(curveLine[j].x, curveLine[j].y, CURVE_CIRCLE_SIZE / 3, CURVE_CIRCLE_SIZE / 3);
			}
		}// Rbutton
	}
}


void dungeonScene::bombPixelCollision(void)
{
	if (teamBomb.gravityOpen)
	{
		//땅에있지도 않고 따라다니는 상태도 아니면, 떨어진다. 
		if (!teamBomb.ground && !teamBomb.follow)
		{
			//tem.geddyState = gFALL;
			teamBomb.groundgrv -= 1.4f;
			teamBomb.y -= teamBomb.groundgrv;
			if (teamBomb.groundgrv < -20) teamBomb.groundgrv = -20;
		}

		// i = x - 크기절반; i< x + 크기의 절반; i++
		for (int i = teamBomb.x - _player->geddy.radius; i < teamBomb.x + _player->geddy.radius; ++i)
		{
			// 만약 플레이어 y값+크기/2 아래 픽셀이 정해진 색이고 && 땅이 아니면
			if (GetPixel(getPixel(), i, _player->geddy.y + _player->geddy.radius) == RGB(255, 0, 255) && !teamBomb.ground)
			{
				teamBomb.ground = true; // 땅이라고 알려줌. 
				teamBomb.y += teamBomb.groundgrv; // 플레이어y += 땅 그래피티 
				teamBomb.groundgrv = 0;
			}
		}
		//바텀 
		for (int i = teamBomb.x - _player->geddy.radius; i < teamBomb.x + _player->geddy.radius; ++i)
		{
			if (GetPixel(getPixel(), i, teamBomb.y + _player->geddy.radius) == RGB(255, 0, 255) && teamBomb.gravity <= 0)
			{
				teamBomb.y += teamBomb.gravity;
				teamBomb.gravity = 20;
				break;
			}
		}
	}//gravityOpen
}//func


void dungeonScene::CurveLineRender(void)
{
	// 던질때 커브 원형들 그리기.
	if (teamBomb.showCurve)
	{
		for (int i = 0; i < CURVE_CIRCLE_LINE; i++)
		{
			IMAGEMANAGER->findImage("curvePoint")->render(getMemDC(), curveLine[i].rc.left, curveLine[i].rc.top);
			//Ellipse(getMemDC(), curveLine[i].rc.left, curveLine[i].rc.top, curveLine[i].rc.right, curveLine[i].rc.bottom);
		}
	}
}


// 구름 _________________________________________________________________________________________________________구름
void dungeonScene:: cloudInit()
{
	for (int i = 0; i < 2; i++)
	{
		cloud[i].life = true;
		cloud[i].waterFall = false;
		cloud[i].frameCount = 0;
		cloud[i].currentX = 0;
		cloud[i].follow = false;
		cloud[i].waterCurrentX = 0;
		cloud[i].waterFrameCount = 0;

		cloud[i].waterState = WF_IDLE; //우물 스테이트
		cloud[i].wfFrameCount = 0;
		cloud[i].wfCurrentX = 0;
		cloud[i].timeFrameCount = 0;
	}
	cloud[0].x = 1250;
	cloud[0].y = 220;
	cloud[1].x = 3000;
	cloud[1].y = 250;

	//500,412
	cloud[0].wbX = 560;
	cloud[0].wbY = 520;
	cloud[1].wbX = 2060;
	cloud[1].wbY = 710;
}

void dungeonScene::cloudUpdate()
{
	// follow시킴. 
	for (int i = 0; i < 2; i++)
	{
		RECT rcTemp;
		if (IntersectRect(&rcTemp, &_player->_player.rc, &cloud[i].rc))
		{
			if (KEYMANAGER->isOnceKeyDown('0'))
			{
				cloud[i].follow = true;
				cloud[i].waterFall = false;
			}
		}
	}

	// 다시 내려놓음. 
	for (int i = 0; i < 2; i++)
	{
		if (cloud[i].follow)
		{
			cloud[i].x = _player->_player.x;
			cloud[i].y = _player->_player.y+50;
			_player->_player.state = FLYHOLD;

			if (KEYMANAGER->isOnceKeyUp(MK_RBUTTON))
			{
				cloud[i].follow = false;
				_player->_player.state = FLYIDLE;
				cloud[i].waterFall = false;
			}
		}
	}

	// 올라탐. 
	RECT rcTemp;
	for (int i = 0; i<2; i++)
	{
		if (!cloud[i].follow) // 겹쳐서 이동이안되서 조건 걸어줌. 
		{
			if (IntersectRect(&rcTemp, &_player->_player.rc, &cloud[i].rc))
			{

				cloud[i].waterFall = true; // 물뿌림 
				_player->_player.ground = true; // 땅이라고 알려줌. 
				_player->_player.y += _player->_player.groundgrv; // 플레이어y += 땅 그래피티 
				_player->miniY += _player->_player.groundgrv;

				_player->_player.groundgrv = 0;
				_player->_player.jumpCount = 0;
				if (_player->_player.fly)
				{
					_player->_player.fly = false;
					_player->_player.state = IDLE;
				}
				if (_player->_player.fall)
				{
					_player->_player.fall = false;
					_player->_player.state = IDLE;
				}
			}
			else
			{
				cloud[i].waterFall = false;
			}
		}
	}


	// 물 뿌림. 
	for (int i = 0; i < 2; i++)
	{
		if (cloud[i].waterFall) 
		{
			cloud[i].waterRC = RectMake(cloud[i].x-30, cloud[i].y+20,60,1000);
		}
		else
		{
			cloud[i].waterRC = RectMake(0,0,10,10);
		}
	}

	// 우물 렉트
	for (int i = 0; i < 2; i++)
	{
		cloud[i].waterBowl = RectMake(cloud[i].wbX, cloud[i].wbY,90,150);
	}

	// 만약 우물렉트랑 구름물 렉트랑 부딧치면, 
	for (int i = 0; i < 2; i++)
	{
			if (IntersectRect(&rcTemp, &cloud[i].waterRC, &cloud[i].waterBowl))
			{
				if (i == 0)
				{
					cloud[i].waterState = WF_FALL;
					cloud[i].timeFrameCount++;
					if (cloud[i].timeFrameCount == 1)
					{
						_player->se9 = true;
					}
						if (cloud[i].timeFrameCount > 340)
					{
						//PostQuitMessage(0);true;
						cloud[i].waterState = WF_LAMPON;
						door[2].open = true;
						door[2].doorState = D_OPEN;
					}
				}
				if (i == 1) // 보스 벽돌이 위로 밀려야함. 
				{
					cloud[i].waterState = WF_FALL;
					cloud[i].timeFrameCount++;
					if (cloud[i].timeFrameCount > 340)
					{
						cloud[i].waterState = WF_LAMPON;
						bossDoor.push = true;
					}
				}
			}
	}
	
}//func

void dungeonScene:: cloudRender()
{
	for (int i = 0; i < 2; i++)
	{
		if (cloud[i].life)
		{
			IMAGEMANAGER->findImage("cloud")->frameRender(getMemDC(), cloud[i].rc.left, cloud[i].rc.top, cloud[i].currentX, 0);
			//Rectangle(getMemDC(), cloud[i].waterBowl.left, cloud[i].waterBowl.top, cloud[i].waterBowl.right, 
			//	cloud[i].waterBowl.bottom);
	    }
	}
}


// 링 ___________________________________________________________________________________________________________링 세로
void dungeonScene::ringInit()
{
	IMAGEMANAGER->addFrameImage("ringHeight", "item/ringHeight.bmp", 840*1.3, 280 * 1.3, 6, 2, true, RGB(255, 0, 255));
    IMAGEMANAGER->addFrameImage("ringHeightBehind", "item/ringHeightBehind.bmp", 840 * 1.3, 280 * 1.3, 6, 2, true, RGB(255, 0, 255));

	for (int i = 0; i < RINGMAX; i++)
	{
		ring[i].w = 20;
		ring[i].h = 150;
		ring[i].frameCount = 0;
		ring[i].currentX = 0;
		ring[i].eatenFrameCount = 0;
		ring[i].life = true;
		ring[i].eaten = false;

		ring[i].eat = false;
		ring[i].eatCount = 0;

		ring[i].ringState = RING_IDLE;
		ring[i].rc = {0,0,10,10}; // 의미없음.디버깅 할때 볼수도 있음. 
	}
	ring[0].x = 3300;
	ring[0].y = 2050;
	ring[4].x = 3450;
	ring[4].y = 2050;

	// 동굴 
	ring[1].x = 600;
	ring[1].y = 2624;
	ring[2].x = 700;
	ring[2].y = 2624;
	ring[3].x = 800;
	ring[3].y = 2624;
}

void dungeonScene::ringUpdate()
{
	RECT tempRC;
	for (int i = 0; i < RINGMAX; i++)
	{
		if (ring[i].life)
		{
			ring[i].rc = RectMake(ring[i].x, ring[i].y, ring[i].w, ring[i].h);
		}
		else
		{
			ring[i].rc = { 0,0,10,10 };
		}
	}

	// 플레이어랑 부딧치면, 
	for (int i = 0; i < RINGMAX; i++)
	{
		// 한번만 들어옴.
		if (IntersectRect(&tempRC, &ring[i].rc, &_player->_player.rc))
		{
			ring[i].ringState = RING_EATEN;

			ring[i].eaten = true;
			ring[i].eat = true;
			break;
		}
	}

	for (int i = 0; i < RINGMAX; i++)
	{
		if (ring[i].eat)
		{
			ring[i].eatCount++;
			if (ring[i].eatCount % 30 == 0) // 피가 한번만 닳게끔. 
			{
				DATABASE->getElementData("player")->coin += 10;
				ring[i].eat = false;
				ring[i].eatCount = 0;
			}
		}
	}

	// 플레이어랑 부딧치면, 
	for (int i = 0; i < RINGMAX; i++)
	{
		if (ring[i].eaten)
		{
			ring[i].eatenFrameCount++;
			if (ring[i].eatenFrameCount > 60)
			{
				ring[i].life = false;
				ring[i].eatenFrameCount = 0;
			}
		}
	}

	ringFrameFunc();
}

void dungeonScene::ringRender()
{
	for (int i = 0; i < RINGMAX; i++)
	{
		if (ring[i].life)
		{
			//Rectangle(getMemDC(), ring[i].rc.left, ring[i].rc.top, ring[i].rc.right, ring[i].rc.bottom);

			IMAGEMANAGER->findImage("ringHeight")->frameRender(getMemDC(), ring[i].rc.left-68, ring[i].rc.top-25,
				ring[i].currentX,ring[i].ringState);
		}
	}
}

// 플레이어 뒤에 렌더 될 링,  
void dungeonScene::ringRenderBehind()
{
	for (int i = 0; i < RINGMAX; i++)
	{
		if (ring[i].life)
		{
			IMAGEMANAGER->findImage("ringHeightBehind")->frameRender(getMemDC(), ring[i].rc.left - 68, ring[i].rc.top - 25,
				ring[i].currentX, ring[i].ringState);
		}
	}
}

void dungeonScene::ringFrameFunc()
{
	for (int i = 0; i < RINGMAX; i++)
	{
		ring[i].frameCount++;
		switch (ring[i].ringState)
		{
		case RING_IDLE:
			if (ring[i].frameCount > 60)
			{
				ring[i].frameCount = 0;
				ring[i].currentX++;
				if (ring[i].currentX > 1)
				{
					ring[i].currentX = 0;
				}
			}
			break;
		case RING_EATEN:
			if (ring[i].frameCount > 12)
			{
				ring[i].frameCount = 0;
				ring[i].currentX++;
				if (ring[i].currentX > 5)
				{
					ring[i].currentX = 0;
				}
			}
			break;
		default:
			break;
		}
	}
}


// 링 ___________________________________________________________________________________________________________링 가로
void dungeonScene::WringInit()
{
	IMAGEMANAGER->addFrameImage("ringWidth", "item/ringWidth.bmp", 840 * 1.3, 280 * 1.3, 6, 2, true, RGB(255, 0, 255));
	
	IMAGEMANAGER->addFrameImage("ringWidthBehind", "item/ringWidthBehind.bmp", 840 * 1.3, 280 * 1.3, 6, 2, true, RGB(255, 0, 255));
	
	for (int i = 0; i < RINGMAX; i++)
	{
		wring[i].w = 150;
		wring[i].h = 20;
		wring[i].frameCount = 0;
		wring[i].currentX = 0;
		wring[i].eatenFrameCount = 0;
		wring[i].life = true;
		wring[i].eaten = false;

		wring[i].eat = false;
		wring[i].eatCount = 0;

		wring[i].wringState = WRING_IDLE;
		wring[i].rc = { 0,0,10,10 }; // 의미없음.디버깅 할때 볼수도 있음. 
	}
	wring[0].x = 500;
	wring[0].y = 200;
	wring[4].x = 500;
	wring[4].y = 300;

	wring[1].x = 200;
	wring[1].y = 1800;
	wring[2].x = 200;
	wring[2].y = 1700;
	wring[3].x = 200;
	wring[3].y = 1600;

	wring[5].x = 2550;
	wring[5].y = 1000;
	wring[6].x = 2550;
	wring[6].y = 1150;
}


void dungeonScene::WringUpdate()
{
	RECT tempRC;
	// 렉트 업데이트
	for (int i = 0; i < RINGMAX; i++)
	{
		if (wring[i].life)
		{
			wring[i].rc = RectMake(wring[i].x, wring[i].y, wring[i].w, wring[i].h);
		}
		else
		{
			wring[i].rc = { 0,0,10,10 };
		}
	}

	// 플레이어랑 부딧치면, 
	for (int i = 0; i < RINGMAX; i++)
	{
		// 한번만 들어옴.
		if (IntersectRect(&tempRC, &wring[i].rc, &_player->_player.rc))
		{
			wring[i].wringState = WRING_EATEN;
			wring[i].eaten = true;
			wring[i].eat = true;
			break;
		}
	}


	for (int i = 0; i < RINGMAX; i++)
	{
		if (wring[i].eat)
		{
			wring[i].eatCount++;
			if (wring[i].eatCount % 30 == 0) // 피가 한번만 닳게끔. 
			{
				DATABASE->getElementData("player")->coin += 10;
				wring[i].eat = false;
				wring[i].eatCount = 0;
			}
		}
	}

	// 플레이어랑 부딧치면, 
	for (int i = 0; i < RINGMAX; i++)
	{
		if (wring[i].eaten)
		{
			wring[i].eatenFrameCount++;
			if (wring[i].eatenFrameCount > 60)
			{
				wring[i].life = false;
				wring[i].eatenFrameCount = 0;
			}
		}
	}

	WringFrameFunc();
}

void dungeonScene::WringRender()
{
	for (int i = 0; i < RINGMAX; i++)
	{
		if (wring[i].life)
		{
			//Rectangle(getMemDC(), wring[i].rc.left, wring[i].rc.top, wring[i].rc.right, wring[i].rc.bottom);

			IMAGEMANAGER->findImage("ringWidth")->frameRender(getMemDC(), wring[i].rc.left - 25, wring[i].rc.top - 90,
				wring[i].currentX, wring[i].wringState);
		}
	}
}

// 플레이어 뒤에 렌더 될 링,  
void dungeonScene::WringRenderBehind()
{
	for (int i = 0; i < RINGMAX; i++)
	{
		if (wring[i].life)
		{
			IMAGEMANAGER->findImage("ringWidthBehind")->frameRender(getMemDC(), wring[i].rc.left - 25, wring[i].rc.top - 90,
				wring[i].currentX, wring[i].wringState);
		}
	}
}

void dungeonScene::WringFrameFunc()
{
	for (int i = 0; i < RINGMAX; i++)
	{
		wring[i].frameCount++;
		switch (wring[i].wringState)
		{
		case WRING_IDLE:
			if (wring[i].frameCount > 60)
			{
				wring[i].frameCount = 0;
				wring[i].currentX++;
				if (wring[i].currentX > 1)
				{
					wring[i].currentX = 0;
				}
			}
			break;
		case WRING_EATEN:
			if (wring[i].frameCount > 12)
			{
				wring[i].frameCount = 0;
				wring[i].currentX++;
				if (wring[i].currentX > 5)
				{
					wring[i].currentX = 0;
				}
			}
			break;
		default:
			break;
		}
	}
}



//effect &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& 시한폭탄 & 플레이어

void dungeonScene::fx1Init(void)
{
	IMAGEMANAGER->addFrameImage("teamBombFX", "effect/teamBombFX.bmp", 448 * 1.5,114*1.5, 8, 1, true, RGB(255, 0, 255));

	fx1.frameCount = 0;
	fx1.currentX = 0;
	fx1.fire = false;
}

void dungeonScene::fx1Update(void)
{
	if (fx1.fire)
	{
		fx1.frameCount++;
		if (fx1.frameCount > 8)
		{
			fx1.frameCount = 0;
			fx1.currentX++;
			if (fx1.currentX > 7)
			{
				fx1.currentX = 0;
			    fx1.fire = false;
			}
		}
	}
}

void dungeonScene::fx1Render(void)
{
	if (fx1.fire)
	{
		IMAGEMANAGER->findImage("teamBombFX")->frameRender(getMemDC(), teamBomb.hitRC.left+40,
			teamBomb.hitRC.top - 80, fx1.currentX, 0);
	}
}


//effect &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& 시한폭탄 & 돌

void dungeonScene::fx2Init(void)
{
	IMAGEMANAGER->addFrameImage("teamBombFX", "effect/teamBombFX.bmp", 448 * 2, 114 * 2, 8, 1, true, RGB(255, 0, 255));

	fx2.frameCount = 0;
	fx2.currentX = 0;
	fx2.fire = false;
}

void dungeonScene::fx2Update(void)
{
	if (fx2.fire)
	{
		fx2.frameCount++;
		if (fx2.frameCount > 8)
		{
			fx2.frameCount = 0;
			fx2.currentX++;
			if (fx2.currentX > 7)
			{
				fx2.currentX = 0;
				fx2.fire = false;
			}
		}
	}
}

void dungeonScene::fx2Render(void)
{
	if (fx2.fire)
	{
		IMAGEMANAGER->findImage("teamBombFX")->frameRender(getMemDC(),puzzleRock.rc.left+70, puzzleRock.rc.top, fx2.currentX, 0);
	}
}