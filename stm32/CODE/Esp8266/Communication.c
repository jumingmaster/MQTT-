#include "Communication.h"
#include "led.h"
#pragma import(__use_no_semihosting)                        
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
    
int _sys_exit(int x) 
{ 
	x = x;
    
} 

int fputc(int ch, FILE *f)
{ 	
	USART_SendData(USART1,ch);  
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
    USART_SendData(USART3,ch);
    while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);
	return ch;
}

static u8 USART_RX_BUF[PACKET_MAX_SIZE];    
static u16 USART_RX_STA=0; 

  void USART1_IRQHandler(void)
{
	u8 res;	

	if(USART1->SR&(1<<5))
	{	 
		res=USART1->DR; 
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
					if(USART_RX_STA>(PACKET_MAX_SIZE-1))USART_RX_STA=0; 
				}		 
			}
		}  		 									     
	} 
    
} 



void Init_ESP_Usart(void)
{
    GPIO_InitTypeDef    GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
    
    //使能APB1时钟、时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    
    //GPIO 复用
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); 
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); 
    
    //初始化GPIO引脚
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(GPIOA,&GPIO_InitStruct);
    
    //初始化串口
    USART_InitStruct.USART_BaudRate 				= 115200;				
	USART_InitStruct.USART_WordLength 			= USART_WordLength_8b;	
	USART_InitStruct.USART_StopBits 				= USART_StopBits_1;		
	USART_InitStruct.USART_Parity 				= USART_Parity_No;		
	USART_InitStruct.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode 					= USART_Mode_Rx | USART_Mode_Tx;	

	USART_Init(USART1, &USART_InitStruct); 
    
    //中断嵌套
	NVIC_InitStruct.NVIC_IRQChannel						= USART1_IRQn;  	
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority		= 0x01;		
	NVIC_InitStruct.NVIC_IRQChannelSubPriority			= 0x01;			
	NVIC_InitStruct.NVIC_IRQChannelCmd					= ENABLE;		

    NVIC_Init(&NVIC_InitStruct);	
	

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART1, ENABLE);
}

//数据包生成
int create_packet(int type, void * msg, mqtt_packet_t * packet)
{
    char tmp[256];
    //清空内存
    memset(packet, '\0', sizeof(mqtt_packet_t));
    
    //类型判断
    switch(type)
    {
        case DoorState:
            strcpy(packet->topic, DOOR_STATE);
            strcpy(packet->message, (char*)msg);
            break;
        
        case FigureprintNums:
            
            strcpy(packet->topic, FP_NUMS);            
            sprintf(packet->message, "%d", *((u8 *)msg));
            break;
        
        case FigureprintOwner:
            strcpy(packet->topic, FP_NAME);
            strcpy(packet->message, (char*)msg);
            break;
        
        case RfidID:
            
            strcpy(packet->topic, RFID_ID);
            for(int i = 0; i < 5; i++)
                packet->message[i] = ((char*)msg)[i];
            break;
        
        case RfidOwner:
            strcpy(packet->topic, RFID_NAME);
            strcpy(packet->message, (char*)msg);
            break;
        
        default:
            return -1;
    }
    return 0;
}

//发送数据包
void send_packet(mqtt_packet_t * packet)
{
    //传输数据,DataLED开灯
    DataLED_ON;
    
    printf("%s",packet->topic);
    
    if(strcmp(packet->topic, RFID_ID) == 0)
    {
        for(int i = 0; i < 5; i++)
            printf("%x", packet->message[i]);
    }
    else
        printf("%s:",packet->message);
    
    printf(":");
    
    DataLED_OFF;
    //传输结束,DataLED关灯
}


