#pragma once

#include "gamenode.h"
#include "button.h"

enum BUTTONKIND
{
	BUTTONKIND_SELECT,
	BUTTONKIND_PLAY,
	BUTTONKIND_STOP,
	BUTTONKIND_PAUSE,
	BUTTONKIND_END
};

class mp3Player : public gameNode
{
private:
	//image* _bgTemp;
	//
	//button* _button[BUTTONKIND_END];
	//button* _currentButton;

	string _musicTitle;
	int _soundIndex;

	vector<string> _vTitle;

public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void play(void);
	virtual void stop(void);
	virtual void pause(void);

	/*
	static void cbSelect(void* obj);
	static void cbPlay(void* obj);
	static void cbStop(void* obj);
	static void cbPause(void* obj);
    */

	void setSoundIndex(int index);
	int getSoundIndex(void) {return _soundIndex;}

	//void setCurrentButton(button* btn) {_currentButton = btn;}
	//button* getCurrentButton(BUTTONKIND bk) {return _button[bk];}
	//
	void setMusicTitle(string title) {_musicTitle = title;}
	string getMusicTitle(int index) {return _vTitle[index];}

	mp3Player(void);
	virtual ~mp3Player(void);
};

