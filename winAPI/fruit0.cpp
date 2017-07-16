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
	tem.hpUp = 5; // ������ �÷��̾� hp += 5 
	tem.life = true;
	tem.temState = STAY;
	tem.angle = PI;
	tem.gravity = 30.0f;
	tem.groundgrv = 0;
	tem.radius = tem.image->getFrameWidth();
	tem.ground = false;
	tem.angle = PI; // ���������� ���������� �ޱۺ���. 
	tem.speed = 12; // ������ ���󰡴� �ӵ�. �ӵ��� ���ϼ��� castGravity���� �������Ѵ�.

	follow = false; // �÷��̾ ����ٴϳ�?
	cast = false; //������?
	castGravity = 0; // ���� ������ų �߷�.
	showCurve = false; // Ŀ�� �����ٶ� ���� ��.

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
	// ī�޶󶧸� ��� �׷���� ��. 
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
			otus->_player.state = FLYHOLD; // ���� ���µ� �ٷ� �÷��̾� ���¿��� �ٲٰ� �־...
		}
	}

	if (follow)
	{
		if (KEYMANAGER->isOnceKeyDown(MK_LBUTTON))
		{
			fruit0eat = true;
			DATABASE->getElementData("player")->currentHP += 5;
			//otus->_player.currentHP += 5; // ��� ������ �ȵ�.
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

		// ������ Ŀ�� ������ �׸���.
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
		// ��������� �Ե����.

		//���������� �ʰ� �������µ� �ƴϸ�, ��������. 
		if (!tem.ground && !follow)
		{
			//tem.geddyState = gFALL;
			tem.groundgrv -= 1.4f;
			tem.y -= tem.groundgrv;
			if (tem.groundgrv < -20) tem.groundgrv = -20;
		}

		////geddy__________________________________________________________________________________________________
		// i = x - ũ������; i< x + ũ���� ����; i++
		for (int i = tem.x - tem.image->getFrameWidth() / 2; i < tem.x + tem.image->getFrameWidth() / 2; ++i)
		{
			// ���� �÷��̾� y��+ũ��/2 �Ʒ� �ȼ��� ������ ���̰� && ���� �ƴϸ�
			if (GetPixel(getPixel(), i, tem.y + tem.image->getFrameWidth() / 2 + 5) == RGB(255, 0, 255) && !tem.ground)
			{
				tem.ground = true; // ���̶�� �˷���. 
				tem.y += tem.groundgrv; // �÷��̾�y += �� �׷���Ƽ 
				tem.groundgrv = 0;
				//tem.geddyState = gIDLE;
			}
		}
		//���� 
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

//���� ����� 
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

// ������ ���� ���� �ø��� ������.
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