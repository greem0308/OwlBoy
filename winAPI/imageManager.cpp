#include "StdAfx.h"
#include "imageManager.h"


imageManager::imageManager(void)
{
}


imageManager::~imageManager(void)
{
}

//�¾�
HRESULT imageManager::init(void)
{
	return S_OK;
}

//����
void imageManager::release(void)
{
	deleteAll();
}

//Ű�� �����ؼ� �̹��� �߰��Ѵ�
image* imageManager::addImage(std::string strKey, int width, int height)
{
	//�߰��Ϸ��� Ű ���� �̹����� �ֳ�?
	image* img = findImage(strKey);

	//�߰��Ϸ��� �̹����� ������ �߰����� �ʰ� ��� �̹��� ����...
	if (img) return img;

	//������ �̹��� ����
	img = new image;

	//�̹��� �����ϰ� �̹����� �ʱ�ȭ�� ������ ������ NULL �����Ѵ�
	if (FAILED(img->init(width, height)))
	{
		SAFE_DELETE(img);
		return NULL;
	}

	//������ �̹����� �ʿ� �߰��Ѵ�
	_mImageList.insert(make_pair(strKey, img));

	//�ش� �̹��� ����
	return img;
}

image* imageManager::addImage(std::string strKey, const char* fileName,
	int width, int height, BOOL tran, COLORREF transColor)
{
	//�߰��Ϸ��� Ű ���� �̹����� �ֳ�?
	image* img = findImage(strKey);

	//�߰��Ϸ��� �̹����� ������ �߰����� �ʰ� ��� �̹��� ����...
	if (img) return img;

	//������ �̹��� ����
	img = new image;

	//�̹��� �����ϰ� �̹����� �ʱ�ȭ�� ������ ������ NULL �����Ѵ�
	if (FAILED(img->init(fileName, width, height, tran, transColor)))
	{
		SAFE_DELETE(img);
		return NULL;
	}

	//������ �̹����� �ʿ� �߰��Ѵ�
	_mImageList.insert(make_pair(strKey, img));

	//�ش� �̹��� ����
	return img;
}

image* imageManager::addImage(std::string strKey, const char* fileName, int x, int y, 
	int width, int height, BOOL tran, COLORREF transColor)
{
	//�߰��Ϸ��� Ű ���� �̹����� �ֳ�?
	image* img = findImage(strKey);

	//�߰��Ϸ��� �̹����� ������ �߰����� �ʰ� ��� �̹��� ����...
	if (img) return img;

	//������ �̹��� ����
	img = new image;

	//�̹��� �����ϰ� �̹����� �ʱ�ȭ�� ������ ������ NULL �����Ѵ�
	if (FAILED(img->init(fileName, (float)x, (float)y, width, height, tran, transColor)))
	{
		SAFE_DELETE(img);
		return NULL;
	}

	//������ �̹����� �ʿ� �߰��Ѵ�
	_mImageList.insert(make_pair(strKey, img));

	//�ش� �̹��� ����
	return img;
}
	
//Ű�� + ������
image* imageManager::addFrameImage(std::string strKey, const char* fileName, float x, float y, 
	int width, int height, int frameX, int frameY, BOOL tran, COLORREF transColor)
{
	//�߰��Ϸ��� Ű ���� �̹����� �ֳ�?
	image* img = findImage(strKey);

	//�߰��Ϸ��� �̹����� ������ �߰����� �ʰ� ��� �̹��� ����...
	if (img) return img;

	//������ �̹��� ����
	img = new image;

	//�̹��� �����ϰ� �̹����� �ʱ�ȭ�� ������ ������ NULL �����Ѵ�
	if (FAILED(img->init(fileName, x, y, width, height, frameX, frameY, tran, transColor)))
	{
		SAFE_DELETE(img);
		return NULL;
	}

	//������ �̹����� �ʿ� �߰��Ѵ�
	_mImageList.insert(make_pair(strKey, img));

	//�ش� �̹��� ����
	return img;
}

image* imageManager::addFrameImage(std::string strKey, const char* fileName, 
	int width, int height, int frameX, int frameY, BOOL tran, COLORREF transColor)
{
	//�߰��Ϸ��� Ű ���� �̹����� �ֳ�?
	image* img = findImage(strKey);

	//�߰��Ϸ��� �̹����� ������ �߰����� �ʰ� ��� �̹��� ����...
	if (img) return img;

	//������ �̹��� ����
	img = new image;

	//�̹��� �����ϰ� �̹����� �ʱ�ȭ�� ������ ������ NULL �����Ѵ�
	if (FAILED(img->init(fileName, width, height, frameX, frameY, tran, transColor)))
	{
		SAFE_DELETE(img);
		return NULL;
	}

	//������ �̹����� �ʿ� �߰��Ѵ�
	_mImageList.insert(make_pair(strKey, img));

	//�ش� �̹��� ����
	return img;
}

//�Ϲݷ���
void imageManager::render(std::string strKey, HDC hdc)
{
	image* img = findImage(strKey);
	if(img) img->render(hdc); 
}

void imageManager::render(std::string strKey, HDC hdc, int destX, int destY)
{
	image* img = findImage(strKey);
	if(img) img->render(hdc, destX, destY); 
}

void imageManager::render(std::string strKey, HDC hdc, int sourX, int sourY, int sourWidth, int sourHeight)
{
	image* img = findImage(strKey);
	if(img) img->render(hdc, sourX, sourY, sourWidth, sourHeight); 
}

void imageManager::render(std::string strKey, HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight)
{
	image* img = findImage(strKey);
	if(img) img->render(hdc, destX, destY, sourX, sourY, sourWidth, sourHeight); 
}

//������ ����
void imageManager::frameRender(std::string strKey, HDC hdc, int destX, int destY)
{
	image* img = findImage(strKey);
	if(img) img->frameRender(hdc, destX, destY); 
}

void imageManager::frameRender(std::string strKey, HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY)
{
	image* img = findImage(strKey);
	if(img) img->frameRender(hdc, destX, destY, currentFrameX, currentFrameY);
}

//���� ����
void imageManager::loopRender(std::string strKey, HDC hdc, const LPRECT drawArea, int offsetX, int offsetY)
{
	image* img = findImage(strKey);
	if(img) img->loopRender(hdc, drawArea, offsetX, offsetY); 
}

//�̹��� ã�´�
image* imageManager::findImage(std::string strKey)
{
	//�ش�Ű �˻��ϰ�..
	mapImageIter key = _mImageList.find(strKey);

	//�ش� Ű���� ã������...
	if (key != _mImageList.end())
	{
		return key->second;
	}

	return NULL;
}

//�̹��� �����
BOOL imageManager::deleteImage(std::string strKey)
{
	//�ش�Ű �˻��ϰ�..
	mapImageIter key = _mImageList.find(strKey);

	//�ش� Ű���� ã����..
	if (key != _mImageList.end())
	{
		//�̹��� ����
		key->second->release();

		//�޸� ����
		SAFE_DELETE(key->second);

		//�ʸ���Ʈ���� ����
		_mImageList.erase(key);

		return TRUE;
	}
	return FALSE;
}

//��ü �����Ѵ�
BOOL imageManager::deleteAll(void)
{
	//�ش�Ű �˻��ϰ�..
	mapImageIter iter = _mImageList.begin();

	//�� ��ü�� ���鼭 �����ش�
	for (; iter != _mImageList.end();)
	{
		//�������� �ݺ��� ������Ű�� �ʴ´�
		if (iter->second != NULL)
		{
			iter->second->release();
			delete iter->second;
			iter = _mImageList.erase(iter);
		}
		else 
		{
			//�ݺ��� ����..
			++iter;
		}
	}

	_mImageList.clear();

	return TRUE;
}

//�̹��� ��� �̸��� ���� �̸����� �˻��Ѵ�
BOOL imageManager::getFileName(std::string& strFile)
{
	if (strFile.empty()) return false;

	//��Ʈ�� ������ ��ġ ��
	size_t pos;

	//��� �ڸ���
	//�ڿ��� ���� '/'���� ������ ���� ã�Ƽ� pos�� �����Ѵ�
	pos = strFile.find_first_of("/");

	//ã������...
	if (pos != std::string::npos)
	{
		//�߶󳽴�
		strFile = strFile.substr(pos + 1, strFile.length());
	}
	else //�� ã������... '\'�� ã�´� 
	{
		//�ٽ� ã�´�
		pos = strFile.find_last_of( "\\" );

		if (pos != std::string::npos)
		{
			//�߶󳽴�
			strFile = strFile.substr(pos + 1, strFile.length());
		}
	}

	//Ȯ���� ¥����
	pos = strFile.find(".");

	if (pos != std::string::npos) strFile = strFile.substr(0, pos);
	else return FALSE;

	return TRUE;
}

