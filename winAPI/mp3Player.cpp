#include "StdAfx.h"
#include "mp3Player.h"


mp3Player::mp3Player(void)
	:_soundIndex(0)
{
}


mp3Player::~mp3Player(void)
{
}

HRESULT mp3Player::init(void)
{
	gameNode::init();

	_soundIndex = 0;

	//IMAGEMANAGER->addFrameImage("btnSelect", "btnSelect.bmp", 500, 100, 1, 2);
	//IMAGEMANAGER->addFrameImage("btnPlay", "btnPlay.bmp", 90, 45, 2, 1, true);
	//IMAGEMANAGER->addFrameImage("btnStop", "btnStop.bmp", 90, 45, 2, 1, true);
	//IMAGEMANAGER->addFrameImage("btnPause", "btnPause.bmp", 90, 45, 2, 1, true);

	//char strKey[128];
	//char strName[128];
	char strSoundKey[128];
	char strSoundName[128];

	for (int i = 0; i < 5; i++)
	{
		//ZeroMemory(strKey, sizeof(strKey)); // 이미지
		//ZeroMemory(strName, sizeof(strName)); // 이미지
		ZeroMemory(strSoundKey, sizeof(strSoundKey));
		ZeroMemory(strSoundName, sizeof(strSoundName));

		//sprintf(strKey, "background_%d", i); // 이미지
		//sprintf(strName, "background_%d.bmp", i); // 이미지
 		sprintf(strSoundKey, "sound_%d", i);
		sprintf(strSoundName, "sound_%d.mp3", i); // 파일 이름이 sound_%d 으로 되어 있음. 

		//IMAGEMANAGER->addImage(strKey, strName, 500, 500);
		SOUNDMANAGER->addSound(strSoundKey, strSoundName, false, false); // 음악이 5개라서 반복문으로 넣었
		_vTitle.push_back(strSoundName);
	}

	//_bgTemp = IMAGEMANAGER->findImage("background_0");

	//_button[BUTTONKIND_SELECT] = new button;
	//_button[BUTTONKIND_SELECT]->init("btnSelect", WINSIZEX / 2, 525,
	//	PointMake(0, 1), PointMake(0, 0), cbSelect, this);

	//_button[BUTTONKIND_PLAY] = new button;
	//_button[BUTTONKIND_PLAY]->init("btnPlay", WINSIZEX / 2 - 25, 575,
	//	PointMake(1, 0), PointMake(0, 0), cbPlay, this);

	//_button[BUTTONKIND_STOP] = new button;
	//_button[BUTTONKIND_STOP]->init("btnStop", WINSIZEX / 2 + 25, 575,
	//	PointMake(1, 0), PointMake(0, 0), cbStop, this);

	//_button[BUTTONKIND_PAUSE] = new button;
	//_button[BUTTONKIND_PAUSE]->init("btnPause", WINSIZEX / 2 - 25, 575,
	//	PointMake(1, 0), PointMake(0, 0), cbPause, this);

	return S_OK;
}

void mp3Player::release(void)
{
	gameNode::release();

}

void mp3Player::update(void)
{
	gameNode::update();

	//for (int i = BUTTONKIND_SELECT; i < BUTTONKIND_END; i++)
	//{
	//	if (i == BUTTONKIND_SELECT || i == BUTTONKIND_STOP) 
	//	{
	//		_button[i]->update();
	//	}
	//}

	//_currentButton->update();
}

void mp3Player::render(void)
{
	//_bgTemp->render(getMemDC());
	//
	//for (int i = BUTTONKIND_SELECT; i < BUTTONKIND_END; i++)
	//{
	//	if (i == BUTTONKIND_SELECT || i == BUTTONKIND_STOP) 
	//	{
	//		_button[i]->render();
	//	}
	//}
	//
	//_currentButton->render();
}

void mp3Player::play(void)
{
	char strSoundKey[128];
	sprintf(strSoundKey, "sound_%d", _soundIndex);
	SOUNDMANAGER->play(strSoundKey);
}

void mp3Player::stop(void)
{
	char strSoundKey[128];
	sprintf(strSoundKey, "sound_%d", _soundIndex);
	SOUNDMANAGER->stop(strSoundKey);
	//this->setCurrentButton(this->getCurrentButton(BUTTONKIND_PLAY));
}

void mp3Player::pause(void)
{
	char strSoundKey[128];
	sprintf(strSoundKey, "sound_%d", _soundIndex);
	if (!SOUNDMANAGER->isPause(strSoundKey))
	{
		SOUNDMANAGER->pause(strSoundKey);
		//this->setCurrentButton(this->getCurrentButton(BUTTONKIND_PLAY));
	}
}

void mp3Player::setSoundIndex(int index)
{
//	char str[128];

	if (index > 4) _soundIndex = 0;
	else _soundIndex = index;

	//sprintf(str, "background_%d", _soundIndex);
	//_bgTemp = IMAGEMANAGER->findImage(str);
}
//
//void mp3Player::cbSelect(void* obj)
//{
//	mp3Player* mp = (mp3Player*)obj;
//
//	char strSoundKey[128];
//	sprintf(strSoundKey, "sound_%d", mp->getSoundIndex());
//
//	if (SOUNDMANAGER->isPlay(strSoundKey))
//	{
//		mp->stop();
//		mp->setSoundIndex(mp->getSoundIndex() + 1);
//		mp->play();
//	}
//}
//
//void mp3Player::cbPlay(void* obj)
//{
//	mp3Player* mp = (mp3Player*)obj;
//	
//	char strSoundKey[128];
//	sprintf(strSoundKey, "sound_%d", mp->getSoundIndex());
//
//	if (!SOUNDMANAGER->isPause(strSoundKey))
//	{
//		mp->play();
//		//mp->setCurrentButton(mp->getCurrentButton(BUTTONKIND_PAUSE));
//	}
//	else 
//	{
//		SOUNDMANAGER->resume(strSoundKey);
//		//mp->setCurrentButton(mp->getCurrentButton(BUTTONKIND_PAUSE));
//	}
//}
//
//void mp3Player::cbStop(void* obj)
//{
//	mp3Player* mp = (mp3Player*)obj;
//	mp->stop();
//}
//
//void mp3Player::cbPause(void* obj)
//{
//	mp3Player* mp = (mp3Player*)obj;
//	mp->pause();
//}
