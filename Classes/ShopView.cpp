#include "ShopView.h"
#include "DataManager.h"
#include "BlueSky.h"
#include "EventType.h"
#include "GameDataModel.h"

ShopView::ShopView(int id)
{
	rootNode = CSLoader::createNode("shop.csb");
	addChild(rootNode);
	//rootNode->setScale(0.1f, 0.1f);
	//rootNode->runAction(Sequence::create(ScaleTo::create(0.2f, 1.03f), ScaleTo::create(0.1f, 1.0f), nullptr));
	shopID = id;
	BTN_ADD_TOUCH_EVENTLISTENER(CheckBox, ShopView, zhuanShiBtn, 10801, "zhuanShiBtn", NULL)
	BTN_ADD_TOUCH_EVENTLISTENER(CheckBox, ShopView, goldBtn, 10802, "goldBtn", NULL)
	BTN_ADD_TOUCH_EVENTLISTENER(Button, ShopView, closeBtn, 10803, "closeBtn", NULL)

	UIGet_Node("FileNode_zuanshi", rootNode, mallItem[E_zuanshi])
	UIGet_Node("FileNode_jinbi", rootNode, mallItem[E_gold])
	currentTitle = id;

	Node*  topNode;
	UIGet_Node("FileNode_1", rootNode, topNode)
		UIGet_Text("Text_gold", topNode, txtGold)
		UIGet_Text("Text_diamond", topNode, txtDiamond)

	Button* btnZuanTejia = static_cast<Button*>(mallItem[E_zuanshi]->getChildByName("Button_zhuanshi"));
	btnZuanTejia->addClickEventListener([this](Ref* psender)
	{
		SimpleAudioEngine::getInstance()->playEffect("sounds/game_button_click.mp3");
		blueSkyDispatchEvent(EventType::ALERT, new AlertVO(1, "warning", "warningBuy", 10804, -1));
	}
	);

	ScrollView*  scrZuanshi, *scrGold;
	UIGet_ScrollView("ScrollView_1", mallItem[E_zuanshi], scrZuanshi)
		UIGet_ScrollView("ScrollView_1", mallItem[E_gold], scrGold)
		Button* btnZuanTejia1 = static_cast<Button*>(scrZuanshi->getChildByName("Button_1"));
	btnZuanTejia1->addClickEventListener([this](Ref* psender)
	{
		SimpleAudioEngine::getInstance()->playEffect("sounds/game_button_click.mp3");
		blueSkyDispatchEvent(EventType::ALERT, new AlertVO(1, "warning", "warningBuy", 10805, -1));
	}
	);

	Button* btnZuanTejia2 = static_cast<Button*>(scrZuanshi->getChildByName("Button_1_0"));
	btnZuanTejia2->addClickEventListener([this](Ref* psender)
	{
		SimpleAudioEngine::getInstance()->playEffect("sounds/game_button_click.mp3");
		blueSkyDispatchEvent(EventType::ALERT, new AlertVO(1, "warning", "warningBuy", 10806, -1));
	}
	);

	Button* btnZuanTejia3 = static_cast<Button*>(scrZuanshi->getChildByName("Button_1_2"));
	btnZuanTejia3->addClickEventListener([this](Ref* psender)
	{
		SimpleAudioEngine::getInstance()->playEffect("sounds/game_button_click.mp3");
		blueSkyDispatchEvent(EventType::ALERT, new AlertVO(1, "warning", "warningBuy", 10807, -1));
	}
	);

	Button* btnZuanTejia4 = static_cast<Button*>(scrZuanshi->getChildByName("Button_1_2_0"));
	btnZuanTejia4->addClickEventListener([this](Ref* psender)
	{
		SimpleAudioEngine::getInstance()->playEffect("sounds/game_button_click.mp3");
		blueSkyDispatchEvent(EventType::ALERT, new AlertVO(1, "warning", "warningBuy", 10808, -1));
	}
	);

	//jinbi
	Button* tnZuanTejia = static_cast<Button*>(mallItem[E_gold]->getChildByName("Button_zhuanshi"));
	tnZuanTejia->addClickEventListener([this](Ref* psender)
	{
		SimpleAudioEngine::getInstance()->playEffect("sounds/game_button_click.mp3");
		blueSkyDispatchEvent(EventType::ALERT, new AlertVO(1, "warning", "warningBuy", 10809, -1));
	}
	);

	Button* tnZuanTejia1 = static_cast<Button*>(scrGold->getChildByName("Button_1"));
	tnZuanTejia1->addClickEventListener([this](Ref* psender)
	{
		SimpleAudioEngine::getInstance()->playEffect("sounds/game_button_click.mp3");
		blueSkyDispatchEvent(EventType::ALERT, new AlertVO(1, "warning", "warningBuy", 10810, -1));
	}
	);

	Button* tnZuanTejia2 = static_cast<Button*>(scrGold->getChildByName("Button_1_0"));
	tnZuanTejia2->addClickEventListener([this](Ref* psender)
	{
		SimpleAudioEngine::getInstance()->playEffect("sounds/game_button_click.mp3");
		blueSkyDispatchEvent(EventType::ALERT, new AlertVO(1, "warning", "warningBuy", 10811, -1));
	}
	);

	Button* tnZuanTejia3 = static_cast<Button*>(scrGold->getChildByName("Button_1_2"));
	tnZuanTejia3->addClickEventListener([this](Ref* psender)
	{
		SimpleAudioEngine::getInstance()->playEffect("sounds/game_button_click.mp3");
		blueSkyDispatchEvent(EventType::ALERT, new AlertVO(1, "warning", "warningBuy", 10812, -1));
	}
	);

	Button* tnZuanTejia4 = static_cast<Button*>(scrGold->getChildByName("Button_1_2_0"));
	tnZuanTejia4->addClickEventListener([this](Ref* psender)
	{
		SimpleAudioEngine::getInstance()->playEffect("sounds/game_button_click.mp3");
		blueSkyDispatchEvent(EventType::ALERT, new AlertVO(1, "warning", "warningBuy", 10813, -1));
	}
	);

}

void ShopView::initView()
{
	switch (shopID)
	{

	case 0:
		showZuanShiView();
		break;
	case 1:
		showGoldView();
		break;

	default:
		break;
	}
}

void ShopView::showMyZuanShi(int num)
{
	txtDiamond->setString(Tools::parseInt2String(num));
}
void ShopView::showMyGold(int num)
{
	txtGold->setString(Tools::parseInt2String(num));
}

ShopView::~ShopView()
{
	BTN_REMOVE_TOUCH_EVENTLISTENER(ShopView, zhuanShiBtn, 10801);
	BTN_REMOVE_TOUCH_EVENTLISTENER(ShopView, goldBtn, 10802);
	BTN_REMOVE_TOUCH_EVENTLISTENER(ShopView, closeBtn, 10803);

	delete rootNode;
	rootNode = NULL;

}

void ShopView::showZuanShiView()
{
	_zhuanShiBtn->setSelected(true);
	_goldBtn->setSelected(false);
	_zhuanShiBtn->setTouchEnabled(false);
	_goldBtn->setTouchEnabled(true);
	mallItem[E_zuanshi]->setVisible(true);
	mallItem[E_gold]->setVisible(false);


}

void ShopView::showGoldView()
{
	_zhuanShiBtn->setSelected(false);
	_goldBtn->setSelected(true);
	_zhuanShiBtn->setTouchEnabled(true);
	_goldBtn->setTouchEnabled(false);
	mallItem[E_zuanshi]->setVisible(false);
	mallItem[E_gold]->setVisible(true);
}
