#include "stdafx.h"
#include "fruit0.h"


fruit0::fruit0()
{
}


fruit0::~fruit0()
{
}


HRESULT fruit0::init(float x, float y)
{
	tem.image = IMAGEMANAGER->addFrameImage("fruit0", "item/fruit0.bmp", 330, 78, 10, 2, true, RGB(255, 0, 255));
	tem.rc = RectMakeCenter(tem.x, tem.y, tem.image->getFrameWidth(), tem.image->getFrameHeight());

	tem.x = x;
	tem.y = y;
	tem.coinUp = 0;
	tem.hpUp = 5; // 먹으면 플레이어 hp += 5 
	tem.life = true;
	tem.temState = STAY;
	tem.angle = PI;
	tem.gravity = 30.0f;
	tem.groundgrv = 0;
	tem.radius = tem.image->getFrameWidth();
	tem.ground = false;
	tem.angle = PI; // 포물선으로 던지기위한 앵글변수. 
	tem.speed = 12; // 던지면 날라가는 속도. 속도를 높일수록 castGravity값도 높여야한다.

	follow = false; // 플레이어를 따라다니냐?
	cast = false; //던졌냐?
	castGravity = 0; // 점점 증가시킬 중력.
	showCurve = false; // 커브 보여줄때 구분 불.

	fruit0eat = false;
	eatFrame = 0;
	return S_OK;
}

void fruit0::release(void)
{
}

void fruit0::update(void)
{
	if (tem.life)
	{
	// 카메라때매 계속 그려줘야 됨. 
	tem.rc = RectMakeCenter(tem.x, tem.y, tem.image->getFrameWidth(), tem.image->getFrameHeight());

	PixelCollision();
	CastFunc();
	CurveLineFunc();

	RECT tempRC;
	if (IntersectRect(&tempRC, &otus->_player.rc, &tem.rc))
	{
		if (KEYMANAGER->isOnceKeyDown('9'))
		{
			follow = true;
			otus->_player.state = FLYHOLD; // 들어는 오는데 바로 플레이어 상태에서 바꾸고 있어서...
		}
	}

	if (follow)
	{
		if (KEYMANAGER->isOnceKeyDown(MK_LBUTTON))
		{
			fruit0eat = true;
			DATABASE->getElementData("player")->currentHP += 5;
			//otus->_player.currentHP += 5; // 요건 적용이 안됨.
		}
	}
	}//life

	eat();    
}

void fruit0::render(void)
{
	if (tem.life)
	{
		// item rc
		Rectangle(getMemDC(), tem.rc.left, tem.rc.top, tem.rc.right, tem.rc.bottom);
		tem.image->frameRender(getMemDC(), tem.rc.left, tem.rc.top); 

		// 던질때 커브 원형들 그리기.
		if (showCurve)
		{
			for (int i = 0; i < CURVE_CIRCLE_LINE; i++)
			{
				Ellipse(getMemDC(), curveLine[i].rc.left, curveLine[i].rc.top, curveLine[i].rc.right, curveLine[i].rc.bottom);
			}
		}
	}//life
}

void fruit0::PixelCollision(void)
{
		if (follow)
		{
			//tem.state = FLYHOLD;
			tem.x = otus->_player.x;
			tem.y = otus->_player.y + 70;
			tem.ground = false;
		}
		// mouse angle  
		// 여기까지가 게디펑션.

		//땅에있지도 않고 점프상태도 아니면, 떨어진다. 
		if (!tem.ground && !follow)
		{
			//tem.geddyState = gFALL;
			tem.groundgrv -= 1.4f;
			tem.y -= tem.groundgrv;
			if (tem.groundgrv < -20) tem.groundgrv = -20;
		}

		////geddy__________________________________________________________________________________________________
		// i = x - 크기절반; i< x + 크기의 절반; i++
		for (int i = tem.x - tem.image->getFrameWidth() / 2; i < tem.x + tem.image->getFrameWidth() / 2; ++i)
		{
			// 만약 플레이어 y값+크기/2 아래 픽셀이 정해진 색이고 && 땅이 아니면
			if (GetPixel(getPixel(), i, tem.y + tem.image->getFrameWidth() / 2 + 5) == RGB(255, 0, 255) && !tem.ground)
			{
				tem.ground = true; // 땅이라고 알려줌. 
				tem.y += tem.groundgrv; // 플레이어y += 땅 그래피티 
				tem.groundgrv = 0;
				//tem.geddyState = gIDLE;
			}
		}
		//바텀 
		for (int i = tem.x - tem.image->getFrameWidth() / 2; i < tem.x + tem.image->getFrameWidth() / 2; ++i)
		{
			if (GetPixel(getPixel(), i, tem.y + tem.image->getFrameWidth() / 2 + 3) == RGB(255, 0, 255) && tem.gravity <= 0)
			{
				tem.y += tem.gravity;
				tem.gravity = 20;
				break;
			}
		}

}//func


void fruit0::CastFunc()
{
	if (follow)
	{
		if (KEYMANAGER->isOnceKeyUp(MK_RBUTTON))
		{		
			cast = true;
			showCurve = false;
			follow = false;
			castGravity = 0;
			otus->_player.state = FLYIDLE;
		}
	}

	if (cast)
	{
		castGravity += 0.5f;
		tem.gravity = 0;
		tem.groundgrv = 0;
		tem.x += cosf(tem.angle) *  tem.speed;
		tem.y += -sinf(tem.angle) * tem.speed + castGravity;
	}
	if (tem.ground)
	{
		cast = false;
		castGravity = 0;
	}
}

//궤적 만들기 
void fruit0::CurveLineFunc()
{
	if (follow)
	{
		if (KEYMANAGER->isStayKeyDown(MK_RBUTTON))
		{
			showCurve = true;
			tem.angle = getAngle(tem.x, tem.y, _ptMouse.x, _ptMouse.y);
			curveLine[0].x = tem.x;
			curveLine[0].y = tem.y;
			curveLine[0].rc = RectMakeCenter(curveLine[0].x, curveLine[0].y, CURVE_CIRCLE_SIZE / 3, CURVE_CIRCLE_SIZE / 3);
			curveLine[0].angle = tem.angle;

			for (int i = 0, j = 1; j < CURVE_CIRCLE_LINE; i++, j++)
			{
				if (j == 1) castGravity = 0.05f;
				else if (j == 2) castGravity = 2.0f;
				else if (j == 3) castGravity = 4.0f;
				else if (j == 4) castGravity = 6.0f;
				else if (j == 5) castGravity = 8.0f;
				else if (j == 6) castGravity = 12.0f;
				else if (j == 7) castGravity = 17.0f;
				else if (j == 8) castGravity = 22.0f;
				else if (j == 9) castGravity = 30.0f;

				curveLine[j].angle = curveLine[i].angle;
				curveLine[j].x = cosf(curveLine[i].angle) * CURVE_CIRCLE_SIZE;
				curveLine[j].y = -sinf(curveLine[i].angle) * CURVE_CIRCLE_SIZE + castGravity;

				curveLine[j].x = curveLine[j].x + curveLine[i].x;
				curveLine[j].y = curveLine[j].y + curveLine[i].y;

				curveLine[j].rc = RectMakeCenter(curveLine[j].x, curveLine[j].y, CURVE_CIRCLE_SIZE / 3, CURVE_CIRCLE_SIZE / 3);
			}
		}// Rbutton
	}
}

// 먹을때 과일 위로 올리는 프레임.
void fruit0::eat()
{
	if (fruit0eat)
	{
		otus->_player.state = FLYEAT;
	
		//DATABASE->getElementData
		eatFrame++;
		if (otus->_player.direction == RIGHT)
		{
			if (eatFrame > 4)
			{
				tem.y -= 5.5f;
				tem.x += 5.5f;
			}
			if (eatFrame > 8)
			{
				tem.y -= 5.5f;
				tem.x += 5.5f;
			}
			if (eatFrame > 12)
			{
				tem.y -= 5.5f;
				tem.x += 5.5f;
			}
			if (eatFrame > 15)
			{
				tem.y -= 5.5f;
				tem.x += 5.5f;
			}
			if (eatFrame > 18)
			{
				tem.y -= 5.5f;
				tem.x += 5.5f;
			}
			if (eatFrame > 21)
			{
				tem.y -= 5.5f;
				tem.x += 5.5f;
			}
			if (eatFrame > 24)
			{
				tem.y -= 5.5f;
				tem.x += 5.5f;
			}
			if (eatFrame > 28)
			{
				tem.y -= 6.5f;
				tem.x += 5.5f;
			}
			if (eatFrame > 32)
			{
				tem.y -= 7.5f;
				tem.x += 5.5f;
			}
			if (eatFrame > 36)
			{
				tem.y -= 8.5f;
			}
			if (eatFrame > 40)
			{
				tem.y -= 10.5f;
			}
			if (eatFrame > 44)
			{
				tem.y -= 10.5f;
				tem.x -= 5.5f;
			}
			if (eatFrame > 47)
			{
				fruit0eat = false;
				tem.life = false;
				eatFrame = 0;
			}
		}


		else if (otus->_player.direction == LEFT)
		{
			if (eatFrame > 4)
			{
				tem.y -= 5.5f;
				tem.x -= 5.5f;
			}
			if (eatFrame > 8)
			{
				tem.y -= 5.5f;
				tem.x -= 5.5f;
			}
			if (eatFrame > 12)
			{
				tem.y -= 5.5f;
				tem.x -= 5.5f;
			}
			if (eatFrame > 15)
			{
				tem.y -= 5.5f;
				tem.x -= 5.5f;
			}
			if (eatFrame > 18)
			{
				tem.y -= 5.5f;
				tem.x -= 5.5f;
			}
			if (eatFrame > 21)
			{
				tem.y -= 5.5f;
				tem.x -= 5.5f;
			}
			if (eatFrame > 24)
			{
				tem.y -= 5.5f;
				tem.x -= 5.5f;
			}
			if (eatFrame > 28)
			{
				tem.y -= 6.5f;
				tem.x -= 5.5f;
			}
			if (eatFrame > 32)
			{
				tem.y -= 7.5f;
				tem.x -= 5.5f;
			}
			if (eatFrame > 36)
			{
				tem.y -= 8.5f;
			}
			if (eatFrame > 40)
			{
				tem.y -= 10.5f;
			}
			if (eatFrame > 44)
			{
				tem.y -= 10.5f;
				tem.x += 5.5f;
			}
			if (eatFrame > 47)
			{
				fruit0eat = false;
				tem.life = false;
				eatFrame = 0;
			}
		}
	}
}