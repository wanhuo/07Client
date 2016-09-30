#ifndef  _MATCHView_
#define  _MATCHView_

#include "cocos2d.h"
#include "BlueSky.h"
#include "cocostudio/CocoStudio.h"

class  MatchView : public BlueSkyView
{
public:
	MatchView();
	~MatchView();
	void initView();

	BTN_TOUCH_HANDLE(Button, closeBtn, 16000);
	BTN_TOUCH_HANDLE(ImageView, Image_bg, 16000);
	void gotoMatch(Ref*  psender);
	void createFastMatch();

	void initFastView();  //满人开赛
	void initNewView();
	void initFeeView();

	void updateFastView(); //满人开赛


public:


	Button*  btnGoto[3];

	//fastview
	Node* matchFastNode;
	Button*  btnCansai;
	Button*  btnTuisai;
	ProgressTimer*  timerPeople;
	Text*   txtNowPeople;
	Text*   txtNeedPeople;	
	
	Node*   nodeGoldInfo;
	Text* txtDiamond;  //钻石
	Text* txtGold;     //金币

	void clickCansai(Ref*  psender);
	void clickTuisai(Ref*  psender);
	void clickBtnCloseFast(Ref* pSender);

};

#endif
