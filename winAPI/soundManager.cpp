#include "StdAfx.h"
#include "soundManager.h"

soundManager::soundManager(void)
{
}

soundManager::~soundManager(void)
{
}

HRESULT soundManager::init(void)
{
	//사운드 시스템 생성
	System_Create(&_system);

	//채널수 설정
	_system->init(TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, 0);

	//사운드 채널 생성
	_sound = new Sound*[TOTALSOUNDBUFFER];
	_channel = new Channel*[TOTALSOUNDBUFFER];

	//초기화
	ZeroMemory(_sound, sizeof(Sound*) * TOTALSOUNDBUFFER);
	ZeroMemory(_channel, sizeof(Channel*) * TOTALSOUNDBUFFER);

	return S_OK;
}

void soundManager::release(void)
{
	if (_channel != NULL || _sound != NULL)
	{
		for (int i = 0; i < TOTALSOUNDBUFFER; i++)
		{
			if (_channel != NULL)
			{
				if (_channel[i]) _channel[i]->stop();
			}
			if (_sound != NULL)
			{
				if (_sound[i]) _sound[i]->release();
			}
		}
	}

	//메모리 지우고..
	SAFE_DELETE_ARRAY(_channel);
	SAFE_DELETE_ARRAY(_sound);

	//시스템 닫는다
	if (_system != NULL)
	{
		_system->release();
		_system->close();
	}

	_mTotalSounds.clear();
}

void soundManager::addSound(string keyName,
	string soundName, bool background, bool loop)
{
	//반복이냐?
	if (loop)
	{
		//배경음임?
		if (background)
		{
			_system->createStream(soundName.c_str(),
				FMOD_LOOP_NORMAL, 0, &_sound[_mTotalSounds.size()]);
		}
		else
		{
			_system->createSound(soundName.c_str(),
				FMOD_LOOP_NORMAL, 0, &_sound[_mTotalSounds.size()]);
		}
	}
	else
	{
		//배경음임?
		if (background)
		{
			_system->createStream(soundName.c_str(),
				FMOD_DEFAULT, 0, &_sound[_mTotalSounds.size()]);
		}
		else
		{
			_system->createSound(soundName.c_str(),
				FMOD_DEFAULT, 0, &_sound[_mTotalSounds.size()]);
		}
	}

	//맵에 사운드를 키값과 함께 넣어준다
	_mTotalSounds.insert(make_pair(keyName, &_sound[_mTotalSounds.size()]));
}

void soundManager::play(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//사운드 플레이~~~
			_system->playSound(FMOD_CHANNEL_FREE,
				_sound[count], false, &_channel[count]);
		}
	}
}

void soundManager::play(string keyName, float volume)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//사운드 플레이~~~
			_system->playSound(FMOD_CHANNEL_FREE,
				_sound[count], false, &_channel[count]);
			_channel[count]->setVolume(volume);
		}
	}
}

// 중지. 
void soundManager::stop(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->stop();
		}
	}
}

// 가서 건드려서 중지시킨다. 
void soundManager::pause(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setPaused(true);
		}
	}
}

// 가서 중지를 다시 펄스로 바꾼다.
void soundManager::resume(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setPaused(false);
		}
	}
}

//정지 되어있는지 확인한다.
bool soundManager::isPause(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	bool temp;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->getPaused(&temp);
		}
	}
	return temp;
}

//플레이가 되고 있는지 확인
bool soundManager::isPlay(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	bool temp;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->isPlaying(&temp);
		}
	}
	return temp;
}

float soundManager::getVolume(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	float temp = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->getVolume(&temp);
		}
	}
	return temp;
}

void soundManager::setVolume(string keyName, float Volume)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setVolume(getVolume(keyName) + Volume);
		}
	}
}

unsigned int soundManager::getTotalLength(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	unsigned int temp = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_sound[count]->getLength(&temp, FMOD_TIMEUNIT_MS);
		}
	}
	return temp;
}

unsigned int soundManager::getLength(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	unsigned int temp = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->getPosition(&temp, FMOD_TIMEUNIT_MS);
		}
	}
	return temp;
}


void soundManager::setLength(string keyName, unsigned int Length)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setPosition(Length, FMOD_TIMEUNIT_MS);
		}
	}
}



//unsigned int soundManager::

//fmod 시스템 갱신한다
void soundManager::update(void)
{
	_system->update();
}

