#pragma once

#include "gamenode.h"
#include "test.h"
#include "startScene.h"
#include "otusHouseScene.h"
#include "VellieScene.h"
#include "storeScene.h"
#include "labScene.h"
#include "bombamanScene.h"
#include "eventBridgeScene.h"
#include "eventScene.h"
#include "dungeonScene.h"
#include "BossScene.h"
#include "collisionTestScene.h"

enum
{
	arrowIDLE, arrowGUN, arrowTHROW
};

class gameStudy : public gameNode
{
private:
	//VellieScene* _velli;
public:
	int arrowFrameCount;
	int arrowCurrentX;
	int arrowCurrentY;
	int arrowState;
	
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void mouseFunc(void);

	gameStudy(void);
	virtual ~gameStudy(void);
};

