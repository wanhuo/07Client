#include "FriendView.h"
#include "SGTools.h"
#include "DataManager.h"
#include "ViewManager.h"
#include "SendDataService.h"
#include "UTF8.h"
#include "TCPSocketService.h"
#include "PlatFormControl.h"
#include "ShopMediator.h"
#include "ShopView.h"
#include "SetMediator.h"
#include "SetView.h"
#pragma  1

//caifu降序
bool greater_caifu(tagFriendParameter & m1, tagFriendParameter & m2)
{
	return m1.dwRmb > m2.dwRmb;
}

bool greater_meili(tagFriendParameter & m1, tagFriendParameter & m2)
{
	return m1.dwLoveLiness > m2.dwLoveLiness;
}

bool greater_shenglv(tagFriendParameter & m1, tagFriendParameter & m2)
{
	return m1.WinRate > m2.WinRate;
}

FriendView::FriendView()
{

	//initData();
	initView();
}


FriendView::~FriendView()
{
}

//去除熟人中的好友
void FriendView::refreshShuren()
{
	DWORD  dwFriendsSize = DATA->vFriends.size();
	DWORD  dwShurenSize = DATA->vFriendsShuRen.size();
	for (int i = 0; i < dwShurenSize; i++)
	{
		for (int j = 0; j < dwFriendsSize; j++)
		{
			if (DATA->vFriendsShuRen.at(i).dwUserID 
				== DATA->vFriends.at(j).dwUserID)
			{
				DATA->vFriendsShuRen.erase(DATA->vFriendsShuRen.begin() + i);
				i--;
				dwShurenSize--;
				break;
			}
		}
	}
	dwShurenSize = DATA->vFriendsShuRen.size();
	for (int i = 0; i < dwShurenSize; i ++)
	{
		if (DATA->vFriendsShuRen.at(i).dwUserID
			== DATA->myBaseData.dwUserID)
		{
			DATA->vFriendsShuRen.erase(DATA->vFriendsShuRen.begin() + i);
			break;
		}
	}


}

void FriendView::initView()
{

	//request friend list
	SEND->sendFriendReq(DATA->myBaseData.dwUserID);

	rootNode = CSLoader::createNode("Friends.csb");
	addChild(rootNode);

	rootNode->setScale(0.8f, 0.8f);
	rootNode->runAction(Sequence::create(
		ScaleTo::create(0.2f, 1.03f),
		ScaleTo::create(0.15f, 1.0f),
		nullptr));
	VIEW->nowViewTag = ViewManager::eViewFriend;

	logV("\n myUserId %d", DATA->myBaseData.dwUserID);
	UIGet_Button("Button_close", rootNode, closeBtn)
		BTN_EVENT(closeBtn, 12301)

		UIGet_Node("FileNode_0", rootNode, ndFriends[0])
		UIGet_Node("FileNode_1", rootNode, ndFriends[1])
		UIGet_Node("FileNode_2", rootNode, ndFriends[2])

		UIGet_CheckBox("CheckBox_0", rootNode, chkFriends[0])
		UIGet_CheckBox("CheckBox_1", rootNode, chkFriends[1])
		UIGet_CheckBox("CheckBox_2", rootNode, chkFriends[2])

		for (int i = 0; i < 3; i ++)
		{
			chkFriends[i]->setTag(i);
			//UIClick(chkFriends[i], FriendView::clickCheckItemFriend);
			UIClickCheck(chkFriends[i], FriendView::clickCheckItemFriends);
		}


		//top
		Node*  topNode;
		UIGet_Node("FileNode_top", rootNode, topNode)
			UIGet_Text("Text_gold", topNode, txtGold)
			UIGet_Text("Text_diamond", topNode, txtDiamond)
			txtGold->setString(Tools::parseInt2String(DATA->myBaseData.lUserScore));
		txtDiamond->setString(Tools::parseInt2String(DATA->myBaseData.rmb));
		Button   *btnAddGold, *btnAddDiamond, *btnSetting;
		UIGet_Button("Button_addGold", topNode, btnAddGold)
			UIGet_Button("Button_addDiamond", topNode, btnAddDiamond)
			UIGet_Button("Button_setting", topNode, btnSetting)
			btnAddGold->addClickEventListener([&](Ref* psender)
		{
			PLayEffect(EFFECT_BTN)
			creatView(new ShopView(1), new ShopMediator());
		}
		);
		btnAddDiamond->addClickEventListener([&](Ref* psender)
		{
			PLayEffect(EFFECT_BTN)
			creatView(new ShopView(0), new ShopMediator());
		}
		);
		btnSetting->addClickEventListener([&](Ref* psender)
		{
			PLayEffect(EFFECT_BTN)
			creatView(new SetView(), new SetMediator());
		}
		);

	
		//friendlist
		UIGet_Text("Text_friendNow", ndFriends[E_friends], txtFriendsNow)
			UIGet_Text("Text_friendMax", ndFriends[E_friends], txtFriendsMax)
			UIGet_ListView("ListView_friends", ndFriends[E_friends], lstFriendsList)

			//ShuRen
			//search layout
			UIGet_ListView("ListView_shuren", ndFriends[E_familiar], lstShuRen)

			Button    *btnSearch, *btnOnkeyAdd, *btnReFind;
			UIGet_Layout("Panel_search", ndFriends[E_familiar], laySearch)
				UIGet_TextField("TextField_1", laySearch, txfFind)
				UIGet_Button("Button_search", laySearch, btnSearch)
				UIGet_Button("Button_14", laySearch, btnOnkeyAdd)
				UIGet_Button("Button_15", laySearch, btnReFind)

				UIClick(btnSearch, FriendView::clickSearch)
				UIClick(btnOnkeyAdd, FriendView::clickOneKeyAdd)
				UIClick(btnReFind, FriendView::clickRefind)

				//push layout

			Button*  btnRefuse, *btnAgree;
			UIGet_ListView("ListView_push", ndFriends[E_familiar], lstPush)
			UIGet_Layout("Panel_push", ndFriends[E_familiar], layPush)
				UIGet_Button("Button_agree", layPush, btnAgree)
				UIGet_Button("Button_refuse", layPush, btnRefuse)


			UIGet_Node("FileNode_search", ndFriends[E_familiar], ndSearch)
			UIGet_ImageView("Image_head", ndSearch, imgSchHead)
			UIGet_Text("Text_name", ndSearch, txtSchName)
			ndSearch->setVisible(false);
			
			//find one node 
			Button  *btnJoin;
			UIGet_Button("Button_join", ndSearch, btnJoin)
			UIClick(btnJoin, FriendView::clickSearchAdd)


	//rank
	UIGet_CheckBox("CheckBox_1", ndFriends[E_ranks], chkRanks[E_RankCaifu])
	UIGet_CheckBox("CheckBox_2", ndFriends[E_ranks], chkRanks[E_RankMeiLi])
	UIGet_CheckBox("CheckBox_3", ndFriends[E_ranks], chkRanks[E_RankShenglv])
	UIGet_ListView("ListView_rank", ndFriends[E_ranks], lstRank)

	UIClick(chkRanks[E_RankCaifu], FriendView::clickRankCaiFu)
	UIClick(chkRanks[E_RankMeiLi], FriendView::clickRankMeiLi)
	UIClick(chkRanks[E_RankShenglv], FriendView::clickRankShengLv)


	if (DATA->vFriendPush.size() > 0)
	{
		currentTitle = E_familiar;
		handleWhich(currentTitle);
		showPushFriends();
	}
	else
	{
		currentTitle = E_friends;
		handleWhich(currentTitle);
		showFriends();
	}

}

void FriendView::clickSearch(Ref* pSender)
{
	PLayEffect(EFFECT_BTN);
	std::string strId;

#if(PlatWhich != PlatWin)
	strId = txfFind->getString();
#else
	strId = UTF8::getInstance()->getString("friend", "test2");
#endif

	if (strId.empty())
	{
		return;
	}
	BYTE isUid = 1;  //默认uid
	for (int i = 0; i < 10; i ++)
	{
		if (strId.at(i) > '9' || strId.at(i) < '0' )
		{
			isUid = 0; 
			break;
		}
	}
	logV("send search %s", strId.c_str());



	struct sendSearch
	{
		BYTE isUid;
		TCHAR wNameId[32];
		WORD wUid[32];
	};

	sendSearch tagSend;


	tagSend.isUid = isUid;
	if (isUid == 0)  //名字
	{
		memcpy(tagSend.wNameId, strId.c_str(), 64);
		SEND_LOGIN->SendData(6, 10, &tagSend, 97);
	} 
	else  //uid
	{
		memcpy(tagSend.wUid, strId.c_str(), 32);
		SEND_LOGIN->SendData(6, 10, &tagSend, 97);

	}
	lstShuRen->setVisible(false);

}

void FriendView::showFriends()
{
	txtFriendsNow->setString(Tools::parseInt2String(DATA->vFriends.size()));
	int itemHeigth = 99;
	int halfItemWidth = 614 * 0.5;
	lstFriendsList->removeAllChildrenWithCleanup(true);
	lstFriendsList->setItemsMargin(itemHeigth + 10);

	int itemSize = DATA->vFriends.size();
	for (int i = 0; i < itemSize ; i++)
	{
		Node* oneNode = CSLoader::createNode("FriendsCell.csb");
		Layout*  oneLayout = Layout::create();
		oneLayout->addChild(oneNode);
		oneNode->setPosition(Vec2(halfItemWidth, -itemHeigth * 0.5));
 		Text  *txtRmb, *txtName, *txtGame, *txtChangci, *txtFree;
		ImageView*  imgHead;
		Button*  btnDelete;
// 

		UIGet_Text("Text_name", oneNode, txtName)
			UIGet_Text("Text_rmb", oneNode, txtRmb)
			UIGet_Text("Text_game", oneNode, txtGame)
			UIGet_Text("Text_changci", oneNode, txtChangci)
			UIGet_Text("Text_free", oneNode, txtFree)
			UIGet_Button("Button_delete", oneNode, btnDelete)

			UIGet_ImageView("Image_head", oneNode, imgHead)

			btnDelete->setTag(i);
			UIClick(btnDelete, FriendView::clickBtnDelete)

			txtName->setString(DATA->vFriends.at(i).szNickName);
		txtName->setTextHorizontalAlignment(TextHAlignment::LEFT);
		txtRmb->setString(Tools::parseInt2String(DATA->vFriends.at(i).dwRmb));
		WORD wChangci = DATA->vFriends.at(i).wServerID;

		if (wChangci == 0)
		{
			txtChangci->setVisible(false);
			txtGame->setVisible(false);
			txtFree->setVisible(true);
		} 
		else
		{
			txtChangci->setVisible(true);
			txtGame->setVisible(true);   //all guandan now
			txtFree->setVisible(false);
			txtChangci->setString(UTF8::getInstance()->getString("roomName", Tools::parseInt2String(wChangci)));
		}

		char headName[64];
		sprintf(headName, "headshot_%d.png", DATA->vFriends.at(i).FaceID);
		imgHead->loadTexture(headName);


		lstFriendsList->pushBackCustomItem(oneLayout);

	}

	Layout*   layoutFate = Layout::create();
	lstFriendsList->pushBackCustomItem(layoutFate);
}

void FriendView::showShuRen()
{
	refreshShuren();
	layPush->setVisible(false);
	laySearch->setVisible(true);
	ndSearch->setVisible(false);
	lstPush->setVisible(false);
	lstShuRen->setVisible(true);
	lstShuRen->removeAllChildrenWithCleanup(true);

	int itemHeigth = 99;
	int halfItemWidth = 304 * 0.5;

	int itemSize = DATA->vFriendsShuRen.size() > 6 ? 6 : DATA->vFriendsShuRen.size();
	for (int i = 0; i < itemSize / 2 ; i++)
	{
		Node* oneNode = CSLoader::createNode("FriendsShurenCell.csb");
		Node* oneNode2 = CSLoader::createNode("FriendsShurenCell.csb");
		Layout*  oneLayout = Layout::create();
		oneLayout->addChild(oneNode);
		oneLayout->addChild(oneNode2);
		oneNode->setPosition(Vec2(halfItemWidth + 6, -itemHeigth * 0.5));
		oneNode2->setPosition(Vec2(halfItemWidth * 3 + 12 , -itemHeigth * 0.5));

		char headName[64];
		Text   *txtName;
		ImageView*  imgHead;
		Button*   btnAdd;

		UIGet_Text("Text_name", oneNode, txtName)
			UIGet_ImageView("Image_head", oneNode, imgHead)
			UIGet_Button("Button_join", oneNode, btnAdd)
			btnAdd->setTag(i * 2);
		UIClick(btnAdd, FriendView::clickShuRenAdd)
			txtName->setString(DATA->vFriendsShuRen.at(i * 2).szNickName);

		sprintf(headName, "headshot_%d.png", DATA->vFriendsShuRen.at(i * 2).wFaceID);
		imgHead->loadTexture(headName);

		Text   *txtName2;
		ImageView*  imgHead2;
		Button*   btnAdd2;

		UIGet_Text("Text_name", oneNode2, txtName2)
			UIGet_ImageView("Image_head", oneNode2, imgHead2)
			UIGet_Button("Button_join", oneNode2, btnAdd2)
			btnAdd2->setTag(i * 2 + 1);
		UIClick(btnAdd2, FriendView::clickShuRenAdd)
			txtName2->setString(DATA->vFriendsShuRen.at(i * 2 + 1).szNickName);

		sprintf(headName, "headshot_%d.png", DATA->vFriendsShuRen.at(i * 2 + 1).wFaceID);
		imgHead2->loadTexture(headName);


		lstShuRen->pushBackCustomItem(oneLayout);
	}
	for (int i = 0; i < itemSize % 2; i++)
	{
		Node* oneNode = CSLoader::createNode("FriendsShurenCell.csb");
		Layout*  oneLayout = Layout::create();
		oneLayout->addChild(oneNode);
		oneNode->setPosition(Vec2(halfItemWidth, -itemHeigth * 0.5));


		char headName[64];
		Text   *txtName;
		ImageView*  imgHead;
		Button*   btnAdd;

		UIGet_Text("Text_name", oneNode, txtName)
			UIGet_ImageView("Image_head", oneNode, imgHead)
			UIGet_Button("Button_join", oneNode, btnAdd)
			btnAdd->setTag(itemSize - 1);
		UIClick(btnAdd, FriendView::clickShuRenAdd)
			txtName->setString(DATA->vFriendsShuRen.at(itemSize - 1).szNickName);

		sprintf(headName, "headshot_%d.png", DATA->vFriendsShuRen.at(itemSize - 1).wFaceID);
		imgHead->loadTexture(headName);



		lstShuRen->pushBackCustomItem(oneLayout);
	}
	//cocos bug  , add this ok
	Layout*   layoutFate = Layout::create();
	lstShuRen->pushBackCustomItem(layoutFate);

}

void FriendView::showPushFriends()
{
	layPush->setVisible(true);
	laySearch->setVisible(false);
	ndSearch->setVisible(false);
	lstPush->setVisible(true);
	lstShuRen->setVisible(false);
	lstPush->removeAllChildrenWithCleanup(true);

	int itemHeigth = 99;
	int halfItemWidth = 304 * 0.5;
	lstPush->setItemsMargin(itemHeigth + 6);

	int itemSize = DATA->vFriendPush.size();
	for (int i = 0; i < itemSize / 2; i++)
	{
		Node* oneNode = CSLoader::createNode("FriendsPushCell.csb");
		Node* oneNode2 = CSLoader::createNode("FriendsPushCell.csb");
		Layout*  oneLayout = Layout::create();
		oneLayout->addChild(oneNode);
		oneLayout->addChild(oneNode2);
		oneNode->setPosition(Vec2(halfItemWidth + 6, -itemHeigth * 0.5));
		oneNode2->setPosition(Vec2(halfItemWidth * 3 + 12, -itemHeigth * 0.5));

		char headName[64];
		Text   *txtName;
		ImageView*  imgHead;
		Button*   btnAgree, *btnRefuse;

		UIGet_Text("Text_name", oneNode, txtName)
			UIGet_ImageView("Image_head", oneNode, imgHead)
			UIGet_Button("Button_Agree", oneNode, btnAgree)
			UIGet_Button("Button_refuse", oneNode, btnRefuse)
			btnAgree->setTag(i * 2);
			btnRefuse->setTag(i * 2);
			UIClick(btnAgree, FriendView::clickOneAgree)
			UIClick(btnRefuse, FriendView::clickOneRefuse)
			txtName->setString(DATA->vFriendPush.at(i * 2).szNickName);

			sprintf(headName, "headshot_%d.png", DATA->vFriendPush.at(i * 2).wFaceID);
		imgHead->loadTexture(headName);

		Text   *txtName2;
		ImageView*  imgHead2;
		Button*   btnAdd2;

		UIGet_Text("Text_name", oneNode2, txtName2)
			UIGet_ImageView("Image_head", oneNode2, imgHead2)
			UIGet_Button("Button_Agree", oneNode, btnAgree)
			UIGet_Button("Button_refuse", oneNode, btnRefuse)
			btnAgree->setTag(i * 2 + 1);
			btnRefuse->setTag(i * 2 + 1);
			UIClick(btnAgree, FriendView::clickOneAgree)
			UIClick(btnRefuse, FriendView::clickOneRefuse)
			txtName2->setString(DATA->vFriendPush.at(i * 2 + 1).szNickName);

		sprintf(headName, "headshot_%d.png", DATA->vFriendPush.at(i * 2 + 1).wFaceID);
		imgHead2->loadTexture(headName);


		lstPush->pushBackCustomItem(oneLayout);
	}
	for (int i = 0; i < itemSize % 2; i++)
	{
		Button*   btnAgree, *btnRefuse;
		Node* oneNode = CSLoader::createNode("FriendsPushCell.csb");
		Layout*  oneLayout = Layout::create();
		oneLayout->addChild(oneNode);
		oneNode->setPosition(Vec2(halfItemWidth, -itemHeigth * 0.5));


		char headName[64];
		Text   *txtName;
		ImageView*  imgHead;
		Button*   btnAdd;

		UIGet_Text("Text_name", oneNode, txtName)
			UIGet_ImageView("Image_head", oneNode, imgHead)

			UIGet_Button("Button_Agree", oneNode, btnAgree)
			UIGet_Button("Button_refuse", oneNode, btnRefuse)
			btnAgree->setTag(itemSize - 1);
			btnRefuse->setTag(itemSize - 1);
			UIClick(btnAgree, FriendView::clickOneAgree)
			UIClick(btnRefuse, FriendView::clickOneRefuse)
			txtName->setString(DATA->vFriendPush.at(itemSize - 1).szNickName);

		sprintf(headName, "headshot_%d.png", DATA->vFriendPush.at(itemSize - 1).wFaceID);
		imgHead->loadTexture(headName);



		lstPush->pushBackCustomItem(oneLayout);
	}
	Layout*   layoutFate = Layout::create();
	lstPush->pushBackCustomItem(layoutFate);

}

void FriendView::clickShuRenAdd(Ref* pSender)
{
	PLayEffect(EFFECT_BTN);
	Button*   btnAdd = static_cast<Button*>(pSender);
	int i = btnAdd->getTag();
	Tools::getInstance()->showSysMsgTouming (UTF8::getInstance()->getString("friend", "req"));
	SEND->sendFriendOption(DATA->myBaseData.dwUserID, DATA->vFriendsShuRen.at(i).dwUserID, esInvite);
	DATA->vFriendsShuRen.erase(DATA->vFriendsShuRen.begin() + i);
	showShuRen();

}



void FriendView::showRanks(DWORD dwWhich)
{
	lstRank->removeAllChildrenWithCleanup(true);

	switch (dwWhich)
	{
	case E_RankCaifu:
		sort(DATA->vFriends.begin(), DATA->vFriends.end(), greater_caifu);
		break;

	case E_RankMeiLi:
		sort(DATA->vFriends.begin(), DATA->vFriends.end(), greater_meili);
		break;

	case E_RankShenglv:
		sort(DATA->vFriends.begin(), DATA->vFriends.end(), greater_shenglv);
		break;

	default:
		break;
	}

	int itemHeigth = 99;
	int halfItemWidth = 614 * 0.5;
	int itemSize = DATA->vFriends.size();
	lstRank->setItemsMargin(itemHeigth + 6);
	for (int i = 0; i < itemSize; i++)
	{
		Node* oneNode = CSLoader::createNode("FriendsRankCell.csb");
		Layout*  oneLayout = Layout::create();
		oneLayout->addChild(oneNode);
		oneNode->setPosition(Vec2(halfItemWidth, -itemHeigth * 0.5));
		Text  *txtRmb, *txtName, *txtGame, *txtChangci, *txtFree, *txtRank;
		ImageView*  imgHead,  *imgRankIcon;

		UIGet_Text("Text_name", oneNode, txtName)
			UIGet_Text("Text_rmb", oneNode, txtRmb)
			UIGet_Text("Text_game", oneNode, txtGame)
			UIGet_Text("Text_changci", oneNode, txtChangci)
			UIGet_Text("Text_free", oneNode, txtFree)
			UIGet_Text("Text_rank", oneNode, txtRank)
			UIGet_ImageView("Image_head", oneNode, imgHead)
			UIGet_ImageView("Image_rankIcon", oneNode, imgRankIcon)
			txtName->setString(DATA->vFriends.at(i).szNickName);
		txtRank->setString(Tools::parseInt2String(i + 1));
		WORD wChangci = DATA->vFriends.at(i).wServerID;
		if (wChangci == 0)
		{
			txtChangci->setVisible(false);
			txtGame->setVisible(false);
			txtFree->setVisible(true);
		}
		else
		{
			txtChangci->setVisible(true);
			txtGame->setVisible(true);   //all guandan now
			txtFree->setVisible(false);
			txtChangci->setString(UTF8::getInstance()->getString("changci", Tools::parseInt2String(wChangci)));
		}
		char iconName[64];
		char winRates[32];
		switch (dwWhich)
		{
		case E_RankCaifu:
			txtRmb->setString(Tools::parseInt2String(DATA->vFriends.at(i).dwRmb));
			sprintf(iconName, "rank%d.png", dwWhich);
			imgRankIcon->loadTexture(iconName);
			break;

		case E_RankMeiLi:
			txtRmb->setString(Tools::parseInt2String(DATA->vFriends.at(i).dwLoveLiness));
			sprintf(iconName, "rank%d.png", dwWhich);
			imgRankIcon->loadTexture(iconName);
			break;

		case E_RankShenglv:
			sprintf(winRates, "%d.%02d%%", 
				DATA->vFriends.at(i).WinRate / 100, DATA->vFriends.at(i).WinRate % 100);
				txtRmb->setString(winRates);
			sprintf(iconName, "rank%d.png", dwWhich);
			imgRankIcon->loadTexture(iconName);
			break;

		default:
			break;
		}



		char headName[64];
		sprintf(headName, "headshot_%d.png", DATA->vFriends.at(i).FaceID);
		imgHead->loadTexture(headName);


		lstRank->pushBackCustomItem(oneLayout);

	}
	Layout*   layoutFate = Layout::create();
	lstRank->pushBackCustomItem(layoutFate);
}



void FriendView::clickCheckItemFriends(Ref*  pSender, CheckBox::EventType type)
{
	PLayEffect(EFFECT_BTN);
	CheckBox*  chk = static_cast<CheckBox*>(pSender);
	int iwhich = chk->getTag();
	if (iwhich < 0 || iwhich >= 3)
	{
		return;
	}
	handleWhich(iwhich);
	switch (iwhich)
	{
	case 0:
		currentTitle = E_friends;
		showFriends();
		break;
	case 1:
		currentTitle = E_familiar;
		if (DATA->vFriendPush.size() == 0)
		{
			showShuRen();
		}
		else
		{
			showPushFriends();
		}
		break;
	case 2:
		currentTitle = E_ranks;
		showCaiFuRank();
		break;
	default:
		break;
	}
}


void FriendView::clickRankCaiFu(Ref* pSender)
{
	PLayEffect(EFFECT_BTN);
	showCaiFuRank();
}

void FriendView::showCaiFuRank()
{
	for (int i = 0; i < E_RankMax; i++)
	{
		chkRanks[i]->setSelected(false);
		chkRanks[i]->setTouchEnabled(true);
	}
	chkRanks[E_RankCaifu]->setSelected(true);
	chkRanks[E_RankCaifu]->setTouchEnabled(false);
	showRanks(E_RankCaifu);
}

void FriendView::clickRankMeiLi(Ref* pSender)
{
	PLayEffect(EFFECT_BTN);
	for (int i = 0; i < E_RankMax; i++)
	{
		chkRanks[i]->setSelected(false);
		chkRanks[i]->setTouchEnabled(true);
	}
	chkRanks[E_RankMeiLi]->setSelected(true);
	chkRanks[E_RankMeiLi]->setTouchEnabled(false);
	showRanks(E_RankMeiLi);
}

void FriendView::clickRankShengLv(Ref* pSender)
{
	PLayEffect(EFFECT_BTN);
	for (int i = 0; i < E_RankMax; i++)
	{
		chkRanks[i]->setSelected(false);
		chkRanks[i]->setTouchEnabled(true);
	}
	chkRanks[E_RankShenglv]->setSelected(true);
	chkRanks[E_RankShenglv]->setTouchEnabled(false);
	showRanks(E_RankShenglv);
}

void FriendView::handleWhich(int iwhich)
{
	for (int i = 0; i < 3; i++)
	{
		chkFriends[i]->setSelected(false);
		chkFriends[i]->setTouchEnabled(true);
		ndFriends[i]->setVisible(false);
	}
	chkFriends[iwhich]->setSelected(true);
	chkFriends[iwhich]->setTouchEnabled(false);
	ndFriends[iwhich]->setVisible(true);
}

void FriendView::clickBtnDelete(Ref* pSender)
{
	PLayEffect(EFFECT_BTN);
	Button*  btnDelete = static_cast<Button*>(pSender);
	int iWhich = btnDelete->getTag();
	SEND->sendFriendOption(DATA->myBaseData.dwUserID, DATA->vFriends.at(iWhich).dwUserID, esDelete);

	DATA->vFriends.erase(DATA->vFriends.begin() + iWhich);
	showFriends();

}

void FriendView::showSearchResult(WORD wFaceId, std::string szNickName)
{
	for (int i = 0; i < DATA->vFriends.size(); i++)
	{
		if (dwSearchUserId == DATA->vFriends.at(i).dwUserID)
		{
			ndSearch->getChildByName("Button_join")->setVisible(false);
			Tools::getInstance()->showSysMsgTouming(UTF8::getInstance()->getString("friend", "alreadyFriend"));
			break;
		}
	}

	logV("showSearch %s",szNickName.c_str());
	char headName[64];
	ndSearch->setVisible(true);

	sprintf(headName, "headshot_%d.png", wFaceId);
	imgSchHead->loadTexture(headName);
	txtSchName->setString(szNickName);
}

void FriendView::clickRefind(Ref* pSender)
{
	PLayEffect(EFFECT_BTN);
	SEND_LOGIN->SendData(6, 14, NULL, 0);
}

void FriendView::clickOneKeyAdd(Ref* pSender)  //一键添加
{
	PLayEffect(EFFECT_BTN);
	Button*   btnAdd = static_cast<Button*>(pSender);
	int dwsize = DATA->vFriendsShuRen.size() > 6 ? 6 : DATA->vFriendsShuRen.size();

	for (int i = 0; i < dwsize; i ++)
	{
		Tools::getInstance()->showSysMsgTouming(UTF8::getInstance()->getString("friend", "req"));
		SEND->sendFriendOption(DATA->myBaseData.dwUserID, DATA->vFriendsShuRen.at(i).dwUserID, esInvite);
	}
	DATA->vFriendsShuRen.erase(DATA->vFriendsShuRen.begin(), DATA->vFriendsShuRen.begin() + dwsize);
	showShuRen();
}

void FriendView::clickSearchAdd(Ref* pSender)
{
	PLayEffect(EFFECT_BTN);
	SEND->sendFriendOption(DATA->myBaseData.dwUserID, dwSearchUserId, esInvite);
	ndSearch->setVisible(false);
	Tools::getInstance()->showSysMsgTouming(UTF8::getInstance()->getString("friend", "req"));
}


void FriendView::clickOneAgree(Ref* pSender)
{
	PLayEffect(EFFECT_BTN);
	Button*   btnAdd = static_cast<Button*>(pSender);
	int i = btnAdd->getTag();
	logV("clickOneAgree i %d, dwUserId %d,  TargetUserID %d ", i, DATA->myBaseData.dwUserID, DATA->vFriendPush.at(i).dwUserID);

	SEND->sendFriendOption(DATA->myBaseData.dwUserID, DATA->vFriendPush.at(i).dwUserID, esAgree);
	Tools::getInstance()->showSysMsgTouming(UTF8::getInstance()->getString("friend", "agree"));
	logV("#####  i %d, TargetUserID %d ", i, DATA->vFriendPush.at(i).dwUserID);
	DATA->vFriendPush.erase(DATA->vFriendPush.begin() + i);
	showPushFriends();

}
void FriendView::clickOneRefuse(Ref* pSender)
{
	PLayEffect(EFFECT_BTN);
	Button*   btnAdd = static_cast<Button*>(pSender);
	int i = btnAdd->getTag();

	SEND->sendFriendOption(DATA->myBaseData.dwUserID, DATA->vFriendPush.at(i).dwUserID, esRefuse);

	DATA->vFriendPush.erase(DATA->vFriendPush.begin() + i);
	showPushFriends();
	Tools::getInstance()->showSysMsgTouming(UTF8::getInstance()->getString("friend", "refuse"));
}

//6,  5
void FriendView::handleFriendOptMe(void*  data)
{
// 	CMD_GP_C_ADD_Friend* pFriendOpt = (CMD_GP_C_ADD_Friend*)data;
// 	tagFriendParameter* addedFriends;
// 	switch (pFriendOpt->wRcStates)
// 	{
// 	case FriendView::ecInviteSuccess:
// 
// 		break;
// 
// 	case FriendView::ecInviteFail:
// 
// 		break;
// 	case FriendView::ecInviteAlready:
// 
// 		break;
// 	case FriendView::ecAgreeSuccess: //同意添加对方为好友
// 		addedFriends = new tagFriendParameter();
// 		addedFriends->dwUserID     = pFriendOpt->dwUserID;
// 		addedFriends->szNickName   = pFriendOpt->szNickName;
// 		addedFriends->dwRmb        = pFriendOpt->dwRmb;
// 		addedFriends->FaceID       = pFriendOpt->FaceID;
// 		addedFriends->wServerID    = pFriendOpt->wServerID;
// 		addedFriends->wKindID      = pFriendOpt->wKindID;
// 		addedFriends->dwLoveLiness = pFriendOpt->dwLoveLiness;
// 		addedFriends->WinRate      = pFriendOpt->WinRate;
// 
// 		DATA->vFriends.push_back(*addedFriends);
// 
// 		break;
// 	case FriendView::ecAgreeFail:
// 
// 		break;
// 	case FriendView::ecAgreeAlready:
// 
// 		break;
// 	case FriendView::ecRefuseSuccess:
// 
// 		break;
// 	case FriendView::ecRefuseFail:
// 
// 		break;
// 	case FriendView::ecRefuseNot:
// 
// 		break;
// 	case FriendView::ecDeleteSuccess:
// 
// 		break;
// 
// 	case FriendView::ecDeleteFail:
// 
// 		break;
// 
// 	case FriendView::ecDeleteNot:
// 
// 		break;
// 
// 	}

}

void FriendView::handleFriendOptHim(void*  data)
{
	CMD_GP_C_ADD_Friend* pFriendOpt = (CMD_GP_C_ADD_Friend*)data;
	std::string strName;
	std::string strAgree;
	tagInviteInfo*  pInvite;
	logV("handleHim States%d", pFriendOpt->wRcStates);
	switch (pFriendOpt->wRcStates)
	{
	case FriendView::ecInviteSuccess:   //对方要加我
		if (VIEW->nowViewTag == ViewManager::eViewFriend 
			&& currentTitle == E_familiar)   //当前正在好友推送界面
		{
			showPushFriends();
		}
		break;

	case FriendView::ecAgreeFail:

		break;
	case FriendView::ecAgreeAlready:

		break;
	case FriendView::ecRefuseSuccess:

		break;
	case FriendView::ecRefuseFail:

		break;
	case FriendView::ecRefuseNot:

		break;
	case FriendView::ecDeleteSuccess:

		break;

	case FriendView::ecDeleteFail:

		break;

	case FriendView::ecDeleteNot:

		break;

	}

}