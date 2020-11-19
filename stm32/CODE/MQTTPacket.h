#ifndef __MQTT__
#define __MQTT__
#include <stm32f4xx.h>

//数据包最大值
#define     PACKET_MAX_SIZE      0x100

//主题名
#define     DOOR_STATE      "state"
#define     FP_NUMS         "fp-nums"
#define     FP_NAME         "fp-name"
#define     RFID_ID         "rfid-id"
#define     RFID_NAME       "rfid-name"

//主题类型
typedef enum
{
    DoorState = 0,     //门禁状态
    FigureprintNums,   //指纹数量
    FigureprintOwner,  //指纹拥有者
    RfidID,           //RFID的ID 
    RfidOwner          //RFID拥有者
}topic_type;


//简单封装数据包
typedef struct
{
    char    topic[64];      //主题
    char    message[64];    //消息
}mqtt_packet_t;


#endif
