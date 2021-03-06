#include "GongPaiActionMediator.h"
#include "ViewManager.h"
GongPaiActionMediator::GongPaiActionMediator(int fromUserID, int toUserID)
{
	fromUser = fromUserID;
	toUser = toUserID;
}

GongPaiActionMediator::~GongPaiActionMediator()
{

}

/**
开始执行函数
*/
void GongPaiActionMediator::OnRegister()
{
	gongPaiActionView = (GongPaiActionView*)getView();

	gongPaiActionView->initView(fromUser, toUser);
}

/**
结束回收执行函数
*/
void GongPaiActionMediator::onRemove()
{

}

/*
事件响应函数
*/
void GongPaiActionMediator::onEvent(int i, void* data)
{
	switch (i)
	{
	case 11601:
		removeView(this);
		break;
	}
}

Layer* GongPaiActionMediator::getLayer()
{
	return VIEW->uiLayer;
}
