#include "AccountView.h"
#include "DataManager.h"
#include "SGTools.h"
#include "SendDataService.h"
#include "TCPSocketService.h"
#include "ConnectGameServiceCommand.h"
#include "LobbyView.h"
#include "LobbyMediator.h"
#include "ViewManager.h"
#include "PlatFormControl.h"


AccountView::AccountView()
{

}

AccountView::~AccountView()
{

}

void AccountView::updateBackTimes(float dt)
{
	leftTime--;
	txtBackTime->setString(Tools::parseInt2String(leftTime));
	if (leftTime <= 0)
	{

#if(AutoPlayPoker == 0)
		if (DATA->bGameCate != DataManager::E_GameBlood)
		{
			clickBtnBack(NULL);
		}
		else
		{
			clickBtnContinune(NULL);
		}

#endif

#if(AutoPlayPoker == 1)
		clickBtnContinune(NULL);
#endif
	}

}

void AccountView::initView()
{
	leftTime = 10;

	if (DATA->bGameCate != DataManager::E_GameCateMatch)
	{
		schedule(schedule_selector(AccountView::updateBackTimes), 1.0f);
		rootNode = CSLoader::createNode("jieShuan.csb");
		addChild(rootNode);

		UIGet_Button("fanHuiBtn", rootNode, btnBack)
			UIClick(btnBack, AccountView::clickBtnBack)

		UIGet_Text("Text_backTime", rootNode, txtBackTime)

		UIGet_Button("continueBtn", rootNode, btnContinue)
		UIClick(btnContinue, AccountView::clickBtnContinune)
		if (DATA->bGameCate == DataManager::E_GameBlood)
		{
			if (DATA->GameEndData.bIsBlood)  //血战打过
			{
				//btnContinue->setVisible(false);
				//btnBack->setPositionX(0);
			}
			else
			{
				//btnBack->setVisible(false);
				//btnContinue->setPositionX(0);

			}
		}
	}

	else if (DATA->bGameCate == DataManager::E_GameCateMatch)  //比赛
	{
		rootNode = CSLoader::createNode("jieShuan_match.csb");
		addChild(rootNode);

		UIGet_Button("fanHuiBtn", rootNode, btnBack)
			UIClick(btnBack, AccountView::clickBtnBack)

		UIGet_Button("nextMatchBtn", rootNode, btnNextMatch)
			UIClick(btnNextMatch, AccountView::clickBtnNextMatch)

	}

}

void AccountView::btnBackHandle()
{
	PlayerInDeskModel *playerInDeskModel = ((PlayerInDeskModel*)getModel(PlayerInDeskModel::NAME));
	int myChair = playerInDeskModel->getServiceChairID(0);
	int myTable = playerInDeskModel->DeskPlayerInfo[myChair].wTableID;
	((SendDataService*)getService(SendDataService::NAME))->sendLeaveTable(myTable, myChair, true);

	//关闭游戏服务器SOCKET
	((TCPSocketService*)getService(TCPSocketService::GAME))->closeMySocket();

	//停掉网络主动监测与心跳
	if (getcontainer()->isScheduled(schedule_selector(ConnectGameServiceCommand::checkNetWorks)))
	{
		getcontainer()->unschedule(schedule_selector(ConnectGameServiceCommand::checkNetWorks));
	}
	if (getcontainer()->isScheduled(schedule_selector(ConnectGameServiceCommand::heartPacket)))
	{
		getcontainer()->unschedule(schedule_selector(ConnectGameServiceCommand::heartPacket));
	}
	//跳转至大厅界面
	blueSkyDispatchEvent(EventType::BACK_TO_HALL_NEW);
	blueSkyDispatchEvent(EventType::BACK_TO_HALL);
	//creatView(new LobbyView(), new LobbyMediator());
	DATA->lobbyview->setTouchEnabled(true);
	DATA->lobbyview->setVisible(true);
	DATA->lobbyview->refreshView();


	blueSkyDispatchEvent(10501);
}

void AccountView::clickBtnBack(Ref* pSender)
{
	PLayEffect(EFFECT_BTN);
	//血战没打过，二次弹框。
	if (DATA->bGameCate == DataManager::E_GameBlood && DATA->GameEndData.bIsBlood == 0)
	{
		ndPubMsg = CSLoader::createNode("publicMessage.csb");
		ndPubMsg->setPosition(WScreen * 0.5, HScreen * 0.5);
		VIEW->maskLayer->addChild(ndPubMsg);
		ndPubMsg->setScale(0.8f, 0.8f);
		ndPubMsg->runAction(Sequence::create(ScaleTo::create(0.2f, 1.03f),
			ScaleTo::create(0.15f, 1.0f), nullptr));
		Text*   txtMsg;
		Button* btnClose, *btnOK, *btnNo;
		ImageView* imgClose;

		UIGet_Text("Text_msg", ndPubMsg, txtMsg)
			UIGet_ImageView("Image_close", ndPubMsg, imgClose)
			UIGet_Button("Button_close", ndPubMsg, btnClose)
			UIGet_Button("Button_No", ndPubMsg, btnNo)
			UIGet_Button("Button_OK", ndPubMsg, btnOK)

			txtMsg->setString(UTF8String("popup", "bloodquit"));
			btnOK->addClickEventListener([this](Ref*  pSender)
		{
				ndPubMsg->removeFromParentAndCleanup(true);
				btnBackHandle();
			
		}

		);
		btnNo->addClickEventListener([this](Ref* pSender)
		{

			ndPubMsg->removeFromParentAndCleanup(true);
		}
			);
	}
	else
	{
		btnBackHandle();
	}
}


//需要判断钱够不够？
void AccountView::clickBtnContinune(Ref* pSender)
{
	PLayEffect(EFFECT_BTN);
	DATAGameData->player[0].pokerArr = {};
	SEND->sendReady();
	blueSkyDispatchEvent(EventType::SHOW_PLAYER_ON_DESK);
	//跳转至打牌界面
	blueSkyDispatchEvent(10501);

}

void AccountView::clickBtnNextMatch(Ref* pSender)
{
	PLayEffect(EFFECT_BTN);
	//发送离开桌子消息
	PlayerInDeskModel *playerInDeskModel = ((PlayerInDeskModel*)getModel(PlayerInDeskModel::NAME));
	int myChair = playerInDeskModel->getServiceChairID(0);
	int myTable = playerInDeskModel->DeskPlayerInfo[myChair].wTableID;
	((SendDataService*)getService(SendDataService::NAME))->sendLeaveTable(myTable, myChair, true);

	//关闭游戏服务器SOCKET
	((TCPSocketService*)getService(TCPSocketService::GAME))->closeMySocket();  //test
	//停掉网络主动监测与心跳
	if (getcontainer()->isScheduled(schedule_selector(ConnectGameServiceCommand::checkNetWorks)))
	{
		getcontainer()->unschedule(schedule_selector(ConnectGameServiceCommand::checkNetWorks));
	}

	if (getcontainer()->isScheduled(schedule_selector(ConnectGameServiceCommand::heartPacket)))
	{
		getcontainer()->unschedule(schedule_selector(ConnectGameServiceCommand::heartPacket));
	}


	DATA->bGameCate = DataManager::E_GameCateMatch;
	creatView(new MatchView(), new MatchMediator());
	blueSkyDispatchEvent(10501);

}

void AccountView::playAccountAction(bool isSuccess)
{
	failActionNode = rootNode->getChildByName("failActionNode");
	failActionNode->setVisible(false);
	winParticle = dynamic_cast<ParticleSystemQuad*>(rootNode->getChildByName("winParticle"));
	winParticle->setVisible(false);
	glodParticle = dynamic_cast<ParticleSystemQuad*>(rootNode->getChildByName("glodParticle"));
	glodParticle->setVisible(false);
	if (!isSuccess)
	{
		failActionNode->setVisible(true);
		failAction = CSLoader::createTimeline("failAction.csb");
		rootNode->runAction(failAction);
		failAction->gotoFrameAndPlay(0, false);
	}
	else
	{
		winParticle->setVisible(true);
		glodParticle->setVisible(true);
	}
}


void AccountView::showSuccessAction(float dt)
{
	glodParticle->setVisible(true);
}
