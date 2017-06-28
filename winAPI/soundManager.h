#pragma once

#include "singletonbase.h"

#include <map>
#include <string>

//fmod.hpp �߰����ش�
#include "inc/fmod.hpp"

//lib ��ũ
#pragma comment (lib, "lib/fmodex_vc.lib")

//ä�� ���� (����~!!!)
#define EXTRACHANNLEBUFFER 5
#define SOUNDBUFFER 10

//�� ���� ����
#define TOTALSOUNDBUFFER (EXTRACHANNLEBUFFER + SOUNDBUFFER)

using namespace FMOD;

class soundManager : public singletonBase <soundManager>
{
private:
	typedef map<string, Sound**> arrSounds;
	typedef map<string, Sound**>::iterator arrSoundsIter;
	typedef map<string, Channel**> arrChannels;
	typedef map<string, Channel**>::iterator arrChannelsIter;

private:
	System* _system;
	Sound** _sound;
	Channel** _channel;

	arrSounds _mTotalSounds;

public:
	HRESULT init(void);
	void release(void);

	void addSound(string keyName, string soundName, bool background, bool loop);
	void play(string keyName);
	void play(string keyName, float volume);
	void stop(string keyName);
	void pause(string keyName);
	void resume(string keyName);

	bool isPause(string keyName);
	bool isPlay(string keyName);

	//���� ���̰� ������.
	unsigned int getLength(string keyName);

	//���� ��� ��ġ ����
	void setPosition(string keyName, unsigned int ms);
	unsigned int getPosition(string keyName);

	//fmod �ý��� �����Ѵ�
	void update(void);

	soundManager(void);
	virtual ~soundManager(void);
};

