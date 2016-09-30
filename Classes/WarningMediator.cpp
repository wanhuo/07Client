#include "WarningMediator.h"

WarningMediator::WarningMediator()
{

}

WarningMediator::~WarningMediator()
{
	delete getView();
	setView(NULL);
}

/**
开始执行函数
*/
void WarningMediator::OnRegister()
{
	warningView = (WarningView*)getView();

	Size size = Director::getInstance()->getVisibleSize();
	getView()->rootNode->setPosition(size.width / 2, size.height / 2);
}

/**
结束回收执行函数
*/
void WarningMediator::onRemove()
{

}

/*
事件响应函数
*/
void WarningMediator::onEvent(int i, void* data)
{
	switch (i)
	{
	case 11401:
		if (warningView->enterEventId >= 0)
		{
			blueSkyDispatchEvent(warningView->enterEventId);
		}
		removeView(this);
		break;
	case 11402:
		if (warningView->cancleEventId >= 0)
		{
			blueSkyDispatchEvent(warningView->cancleEventId);
		}
		removeView(this);
		break;
	case 11403:
		if (warningView->enterEventId >= 0)
		{
			blueSkyDispatchEvent(warningView->enterEventId);
		}
		removeView(this);
		break;
	case 11404:
		if (warningView->cancleEventId >= 0)
		{
			blueSkyDispatchEvent(warningView->cancleEventId);
		}
		removeView(this);
		break;
	}
}

Layer* WarningMediator::getLayer()
{
	return ((UILayerService*)getService(UILayerService::NAME))->TopLayer;
}