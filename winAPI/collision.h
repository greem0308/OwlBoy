#pragma once

namespace MY_UTIL
{
	//pt구조체
	typedef struct tagPoint
	{
		float x;
		float y;

		tagPoint() : x(0.0f), y(0.0f) {}
		tagPoint(float x, float y) : x(x), y(y) {}

		void move(float dx, float dy)
		{
			x += dx;
			y += dy;
		}
	} MYPOINT, *LPPOINT;

	//원 구조체
	typedef struct tagCircle
	{
		float x;		//원 중심
		float y;		//원 중심
		float r;		//원 반지름

		tagCircle() : x(0.0f), y(0.0f), r(0.0f) {}
		tagCircle(float x, float y, float r) : x(x), y(y), r(r) {}

		void move(float dx, float dy)
		{
			x += dx;
			y += dy;
		}

		void set(float centerX, float centerY, float radius)
		{
			x = centerX;
			y = centerY;
			r = radius;
		}

		void setCenterPos(float posX, float posY)
		{
			x = posX;
			y = posY;
		}

		void render(HDC hdc)
		{
			int x = FLOAT_TO_INT(x);
			int y = FLOAT_TO_INT(y);
			int r = FLOAT_TO_INT(r);

			Ellipse(
				hdc,
				x - r,
				y - r,
				x + r,
				y + r);
		}
	}MYCIRCLE, *LPMYCIRCLE;

	//사각형 구조체
	typedef struct tagMYRECT
	{
		float _left;
		float _top;
		float _right;
		float _bottom;

		tagMYRECT() : _left(0.0f), _top(0.0f), _right(0.0f), _bottom(0.0f) {}
		tagMYRECT(float left, float top, float right, float bottom) : _left(left), _top(top), _right(right), _bottom(bottom) {}

		void move(float dx, float dy)
		{
			_left += dx;
			_right += dx;
			_top += dy;
			_bottom += dy;
		}

		void set(float left, float top, float right, float bottom)
		{
			_left = left;
			_top = top;
			_right = right;
			_bottom = bottom;
		}

		void setLeftTopPos(float left, float top)
		{
			float width = getWidth();
			float height = getHeight();
			
			_left = left;
			_top = top;
			_right = left + width;
			_bottom = top + height;
		}

		void setCenterPos(float centerX, float centerY)
		{
			float halfW = getWidth() * 0.5f;
			float halfH = getHeight() * 0.5f;
			
			_left = centerX - halfW;
			_top = centerY - halfH;
			_right = centerX + halfW;
			_bottom = centerY + halfH;
		}

		void render(HDC hdc)
		{
			Rectangle(
				hdc,
				FLOAT_TO_INT(_left),
				FLOAT_TO_INT(_top),
				FLOAT_TO_INT(_right),
				FLOAT_TO_INT(_bottom)
				);
		}

		float getWidth(void) {return _right - _left;}
		float getHeight(void) {return _bottom - _top;}
	} MYRECT, *LPMYRECT;

	//점과 사각형 충돌
	bool checkPointInRect(const RECT& rc, const POINT& pt);
	bool checkPointInRect(const RECT& rc, int x, int y);
	bool checkPointInRect(const MYRECT& rc, float x, float y);
	bool checkPointInRect(const MYRECT& rc, const MYPOINT& pt);

	//점과 원 충돌
	bool checkPointInCircle(float cx, float cy, float cr, const MYPOINT &pt);
	bool checkPointInCircle(float cx, float cy, float cr, float x, float y);
	bool checkPointInCircle(const MYCIRCLE& cir, const MYPOINT &pt);
	bool checkPointInCircle(const MYCIRCLE& cir, float x, float y);

	//사각형과 사각형 충돌
	bool isCollision(const RECT& rc1, const RECT& rc2);
	bool isCollision(const MYRECT& rc1, const MYRECT& rc2);

	//원과 원 충돌
	bool isCollision(const MYCIRCLE& cir1, const MYCIRCLE& cir2);

	//원과 사각형 충돌
	bool isCollision(const MYCIRCLE& cir, const RECT& rc);
	bool isCollision(const MYCIRCLE& cir, const MYRECT& rc);

	//충돌 후 밀림...
	bool isCollision_notinter(const RECT& rcHold, RECT& rcMove);
	bool isCollision_notinter(const MYRECT& mrcHold, MYRECT& mrcMove);
	bool isCollision_notinter(const MYCIRCLE& cirHold, MYCIRCLE& cirMove);
}



