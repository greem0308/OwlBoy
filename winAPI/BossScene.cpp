#include "stdafx.h"
#include "BossScene.h"


BossScene::BossScene()
{
}


BossScene::~BossScene()
{
}


HRESULT BossScene::init(void)
{
	gameNode::init();
	IMAGEMANAGER->addImage("bossTalk_0", "UI/bossTalk_0.bmp", 150, 50, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("bossTalk_1", "UI/bossTalk_1.bmp", 150, 50, true, RGB(255, 0, 255));

	//etc
	IMAGEMANAGER->addFrameImage("bossShip", "boss/bossShip.bmp",1293*1.8,283*1.8,3,1,true,RGB(255,0,255));

	IMAGEMANAGER->addImage("bglayer1", "boss/bossBG_noCamera_IN_layer1.bmp", 1280, 720, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("bglayer2", "boss/bossBG_noCamera_IN_layer2.bmp", 1280, 720, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("bglayer3", "boss/bossBG_noCamera_IN_layer3.bmp", 1280, 720, true, RGB(255, 0, 255));

	// waterFall
	IMAGEMANAGER->addFrameImage("waterfall1", "boss/waterfall1.bmp",60*2,910 *2, 3, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("waterfall2", "boss/waterfall2.bmp",30 *2,560 *2, 3, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("waterfallHitFloor", "boss/waterfallHitFloor.bmp", 192* 2, 19 * 2, 4, 1, true, RGB(255, 0, 255));

	// rock
	IMAGEMANAGER->addImage("rockRight", "boss/rockRight.bmp", 270 * 1.2, 50 * 1.2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("rockLeft", "boss/rockLeft.bmp", 270 * 1.2, 50 * 1.2, true, RGB(255, 0, 255));

	//sky
	IMAGEMANAGER->addImage("skyLayer1", "boss/skyLayer1_1280.bmp", 1280, 720, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("skyLayer2", "boss/skyLayer2_1280.bmp", 1280, 720, true, RGB(255, 0, 255));

	// pixelPink
	IMAGEMANAGER->addImage("bossBG_pixelPink", "boss/bossBG_pixelPink.bmp", 1280,720, false);
	IMAGEMANAGER->addImage("boss_blue", "boss/boss_blue.bmp", 1280, 720, false);
	IMAGEMANAGER->addImage("boss_green", "boss/boss_green.bmp", 1280, 720, false);
	IMAGEMANAGER->addImage("boss_yellow", "boss/boss_yellow.bmp", 1280, 720, false);

	_player = new player;
	_player->init(1170,220);
	_player->playNum = 7;
	_player->geddy.x = 1170;
	_player->geddy.y = 220;

	rc = RectMake(WINSIZEX / 2 - 300, WINSIZEY / 2, 50, 50);

	cameraX = 0;
	cameraY = 0;

	ship.x = 300;
	ship.y = 190;
	ship.frameCount = 0;
	ship.currentX = 0;

	layer1_offsetX = 0;
	layer2_offsetX = 0;

	// waterfall
	for (int i = 0; i < 3; i++)
	{
		waterFall[i].frameCount=0;
		waterFall[i].currentX=0;
	}
	waterFall[0].x = 170;
	waterFall[0].y = -1160;
	waterFall[1].x = 1160;
	waterFall[1].y = 300;
	// 밑에 돌에 부딧치면 생기는 거품 
	waterFall[2].x = 150;
	waterFall[2].y = 630;

	bossInit();
	shipInit();
	shipEffectInit();

	boss._fire2 = new missileM2;
	boss._fire2->init(10, 500);

	// 공격 패턴 
	bossAttack = false; // 트루되면 공격. 
	cannonAttack = false; // 트루되면 공격
	attackCount = 0; 
	attackPettern = 1; // 현재 패턴 상태. 
	bossIN = false;
	bossOUT = false;
	bossIN2 = false; //패턴 2를 위한 불. 
	bossOUT2= false;
	bossIN3 = false; //패턴 3를 위한 불. 
	bossFirstFollow = false;
	
	shipAttackedCount2 = 0;
	shipAttackedCount3 = 0;
	realCount2 = 0;
	realCount3 = 0;

	boss.downDistance = 0;
	boss.downRC = RectMake(boss.down.x, boss.down.y,10,10);

	// 이펙트 시간. 
	fxFrameCount=0;
	fxCurrentX=0;
	
	fx3Init();

	return S_OK;
}

void BossScene::release(void)
{
	gameNode::release();
	//SAFE_DELETE(_player);
}

void BossScene::update(void)
{
	gameNode::update();
	_player->update();
	boss._fire2->update();
	shipUpdate();
	shipEffectUpdate();
	fx3Update();

	bgFrameFunc(); // 배경 애니메이션, 렉트 업데이트

	bossUpdate();
	
	//if (KEYMANAGER->isOnceKeyDown(MK_LBUTTON))
	//{
	//	DATABASE->getElementData("player")->currentHP = _player->_player.currentHP;
	//	SCENEMANAGER->changeScene("VellieScene");
	//}

	// shipRock
	for (int i = 0; i < 3; i++)
	{
		if (shipRock[i].life)
		{
			shipRock[i].rc = RectMake(shipRock[i].x, shipRock[i].y, 90, 50);
		}
	}
}

void BossScene::render(void)
{
	bgRender(); // 배경 렌더

	shipRender();

	bossRender();
	fx3Render();

	shipEffectRender();

	boss._fire2->render();

	//char str[128];
	//SetTextColor(getMemDC(), RGB(255,0,0));
	//HFONT myFont = CreateFont(25, 0, 0, 0, 1, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, TEXT("HY궁서B"));
	//SelectObject(getMemDC(), myFont);

	//sprintf(str, "%d", attackCount);
	//TextOut(getMemDC(), 1200, 50, str, strlen(str));

	//sprintf(str, "%2.0f", boss.downDistance);
	//TextOut(getMemDC(), 1200, 100, str, strlen(str));
	//
	//sprintf(str, "bossIN %d", bossIN);
	//TextOut(getMemDC(), 1100, 150, str, strlen(str));
	//sprintf(str, "bossOUT %d", bossOUT);
	//TextOut(getMemDC(), 1100, 200, str, strlen(str));
	//sprintf(str, "bossIN2 %d", bossIN2);
	//TextOut(getMemDC(), 1100, 250, str, strlen(str));
	//sprintf(str, "aP %d", attackPettern);
	//TextOut(getMemDC(), 1100, 300, str, strlen(str));

	//sprintf(str, "dpc %d", boss.downFrameCount);
	//TextOut(getMemDC(), 1100, 350, str, strlen(str));
	//sprintf(str, "bossIN3 %d", bossIN3);
	//TextOut(getMemDC(), 1100, 400, str, strlen(str));
	//sprintf(str, "bossOUT2 %d", bossOUT2);
	//TextOut(getMemDC(), 1100, 450, str, strlen(str));

	//DeleteObject(myFont);

	//Rectangle(getMemDC(), boss.downRC.left, boss.downRC.top, boss.downRC.right, boss.downRC.bottom);

	IMAGEMANAGER->findImage("bglayer1")->render(getMemDC());
	_player->render();

}

void BossScene::bgRender()
{
	// 뒤에 픽셀배경들 안되게끔.
	IMAGEMANAGER->findImage("mapImage")->render(getMemDC()); // 원래
	IMAGEMANAGER->findImage("mapImage")->render(getPixel()); // 핑크
	IMAGEMANAGER->findImage("mapImage")->render(getPixelBlue()); // 블루
	IMAGEMANAGER->findImage("mapImage")->render(getPixelGreen()); // 그린
	IMAGEMANAGER->findImage("mapImage")->render(getPixelYellow()); // 옐로우

	IMAGEMANAGER->findImage("bossBG_pixelPink")->render(getPixel());
	IMAGEMANAGER->findImage("boss_blue")->render(getPixelBlue());
	IMAGEMANAGER->findImage("boss_green")->render(getPixelGreen());
	IMAGEMANAGER->findImage("boss_yellow")->render(getPixelYellow());

	RECT loopRC = RectMake(0, 0, 1280, 720);
	IMAGEMANAGER->findImage("skyLayer2")->loopRender(getMemDC(), &loopRC, layer2_offsetX, 0);
	IMAGEMANAGER->findImage("skyLayer1")->loopRender(getMemDC(), &loopRC, layer1_offsetX, 0);

	IMAGEMANAGER->findImage("bglayer3")->render(getMemDC());

	IMAGEMANAGER->findImage("waterfall2")->frameRender(getMemDC(), waterFall[1].rc.left, waterFall[1].rc.top, waterFall[1].currentX, 0);

	IMAGEMANAGER->findImage("bglayer2")->render(getMemDC());

	IMAGEMANAGER->findImage("waterfall1")->frameRender(getMemDC(), waterFall[0].rc.left, waterFall[0].rc.top, waterFall[0].currentX, 0);
	IMAGEMANAGER->findImage("waterfallHitFloor")->frameRender(getMemDC(), waterFall[2].rc.left, waterFall[2].rc.top, waterFall[2].currentX, 0);

	// 떨어질 돌 렌더 ____________________________________________________________________________________________________
	if (shipRock[0].life)
	{
		IMAGEMANAGER->findImage("piraterock1")->render(getMemDC(), shipRock[0].rc.left - 10, shipRock[0].rc.top - 15);
	}
	if (shipRock[1].life)
	{
		IMAGEMANAGER->findImage("piraterock2")->render(getMemDC(), shipRock[1].rc.left - 10, shipRock[1].rc.top - 15);
	}
	if (shipRock[2].life)
	{
		IMAGEMANAGER->findImage("piraterock3")->render(getMemDC(), shipRock[2].rc.left - 65, shipRock[2].rc.top - 200);
	}
	// 떨어질돌 렌더.  살아 있는 돌만.. 
	for (int i = 0; i < 3; i++)
	{
		//Rectangle(getMemDC(), shipRock[i].rc.left, shipRock[i].rc.top, shipRock[i].rc.right, shipRock[i].rc.bottom);
	}

	
	IMAGEMANAGER->findImage("bossShip")->frameRender(getMemDC(), ship.rc.left, ship.rc.top, ship.currentX, 0);
	IMAGEMANAGER->findImage("rockRight")->render(getMemDC(),280,630);
	IMAGEMANAGER->findImage("rockLeft")->render(getMemDC(), 700,630);
}


// boss _______________________________________________________________________________________________________ boss
void BossScene::bossInit(void)
{
	//boss
	IMAGEMANAGER->addFrameImage("bossBodyLeft", "boss/bossBodyLeft.bmp", 1265 * 1.8, 770 * 1.8, 11, 7, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("bossBodyRight", "boss/bossBodyRight.bmp", 1265 * 1.8, 770 * 1.8, 11, 7, true, RGB(255, 0, 255));
	
	//cannon
	IMAGEMANAGER->addFrameImage("cannonINOUT", "boss/cannonINOUT.bmp", 730 * 1.8, 291 * 1.8, 10, 3, true, RGB(255, 0, 255));

	IMAGEMANAGER->addFrameImage("bossGun", "boss/bossGun.bmp", 900*1.8, 2400 *1.8, 6, 16, true, RGB(255, 0, 255));
	
	IMAGEMANAGER->addFrameImage("bossFollowerLeft", "boss/bossFollowerLeft.bmp", 891 * 1.4, 210 * 1.4, 11, 3, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("bossFollowerRight", "boss/bossFollowerRight.bmp", 891 * 1.4, 210 * 1.4, 11, 3, true, RGB(255, 0, 255));

	boss.angle = PI; 
	boss.bossState = bIDLE;
	boss.x = 300;
	boss.y = 620;

	// 이 지점에 와서 내려감. 
	boss.down.x = 300;
	boss.down.y = 200;

	// 팔로워 좌표
	boss.fx = 300;
	boss.fy = 0;

	boss.life = true;
	boss.follow = false;
	boss.frameCount = 0;
	boss.currentX = 0;
	boss.gunFrameCount = 0;
	boss.gunCurrentX = 0;
	boss.gunCurrentY = 0;
	boss.shootFrameCount = 0;
	boss.shootCurrentX = 0;
	
	boss.bodyFrameX = 0;
	boss.bodyFrameY = 0;

	boss.speed = 10.0f;
	
	boss.FframeCount=0;
	boss.FcurrentX=0;

	boss.bossState = bIDLE;
	boss.shootState = bossNONESHOOT;
	boss.direction = bossRIGHT;
	boss.bfState = BF_SLEEP;

	boss.bodyExcelY = 0;

	boss.downFrameCount = 0;

	start = false;

	talk = false;
	talkFrame = 0;
	talkCurrentX = 0;

	bossTalk = false;
	bossTalkFrame = 0;

	boss.followerExcelX = 0;
	boss.followerExcelY = 0;
	boss.followerExcelY_Down = 0;

	// 보스가 갈수 있는 거리 제한. 
	maxTop.x = 380; // 맨왼쪽
	maxBottom.x = 1000; //맨 오른쪽. 
	maxTop.y;
	maxBottom.y;

	shipShakeFrame = 0;
	shipShakeUp = true;
	shipShakeDown = false;

	boss.excelUp = 0;
	boss.excelDown = 0;
	shake = false;

	boss.hitCheck = false;
	boss.hitCount = 0;
	boss.attackCount = 0;

	boss.gFrameCount = 0;  // 2초에 한번 쏨.

	// 플레이어를 위한.
	hurt = false;
	hitCount = 0;

	inoutState = CANNON_IDLE;
	inoutFrameCount=0;
	inoutCurrentX=0;
	cannonInoutX = 545;
	cannonInoutY = 455;
}

void BossScene::bossUpdate(void)
{
	RECT rcTemp;
	bossStart();
	bossPettern();
	bossFunc(); // 각도

	boss.bodyRC = RectMakeCenter(boss.x, boss.y, 60, 60);
	boss.gunRC = RectMakeCenter(boss.x, boss.y, 10, 10);
	boss.followerRC = RectMakeCenter(boss.fx, boss.fy, 40, 40);
	
	// 보스,캐논인아웃, 팔로워 애니메이션
	bossFrameFunc();
	bossFollowerFunc(); 
	cannonINPUTframeFunc(); // 캐논인아웃 애니메이션

	bossCollision();

	//만약 player한테 맞으면,
	if (boss.hitCheck)
	{
		boss.hitCount++;
		if (boss.hitCount % 20 == 0)
		{
			//PostQuitMessage(0);
			attackCount++;
			boss.bossState = bHURT;
			boss.hitCheck = false;
		}
	}
}

void BossScene::bossRender(void)
{
	if (talk)
	{
		IMAGEMANAGER->findImage("bossTalk_0")->render(getMemDC(), boss.bodyRC.left-40, boss.bodyRC.top - 80);
	}
	if (bossTalk)
	{
		IMAGEMANAGER->findImage("bossTalk_1")->render(getMemDC(), boss.bodyRC.left - 40, boss.bodyRC.top - 80);
	}

	IMAGEMANAGER->findImage("cannonINOUT")->frameRender(getMemDC(), cannonInoutX-4, cannonInoutY+5, inoutCurrentX, inoutState);

	//보스 이미지
	if (boss.life)
	{
		if (boss.direction == bossRIGHT)
		{
			IMAGEMANAGER->findImage("bossBodyRight")->frameRender(getMemDC(), boss.bodyRC.left-70,
				boss.bodyRC.top - 70, boss.bodyFrameX, boss.bossState);
			IMAGEMANAGER->findImage("bossFollowerRight")->frameRender(getMemDC(), boss.followerRC.left -40, boss.followerRC.top - 40, boss.FcurrentX,boss.bfState);
		}
		else if (boss.direction == bossLEFT)
		{
			IMAGEMANAGER->findImage("bossBodyLeft")->frameRender(getMemDC(), boss.bodyRC.left - 70, boss.bodyRC.top - 70,
				boss.bodyFrameX, boss.bossState);
			IMAGEMANAGER->findImage("bossFollowerLeft")->frameRender(getMemDC(), boss.followerRC.left-40, boss.followerRC.top - 40, boss.FcurrentX, boss.bfState);
		}

		if (!bossIN && !bossOUT && !bossOUT2 && !bossIN2 && !bossIN3) //계속 행동이 붙잡혀있어서 이 조건을 걸어줌. 
		{
			IMAGEMANAGER->findImage("bossGun")->frameRender(getMemDC(), boss.gunRC.left - 125, boss.gunRC.top - 115,
				boss.shootCurrentX, boss.gunCurrentX);
		}
		//Rectangle(getMemDC(), boss.bodyRC.left, boss.bodyRC.top, boss.bodyRC.right, boss.bodyRC.bottom);
		//Rectangle(getMemDC(), boss.gunRC.left, boss.gunRC.top, boss.gunRC.right, boss.gunRC.bottom);
		//Rectangle(getMemDC(), boss.followerRC.left, boss.followerRC.top, boss.followerRC.right, boss.followerRC.bottom);
	}
}

//패턴  bossAttackFunc(),bossINupdate(),bossOUTupdate()  함수들 작동하는 곳 
void BossScene::bossPettern()
{
	RECT rcTemp;

	  if (start)
	  {
		  if (bossFirstFollow)
		  {
			  // 만약 스타트되면, 팔로어가 내려옴. 
			  if (!boss.follow)
			  {
				  boss.bfState = BF_WAKEUP;
				  boss.followerExcelY += 0.15f;
				  boss.fy += boss.followerExcelY;
				  if (IntersectRect(&rcTemp, &boss.bodyRC, &boss.followerRC))
				  {
					  boss.follow = true;
					  bossFirstFollow = false;
					  boss.followerExcelY = 0;
					  bossAttack = true;
					  inoutState = CANNON_IDLE;
				  }
			  }
		  } // if bossFirstFollow
		}//start
	  
	  if (boss.follow)
	  {
		  // 몸
		  boss.bossState = bIDLE;
		  boss.bfState = BF_MOVE; // 팔로워
		  boss.x = boss.fx;
		  boss.y = boss.fy + 50;
		  // 이제 위아래로 가속도 받아서 왔다갔다 해야하는데...
	  }

	  //왔다 갔다 공격. 
	  bossAttackFunc(); 

			// 들어갈때 나올때
			bossINupdate();
			bossOUTupdate();


			 // 처음 attack1 에서 10번 맞으면 대포로 전환되야 됨.. 
			if (attackPettern == 1 && attackPettern != 2 && attackPettern != 3)
			{
				if (attackCount > 4)
				{
					bossAttack = false;
					bossIN = true;
				}
			}

			if (attackPettern == 2)
			{
				if (attackCount > 7)
				{
					bossAttack = false;
					bossIN2 = true;
				}
			}
			
			if (attackPettern == 3)
			{
				if (attackCount > 10)
				{
					bossAttack = false;
					bossIN3 = true;
				}
			}
			// 배 대포랑, 위에 돌 충돌하면 떨어지게 하는 함수. 
			shipRockCollision();
			// 배 부서지는 이펙트 
			// 패턴을 변경시킴.  // 게임오버 시킬 부분.
			shipEffectInPettern();
}

void BossScene::shipRockCollision()
{
	RECT rcTemp;
	if (attackPettern == 1)
	{
		// 캐논 대포알과  위의 첫번째 돌이 충돌 하면, 첫번째 돌이 떨어진다. __________________
		for (int j = 0; j < _cannon._bullet->getVBullet().size(); j++)
		{
			if (IntersectRect(&rcTemp, &shipRock[0].rc, &_cannon._bullet->getVBullet()[j].rc))
			{
				shipRock[0].fall = true;
				break;
			}
		}
	}
	if (attackPettern == 2)
	{
		// 캐논 대포알과  위의 첫번째 돌이 충돌 하면, 첫번째 돌이 떨어진다. __________________
		for (int j = 0; j < _cannon._bullet->getVBullet().size(); j++)
		{
			if (IntersectRect(&rcTemp, &shipRock[1].rc, &_cannon._bullet->getVBullet()[j].rc))
			{
				shipAttackedCount2++;
				if (shipAttackedCount2 % 20 == 0) // 피가 한번만 닳게끔. 
				{
					shipAttackedCount2 = 0;
					realCount2 += 1;
				}
				if (realCount2 >= 1)
				{
					shipRock[1].fall = true;
				}
				break;
			}
		}
	}
	if (attackPettern == 3)
	{
		// 캐논 대포알과  위의 첫번째 돌이 충돌 하면, 첫번째 돌이 떨어진다. __________________
		for (int j = 0; j < _cannon._bullet->getVBullet().size(); j++)
		{
			if (IntersectRect(&rcTemp, &shipRock[2].rc, &_cannon._bullet->getVBullet()[j].rc))
			{
				shipAttackedCount3++;
				if (shipAttackedCount3 % 20 == 0) // 피가 한번만 닳게끔. 
				{
					shipAttackedCount3 = 0;
					realCount3 += 1;
				}
				if (realCount3 >= 3)
				{
					shipRock[2].fall = true;
				}
				break;
			}
		}
	}
}

// 배 부서지는 이펙트 
void BossScene::shipEffectInPettern()
{
	// 만약 fall이 트루라면 떨어져서 배에 충격을 준다. (단계별로.)
	for (int i = 0; i < 3; i++)
	{
		if (shipRock[i].fall)
		{
			if (!shipRock[i].life) continue;
			else
			{
				if (shipRock[i].y < 680)
				{
					shipRock[i].shipOffsetY += 0.1f;
					shipRock[i].y += shipRock[i].shipOffsetY;
				}
				if (shipRock[i].y > 680)
				{
					shipRock[i].shipOffsetY = 0;
					for (int i = 0; i < 5; i++)
					{
						fx[i].life = true;
					}
					if (i == 0) // 첫번째 돌이 떨어졌다면, 배 어택받은 애니메이션 돌린다. 
					{
						fxFrameCount++;
						if (fxFrameCount > 90)
						{
							for (int i = 0; i < 5; i++)
							{
								fx[i].life = false;
							}
							bossOUT = true;
							cannonAttack = false;
							shipRock[i].life = false;
						}
					}
					if (i == 1) // 두번째 돌이 떨어졌다면, 3초 어택받은 애니메이션 돌린다. 
					{
						fxFrameCount++;
						if (fxFrameCount > 150)
						{
							for (int i = 0; i < 5; i++)
							{
								fx[i].life = false;
							}
							bossOUT2 = true;
							cannonAttack = false;
							shipRock[i].life = false;
						}
					}
					if (i == 2) // 세번째 돌이 떨어졌다면, 8초 배 어택받은 애니메이션 돌린다. 
					{
						fxFrameCount++;
						if (fxFrameCount == 0)
						{
							_player->se10 = true;
						}
						if (fxFrameCount > 360)
						{
							for (int i = 0; i < 5; i++)
							{
								fx[i].life = false;
							}
						}
						cannonAttack = false;
						shipRock[i].life = false;
						// 여기다 게임오버 넣어줘야함. 마지막에.
					}
				}// if 680
			} // if(life == true)
			} // if fall
	} // for
}

// 각도랑 총쏘고 애니메이션 관련...
void BossScene::bossFunc(void)
{
	// 보스와 플레이어와의 각도 
	boss.angle = getAngle(boss.x, boss.y, _player->_player.x, _player->_player.y);

	if (boss.follow)
	{
		if (boss.angle >= PI * 2) boss.angle -= PI * 2;
		boss.gunCurrentX = int(boss.angle / PI_8); // 총 이미지 프레임. 


		// 총든 팔 프레임Y  
		if (boss.shootState == bossSHOOT)
		{
			boss.shootFrameCount++;
			if (boss.shootFrameCount > 3)
			{
				boss.shootFrameCount = 0;
				boss.shootCurrentX++;
				if (boss.shootCurrentX > 5)
				{
					boss.shootCurrentX = 0;
					boss.shootState = bossNONESHOOT;
				}
			}
		}
	}// follow 
}

// 배 왔다갔다 총쏨. 
void BossScene::bossAttackFunc()
{
	// 상단 하단 경계점을 두자. 
	if (bossAttack && !bossIN && !bossOUT && !bossIN2 && !bossOUT2 && !bossIN3)
	{
		if (!shake)
		{
			if (boss.fy > boss.down.y + 200)
			{
				boss.followerExcelY += 0.1f;
				boss.fy -= boss.followerExcelY;
			}
			if (boss.fy <= boss.down.y + 200)
			{
				shake = true;
				boss.followerExcelY = 0;
			}
		}

		//흔들면서 총쏨
		if (shake)
		{
			// 플레이어 따라오는데, 거리가 130이하면 안따라오게...
			int getDis = getDistance(boss.fx, boss.fy, _player->_player.x, _player->_player.y);
			if (boss.fx < _player->_player.x && getDis > 130)
			{
				boss.fx += 1.2f;
			}
			if (boss.fx > _player->_player.x && getDis > 130)
			{
				boss.fx -= 1.2f;
			}

			// 위아래 왔다갔다.
			if (shipShakeUp && !shipShakeDown)
			{
				boss.excelDown += 0.085f;
				boss.excelUp = 0;
				boss.fy -= boss.excelDown;
			}
			if (shipShakeDown && !shipShakeUp)
			{
				boss.excelUp += 0.08f;
				boss.excelDown = 0;
				boss.fy += boss.excelUp;
			}

			shipShakeFrame++;
			if (shipShakeFrame < 70)
			{
				shipShakeUp = true;
				shipShakeDown = false;
			}
			if (shipShakeFrame >= 70)
			{
				shipShakeUp = false;
				shipShakeDown = true;
			}
			if (shipShakeFrame >= 140)
			{
				shipShakeFrame = 0;
			}

			// 몇초에 한번 총을 쏠건지
			boss.gFrameCount++;
			if (boss.gFrameCount > 300)
			{
				boss.shootState = bossSHOOT;

				if (attackPettern == 1) //3발 
				{
					_player->se8 = true;
					fx3.fire = true;
					boss._fire2->fire(boss.x, boss.y, boss.angle, boss.gunCurrentX + 1, 5.0f);
					boss._fire2->fire(boss.x, boss.y, boss.angle + PI / 8, boss.gunCurrentX, 5.0f);
					boss._fire2->fire(boss.x, boss.y, boss.angle - PI / 8, boss.gunCurrentX - 1, 5.0f);
					boss.gFrameCount = 0;
				}
				if (attackPettern == 2) //5발 
				{
					_player->se8 = true;

					fx3.fire = true;
					boss._fire2->fire(boss.x, boss.y, boss.angle, boss.gunCurrentX, 7.0f);
					boss._fire2->fire(boss.x, boss.y, boss.angle + PI / 8, boss.gunCurrentX + 1, 7.0f);
					boss._fire2->fire(boss.x, boss.y, boss.angle - PI / 8, boss.gunCurrentX - 1, 7.0f);
					boss._fire2->fire(boss.x, boss.y, boss.angle + (PI / 8 + PI / 8), boss.gunCurrentX + 2, 7.0f);
					boss._fire2->fire(boss.x, boss.y, boss.angle - (PI / 8 + PI / 8), boss.gunCurrentX - 2, 7.0f);
					boss.gFrameCount = 0;
				}
				if (attackPettern == 3) //7발 
				
				{_player->se8 = true;

					fx3.fire = true;
					boss._fire2->fire(boss.x, boss.y, boss.angle, boss.gunCurrentX, 8.0f);
					boss._fire2->fire(boss.x, boss.y, boss.angle + PI / 8, boss.gunCurrentX + 1, 8.0f);
					boss._fire2->fire(boss.x, boss.y, boss.angle - PI / 8, boss.gunCurrentX - 1, 8.0f);
					boss._fire2->fire(boss.x, boss.y, boss.angle + (PI / 8 + PI / 8), boss.gunCurrentX + 2, 8.0f);
					boss._fire2->fire(boss.x, boss.y, boss.angle - (PI / 8 + PI / 8), boss.gunCurrentX - 2, 8.0f);
					boss._fire2->fire(boss.x, boss.y, boss.angle + (PI / 8 + PI / 8 + PI / 8), boss.gunCurrentX + 3, 8.0f);
					boss._fire2->fire(boss.x, boss.y, boss.angle - (PI / 8 + PI / 8 + PI / 8), boss.gunCurrentX - 3, 8.0f);
					boss.gFrameCount = 0;
				}
			}
		}//shake
	}//attack1
}

void BossScene::bossINupdate()
{
	RECT rcTemp;
	// 공격이 끝나면, 대포로 전환하기 위해서 내려가서 배안으로 들어가자. 
	if (bossIN && boss.life && attackPettern == 1)
	{
		boss.downDistance = getDistance(boss.down.x, boss.down.y, boss.fx, boss.fy);

		if (boss.downDistance > 10)
		{
			// 점점 다운포인트좌표로 이동. 
			if (boss.fx < boss.down.x) boss.fx += 2;
			if (boss.fx > boss.down.x) boss.fx -= 2;
			if (boss.fy < boss.down.y) boss.fy += 2;
			if (boss.fy > boss.down.y) boss.fy -= 2;
		}
		// 거리가 가까워졌으면 내려간다. 해당 시간동안. 
		if (boss.downDistance < 10)
		{
			boss.downFrameCount++;

			boss.follow = false;
			if (boss.downFrameCount < 120)
			{
				boss.bossState = bFALL;
				boss.bodyExcelY += 0.05f;
				boss.y += boss.bodyExcelY;
			}
			if (boss.downFrameCount > 120)
			{
				boss.bodyExcelY = 0;

				// 내려갔으면 x좌표가 이동. 배 입구 쪽으로. 
				if (boss.x < 610) // 배입구 x좌표 보다 작으면 
				{
					boss.x += 2.0f;
					boss.direction = bossRIGHT;
					boss.bossState = bRUN;
				}
				if (boss.x >= 610) // 배입구에 도달했으면 
				{
					boss.life = false; // 사라지게 한다.
					inoutState = CANNON_IN;
					cannonAttack = true;
					bossIN = false;
					boss.downFrameCount = 0;
					attackCount = 0;
				}
			}
		}//else if
	}// bossIN

	// 패턴 2 를위해 또 만듬....___________________________________________________________________________________bossIN2
	 // 공격이 끝나면, 대포로 전환하기 위해서 내려가서 배안으로 들어가자. 
	if (bossIN2 && boss.life && attackPettern == 2)
	{
		boss.downDistance = getDistance(boss.down.x, boss.down.y, boss.fx, boss.fy);

		if (boss.downDistance > 10)
		{
			// 점점 다운포인트좌표로 이동. 
			if (boss.fx < boss.down.x) boss.fx += 2;
			if (boss.fx > boss.down.x) boss.fx -= 2;
			if (boss.fy < boss.down.y) boss.fy += 2;
			if (boss.fy > boss.down.y) boss.fy -= 2;
		}
		// 거리가 가까워졌으면 내려간다. 해당 시간동안. 
		if (boss.downDistance < 10)
		{
			boss.downFrameCount++;

			boss.follow = false;
			if (boss.downFrameCount < 120)
			{
				boss.bossState = bFALL;
				boss.bodyExcelY += 0.05f;
				boss.y += boss.bodyExcelY;
			}
			if (boss.downFrameCount > 120)
			{
				boss.bodyExcelY = 0;

				// 내려갔으면 x좌표가 이동. 배 입구 쪽으로. 
				if (boss.x < 610) // 배입구 x좌표 보다 작으면 
				{
					boss.x += 2.0f;
					boss.bossState = bRUN;
					boss.direction = bossRIGHT;
				}
				if (boss.x >= 610) // 배입구에 도달했으면 
				{
					inoutState = CANNON_IN; 
					boss.life = false; // 사라지게 한다.
					cannonAttack = true; 
					bossIN2 = false; 
					boss.downFrameCount = 0;
					attackCount = 0;
				}
			}
		}//else if
	}// bossIN

	 // 패턴 3를위해 또 만듬....___________________________________________________________________________________bossIN3
	 // 공격이 끝나면, 대포로 전환하기 위해서 내려가서 배안으로 들어가자. 
	if (bossIN3 && boss.life && attackPettern == 3)
	{
		boss.downDistance = getDistance(boss.down.x, boss.down.y, boss.fx, boss.fy);

		if (boss.downDistance > 10)
		{
			// 점점 다운포인트좌표로 이동. 
			if (boss.fx < boss.down.x) boss.fx += 2;
			if (boss.fx > boss.down.x) boss.fx -= 2;
			if (boss.fy < boss.down.y) boss.fy += 2;
			if (boss.fy > boss.down.y) boss.fy -= 2;
		}
		// 거리가 가까워졌으면 내려간다. 해당 시간동안. 
		if (boss.downDistance < 10)
		{
			boss.downFrameCount++;

			boss.follow = false;
			if (boss.downFrameCount < 120)
			{
				boss.bossState = bFALL;
				boss.bodyExcelY += 0.05f;
				boss.y += boss.bodyExcelY;
			}
			if (boss.downFrameCount > 120)
			{
				boss.bodyExcelY = 0;

				// 내려갔으면 x좌표가 이동. 배 입구 쪽으로. 
				if (boss.x < 610) // 배입구 x좌표 보다 작으면 
				{
					boss.x += 2.0f;
					boss.bossState = bRUN;
					boss.direction = bossRIGHT;
				}
				if (boss.x >= 610) // 배입구에 도달했으면 
				{
					inoutState = CANNON_IN;
					boss.life = false; // 사라지게 한다.
					cannonAttack = true;
					bossIN3 = false; 
					//boss.downFrameCount = 0;
					attackCount = 0;
				}
			}
		}//else if
	}// bossIN

}

void BossScene::bossOUTupdate()
{
	RECT rcTemp;
	// 배 부서짐이 끝나면 플레이어가 해당위치로 가서 다시 팔로우되고, 어택.
	if (bossOUT)
	{
		boss.bossState = bRUN;
		boss.direction = bossLEFT;

		boss.life = true;
		if (boss.x > boss.down.x)
		{
			boss.x -= 3.0f;
		}
		if (boss.x <= boss.down.x)
		{
			boss.x = boss.down.x;
			boss.bossState = bIDLE;
			if (!boss.follow)
			{
				boss.bfState = BF_WAKEUP;
				boss.followerExcelY += 0.15f;
				boss.fy += boss.followerExcelY;
				if (IntersectRect(&rcTemp, &boss.bodyRC, &boss.followerRC))
				{
					inoutState = CANNON_OUT;
					boss.followerExcelY = 0;
					attackPettern = 2;
					bossFirstFollow = true;
					bossOUT = false;
					bossIN = false;
					shake = false; 
				}
			}
		}
	} // bossOUT

	  // 배 부서짐이 끝나면 플레이어가 해당위치로 가서 다시 팔로우되고, 어택.
	if (bossOUT2)
	{
		boss.bossState = bRUN;
		boss.direction = bossLEFT;

		boss.life = true;
		if (boss.x > boss.down.x)
		{
			boss.x -= 3.0f;
		}
		if (boss.x <= boss.down.x)
		{
			boss.x = boss.down.x;
			boss.bossState = bIDLE;
			if (!boss.follow)
			{
				boss.bfState = BF_WAKEUP;
				boss.followerExcelY += 0.15f;
				boss.fy += boss.followerExcelY;
				if (IntersectRect(&rcTemp, &boss.bodyRC, &boss.followerRC))
				{
					inoutState = CANNON_OUT;
					boss.followerExcelY = 0;
					attackPettern = 3;
					bossFirstFollow = true;
					bossOUT2 = false;
					bossIN = false;
					bossIN2 = false;
					shake = false;
				}
			}
		}
	} // bossOUT2
}

//몸
void BossScene::bossFrameFunc(void)
{
	// 몸 프레임. 
	// 오른쪽 중간. 
	if (!bossIN && !bossOUT && !bossOUT2 && !bossIN2 && !bossIN3 && boss.bossState != bHURT) //계속 행동이 붙잡혀있어서 이 조건을 걸어줌. 
	{
		if (-PI / 4 < boss.angle && boss.angle <= PI / 8)
		{
			boss.direction = bossRIGHT;
			boss.bossState = bMIDDLE;
		}
		// 오른쪽 위
		if (PI / 8 < boss.angle && boss.angle <= PI / 2)
		{
			boss.direction = bossRIGHT;
			boss.bossState = bTOP;
		}
		// 오른쪽 아래
		if (PI + PI / 2 < boss.angle && boss.angle <= PI + (PI / 2) + (PI / 2))
		{
			boss.direction = bossRIGHT;
			boss.bossState = bDOWN;
		}
		// 왼쪽 중간. 
		if (PI - PI / 8 < boss.angle && boss.angle <= PI + PI / 8)
		{
			boss.direction = bossLEFT;
			boss.bossState = bMIDDLE;
		}
		// 왼쪽 위. 
		if (PI / 2 < boss.angle && boss.angle <= PI - PI / 8)
		{
			boss.direction = bossLEFT;
			boss.bossState = bTOP;
		}
		// 왼쪽 아래. 
		if (PI + PI / 8 < boss.angle && boss.angle <= PI + PI / 2)
		{
			boss.direction = bossLEFT;
			boss.bossState = bDOWN;
		}
	}

	// 몸
	if (boss.shootState == bossNONESHOOT)
	{
	switch (boss.bossState)
	{
	case bTOP:
		boss.bodyFrameX = 0;
		break;

	case bMIDDLE:
		boss.bodyFrameX = 0;
		break;

	case bDOWN:
		boss.bodyFrameX = 0;
		break;

	case bIDLE:
		boss.bodyFrameX = 0;
		break;

	case bRUN:
	   // boss.bodyFrameX = 0;
		//boss.bodyFrameY = 11;
		boss.frameCount++;
		if (boss.frameCount > 5)
		{
			boss.frameCount = 0;
			boss.bodyFrameX++;
			if (boss.bodyFrameX > 5)
			{
				boss.bodyFrameX = 0;
			}
		}
		break;

	case bFALL:
		//boss.bodyFrameY = 6;
	    //boss.bodyFrameX = 0;
		boss.frameCount++;
		if (boss.frameCount > 5)
		{
			boss.frameCount = 0;
			boss.bodyFrameX++;
			if (boss.bodyFrameX > 1)
			{
				boss.bodyFrameX = 0;
			}
		}
		break;
	case bHURT:
		boss.bodyFrameY = 6;
		boss.frameCount++;
		if (boss.frameCount > 5)
		{
			boss.frameCount = 0;
			boss.bodyFrameX++;
			if (boss.bodyFrameX > 1)
			{
				boss.bodyFrameX = 0;
			}
		}
		break;
	case bNONE:
		boss.bodyFrameX = 0;
		break;
	default:
		break;
		}
	}

	if (boss.shootState == bossSHOOT)
	{
		switch (boss.bossState)
		{
	case bTOP:
		boss.frameCount++;
		if (boss.frameCount > 4)
		{
			boss.frameCount = 0;
			boss.bodyFrameX++;
			if (boss.bodyFrameX > 6)
			{
				boss.bodyFrameX = 0;
				boss.bossState = bIDLE;
			}
		}
		break;

	case bMIDDLE:
		boss.frameCount++;
		if (boss.frameCount > 4)
		{
			boss.frameCount = 0;
			boss.bodyFrameX++;
			if (boss.bodyFrameX > 6)
			{
				boss.bodyFrameX = 0;
				boss.bossState = bIDLE;
			}
		}
		break;
	case bDOWN:
		boss.frameCount++;
		if (boss.frameCount > 4)
		{
			boss.frameCount = 0;
			boss.bodyFrameX++;
			if (boss.bodyFrameX > 6)
			{
				boss.bodyFrameX = 0;
				boss.bossState = bIDLE;
			}
		}
		break;

	case bIDLE:
		boss.frameCount++;
		if (boss.frameCount > 4)
		{
			boss.frameCount = 0;
			boss.bodyFrameX++;
			if (boss.bodyFrameX > 10)
			{
				boss.bodyFrameX = 0;
			}
		}
		break;

	case bRUN:
		boss.frameCount++;
		if (boss.frameCount > 4)
		{
			boss.frameCount = 0;
			boss.bodyFrameX++;
			if (boss.bodyFrameX > 5)
			{
				boss.bodyFrameX = 0;
			}
		}
		break;

	case bFALL:
		boss.bodyFrameY = 11;
		boss.frameCount++;
		if (boss.frameCount > 4)
		{
			boss.frameCount = 0;
			boss.bodyFrameX++;
			if (boss.bodyFrameX > 1)
			{
				boss.bodyFrameX = 0;
			}
		}
		break;
	case bHURT:
		boss.bodyFrameY = 6;
		boss.frameCount++;
		if (boss.frameCount > 10)
		{
			boss.frameCount = 0;
			boss.bodyFrameX++;
			if (boss.bodyFrameX > 1)
			{
				boss.bodyFrameX = 0;
			}
		}
		break;
	case bNONE:
		boss.bodyFrameX = 0;
		break;
	default:
		break;
	}
 }
}

void BossScene::bossFollowerFunc()
{
	switch (boss.bfState)
	{
	case BF_MOVE:
		boss.FframeCount++;
		if (boss.FframeCount > 10)
		{
			boss.FframeCount = 0;
			boss.FcurrentX++;
			if (boss.FcurrentX > 5)
			{
				boss.FcurrentX = 0;
			}
		}
		break;

	case BF_SLEEP:
		boss.FframeCount++;
		if (boss.FframeCount > 10)
		{
			boss.FframeCount = 0;
			boss.FcurrentX++;
			if (boss.FcurrentX > 1)
			{
				boss.FcurrentX = 0;
			}
		}
		break;

	case BF_WAKEUP:
		boss.FframeCount++;
		if (boss.FframeCount > 10)
		{
			boss.FframeCount = 0;
			boss.FcurrentX++;
			if (boss.FcurrentX > 10)
			{
				boss.FcurrentX = 0;
			}
		}
		break;
	
	default:
		break;
	}
}

void BossScene:: cannonINPUTframeFunc()
{
	inoutFrameCount++;
	switch (inoutState)
	{
	case CANNON_IDLE:
		inoutCurrentX = 0;
		break;

	case CANNON_IN:
		if (inoutFrameCount > 10)
		{
			inoutFrameCount = 0;
			inoutCurrentX++;
			if (inoutCurrentX > 9)
			{
				inoutCurrentX = 0;
				inoutState = CANNON_IDLE;
			}
		}
		break;

	case CANNON_OUT:
		if (inoutFrameCount > 10)
		{
			inoutFrameCount = 0;
			inoutCurrentX++;
			if (inoutCurrentX > 9)
			{
				inoutCurrentX = 0;
				inoutState = CANNON_IDLE;
			}
		}
		break;

	default:
		break;
	}
}

// 어떻게 보스를 건드리면 시작하나?
void BossScene::bossStart()
{
	RECT rcTemp;
	if (!start)
	{
		bossTalkFrame++;
		if (bossTalkFrame > 120 && bossTalkFrame < 260)
		{
			talk = true; // 느낌표 톡 나옴. 
		}
		//if (bossTalkFrame >= 240 && bossTalkFrame < 360)
		//{
		//	talk = false;
		//	bossTalk = true; // 톡 내용 나옴. 
		//}
		if (bossTalkFrame >= 320) 
		{
			talk = false;
			//bossTalk = false;
			start = true;
			bossFirstFollow = true;
			bossTalkFrame = 0;
		}
	}
}


// ship _____________________________________________________________________________________________________ship
void BossScene::shipInit()
{
	//적  돌.
	IMAGEMANAGER->addFrameImage("CannonBall", "Scene/event/CannonBall.bmp", 640, 656, 16, 16, true, RGB(255, 0, 255));
	//적 총구 기다림. 
	IMAGEMANAGER->addFrameImage("canonWait", "Scene/event/canonWait.bmp", 900 * 1.4, 2400 * 1.4, 6, 16, true, RGB(255, 0, 255));
	//적 총구 슛. 
	IMAGEMANAGER->addFrameImage("canonShoot", "Scene/event/canonShoot.bmp", 900 * 1.4, 2400 * 1.4, 6, 16, true, RGB(255, 0, 255));
	
	// 위에 떨어질 돌.
	IMAGEMANAGER->addImage("piraterock1", "boss/piraterock1.bmp", 86 * 1.4, 47 * 1.4, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("piraterock2", "boss/piraterock2.bmp", 86 * 1.4, 47 * 1.4, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("piraterock3", "boss/piraterock3.bmp", 154 * 1.4, 184 * 1.4, true, RGB(255, 0, 255));

	_cannon._bullet = new bulletM1;
	_cannon._bullet->init(10, 1000);

	//대포 셋업
	_cannon.angle = PI;

	_cannon.cannon = 365; //선 길이. 
	_cannon.radius = 50;

	// 이 enemy의 중심좌표. 
	_cannon.center.x = 785; //중심점.  
	_cannon.center.y = 420;

	// 쏠 총구의 좌표.
	_cannon.shootAngle = -1;

	_cannon.fire = false;

	_cannon.frameCount = 0; // 총알 날리기전 텀 프레임. 
	_cannon.currentX = 0;
	_cannon.frameCount2 = 0; // 총알 연속 날리기전 텀 프레임. 
	_cannon.currentX2 = 0;
	_cannon.frameCount3 = 0; // 총알 연속 날리기전 텀 프레임. 
	_cannon.currentX3 = 0;

	cannonFrameNum = 8;
	
	gun1State = gun1IDLE;
	gunFrameCount = 0;
	gunCurrentX = 0;
	gunCurrentY = 9;

	for (int i = 0; i < 3; i++)
	{
		shipRock[i].life = true;
		shipRock[i].x = 565;
		shipRock[i].y = 25;
		shipRock[i].shipOffsetY = 0;
		shipRock[i].fall = false;
	}
}

void BossScene::shipUpdate()
{
	_cannon._bullet->update();

	if (cannonAttack)
	{
		// 불빛 돌고,각도 잡아 쏨.
		shipShoot();

		// 총구 가로 애니메이션. 
		gunFrameFunc();
	}
}

void BossScene::shipRender()
{
	//HPEN MyPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	//SelectObject(getMemDC(), MyPen);
	//
	//// 직접쏘는 총구 각도위치 알려준다.
	//LineMake(getMemDC(), _cannon.center.x, _cannon.center.y,
	//	_cannon.cannonEnd.x, _cannon.cannonEnd.y);
	//EllipseMakeCenter(getMemDC(), _cannon.center.x, _cannon.center.y,
	//	_cannon.radius, _cannon.radius);
	

	// 총구 애니메이션
	if (gun1State == gun1IDLE)
	{
		IMAGEMANAGER->findImage("canonWait")->frameRender(getMemDC(), _cannon.center.x - 100,
			_cannon.center.y - 100, gunCurrentX, gunCurrentY);
	}
	if (gun1State == gun1WAIT)
	{
		IMAGEMANAGER->findImage("canonWait")->frameRender(getMemDC(), _cannon.center.x - 100,
			_cannon.center.y - 100, gunCurrentX, gunCurrentY);
	}
	if (gun1State == gun1SHOOT)
	{
		IMAGEMANAGER->findImage("canonShoot")->frameRender(getMemDC(), _cannon.center.x - 100,
			_cannon.center.y - 100, gunCurrentX, gunCurrentY);
	}
	
	//DeleteObject(MyPen);

	_cannon._bullet->render();
}


void BossScene::shipShoot()
{
	// 끝점.  각도 * 길이 * 중심점. 
	_cannon.cannonEnd.x = cosf(_cannon.shootAngle) * _cannon.cannon + _cannon.center.x;
	_cannon.cannonEnd.y = -sinf(_cannon.shootAngle) * _cannon.cannon + _cannon.center.y;

	_cannon.shootAngle = getAngle(_cannon.center.x, _cannon.center.y, _player->_player.x, _player->_player.y);

	gunCurrentY = int(_cannon.shootAngle / PI_8); // 16방향

	if (_cannon.angle >= PI * 2) _cannon.angle -= PI * 2;

	_cannon.fire = true;
	if (_cannon.fire)
	{
		if (_cannon.frameCount < 120)
		{
			gun1State = gun1WAIT;
		}
		else
		{
			gun1State = gun1SHOOT;
		}
		_cannon.frameCount++;
		_cannon.frameCount2++;
		_cannon.frameCount3++;

			cannonFrameNum = int(_cannon.shootAngle / PI_8); // 총구 프레임. 
			if (_cannon.angle >= PI * 2) _cannon.angle -= PI * 2;
			if (_cannon.shootAngle >= PI * 2) _cannon.shootAngle -= PI * 2;

		if (attackPettern == 1 && attackPettern != 2)
		{
			if (_cannon.frameCount >= 180)
			{
				gun1State = gun1SHOOT;
				_cannon._bullet->fire(_cannon.center.x, _cannon.center.y, _cannon.shootAngle, 10.0f);
				_player->se4 = true;

				_cannon.frameCount = 0;
			}
		}

		if (attackPettern == 2 && attackPettern != 1)
		{
			if (_cannon.frameCount2 >= 160)
			{
				gun1State = gun1SHOOT;
				_cannon._bullet->fire(_cannon.center.x, _cannon.center.y, _cannon.shootAngle, 10.0f);
				_player->se4 = true;

				_cannon.frameCount2 = 0;
			}
			if (_cannon.frameCount >= 180)
			{
				gun1State = gun1SHOOT;
				_cannon._bullet->fire(_cannon.center.x, _cannon.center.y, _cannon.shootAngle, 10.0f);
				_player->se4 = true;

				_cannon.frameCount = 0;
			}
		}
		if (attackPettern == 3)
		{
			if (_cannon.frameCount3 >= 140)
			{
				gun1State = gun1SHOOT;
				_cannon._bullet->fire(_cannon.center.x, _cannon.center.y, _cannon.shootAngle, 10.0f);
				_player->se4 = true;

				_cannon.frameCount3 = 0;
			}
			if (_cannon.frameCount2 >= 160)
			{
				gun1State = gun1SHOOT;
				_cannon._bullet->fire(_cannon.center.x, _cannon.center.y, _cannon.shootAngle, 10.0f);
				_player->se4 = true;

				_cannon.frameCount2 = 0;
			}
			if (_cannon.frameCount >= 180)
			{
				gun1State = gun1SHOOT;
				_cannon._bullet->fire(_cannon.center.x, _cannon.center.y, _cannon.shootAngle, 10.0f);
				_player->se4 = true;

				_cannon.frameCount = 0;
			}
		}
		_cannon.fire = false;
	} // cannon.fire
}


void BossScene::gunFrameFunc(void)
{
	switch (gun1State)
	{
	case gun1IDLE:
		gunCurrentX = 0;
		break;

	case gun1WAIT:
		gunFrameCount++;
		if (gunFrameCount > 5)
		{
			gunFrameCount = 0;
			gunCurrentX++;
			if (gunCurrentX > 5)
			{
				gunCurrentX = 0;
			}
		}
		break;

	case gun1SHOOT:
		gunFrameCount++;
		if (gunFrameCount > 15)
		{
			gunFrameCount = 0;
			gunCurrentX++;
			if (gunCurrentX > 5)
			{
				gunCurrentX = 0;
				gun1State = gun1IDLE;
			}
		}
		break;
	default:
		break;
	}
}


//   미사일 , 충돌 , 픽셀 관련.. 
void BossScene::removeMissile(int arrNum)
{
	if (boss._fire2)
	{
		boss._fire2->removeMissile(arrNum);
	}
}
void BossScene::removeMissile1(int arrNum)
{
	if (_cannon._bullet)
	{
		_cannon._bullet->removeBullet(arrNum);
	}
}


void BossScene::bossCollision()
{
	RECT tempRC;
   // 플레이어 렉트랑 보스총알 
   // 보스 총알과 플레이어가 충돌 _____________________________________________________________________________________
	for (int j = 0; j < boss._fire2->getVBullet().size(); j++)
	{
		if (IntersectRect(&tempRC, &_player->_player.rc, &boss._fire2->getVBullet()[j].rc))
		{
			hurt = true;
			boss._fire2->removeMissile(j);
			break;
		}
	}
	
	// 대포와 플레이어가 충돌
	for (int j = 0; j < _cannon._bullet->getVBullet().size(); j++)
	{
		if (IntersectRect(&tempRC, &_player->_player.rc, &_cannon._bullet->getVBullet()[j].rc))
		{
			hurt = true;
			_cannon._bullet->removeBullet(j);
			break;
		}
	}

   //플레이어 총알 보스 몸통
	for (int j = 0; j < _player->_player._fire->getVBullet().size(); j++)
	{
		if (IntersectRect(&tempRC, &_player->_player._fire->getVBullet()[j].rc, &boss.bodyRC))
		{
			boss.hitCheck = true;
			boss.bossState = bHURT;
			_player->removeMissile(j);
		}
	}


	if (hurt)
	{
		hitCount++;
		if (hitCount % 20 == 0) // 피가 한번만 닳게끔. 
		{
			_player->_player.state = HURT;
			_player->geddy.cast = true;
			_player->geddy.follow = false;
			_player->_player.x -= 35;
			DATABASE->getElementData("player")->currentHP -= 2;
			hurt = false;
			hitCount = 0;
		}
	}
}

bool BossScene::blueCollision(void)
{
	//player.x 왼쪽+5 가 파랑색이면, 
	if (GetPixel(getPixelBlue(), boss.x - 25, boss.y) == RGB(0, 0, 255))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool BossScene::pinkCollision(void)
{
	//player.x 밑+5 가 분홍색 이면, 
	if (GetPixel(getPixel(), boss.x, boss.y + 25 ) == RGB(255, 0, 255))
	{
		return true;
	}
	else
	{
		return false;
	}
}



// 폭포, 물 프레임하고 렉트
void BossScene::bgFrameFunc()
{
	// 루프 하늘 
	layer1_offsetX += 0.3f;
	layer2_offsetX -= 0.35f;

	//rc
	ship.rc = RectMake(ship.x, ship.y, 640, 283);
	for (int i = 0; i < 3; i++)
	{
		waterFall[i].rc = RectMake(waterFall[i].x, waterFall[i].y, 10, 10);
	}

	// frame ///////////
	// 배
	ship.frameCount++;
	if (ship.frameCount > 15)
	{
		ship.frameCount = 0;
		ship.currentX++;
		if (ship.currentX > 1)
		{
			ship.currentX = 0;
		}
	}

	//폭포 
	for (int i = 0; i < 3; i++)
	{
		waterFall[i].frameCount++;
		if (waterFall[i].frameCount > 10)
		{
			waterFall[i].frameCount = 0;
			waterFall[i].currentX++;
			if (i == 2)
			{
				if (waterFall[i].currentX > 2)
				{
					waterFall[i].currentX = 0;
				}
			}
			else
			{
				if (waterFall[i].currentX > 1)
				{
					waterFall[i].currentX = 0;
				}
			}
		}
	}
}




//shipEffectFunc__________________________________________________________________________________________shipEffectFunc
void BossScene::shipEffectInit()
{
	IMAGEMANAGER->addFrameImage("shipAttackedEffect", "effect/shipAttackedEffect.bmp", 4310*1.8, 283 * 1.8, 10, 1, true, RGB(255, 0, 255));

	IMAGEMANAGER->addFrameImage("explosionEffect1", "effect/explosionEffect1.bmp", 1872, 128,18, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("explosionEffect2", "effect/explosionEffect2.bmp", 1615,85, 19, 1, true, RGB(255, 0, 255));

	for (int i = 0; i < 5; i++)
	{
		fx[i].life = false;
		fx[i].frameCount = 0;
		fx[i].currentX = 0;
	}

	fx[0].x = 300;
	fx[0].y = 190;

	fx[1].x = 390;
	fx[1].y = 320;
	fx[3].x = 780;
	fx[3].y = 450;

	fx[2].x = 710;
	fx[2].y = 150;
	fx[4].x = 480;
	fx[4].y = 500;
}

void BossScene::shipEffectUpdate()
{
	for (int i = 0; i < 5; i++)
	{
		if (!fx[i].life) continue;
		fx[i].rc = RectMake(fx[i].x, fx[i].y, 50,50);
	}

	for (int i = 0; i < 5; i++)
	{
		fx[i].frameCount++;
		if (fx[i].frameCount > 6)
		{
			fx[i].frameCount = 0;
			if (i == 0)
			{
				fx[i].currentX++;
				if (fx[i].currentX > 9)
				{
					fx[i].currentX = 0;
				}
			}
			if (i == 1 )
			{
				fx[i].currentX++;
				if (fx[i].currentX > 17)
				{
					fx[i].currentX = 0;
				}
			}
			if (i == 3)
			{
				fx[i].currentX++;
				if (fx[i].currentX > 17)
				{
					fx[i].currentX = 0;
				}
			}
			if (i == 2)
			{
				fx[i].currentX++;
				if (fx[i].currentX > 18)
				{
					fx[i].currentX = 0;
				}
			}
			if (i == 4)
			{
				fx[i].currentX++;
				if (fx[i].currentX > 18)
				{
					fx[i].currentX = 0;
				}
			}
		}
	}
}

void BossScene::shipEffectRender()
{
	if (fx[0].life)
	{
		IMAGEMANAGER->findImage("shipAttackedEffect")->frameRender(getMemDC(), fx[0].rc.left, fx[0].rc.top, fx[0].currentX,0);
	}
	if (fx[1].life)
	{
		IMAGEMANAGER->findImage("explosionEffect1")->frameRender(getMemDC(), fx[1].rc.left, fx[1].rc.top, fx[1].currentX, 0);
	}
	if (fx[3].life)
	{
		IMAGEMANAGER->findImage("explosionEffect1")->frameRender(getMemDC(), fx[3].rc.left, fx[3].rc.top, fx[3].currentX, 0);
	}
	if (fx[2].life)
	{
		IMAGEMANAGER->findImage("explosionEffect2")->frameRender(getMemDC(), fx[2].rc.left, fx[2].rc.top, fx[2].currentX, 0);
	}
	if (fx[4].life)
	{
		IMAGEMANAGER->findImage("explosionEffect2")->frameRender(getMemDC(), fx[4].rc.left, fx[4].rc.top, fx[4].currentX, 0);
	}
}




// effect *&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& effect
void BossScene::fx3Init(void)
{
	IMAGEMANAGER->addFrameImage("bossEffect", "boss/bossEffect.bmp", 2000,500, 4, 1, true, RGB(255, 0, 255));

	fx3.frameCount = 0;
	fx3.currentX = 0;
	fx3.fire = false;
}

void BossScene::fx3Update(void)
{
	if (fx3.fire)
	{
		fx3.frameCount++;
		if (fx3.frameCount > 6)
		{
			fx3.frameCount = 0;
			fx3.currentX++;
			if (fx3.currentX > 3)
			{
				fx3.currentX = 0;
				fx3.fire = false;
			}
		}
	}

}

void BossScene::fx3Render(void)
{
	if (fx3.fire)
	{
		IMAGEMANAGER->findImage("bossEffect")->frameRender(getMemDC(), boss.x - 265, boss.y - 260, fx3.currentX, 0);
	}
}
