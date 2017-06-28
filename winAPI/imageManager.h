#pragma once

#include "singletonbase.h"
#include "image.h"
#include <string>
#include <map>

//====================================================
//####### 2017.03.20 ������� image class #############
//====================================================

class imageManager : public singletonBase<imageManager>
{
public:
	typedef std::map<std::string, image*> mapImageList;
	typedef std::map<std::string, image*>::iterator mapImageIter;

private:
	mapImageList _mImageList;

	//�̹��� ��� �̸��� ���� �̸����� �˻��Ѵ�
	BOOL getFileName(std::string& strFile);

public:
	//�¾�
	HRESULT init(void);

	//����
	void release(void);

	//Ű�� �����ؼ� �̹��� �߰��Ѵ�
	image* addImage(std::string strKey, int width, int height);
	image* addImage(std::string strKey, const char* fileName,
		int width, int height, BOOL tran = FALSE, 
		COLORREF transColor = RGB(255, 0, 255));
	image* addImage(std::string strKey, const char* fileName, int x, int y, 
		int width, int height, BOOL tran = FALSE, 
		COLORREF transColor = RGB(255, 0, 255));
	
	//Ű�� + ������
	image* addFrameImage(std::string strKey, const char* fileName, float x, float y, 
		int width, int height, int frameX, int frameY, BOOL tran = FALSE, 
		COLORREF transColor = RGB(255, 0, 255));
	image* addFrameImage(std::string strKey, const char* fileName, 
		int width, int height, int frameX, int frameY, BOOL tran = FALSE, 
		COLORREF transColor = RGB(255, 0, 255));

	//�̹��� ã�´�
	image* findImage(std::string strKey);

	//�̹��� �����
	BOOL deleteImage(std::string strKey);

	//��ü �����Ѵ�
	BOOL deleteAll(void);

	//�Ϲݷ���
	void render(std::string strKey, HDC hdc);
	void render(std::string strKey, HDC hdc, int destX, int destY);
	void render(std::string strKey, HDC hdc, int sourX, int sourY, int sourWidth, int sourHeight);
	void render(std::string strKey, HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight);

	//������ ����
	void frameRender(std::string strKey, HDC hdc, int destX, int destY);
	void frameRender(std::string strKey, HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY);

	//���� ����
	void loopRender(std::string strKey, HDC hdc, const LPRECT drawArea, int offsetX, int offsetY);

	imageManager(void);
	~imageManager(void);
};

