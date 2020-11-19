#include "MQTTPacket.h"
#include "data.h"
#include "Communication.h"
#include "ble.h"
#include "beep.h"
#include "led.h"
#include "delay.h"
#include "timer.h"
#include "OLED_I2C.h"
#include "keyboard.h"
#include "as608.h"
#include "usart2.h"
#include "MFRC522.h"
#include <string.h>
#include <stdbool.h>

extern u8  mfrc552pidbuf[18];
extern u8  card_pydebuf[2];
extern u8  card_numberbuf[5];
static u8 USART_RX_BUF[256]; 
static u8  card_key0Abuf[6]={0xff,0xff,0xff,0xff,0xff,0xff};
extern u16 ValidN;
extern SysPara AS608Para;                      
int BLE_RX_FLAG;
static u8 USART_RX_BUF[256];    
static u16 USART_RX_STA=0; 

//加卡（调试用）
void MFRC522ADD(void)
{
	u8 i,status,card_size;
	
	status=MFRC522_Request(0x52, card_pydebuf);			//寻卡
	
	if(status==0)		//如果读到卡
	{
        BeepRing;
        delay_ms(100);
        BeepCalm;
		printf("mqtt_debug读到卡\n");
		status=MFRC522_Anticoll(card_numberbuf);						//防撞处理			
		card_size=MFRC522_SelectTag(card_numberbuf);					//选卡
		status=MFRC522_Auth(0x60, 4, card_key0Abuf, card_numberbuf);	//验卡
		status=MFRC522_Write(4, card_numberbuf);							//写卡（写卡要小心，特别是各区的块3）
		//status=MFRC522_Read(4, card_readbuf);							//读卡
		if(status != MI_OK)
		{
			printf("mqtt_debug写卡失败\n");
		}
		else
			printf("mqtt_debug写卡成功\n");
		
		
		printf("mqtt_debug卡片类型 : %x \n",card_pydebuf[0]);
		//卡序列号显，最后一字节为卡的校验码
		printf("卡ID:");
		for(i=0;i<4;i++)
		{
			
			printf("mqtt_debug%x",card_numberbuf[i]);
			
		}
		
		printf("\n");
		//卡容量显示，单位为Kbits
	
		printf("mqtt_debug卡片容量 : %dKB\n",card_size);
		
        
	}
}

//录指纹
void Add_FR(void)
{
	u8 i,ensure ,processnum=0;
	u16 ID;
	while(1)
	{
		switch (processnum)
		{
			case 0:
				i++;
				printf("mqtt_debug请按指纹\n");
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					BeepRing;
					ensure=PS_GenChar(CharBuffer1);//生成特征
					BeepCalm;
					if(ensure==0x00)
					{
						printf("mqtt_debug指纹正常\n");
						i=0;
						processnum=1;//跳到第二步						
					}else printf("mqtt_debug%s",EnsureMessage(ensure));				
				}else printf("mqtt_debug%s",EnsureMessage(ensure));					
				break;
			
			case 1:
				i++;
				printf("mqtt_debug请按再按一次指纹\n");
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					BeepRing;
					ensure=PS_GenChar(CharBuffer2);//生成特征
					BeepCalm;
					if(ensure==0x00)
					{
						printf("mqtt_debug指纹正常\n");
						i=0;
						processnum=2;//跳到第三步
					}else printf("mqtt_debug%s",EnsureMessage(ensure));		
				}else printf("mqtt_debug%s",EnsureMessage(ensure));		
				break;

			case 2:
				printf("mqtt_debug对比两次指纹\n");
				ensure=PS_Match();
				if(ensure==0x00) 
				{
					
					printf("mqtt_debug对比成功,两次指纹一样\n");
					processnum=3;//跳到第四步
				}
				else 
				{
					
					printf("mqtt_debug对比失败，请重新录入指纹\n");
					printf("mqtt_debug%s",EnsureMessage(ensure));		
					i=0;
					processnum=0;//跳回第一步		
				}
				delay_ms(1200);
				break;

			case 3:
				printf("mqtt_debug生成指纹模板\n");
				ensure=PS_RegModel();
				if(ensure==0x00) 
				{
					printf("mqtt_debug生成指纹模板成功\n");
					processnum=4;//跳到第五步
				}else {processnum=0;printf("%s",EnsureMessage(ensure));}
				delay_ms(1200);
				break;
				
			case 4:	
				
				printf("mqtt_debug请输入储存ID,按Enter保存\n");
				printf("mqtt_debug0=< ID <=299\n");
				do
					ID = Get_Num(3);//mistake
				while(!(ID<AS608Para.PS_max));//输入ID必须小于最大存储数值
				ensure=PS_StoreChar(CharBuffer2,ID);//储存模板
				if(ensure==0x00) 
				{			
						
					printf("mqtt_debug录入指纹成功\n");
					PS_ValidTempleteNum(&ValidN);//读库指纹个数
					printf("mqtt_debugAS608Para.PS_max-ValidN:%d\n",ValidN);
					delay_ms(1500);
					return ;
				}else {processnum=0;printf("mqtt_debug%s",EnsureMessage(ensure));}					
				break;				
		}
		delay_ms(400);
		if(i==5)//超过5次没有按手指则退出
		{
			break;	
		}				
	}
}
//删除指纹
void Del_FR(void)
{
	u8  ensure;
	u16 num;
	
	printf("mqtt_debug删除指纹\n");
	printf("mqtt_debug请输入指纹ID按Enter发送\n");
	printf("mqtt_debug0=< ID <=299");
	delay_ms(50);
	num= Get_Num(3);//获取返回的数值
	if(num==0xFFFF)
		return; //返回主页面
	else if(num==0xFF00)
		ensure=PS_Empty();//清空指纹库
	else 
		ensure=PS_DeletChar(num,1);//删除单个指纹
	if(ensure==0)
	{
		
		printf("删除指纹成功\n");		
	}
  else
		printf("mqtt_debug%s",EnsureMessage(ensure));		
	delay_ms(1200);
	PS_ValidTempleteNum(&ValidN);//读库指纹个数
	printf("mqtt_debugAS608Para.PS_max-ValidN: %d\n",ValidN);

}

//舵机测试
void SteeringGearTest(void)
{
    TIM_SetCompare3(TIM4,195);
    delay_s(1);
    TIM_SetCompare3(TIM4,175);
    delay_s(1);
    
}

//蓝牙中断服务
void USART3_IRQHandler(void)
{
	u8 res;	
    
	if(USART3->SR&(1<<5))
	{	 
		res=USART3->DR; 
		if((USART_RX_STA&0x8000)==0)
		{
			if(USART_RX_STA&0x4000)
			{
				if(res!=0x0a)USART_RX_STA=0;
				else USART_RX_STA|=0x8000;	
			}else
			{	
				if(res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=res;
					USART_RX_STA++;
					if(USART_RX_STA> 255 )USART_RX_STA=0; 
				}		 
			}
		}  		 									     
	} 
    BLE_RX_FLAG = 1;
}   

//蓝牙控制门锁测试
void BleUnlockTest(void)
{
    if(BLE_RX_FLAG)
    {
            
       if(strcmp((const char *)USART_RX_BUF, UNLOCK) == 0)
       {
           TIM_SetCompare3(TIM4,195);
           printf("mqtt_debug开门:\n");
           memset(USART_RX_BUF, 0, 256);
           USART_RX_STA = 0;
           BLE_RX_FLAG = 0;
       }
       else if(strcmp((const char *)USART_RX_BUF, LOCK) == 0)
       {
           TIM_SetCompare3(TIM4,175);
           printf("mqtt_debug关门:\n");
           memset(USART_RX_BUF, 0, 256);
           USART_RX_STA = 0;
           BLE_RX_FLAG = 0;
       }
    }
    
}

void LED_Test(void)
{
    AlarmLED_ON;
    delay_ms(500);
    DataLED_ON;
    delay_ms(500);
    FigurePrintLED_ON;
    delay_ms(500);
    RuntimeLED_ON;
    delay_ms(500);
    AlarmLED_OFF;
    delay_ms(500);
    DataLED_OFF;
    delay_ms(500);
    FigurePrintLED_OFF;
    delay_ms(500);
    RuntimeLED_OFF;
    
}


void KeyBoardTest(void)
{
   
    int sel;
    while(1)
    {
        if(KEY_work())
        {
            sel = KEY_Scan();
            sel = KEY_Fun(sel);
            printf("%c\n", sel);
            if(sel == '#')
                break;
        }
    }
}
