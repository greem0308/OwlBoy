#pragma once

#include "gamenode.h"
#include <string>

class collisionTestScene : public gameNode
{
private:
	string _text;

	MYCIRCLE _cir;
	MYCIRCLE _cirMove;

	MYRECT _rc;
	MYRECT _rcMove;

public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	collisionTestScene(void);
	virtual ~collisionTestScene(void);
};

