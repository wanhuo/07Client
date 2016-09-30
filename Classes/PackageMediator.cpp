#include "PackageMediator.h"

PackageMediator::PackageMediator()
{

}

PackageMediator::~PackageMediator()
{
	delete getView();
	setView(NULL);
}

/**
��ʼִ�к���
*/
void PackageMediator::OnRegister()
{
	packageView = (PackageView*)getView();

	PokerGameModel* pokerGameModel = ((PokerGameModel*)getModel(PokerGameModel::NAME));
	packageView->packageItem = pokerGameModel->packageItem;

	packageView->initView();
}

/**
��������ִ�к���
*/
void PackageMediator::onRemove()
{

}

/*
�¼���Ӧ����
*/
void PackageMediator::onEvent(int i, void* data)
{
	int index = -1;
	switch (i)
	{
	case 12001:
		removeView(this);
		break;
	case 12002:
		clickUseBtnHander();
		break;
	case 12003:
		index = *(int*)data;
		nowIndex = index;
		packageView->showItemInfo(index);
		break;
	}
}

Layer* PackageMediator::getLayer()
{
	return ((UILayerService*)getService(UILayerService::NAME))->uiLayer;
}

void PackageMediator::clickUseBtnHander()
{
	//;
	//unsigned long dwUserID = DATA->myBaseData.dwUserID;

	////ʹ�õ��ǵ�nowIndex����Ʒ
	//((SendDataService*)getService(SendDataService::NAME))->sendUseItem(dwUserID, packageView->packageItem.at(nowIndex).dwPropID, 1, dwUserID, 1);
}