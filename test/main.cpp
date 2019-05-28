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
	LONG userid = sdk.Init(login,devinfo);
	if (userid< 0)
	{
		printf("Init failed,ret = %d\n",userid);
		return ret;
	}
	printf("userid = %d\n",userid);
	NET_DVR_TIME startTime;
	startTime.dwYear = 2019;
	startTime.dwMonth = 5;
	startTime.dwDay = 28;
	startTime.dwHour = 10;
	startTime.dwMinute = 0;
	startTime.dwSecond = 0;
	
	NET_DVR_TIME stopTime;
	stopTime.dwYear = 2019;
	stopTime.dwMonth = 5;
	stopTime.dwDay = 28;
	stopTime.dwHour =10;
	stopTime.dwMinute = 0;
	stopTime.dwSecond =20;
	LONG chanl = 36;
	char* fileName = "./test36.mp4";
#define SAVEFILE_BY_TIME
//#define PLAYBACK_BY_NAME

#ifdef SAVEFILE_BY_TIME
	ret = sdk.SaveFileByTime(userid,startTime,stopTime,chanl,fileName);
	if (ret !=0)
	{
		printf("SavefileByTime failed,ret = %d\n",ret);
		return ret;
	}
#endif

#ifdef PLAYBACK_BY_TIME
	ret = sdk.PlayBackByTime(userid,startTime,stopTime,chanl,PlayBackCallback);
	if (ret!=0)
	{
		printf("Play back by time failed,err = %d\n",ret);
		return ret;
	}
#endif

	NET_DVR_FIND_DATA file_data;
	ret = sdk.FindFile(userid,startTime,stopTime,chanl,file_data);
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

#ifdef SAVEFILE_BY_NAME
	ret = sdk.SaveFileByName(userid,file_data.sFileName,"./peng.mp4");
	if (ret !=0)
	{
		printf("@@@@ savefile by name failed,err = %d\n",ret);
	}
#endif

#ifdef PLAYBACK_BY_NAME
	ret = sdk.PlayBackByName(userid,file_data.sFileName,PlayBackCallback);
	if (ret!=0)
	{
		printf("@@@@ play back by name failed,err = %d\n",ret);
	}
	sleep(10);
#endif

	ret = sdk.Destory(userid);
	if (ret!= 0)
	{
		printf("destroy failed,err = %d\n",ret);
		return ret;
	}

	return 0;
}
