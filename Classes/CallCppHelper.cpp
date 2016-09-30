
#include "UTF8.h"
#include "CallCppHelper.h"
#include "SocketInitCommand.h"
#include "BlueSky.h"
#include "EventType.h"
#include "SGTools.h"
#include "TCPSocketService.h"



#if(SDKWhich == SDK_YIKE)
#include "platform/android/jni/JniHelper.h"
#endif


static CallCppHelper *_instance = NULL;


CallCppHelper *  CallCppHelper::getInstance()
{
	if (_instance == NULL)  
	{
		_instance = new CallCppHelper();
		log("cocos2d-x %s", FileUtils::getInstance()->getWritablePath().c_str());

#if(SDKWhich == SDK_NULL)
		//userId���ļ�
		if (UserDefault::getInstance()->getBoolForKey("first_time", true))
		{
			//����������״μ��ز���
			UserDefault::getInstance()->setBoolForKey("first_time", false);
			time_t tt;
			time(&tt);
			struct tm *tm;
			tm = localtime(&tt);
			int year = tm->tm_year + 1900;
			int month = tm->tm_mon + 1;
			int day = tm->tm_mday;
			int hour = tm->tm_hour;
			int min = tm->tm_min;
			int sec = tm->tm_sec;
			_instance->randomNum = int(rand_0_1() * 10000);
			sprintf(_instance->mUid, "%02d%02d%02d%04d",
				month, day, hour,_instance->randomNum);
			sprintf(_instance->mTokenId, "%04d%02d%02d%02d%02d%02d%04d",
				year, month, day, hour, min, sec, _instance->randomNum);
			UserDefault::getInstance()->setStringForKey("userId", _instance->mUid);
			UserDefault::getInstance()->setStringForKey("TokenId", _instance->mTokenId);
		}
		else
		{
			std::string strUid;
			std::string strTokenId;
			strUid = UserDefault::getInstance()->getStringForKey("userId", "12345678901");
			strTokenId = UserDefault::getInstance()->getStringForKey("TokenId", "1234567891");

			memcpy(_instance->mUid, strUid.c_str(), strUid.length() + 1);
			memcpy(_instance->mTokenId, strTokenId.c_str(), strTokenId.length() + 1);
		}
#endif

	}
	return _instance;
};

#if(SDKWhich == SDK_YIKE)
void CallCppHelper::callJavaPayment(int goodsId, int orderNum, int gold, int pay_amount)
{
	//gettimeofday()

	JniMethodInfo info;
	bool ret = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "payHandle", "(IIII)V");
	if (ret)
	{
		log("cocos2d-x callJava payment success in callJavaPayment\n");
		info.env->CallStaticVoidMethod(info.classID, info.methodID, goodsId, orderNum, gold, pay_amount);
	}

}
#endif

void CallCppHelper::sendLoginData()
{
	log("cocos2dx sendLoginData");
	registLogin *rLogin = new registLogin();
	rLogin->type = 1;
	rLogin->szAccounts = "adsfljliijk";
	rLogin->szLogonPass = "123543444";
	blueSkyDispatchEvent(EventType::SOCKET_INIT, rLogin);	
}


void CallCppHelper::sendOrderNum()
{
	log("cocos2d-x sendOrderNum %s", mOrderNum);
	unsigned char cbBuffer[SOCKET_TCP_PACKET];
	//1, 6,  ��ֵ��Ϣ��
	memcpy(cbBuffer, mOrderNum, 32);
	((TCPSocketService *)getService(TCPSocketService::LOGIN))->SendData(1, 6, cbBuffer, 32);
	log("cocos2d-x sendOrderNum seccess after!\n");
}


void CallCppHelper::PostToken(char* uid, char* access_token)
{
	HttpRequest* request = new (std::nothrow) HttpRequest();
	request->setUrl(UTF8::getInstance()->getString("net", "webUrl").c_str());    //web ������
	request->setRequestType(HttpRequest::Type::POST);
	std::vector<std::string> headers;
	request->setHeaders(headers);
	request->setResponseCallback(CC_CALLBACK_2(CallCppHelper::onHttpRequestCompleted, this));

	


	char postData[256];
	sprintf(postData, "uid=%s&access_token=%s", uid, access_token);


	request->setRequestData(postData, strlen(postData));
	request->setTag("POST test2");
	HttpClient::getInstance()->send(request);
	request->release();
}



void CallCppHelper::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{
	if (!response)
	{
		return;
	}

	if (!response->isSucceed())
	{
		return;
	}
}
#if(SDKWhich == SDK_YIKE)
char* CallCppHelper::jstringTostring(JNIEnv* env, jstring jstr)
{
	char* rtn = NULL;
	jclass clsstring = env->FindClass("java/lang/String");
	jstring strencode = env->NewStringUTF("utf-8");
	jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
	jbyteArray barr = (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
	jsize alen = env->GetArrayLength(barr);
	jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
	if (alen > 0)
	{
		rtn = (char*)malloc(alen + 1);
		memcpy(rtn, ba, alen);
		rtn[alen] = 0;
	}
	env->ReleaseByteArrayElements(barr, ba, 0);
	return rtn;
}
#endif



