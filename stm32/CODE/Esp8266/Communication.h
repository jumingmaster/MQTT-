#ifndef  _RASPI_H_
#define  _RASPI_H_
#include "MQTTPacket.h"
#include <string.h>
#include <stdio.h>

/***********************************/
//ESP模块连接串口初始化
//通过串口发送数据给ESP8266核心板
//核心板作为stm32的TCP/IP协议栈
//并且是网络应用层,使用MQTT发送
//stm32的数据，上传到数据可视化平台,时序数据库
//参数:nil
//返回值:nul
void Init_ESP_Usart(void);
/***********************************/


/******************************************************************/
//数据包生成

//参数:
//  type: 消息主题类型,可在MQTTPacket.h文件中查看（topic_type枚举）
//  msg:  消息内容
//  packet：修改packet的内存，生成数据包

//返回值:0为成功,-1为失败
int create_packet(int type, void * msg, mqtt_packet_t * packet);
/*****************************************************************/


/**************************************************/
//
//发送数据包
//参数:mqtt_packet_t 结构体
//返回值:nil
void send_packet(mqtt_packet_t * packet);
/**************************************************/
#endif
