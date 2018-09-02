#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <wiringPi.h>

//rasp GPIO
#define GPIO_17		0	//Pin - wiringPi pin 0 is BCM_GPIO 17
#define GPIO_18		1	//Pin - wiringPi pin 1 is BCM_GPIO 18 for PWM
#define GPIO_27		2	//Pin - wiringPi pin 2 is BCM_GPIO 27
#define GPIO_22		3	//Pin - wiringPi pin 3 is BCM_GPIO 22
#define GPIO_23		4	//Pin - wiringPi pin 4 is BCM_GPIO 23
#define GPIO_24		5	//Pin - wiringPi pin 5 is BCM_GPIO 24
#define GPIO_25		6	//Pin - wiringPi pin 6 is BCM_GPIO 25
#define GPIO_04		7	//Pin - wiringPi pin 7 is BCM_GPIO 04

#define FILE_LED_STAUS      "/tmp/tactsw-led.tmp"
#define FILE_BEEP_STAUS     "/tmp/beep.tmp"
#define FILE_ALM_STAUS      "/tmp/alm.tmp"

//int nGPIO_Led[4] ={GPIO_17,GPIO_18,GPIO_22,GPIO_23};
int nGPIO_Led[4] ={GPIO_23,GPIO_22,GPIO_18,GPIO_17};


//プロトタイプ宣言
int FileWiteStatus(char *fname,int nNo);
int FileReadStatus(char *fname);
int SetStatusLED(int *npLedNo,int nMode);
/***********************************************************************************
*	タクトスイッチによる４状態を作る(LED表示あり)
***********************************************************************************/
int main(int argc, char *argv[])
{

    int nTmp,nTactSwitch=0,nLedNo=0,nBeep,nAlm,nLoopCnt=0;
	int nDailyInStatus = 0,	nDailyOutStatus = 0;

	time_t now;
	struct tm *pLocalNow;


    if(FileWiteStatus(FILE_LED_STAUS,nLedNo) == -1)return -1;       //LED状態をファイルにファイルに書き込み　初期値
    if(FileWiteStatus(FILE_BEEP_STAUS,0) == -1)return -1;           //BEEP状態をファイルにファイルに書き込み　初期値
    if(FileWiteStatus(FILE_ALM_STAUS,0) == -1)return -1;            //ALM状態をファイルにファイルに書き込み　初期値

	wiringPiSetup();
	
	pinMode(GPIO_04, OUTPUT);
	digitalWrite(GPIO_04, HIGH);    //ALM(発生)

	pinMode(GPIO_27, OUTPUT);
	digitalWrite(GPIO_27, LOW);    //OK

	pinMode(GPIO_24, INPUT);

	pinMode(nGPIO_Led[0], OUTPUT);
	digitalWrite(nGPIO_Led[nLedNo], HIGH);

	pinMode(nGPIO_Led[1], OUTPUT);
	digitalWrite(nGPIO_Led[1], LOW);

	pinMode(nGPIO_Led[2], OUTPUT);
	digitalWrite(nGPIO_Led[2], LOW);

	pinMode(nGPIO_Led[3], OUTPUT);
	digitalWrite(nGPIO_Led[3], LOW);

	pinMode(GPIO_25, OUTPUT);
	digitalWrite(GPIO_25, HIGH);    //beep



	for (;;) {
	    usleep(100000);    //100ms

        nLoopCnt++;
        if(nLoopCnt>50){	//約5sec
            if(gethostbyname("yahoo.co.jp") == 0){  //インターネット接続しているか
            	digitalWrite(GPIO_04, HIGH);    //ALM(発生)
            }
            else{
            	digitalWrite(GPIO_04, LOW);    //ALM
            }
            nLoopCnt=0;

			now = time(NULL);
			pLocalNow = localtime(&now);
			if(pLocalNow->tm_hour == 0){		//0時にリセット
				nDailyInStatus = 0;
				nDailyOutStatus = 0;
			}
			if(pLocalNow->tm_hour == 3 && nDailyInStatus == 0){
				if(SetStatusLED(&nLedNo,0) == -1)break;	//LED状態設定(3時に出勤にする)
				nDailyInStatus = 1;
			}
			if(pLocalNow->tm_min == 15 && nDailyOutStatus == 0){
				if(SetStatusLED(&nLedNo,1) == -1)break;	//LED状態設定(15時に退勤にする)
				nDailyOutStatus = 1;
			}
        }

        nTmp = digitalRead(GPIO_24);
        if(nTmp != nTactSwitch){                    //押された？
            nTactSwitch = nTmp;
            printf("%d\n",nTactSwitch);
            if(nTactSwitch == 0){                   //離された？
				if(SetStatusLED(&nLedNo,-1) == -1)break;	//LED状態設定
                printf("ledNo:%d\n",nLedNo);
            }
                
        }


        nBeep = FileReadStatus(FILE_BEEP_STAUS);                         //BEEP状態をファイルから読み込み
        if(nBeep == -1)break;
        if(nBeep == 1){
        	digitalWrite(GPIO_27, HIGH);     //OK

        	//wav
        	system("aplay -q -D plughw:1,0 /root/aquestalkpi/wav/ok3.wav");	   //wav
            if(FileWiteStatus(FILE_BEEP_STAUS,0) == -1)break;          //BEEP状態をファイルに書き込み beep停止
            
        	digitalWrite(GPIO_27, LOW);     //OK
        }

        nAlm = FileReadStatus(FILE_ALM_STAUS);                         //ALM状態をファイルから読み込み
        if(nAlm == -1 || nAlm == 2)break;                               //2:実行プロセス数エラー
        if(nAlm == 1){
        	digitalWrite(GPIO_04, HIGH);     //ALM(発生にする)
        	system("aplay -q -D plughw:1,0 /root/aquestalkpi/wav/ng3.wav");	   //wav
        	system("aplay -q -D plughw:1,0 /root/aquestalkpi/wav/card-not-use.wav");	   //wav
            if(FileWiteStatus(FILE_ALM_STAUS,0) == -1)break;          //ALM状態をファイルに書き込み ALM停止
        	digitalWrite(GPIO_04, LOW);     //ALM
        }
	}

	digitalWrite(nGPIO_Led[0], LOW);
	digitalWrite(nGPIO_Led[1], LOW);
	digitalWrite(nGPIO_Led[2], LOW);
	digitalWrite(nGPIO_Led[3], LOW);
	digitalWrite(GPIO_25, HIGH);    //beep(off)
	digitalWrite(GPIO_04, HIGH);     //ALM(発生にする)
	digitalWrite(GPIO_27, LOW);     //OK


	return 0;

}




/****************************************************************
*   LED状態設定
*   npLedNo:0..3
*	nMode -1:01230123を繰り返す 0..3:指定位置を設定する
*   return -1:NG 0:OK
****************************************************************/
int SetStatusLED(int *npLedNo,int nMode)
{
  	digitalWrite(nGPIO_Led[*npLedNo], LOW);
	if(nMode == -1){
    	(*npLedNo)++;
	}
	else{
		*npLedNo=nMode;
	}
    if(*npLedNo >= 4){
	    *npLedNo=0;
   	}
    if(FileWiteStatus(FILE_LED_STAUS,*npLedNo) == -1)return -1;          //LED状態をファイルに書き込み
    digitalWrite(nGPIO_Led[*npLedNo], HIGH);

	return 0;
}

/****************************************************************
*       状態をファイルに書き込み
*   No:0..9
*   return -1:NG 0:OK
****************************************************************/
int FileWiteStatus(char *fname,int nNo)
{
    FILE *fp;
    int val=0x30;
    
    if((fp = fopen(fname, "w")) == 0 ) {
		printf("fopen err\n");
		return -1;
	}

    val += nNo;
	fwrite(&val, 1,1, fp);
	fclose(fp);
	
    return 0;
}
/****************************************************************
*       状態をファイルから読み込み
*   return -1:NG 0..9:OK
****************************************************************/
int FileReadStatus(char *fname)
{
    FILE *fp;
    int val;
    
    if((fp = fopen(fname, "rb")) == 0 ) {
		printf("fopen err\n");
		return -1;
	}

	fread(&val, 1,1, fp);
	fclose(fp);
	
    return val-0x30;
}

