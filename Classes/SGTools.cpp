#include "SGTools.h"
#include "PlatFormControl.h"

static Tools *_instance = NULL;

Tools *  Tools::getInstance()
{
	if (_instance == NULL)
	{
		_instance = new Tools();
		_instance->initTools();
	}
	return _instance;
};

void Tools::initTools()
{

}

void Tools::showSysMsg(std::string msg, std::function<void(Ref*)> callFunc)
{
	Node* rootNode = CSLoader::createNode("systemMsg.csb");
	rootNode->setPosition(WScreen * 0.5, HScreen * 0.5);
	Text*   txtMsg;
	Button* btnClose, *btnEnter;
	ImageView* imgClose;

	UIGet_Text("msgText", rootNode, txtMsg)
		UIGet_ImageView("Image_38", rootNode, imgClose)
		UIGet_Button("Button_close", rootNode, btnClose)
		UIGet_Button("Button_1", rootNode, btnEnter)
		imgClose->addClickEventListener(callFunc);
		btnClose->addClickEventListener(callFunc);
		btnEnter->addClickEventListener(callFunc);
 //		UIClick(imgClose, Tools::closeSysMsg)
// 		UIClick(btnClose, Tools::closeSysMsg)
		txtMsg->setString(msg);
	getcontainer()->addChild(rootNode, 40000);
}
void Tools::closeSysMsgTouming(Node*  psender)
{
	psender->removeFromParentAndCleanup(true);
}

void Tools::showSysMsgTouming(std::string msg, float x, float y, Color3B color, bool isShowBg)
{
	Node* rootNode = CSLoader::createNode("sysMsgTouming.csb");
	Vec2 basePos = Vec2(WScreen * 0.5, HScreen * 0.5);
	basePos.add(Vec2(x, y));
	rootNode->setPosition(basePos);
	Text*   txtMsg;
	ImageView*  imgBg;
	UIGet_ImageView("Image_38", rootNode, imgBg)
	if (!isShowBg)
	{
		imgBg->setVisible(false);
	}
	UIGet_Text("msgText", rootNode, txtMsg)
		txtMsg->setString(msg);
	txtMsg->setColor(color);
	getcontainer()->addChild(rootNode, 40000);
	rootNode->runAction(Sequence::create(
		//MoveBy::create(0.7f, Vec2(0, 10)),
		DelayTime::create(0.5f),
		FadeOut::create(0.5f),
		CallFuncN::create(CC_CALLBACK_1(Tools::closeSysMsgTouming , this)),
		NULL

		));

}

void Tools::showSysMsgLogin(std::string msg, float x, float y, Color3B color)
{
	Node* rootNode = CSLoader::createNode("sysMsgTouming.csb");
	Vec2 basePos = Vec2(WScreen * 0.5, HScreen * 0.5);
	basePos.add(Vec2(x, y));
	rootNode->setPosition(basePos);
	Text*   txtMsg;
	UIGet_Text("msgText", rootNode, txtMsg)
		ImageView* img38;
	UIGet_ImageView("Image_38", rootNode, img38)
		txtMsg->setFontSize(18);
		//img38->setScaleY(0.8);
		txtMsg->setString(msg);
	txtMsg->setColor(color);
	getcontainer()->addChild(rootNode, 40000);
	rootNode->runAction(Sequence::create(
		MoveBy::create(0.4f, Vec2(0, -20)),
		DelayTime::create(1.5f),
		FadeOut::create(1.0f),
		CallFuncN::create(CC_CALLBACK_1(Tools::closeSysMsgTouming, this)),
		NULL

		));

}

void Tools::closeSysMsg(Ref*  psender)
{
	Node*  img = static_cast<Node*>(psender);
	img->getParent()->removeFromParentAndCleanup(true);
}


void Tools::shakeWnd(Node* layout)
{
	EaseBackOut* moveByStart = EaseBackOut::create(MoveBy::create(0.05f,  Vec2(0, 10)));
	EaseBackOut* moveBy2 = EaseBackOut::create(MoveBy::create(0.05f, Vec2(0, -10)));
	EaseBackOut* moveBy3 = EaseBackOut::create(MoveBy::create(0.05f, Vec2(0, 10)));
	EaseBackOut* moveByEnd = EaseBackOut::create(MoveBy::create(0.02f, Vec2(0, 0)));
	FiniteTimeAction* seAction = Sequence::create(moveByStart, moveBy2, moveBy3, moveBy2, moveBy3,moveBy2, moveBy3, moveByEnd, nullptr);
	layout->runAction(seAction);
}


string Tools::parseInt2String(int param)
{
	char buffer[64] = "";
	sprintf(buffer, "%d", param);
	return buffer;
}

string Tools::parseLL2String(long long param)
{
	char buffer[64] = "";
	sprintf(buffer, "%lld", param);
	return buffer;
}

int Tools::parseString2Int(string& param)
{
	int ret = atoi(param.c_str());
	return ret;
}

void Tools::split(const string& src, const string& separator, vector<string>& dest)
{
	string str = src;
	string substring;
	string::size_type start = 0, index;

	do
	{
		index = str.find_first_of(separator,start);
		if (index != string::npos)
		{    
			substring = str.substr(start,index-start);
			dest.push_back(substring);
			start = str.find_first_not_of(separator,index);
			if (start == string::npos) return;
		}
	}while(index != string::npos);

	//the last token
	substring = str.substr(start);
	dest.push_back(substring);
}

int Tools::createRandom(int start, int end)
{
    int randomValue = (int)(CCRANDOM_0_1() * end + start);
    return randomValue;
    
}

bool Tools::isDayOrLight()
{
    time_t lt = time(NULL);
    struct tm * ptm = localtime(&lt);
    int hours =  ptm->tm_hour;
    if (hours < 6 || hours >= 18) 
    {
        return false;
    }
    else
    {
        return true;
    }
}


string Tools::getFileByName(string pFileName)
{
   string path = FileUtils::getInstance()->getWritablePath() + pFileName;
   FILE* file = fopen(path.c_str(), "r");
   if (file) 
   {
       char* buf;  
	   int len;    
       fseek(file, 0, SEEK_END);  
       len = ftell(file);         
       rewind(file);              
       buf = (char*)malloc(sizeof(char) * len + 1);
       if (!buf) 
	   {
           return NULL;
       }
       int rLen = fread(buf, sizeof(char), len, file);
       buf[rLen] = '\0';
       string result = buf;
       fclose(file);
       free(buf);
       return result;
   }
   return NULL;
}


//    FILE* file = fopen(path.c_str(), "a+");  
//    if (file) 
//    {
//        fputs(pContent, file);
//        fclose(file);
//    }


void Tools::logFile(const char * lpFormat, ...)
{

	time_t tt;
	time(&tt);
	struct tm *tm;
	tm = localtime(&tt);
	int month = tm->tm_mon + 1;
	int day = tm->tm_mday;
	int hour = tm->tm_hour;
	int min = tm->tm_min;
	int sec = tm->tm_sec;
	char  fileName[128];
	char  dataName[64];
	sprintf(fileName, "%02d%02d%", month, day);
	sprintf(dataName, "\n[ %02d:%02d:%02d ]    ", hour, min, sec);


	auto fileUtils = cocos2d::FileUtils::getInstance();
	
	string path;
#if(PlatWhich == PlatAdr) //Android
	{
		path = "/mnt/sdcard/guandan/";
		if (!fileUtils->isDirectoryExist(path))
		{
			fileUtils->createDirectory(path);
		}
		path = path + fileName;
	}
#else
	{
		path = FileUtils::getInstance()->getWritablePath() + fileName;
	}
#endif

	FILE* m_File = fopen(path.c_str(), "a+");

	if (m_File)
	{
		fputs(dataName, m_File);
		va_list args;
		va_start(args, lpFormat);
		vfprintf(m_File, lpFormat, args);
		va_end(args);
		fflush(m_File);

		fclose(m_File);
	}
	return;

}

void Tools::getCurrentTime(char*  times)
{
	time_t nowTime = time(NULL);
	struct tm *tm = localtime(&nowTime);
	sprintf(times, "[%02d:%02d:%02d] ", tm->tm_hour, tm->tm_min, tm->tm_sec);
}

double Tools::getMillSecond()
{
	struct timeval tv;
	gettimeofday(&tv, nullptr);

	log("CurrentTime MillSecond %f", (double)tv.tv_sec * 1000 + (double)tv.tv_usec / 1000);

	return (double)tv.tv_sec * 1000 + (double)tv.tv_usec / 1000;
}



