#include<string.h>
#include<unistd.h>
#include "HIK_SDK.h"

void PlayBackCallback(LONG lPlayHandle,DWORD dwDataType,BYTE  *pBuffer,DWORD  dwBufSize,DWORD  dwUser)
{
	if (dwBufSize > 0)
	{
		printf("dataType = %d,datasize = %d\n",dwDataType,dwBufSize);
	}else
	{
		printf("datasize = 0\n");
	}
}
int main()
{
	int ret =0;
	NET_DVR_USER_LOGIN_INFO login;
	strcpy(login.sDeviceAddress,"10.58.123.42");
	strcpy(login.sUserName,"admin");
	strcpy(login.sPassword,"abc123456");
	login.wPort = 8000;
	login.bUseAsynLogin = FALSE;
	
	HIK_SDK sdk;

	NET_DVR_DEVICEINFO_V40 devinfo = {0};
	long user  = sdk.Init(login,devinfo);
	
	if (ret!= 0)
	{
		printf("Init failed,ret = %d\n",ret);
		return ret;
	}else{
		printf("init success");
	}

	NET_DVR_TIME startTime;
	startTime.dwYear = 2019;
	startTime.dwMonth = 7;
	startTime.dwDay = 7;
	startTime.dwHour = 10;
	startTime.dwMinute = 0;
	startTime.dwSecond = 0;
	
	NET_DVR_TIME stopTime;
	stopTime.dwYear = 2019;
	stopTime.dwMonth = 7;
	stopTime.dwDay = 7;
	stopTime.dwHour =10;
	stopTime.dwMinute = 0;
	stopTime.dwSecond =10;
	LONG chanl = 33;
	char* fileName = "./test123.flv";
#define SAVEFILE_BY_TIME
//#define PLAYBACK_BY_NAME

#ifdef SAVEFILE_BY_TIME
	ret = sdk.SaveFileByTime(user,startTime,stopTime,chanl,fileName);
	if (ret !=0)
	{
		printf("SavefileByTime failed,ret = %d\n",ret);
		return ret;
	}else{
		printf("SaveFileByName Success!");
		}
#endif

#ifdef PLAYBACK_BY_TIME
	ret = sdk.PlayBackByTime(user,startTime,stopTime,chanl,PlayBackCallback);
	if (ret!=0)
	{
		printf("Play back by time failed,err = %d\n",ret);
		return ret;
	}
#endif

#if 0
	NET_DVR_FIND_DATA file_data;
	ret = sdk.FindFile(user,startTime,stopTime,chanl,file_data);
	if (ret != 0)
	{
		printf("@@@ Find file filed\n");
	}else{
		printf("@@@ find file success\n");
		if (file_data.dwFileSize>0)
		{
			printf("filename = %s, filesize = %d\n",file_data.sFileName,file_data.dwFileSize);
		}
	}
#endif

#ifdef SAVEFILE_BY_NAME
	ret = sdk.SaveFileByName(user,file_data.sFileName,"./peng.mp4");
	if (ret !=0)
	{
		printf("@@@@ savefile by name failed,err = %d\n",ret);
	}
#endif

#ifdef PLAYBACK_BY_NAME
	ret = sdk.PlayBackByName(user,file_data.sFileName,PlayBackCallback);
	if (ret!=0)
	{
		printf("@@@@ play back by name failed,err = %d\n",ret);
	}
	sleep(10);
#endif

	ret = sdk.Destory(user);
	if (ret!= 0)
	{
		printf("destroy failed,err = %d\n",ret);
		return ret;
	}

	return 0;
}
