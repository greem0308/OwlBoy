#pragma once

#include "singletonbase.h"
#include <map>

class image;
class animation;

class keyAnimationManager : public singletonBase <keyAnimationManager>
{
private:
	typedef map<string, animation*> arrAnimation;
	typedef map<string, animation*>::iterator arrAnimationIter;

private:
	arrAnimation _mTotalAnimations;

public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	void addDefPlayFrameAnimation(string aniKeyName, 
		char* imgKeyName, int fps, bool reverse, bool loop);

	void addArrayFrameAnimation(string aniKeyName, 
		char* imgKeyName, int* arr, int arrLen, int fps, bool loop);

	void addCoordinateFrameAnimation(string aniKeyName, 
		char* imgKeyName, int start, int end, int fps, bool reverse, bool loop);

	void start(string aniKeyName);
	void stop(string aniKeyName);
	void pause(string aniKeyName);
	void resume(string aniKeyName);
	bool isPlay(string aniKeyName);

	animation* findAnimation(string aniKeyName);


	void deleteAll(void);

	keyAnimationManager(void);
	virtual ~keyAnimationManager(void);
};

