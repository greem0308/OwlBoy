#include "StdAfx.h"
#include "collision.h"

namespace MY_UTIL
{
	//���� �簢�� �浹
	bool checkPointInRect(const RECT& rc, const POINT& pt)
	{
		if ((rc.left <= pt.x && pt.x <= rc.right) && 
			(rc.top <= pt.y && pt.y <= rc.bottom))
		{
			return true;
		}
		return false;
	}

	bool checkPointInRect(const RECT& rc, int x, int y)
	{
		if ((rc.left <= x && x <= rc.right) && 
			(rc.top <= y && y <= rc.bottom))
		{
			return true;
		}
		return false;
	}

	bool checkPointInRect(const MYRECT& rc, float x, float y)
	{
		if ((rc._left <= x && x <= rc._right) && 
			(rc._top <= y && y <= rc._bottom))
		{
			return true;
		}
		return false;
	}

	bool checkPointInRect(const MYRECT& rc, const MYPOINT& pt)
	{
		if ((rc._left <= pt.x && pt.x <= rc._right) && 
			(rc._top <= pt.y && pt.y <= rc._bottom))
		{
			return true;
		}
		return false;
	}

	//���� �� �浹
	bool checkPointInCircle(float cx, float cy, float cr, const MYPOINT &pt)
	{
		//x, y �Ÿ�
		float deltaX = pt.x - cx;
		float deltaY = pt.y - cy;

		//�Ÿ��� ����
		float d = deltaX * deltaX + deltaY * deltaY;

		//�������� ����
		float r = cr * cr;

		if (r < d) return false;
		return true;
	}

	bool checkPointInCircle(float cx, float cy, float cr, float x, float y)
	{
		//x, y �Ÿ�
		float deltaX = x - cx;
		float deltaY = y - cy;

		//�Ÿ��� ����
		float d = deltaX * deltaX + deltaY * deltaY;

		//�������� ����
		float r = cr * cr;

		if (r < d) return false;
		return true;
	}

	bool checkPointInCircle(const MYCIRCLE& cir, const MYPOINT &pt)
	{
		//x, y �Ÿ�
		float deltaX = pt.x - cir.x;
		float deltaY = pt.y - cir.y;

		//�Ÿ��� ����
		float d = deltaX * deltaX + deltaY * deltaY;

		//�������� ����
		float r = cir.r * cir.r;

		if (r < d) return false;
		return true;
	}

	bool checkPointInCircle(const MYCIRCLE& cir, float x, float y)
	{
		//x, y �Ÿ�
		float deltaX = x - cir.x;
		float deltaY = y - cir.y;

		//�Ÿ��� ����
		float d = deltaX * deltaX + deltaY * deltaY;

		//�������� ����
		float r = cir.r * cir.r;

		if (r < d) return false;
		return true;
	}

	//�簢���� �簢�� �浹
	bool isCollision(const RECT& rc1, const RECT& rc2)
	{
		if ((rc1.left <= rc2.right && rc1.right >= rc2.left) &&
			(rc1.top <= rc2.bottom && rc1.bottom >= rc2.top))
		{
			return true;
		}
		return false;
	}

	bool isCollision(const MYRECT& rc1, const MYRECT& rc2)
	{
		if ((rc1._left <= rc2._right && rc1._right >= rc2._left) &&
			(rc1._top <= rc2._bottom && rc1._bottom >= rc2._top))
		{
			return true;
		}
		return false;
	}

	//���� �� �浹
	bool isCollision(const MYCIRCLE& cir1, const MYCIRCLE& cir2)
	{
		float deltaX = cir2.x - cir1.x;
		float deltaY = cir2.y - cir1.y;
		float d = deltaX * deltaX + deltaY * deltaY;

		float r = cir1.r + cir2.r;
		float rSquare = r * r;

		if (d > rSquare) return false;
		return true;
	}

	//���� �簢�� �浹
	bool isCollision(const MYCIRCLE& cir, const RECT& rc)
	{
		int centerX = FLOAT_TO_INT(cir.x);
		int centerY = FLOAT_TO_INT(cir.y);
		int radius = FLOAT_TO_INT(cir.r);

		if ((rc.left <= centerX && centerX <= rc.right) ||
			(rc.top <= centerY && centerY <= rc.bottom))
		{
			//������ ũ�⸸ŭ �簢�� Ȯ����� �ش�...
			RECT exRect;
			exRect.left = rc.left - radius;
			exRect.right = rc.right + radius;
			exRect.top = rc.top - radius;
			exRect.bottom = rc.bottom + radius;

			//Ȯ���� �簢�� �ȿ� ���� �߽����� �� �ִ��� Ȯ���Ѵ�...
			if ((exRect.left <= centerX && centerX <= exRect.right) &&
				(exRect.top <= centerY && centerY <= exRect.bottom))
			{
				return true;
			}
		}
		else
		{
			if (checkPointInCircle(cir, (float)rc.left, (float)rc.top)) return true;
			if (checkPointInCircle(cir, (float)rc.right, (float)rc.top)) return true;
			if (checkPointInCircle(cir, (float)rc.left, (float)rc.bottom)) return true;
			if (checkPointInCircle(cir, (float)rc.right, (float)rc.bottom)) return true;
		}
		return false;
	}

	bool isCollision(const MYCIRCLE& cir, const MYRECT& rc)
	{
		int centerX = FLOAT_TO_INT(cir.x);
		int centerY = FLOAT_TO_INT(cir.y);
		int radius = FLOAT_TO_INT(cir.r);

		if ((rc._left <= centerX && centerX <= rc._right) ||
			(rc._top <= centerY && centerY <= rc._bottom))
		{
			//������ ũ�⸸ŭ �簢�� Ȯ����� �ش�...
			RECT exRect;
			exRect.left = rc._left - radius;
			exRect.right = rc._right + radius;
			exRect.top = rc._top - radius;
			exRect.bottom = rc._bottom + radius;

			//Ȯ���� �簢�� �ȿ� ���� �߽����� �� �ִ��� Ȯ���Ѵ�...
			if ((exRect.left <= centerX && centerX <= exRect.right) &&
				(exRect.top <= centerY && centerY <= exRect.bottom))
			{
				return true;
			}
		}
		else
		{
			if (checkPointInCircle(cir, (float)rc._left, (float)rc._top)) return true;
			if (checkPointInCircle(cir, (float)rc._right, (float)rc._top)) return true;
			if (checkPointInCircle(cir, (float)rc._left, (float)rc._bottom)) return true;
			if (checkPointInCircle(cir, (float)rc._right, (float)rc._bottom)) return true;
		}
		return false;
	}

	//�浹 �� �и�...
	bool isCollision_notinter(const RECT& rcHold, RECT& rcMove)
	{
		RECT rcTemp;

		if (!IntersectRect(&rcTemp, &rcHold, &rcMove)) return false;
	
		//����� �������� �浹��...

		int tempW = rcTemp.right - rcTemp.left;
		int tempH = rcTemp.bottom - rcTemp.top;

		//�����浹...!!!
		if (tempW > tempH)
		{
			//������ �浹!!
			if (rcTemp.top == rcHold.top)
			{
				OffsetRect(&rcMove, 0, -tempH);
			}
			//�Ʒ����� �浹!!
			else if (rcTemp.bottom == rcHold.bottom)
			{
				OffsetRect(&rcMove, 0, tempH);
			}
		}

		//�����浹...!!
		else 
		{
			//������ �浹!!
			if (rcTemp.left == rcHold.left)
			{
				OffsetRect(&rcMove, -tempW, 0);
			}
			//�Ʒ����� �浹!!
			else if (rcTemp.right == rcHold.right)
			{
				OffsetRect(&rcMove, tempW, 0);
			}
		}
		
		return true;
	}

	bool isCollision_notinter(const MYRECT& mrcHold, MYRECT& mrcMove)
	{
		RECT rcHold;
		rcHold.left = FLOAT_TO_INT(mrcHold._left);
		rcHold.top = FLOAT_TO_INT(mrcHold._top);
		rcHold.right = FLOAT_TO_INT(mrcHold._right);
		rcHold.bottom = FLOAT_TO_INT(mrcHold._bottom);

		RECT rcMove;
		rcMove.left = FLOAT_TO_INT(mrcMove._left);
		rcMove.top = FLOAT_TO_INT(mrcMove._top);
		rcMove.right = FLOAT_TO_INT(mrcMove._right);
		rcMove.bottom = FLOAT_TO_INT(mrcMove._bottom);

		RECT rcTemp;

		if (!IntersectRect(&rcTemp, &rcHold, &rcMove)) return false;
	
		//����� �������� �浹��...

		int tempW = rcTemp.right - rcTemp.left;
		int tempH = rcTemp.bottom - rcTemp.top;

		//�����浹...!!!
		if (tempW > tempH)
		{
			//������ �浹!!
			if (rcTemp.top == rcHold.top)
			{
				OffsetRect(&rcMove, 0, -tempH);
			}
			//�Ʒ����� �浹!!
			else if (rcTemp.bottom == rcHold.bottom)
			{
				OffsetRect(&rcMove, 0, tempH);
			}
		}

		//�����浹...!!
		else 
		{
			//������ �浹!!
			if (rcTemp.left == rcHold.left)
			{
				OffsetRect(&rcMove, -tempW, 0);
			}
			//�Ʒ����� �浹!!
			else if (rcTemp.right == rcHold.right)
			{
				OffsetRect(&rcMove, tempW, 0);
			}
		}

		mrcMove._left = static_cast<float>(rcMove.left);
		mrcMove._top = static_cast<float>(rcMove.top);
		mrcMove._right = static_cast<float>(rcMove.right);
		mrcMove._bottom = static_cast<float>(rcMove.bottom);

		return true;
	}

	bool isCollision_notinter(const MYCIRCLE& cirHold, MYCIRCLE& cirMove)
	{
		//�Ÿ� ���Ѵ�
		float deltaX = cirMove.x - cirHold.x;
		float deltaY = cirMove.y - cirHold.y;
		float dist = sqrtf(deltaX * deltaX + deltaY * deltaY);

		//������ ���Ѵ�
		float radius = cirHold.r + cirMove.r;

		//�浹 üũ
		if (dist < radius)
		{
			float angle = getAngle(cirHold.x, cirHold.y, cirMove.x, cirMove.y);

			float moveDelta = radius - dist;
			float moveX = cosf(angle) * moveDelta;
			float moveY = -sinf(angle) * moveDelta;
		
			cirMove.move(moveX, moveY);

			return true;
		}

		return false;
	}
};

