#ifndef  _LoginView_
#define  _LoginView_



#include "cocos2d.h"
#include "BlueSky.h"
#include "cocostudio/CocoStudio.h"
class  LoginView : public BlueSkyView
{
public:
	LoginView();
	~LoginView();
	cocostudio::timeline::ActionTimeline* cloudAction;
	Node* cloudNode;



private:
	void runLogo();

};
#endif
