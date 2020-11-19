#include "MQTTPacket.h"
#include "Communication.h"
#include "ble.h"
#include "beep.h"
#include "exti.h"
#include "led.h"
#include "data.h"
#include "delay.h"
#include "timer.h"
#include "OLED_I2C.h"
#include "keyboard.h"
#include "as608.h"
#include "usart2.h"
#include "MFRC522.h"
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

/******************全局变量******************************/
mqtt_packet_t message;                  //数据包
u8 ensure;                              //指纹返回码
SysPara AS608Para;                      //指纹信息结构体
u16 ValidN;                             //指纹数量
bool PR = false;                        //指纹匹对值
char password1[] = "123456";            //本地密码1
char password2[] = "758313";            //本地密码2
static u8 read_flag = 1;	               //按键定时标识位
//MFRC522数据区
u8  mfrc552pidbuf[18];
u8  card_pydebuf[2];
u8  card_numberbuf[5];
u8  card_key0Abuf[6]={0xff,0xff,0xff,0xff,0xff,0xff};
u8  card_readbuf[16] = {0};
/******************************************************/

/***********************函数***************************/
//主菜单
void menu(void);
//读指纹
void press_FR(void);
//开门
void OpenDoor(void);
//关门
void CloseDoor(void);
//报警
void Alarm(void);
//中文字符串打印
void ShowCNStr(int xpos, int ypos, int nums, ...);
//读卡
u8 MFRC522Read(void);
//与指纹模块握手
void ShakeHand(void);

///////////测试单元//////////////////
//加卡(管理员用)                 ///
void MFRC522ADD(void);           ///
//录指纹(管理员用)              ///
void Add_FR(void);              ///
//删指纹(管理员用)             ///
void Del_FR(void);             ///
//舵机测试                     ///
void SteeringGearTest(void);   ///
//蓝牙解锁测试                ///
void BleUnlockTest(void);     ///
//LED测试                     /// 
void LED_Test(void);         ///
//键盘测试                   ///
void KeyBoardTest(void);     ///
/////////////////////////////////
/*****************************************************/


/******************************************主程序入口*************************************/
int main(void)
{
    int sel = 0;
    int i = 0;
    int reval;
    /*********************************系统初始化****************************************/
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //加入中断策略组
    Init_ESP_Usart();                               //ESP模块连接串口初始化
    usart2_init();                                  //指纹模块通信串口初始化
    Key_Init();                                     //键盘初始化
    Board_Key_Init();                               //关门按键初始化
    Init_BLE();                                     //蓝牙初始化
    Init_Beep();                                    //蜂鸣器初始化
    TIM7_Int_Init(100-1,8400-1);		              //通用定时器7初始化(指纹模块使用) 
    TIM3_Init();                                   //通用定时器3初始化(消除按键抖动)    
    Init_LED();                                     //LED初始化
    TIM4_LockControl_Init();                        //舵机初始化
    STM32_SPI3_Init();                              //SPI初始化
    PS_StaGPIO_Init();                              //指纹触碰感应初始化
    Init_Delay();                                   //嘀嗒定时器初始化
    I2C_Configuration();                            //配置CPU的硬件I2C
    OLED_Init();                                    //OLED屏初始化
    ShakeHand();                                   //与指纹模块握手
    /*******************************************************************************/
    
                                        /*调试专用*/
    /*************************************DEBUG*************************************/
    //SteeringGearTest();   //舵机测试      
    //Add_FR();             //录指纹(管理员用)
    //Del_FR();             //删指纹(管理员用)
    //MFRC522ADD();         //加卡(管理员用
    //BleUnlockTest();      // 蓝牙解锁测试 
    //LED_Test();          //LED测试
    //KeyBoardTest();      //键盘测试 
    /*******************************************************************************/
    
    /***************************用户界面初始化*************************************/
    CloseDoor();//关门
    RuntimeLED_ON;//系统运行状态灯
    OLED_CLS();                                                      //清屏
    OLED_ShowStr(30,3,(unsigned char *)"Welcome", 2);                //欢迎
    delay_s(2);
    menu();                                                          //主界面
    /*******************************************************************************/
    
    while(1)
    {
        //重置RFID
        MFRC522_Initializtion();
        if(KEY_work() && read_flag == 1)
        {
            read_flag = 0;
            BeepRing;
            delay_ms(100);
            BeepCalm;
            sel = KEY_Scan();
            sel = KEY_Fun(sel);
            
            switch(sel)
            {
                case '1':
                Bowl_OFF();
                printf("mqtt_debug请按指纹(3 sec)\n:");
                    OLED_CLS();
                        
                    ShowCNStr(30, 2, 4, 22, 21, 8, 9);
                    delay_s(3);
                    press_FR();
                    Bowl_OFF();
                    //指纹开门
                    if(PR)
                    {
                        OLED_CLS();
                        ShowCNStr(64,3, 2, 18, 20);
                        OpenDoor();
                        delay_s(3);
                        //关门
                        OLED_CLS();
                        ShowCNStr(64,3, 2, 19, 20);
                        CloseDoor();
                        
                        PR = false;
                        menu();
                        Bowl_ON();
                        break;
                    }
                    else
                    {
                        printf("mqtt_debug指纹错误\n:");
                        ShowCNStr(30,2, 4, 8, 9, 23, 24);
                        Alarm();
                        menu();
                        break;
                    }

                    
                
                case '2':
                    Bowl_OFF();
                    printf("mqtt_debug\r\n请输入密码:");
                    OLED_CLS();
                    ShowCNStr(30,0, 4, 12, 13, 25, 26);
                    char passwd[6];
                    TIM_Cmd(TIM3, ENABLE);
                    i = 0;
                    int pos = 32;
                    while(i < 6)    //输入6位密码
                    {
                         if(KEY_work() && read_flag == 1)
                         {
                            read_flag = 0;
                            BeepRing;
                            delay_ms(100);
                            BeepCalm;
                            sel = KEY_Scan();
                            OLED_ShowStr(pos + 16, 4, (unsigned char *)"*  ", 2);
                            pos += 8;
                            passwd[i] = KEY_Fun(sel);
                            printf("mqtt_debug%c\n:", passwd[i]);
                            i++;
                            TIM_Cmd(TIM3, ENABLE);
                         }
                    }
                    if(strncmp(password1, passwd, strlen(password1)) == 0 ||    //密码正确
                        strncmp(password2, passwd, strlen(password2)) == 0)   
                    {
                        printf("mqtt_debug密码正确\n:");
                        OLED_CLS();
                        ShowCNStr(30, 2, 4, 25, 26, 27, 28);
                        OpenDoor();
                        delay_s(3);
                        CloseDoor();
                        Bowl_ON();
                    }
                    else                                //密码错误
                    {
                        
                        printf("mqtt_debug密码错误\n:");
                        OLED_CLS();
                        ShowCNStr(30, 2, 4, 25, 26, 23, 24);
                        Alarm();
                    }
                    
                    menu();
                    break;
                
                case '3':
                    Bowl_OFF();
                    printf("mqtt_debug请刷卡\n:");
                    OLED_CLS();
                    ShowCNStr(50, 2, 3, 22, 10, 11);
                    i = 0;
                    while(i < 5)
                    {
                        if((reval = MFRC522Read()))   //卡正确
                        {
                            
                            printf("mqtt_debug解锁成功\n:");
                            OLED_CLS();
                            ShowCNStr(30,2, 4, 18, 7, 0, 1);
                            create_packet(RfidID, card_numberbuf, &message);
                            send_packet(&message);
                            OpenDoor();
                            delay_s(3);
                            CloseDoor();
                            menu();
                            break;
                        }
                       
                            
                        delay_s(10);
                        i++;
                    }
                    
                    if(!reval)
                    {
                       printf("mqtt_debug解锁失败\n:");
                       OLED_CLS();
                       ShowCNStr(30,2, 4, 6, 7, 4, 5);
                       Alarm();
                       break;
                    }
                    break;
                    
                
                default:
                    break;
            
            }
            TIM_Cmd(TIM3, ENABLE);
        }
        TIM_Cmd(TIM3, ENABLE);
        sel = 0;
    }
    delay_s(1);
}

//主菜单
void menu(void)
{
    OLED_CLS();
    OLED_ShowStr(0,1,(unsigned char *)"1.FigurePrint Unlock", 1);
    OLED_ShowStr(0,3,(unsigned char *)"2.Key         Unlock", 1);
    OLED_ShowStr(0,5,(unsigned char *)"3.RFID        Unlock", 1);
}
//开门
void OpenDoor(void)
{
    TIM_SetCompare3(TIM4,195);
    create_packet(DoorState, "open", &message);
    send_packet(&message);
}
//关门
void CloseDoor(void)
{
    TIM_SetCompare3(TIM4,175);
    create_packet(DoorState, "close", &message);
    send_packet(&message);
}



//刷指纹
void press_FR(void)
{
	SearchResult seach;
	u8 ensure;
	ensure=PS_GetImage();
	if(ensure==0x00)//获取图像成功 
	{	
        Bowl_ON();
		BeepRing;//打开蜂鸣器	
		ensure=PS_GenChar(CharBuffer1);
		if(ensure==0x00) //生成特征成功
		{		
			BeepCalm;//关闭蜂鸣器	
			ensure=PS_HighSpeedSearch(CharBuffer1,0,AS608Para.PS_max,&seach);
			if(ensure==0x00)//搜索成功
			{				
				PR = true;
				printf("mqtt_debug刷指纹成功\n:");				
				char str[50];
				sprintf(str,"mqtt_debug确有此人,ID%d  匹配得分%d:\n",seach.pageID,seach.mathscore);
				printf("mqtt_debug%s\n:",(u8*)str);
			}
			else 
			{
				PR = false;
				printf("mqtt_debug%s\n:",EnsureMessage(ensure));			
			}
	  }
		else
			printf("mqtt_debug%s\n:",EnsureMessage(ensure));
	 BeepCalm;//关闭蜂鸣器
	 delay_ms(600);
	}
		
}



//报警
void Alarm(void)
{
	u8 _time;
	for(_time = 0; _time < 5; _time++)
	{
		BeepRing;
        create_packet(DoorState, "open", &message);
        send_packet(&message);
		delay_ms(400);
		BeepCalm;
        create_packet(DoorState, "close", &message);
        send_packet(&message);
		delay_ms(200);
        
        
	}
}

//中文打印
void ShowCNStr(int xpos, int ypos, int nums, ...)
{
    int val;
    va_list args;
    va_start(args, nums);
    for(int i = 0; i < nums; i++, xpos += 16)
    {
        val = va_arg(args, int);
        OLED_ShowCN(xpos, ypos, val);
        
    }
    va_end(args);
  
}



//读卡
u8 MFRC522Read(void)
{
	u8 i,status,card_size;
	u8 flag = 0;
	
	status=MFRC522_Request(0x52, card_pydebuf);			//寻卡
	
	if(status==0)		//如果读到卡
	{
        BeepRing;
        delay_ms(100);
        BeepCalm;
		printf("mqtt_debug读到卡\n:");
		status=MFRC522_Anticoll(card_numberbuf);						//防撞处理
		if(status != MI_OK)
		{
			printf("mqtt_debug防撞处理失败\n:");
		}
		else
			printf("mqtt_debug防撞处理成功\n:");
		
		card_size=MFRC522_SelectTag(card_numberbuf);					//选卡
		if(status != MI_OK)
		{
			printf("mqtt_debug选卡失败\n:");
		}
		else
			printf("mqtt_debug选卡成功\n:");
		
		status=MFRC522_Auth(0x60, 4, card_key0Abuf, card_numberbuf);	//验卡
		if(status != MI_OK)
		{
			printf("mqtt_debug验卡失败\n:");
		}
		else
			printf("mqtt_debug验卡成功\n:");
		
		status=MFRC522_Read(4, card_readbuf);							//读卡
		if(status != MI_OK)
		{
			printf("mqtt_debug读卡失败\n:");
            flag = 0;
            return flag;
		}
		else
			printf("mqtt_debug读卡成功\n:");
		

		for(i=0; i<4; i++)
		{
			if(card_readbuf[i] != card_numberbuf[i])
			{
				flag = 0;
				break;
			}
			
			flag = 1;
		}
			
	}
	return flag;
}

void TIM3_IRQHandler(void)
{
	
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		/*处理的程序段*/
		read_flag = 1;
		
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update); //清除中断标志位
	TIM_Cmd(TIM3, DISABLE);		//关闭使能
}


//与指纹模块握手
void ShakeHand(void)
{
    while(1)
    {
        while(PS_HandShake(&AS608Addr))//与AS608模块握手
        {		
            delay_ms(400);
            printf("mqtt_debug未检测到模块!!!\n:");
            delay_ms(800);
            printf("mqtt_debug尝试连接模块...\n:");		  
        }
        printf("mqtt_debugSuccessful connected \n:");
        printf("mqtt_debug波特率%d   地址%x\n:",57600, AS608Addr);
        delay_ms(15);
        ensure=PS_ValidTempleteNum(&ValidN);//读库指纹个数
        if(ensure!=0x00)
        {
            printf("mqtt_debug%s\n:",EnsureMessage(ensure));//显示确认码错误信息	
            
        }
        ensure=PS_ReadSysPara(&AS608Para);  //读参数 
        
        if(ensure==0x00)
        {
            create_packet(FigureprintNums, &ValidN, &message);
            send_packet(&message);
            printf("mqtt_debug\r\n库容量   %d对比等级%d\n:",AS608Para.PS_max-ValidN,AS608Para.PS_level);
            break;
        }
        else
            printf("mqtt_debug%s\n:",EnsureMessage(ensure));//显示确认码错误信息	
    
    }

}

