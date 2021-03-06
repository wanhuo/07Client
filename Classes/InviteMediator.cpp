#include "InviteMediator.h"
#include "UILayerService.h"
#include "ViewManager.h"
InviteMediator::InviteMediator()
{

}

InviteMediator::~InviteMediator()
{

}

/**
开始执行函数
*/
void InviteMediator::OnRegister()
{
	inviteView = (InviteView*)getView();

	inviteView->initView();

	Size size = Director::getInstance()->getVisibleSize();
	getView()->rootNode->setPosition(size.width / 2, size.height / 2);
}

/**
结束回收执行函数
*/
void InviteMediator::onRemove()
{

}

void InviteMediator::handleOndeskInfo(void* data)
{
	OnDeskPlayerInfo  onDeskInfo = *(OnDeskPlayerInfo*)data;
	int clientChairId = DATAPlayerIndesk->chair[onDeskInfo.wChairID % 4];


}

/*
事件响应函数
*/
void InviteMediator::onEvent(int i, void* data)
{

	switch (i)
	{
	case 17000:
		PLayEffect(EFFECT_BTN)
		clickCloseBtnHander();
		break;

	case OTHER_PLAYER_ON_DESK:
		handleOndeskInfo(data);
		break;
	}
}

Layer* InviteMediator::getLayer()
{
	return VIEW->uiLayer;
}

void InviteMediator::clickCloseBtnHander()
{
	removeView(this);
}
