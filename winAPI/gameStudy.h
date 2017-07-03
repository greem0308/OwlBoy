#pragma once

#include "gamenode.h"
#include "test.h"
#include "startScene.h"
#include "otusHouseScene.h"
#include "VellieScene.h"

class gameStudy : public gameNode
{
private:
	//VellieScene* _velli;
public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);
	//virtual void velliCameraMove();
	gameStudy(void);
	virtual ~gameStudy(void);
};

