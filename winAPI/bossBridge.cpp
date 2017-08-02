#include "stdafx.h"
#include "bossBridge.h"


bossBridge::bossBridge()
{
}


bossBridge::~bossBridge()
{
}

HRESULT bossBridge::init(void)
{
	IMAGEMANAGER->addImage("bglayer", "boss/bossBG_noCamera_enter.bmp", 1280,720, true, RGB(255, 0, 255));

	//sky
	IMAGEMANAGER->addImage("skyLayer1", "boss/skyLayer1_1280.bmp", 1280, 720, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("skyLayer2", "boss/skyLayer2_1280.bmp", 1280, 720, true, RGB(255, 0, 255));

	// pixelPink
	IMAGEMANAGER->addImage("bridgeBG_pixelPink", "boss/bridgeBG_pixelPink.bmp", 1280, 720, false);
	IMAGEMANAGER->addImage("bossBridge_blue", "boss/bossBridge_blue.bmp", 1280, 720, false);
	IMAGEMANAGER->addImage("bossBridge_green", "boss/bossBridge_green.bmp", 1280, 720, false);
	IMAGEMANAGER->addImage("bossBridge_yellow", "boss/bossBridge_yellow.bmp", 1280, 720, false);
	
	IMAGEMANAGER->addImage("glass", "boss/glass.bmp", 1280, 720, true,RGB(255,0,255));
	
	layer1_offsetX = 0;
	layer2_offsetX = 0;

	_player = new player;
	_player->init(780,650);
	_player->playNum = 5;

	ringInit();
	rockInit();

	toBoss.x = 0;
	toBoss.y = 100;

	_im = new itemManager;
	_im->init();
	_im->setPlayer(_player);

	itemShowFrame = 0;

	return S_OK;
}

void   bossBridge::release(void)
{
}

void   bossBridge::update(void)
{
	_player->update();
	ringUpdate();
	rockUpdate();
	_im->update();
	Create();

	layer1_offsetX += 0.3f;
	layer2_offsetX -= 0.35f;

	toBoss.rc = RectMake(toBoss.x, toBoss.y,20,200);

	RECT tempRC;
	if (IntersectRect(&tempRC,&toBoss.rc,&_player->_player.rc))
	{
		SCENEMANAGER->changeScene("BossScene");
	}
}

void   bossBridge::render(void)
{
	// �ڿ� �ȼ����� �ȵǰԲ�.
	IMAGEMANAGER->findImage("mapImage")->render(getMemDC()); // ����
	IMAGEMANAGER->findImage("mapImage")->render(getPixel()); // ��ũ
	IMAGEMANAGER->findImage("mapImage")->render(getPixelBlue()); // ���
	IMAGEMANAGER->findImage("mapImage")->render(getPixelGreen()); // �׸�
	IMAGEMANAGER->findImage("mapImage")->render(getPixelYellow()); // ���ο�

	IMAGEMANAGER->findImage("bridgeBG_pixelPink")->render(getPixel());
	IMAGEMANAGER->findImage("bossBridge_blue")->render(getPixelBlue());
	IMAGEMANAGER->findImage("bossBridge_green")->render(getPixelGreen());
	IMAGEMANAGER->findImage("bossBridge_yellow")->render(getPixelYellow());

	RECT loopRC = RectMake(0,0,1280,720);
	IMAGEMANAGER->findImage("skyLayer2")->loopRender(getMemDC(),&loopRC, layer2_offsetX,0);
	IMAGEMANAGER->findImage("skyLayer1")->loopRender(getMemDC(), &loopRC, layer1_offsetX, 0);

	IMAGEMANAGER->findImage("bglayer")->render(getMemDC());

	//Rectangle(getMemDC(), toBoss.rc.left, toBoss.rc.top, toBoss.rc.right, toBoss.rc.bottom);

	rockRender();
	ringRenderBehind();
	_player->render();
	ringRender();
	_im->render();

	IMAGEMANAGER->findImage("glass")->render(getMemDC());
}



// �� ___________________________________________________________________________________________________________�� ����
void bossBridge::ringInit()
{
	IMAGEMANAGER->addFrameImage("ringHeight", "item/ringHeight.bmp", 840 * 1.3, 280 * 1.3, 6, 2, true, RGB(255, 0, 255));
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

		ring[i].ringState = RING_IDLE;
		ring[i].rc = { 0,0,10,10 }; // �ǹ̾���.����� �Ҷ� ������ ����. 
	}

	ring[0].x = 100;
	ring[0].y = 100;
	ring[1].x = 200;
	ring[1].y = 100;
	ring[2].x = 300;
	ring[2].y = 100;
	ring[3].x = 1000;
	ring[3].y = 100;
}

void bossBridge::ringUpdate()
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

	// �÷��̾�� �ε�ġ��, 
	for (int i = 0; i < RINGMAX; i++)
	{
		// �ѹ��� ����.
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
			if (ring[i].eatCount % 30 == 0) // �ǰ� �ѹ��� ��Բ�. 
			{
				DATABASE->getElementData("player")->coin += 10;
				ring[i].eat = false;
				ring[i].eatCount = 0;
			}
		}
	}

	// �÷��̾�� �ε�ġ��, 
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

void bossBridge::ringRender()
{
	for (int i = 0; i < RINGMAX; i++)
	{
		if (ring[i].life)
		{
			//Rectangle(getMemDC(), ring[i].rc.left, ring[i].rc.top, ring[i].rc.right, ring[i].rc.bottom);

			IMAGEMANAGER->findImage("ringHeight")->frameRender(getMemDC(), ring[i].rc.left - 68, ring[i].rc.top - 25,
				ring[i].currentX, ring[i].ringState);
		}
	}
}

// �÷��̾� �ڿ� ���� �� ��,  
void bossBridge::ringRenderBehind()
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

void bossBridge::ringFrameFunc()
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


//rock _______________________________________________________________________________________________rock
void bossBridge::rockInit()
{
	IMAGEMANAGER->addImage("checkRock", "boss/checkRock.bmp", 147, 470, true, RGB(255, 0, 255));

	rock.lock = true;
	rock.checkRCshow = true;
	rock.frameCount = 0;

	rock.x = 760;
	rock.y = 720;
	rock.w = 147;
	rock.h = 470;

	rock.cx = 760;
	rock.cy = 240;
	rock.cw = 147;
	rock.ch = 20;
}

void bossBridge::rockUpdate()
{
	rock.rc = RectMake(rock.x, rock.y, rock.w, rock.h);
	
	if (rock.checkRCshow)
	{
		rock.checkRC = RectMake(rock.cx, rock.cy, rock.cw, rock.ch);
		RECT rcTemp;
		if (IntersectRect(&rcTemp, &_player->_player.rc, &rock.checkRC))
		{
			rock.lock = false;
		}
	}

	if (!rock.lock)
	{
		rock.y -= 1.2f;
		rock.frameCount++;
		if (rock.frameCount > 380)
		{
			rock.lock = true;
			rock.checkRCshow = false;
		}
	}

	// �ö�� ��Ʈ ���� �÷��̾� �������� �ְ�. 
	if (_player->_player.rc.left > rock.rc.left && _player->_player.rc.right < rock.rc.right
		&& _player->_player.rc.bottom > rock.rc.top)
	{
		_player->_player.rc.bottom = rock.rc.top;
		_player->_player.rc.top = rock.rc.top - 50;
		if (_player->_player.y > 355) { _player->_player.y = 360; }
	}
}

void bossBridge::rockRender()
{
	if (rock.checkRCshow)
	{
		//Rectangle(getMemDC(), rock.checkRC.left, rock.checkRC.top, rock.checkRC.right, rock.checkRC.bottom);
	}
	IMAGEMANAGER->findImage("checkRock")->render(getMemDC(), rock.rc.left, rock.rc.top);
}


// init()�� ���ָ� �ѹ��� ��ġ�ǰ� ī�޶� �۵��ؼ� 
// �÷��̾ ������� ���� ������ ��ġ �����Ǿ�����
void bossBridge::Create()
{
	itemShowFrame++;
	if (itemShowFrame == 1)
	{
		//���� �ɱ�.
		_im->setFruit1(230, 254);
		_im->setFruit0(280,254);
		_im->setFruit1(340,254);
		_im->setFruit2(390,254);
		_im->setFruit2(430, 254);
		_im->setFruit0(490, 254);
		_im->setFruit0(515, 254);
		_im->setFruit1(540, 254);
	}
}


// ����Ǵµ� 
void bossBridge::Delete(void)
{
	for (int i = 0; i < _im->getItem().size(); ++i)
	{
		//if (_im->getItem()[i]->tem.x <300) _im->removeItem(i);
	}
}