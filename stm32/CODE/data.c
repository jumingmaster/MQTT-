#include "data.h"
#include <string.h>
#include "usart2.h"
//车间信息数据初始化
void Info_Init(person_info_t * info)
{
    strcpy(info[0].name, "Henry");
    info[0].fp_id = 20;
    strcpy((char *)info[0].rfid_id[0], "f74c5bb555");
    strcpy((char *)info[0].rfid_id[1], "8a1f381f9");
    
}


//查找卡主人
const char * Find_With_CardID(const person_info_t * head, const char * CARD_ID)
{
    for(int i = 0; i < sizeof(head)/sizeof(person_info_t); i++)
    {
        if(strcmp((char *)head[i].rfid_id[0], CARD_ID) == 0)
            return head[i].name;
        
        else 
            if(strcmp((char *)head[i].rfid_id[1], CARD_ID) == 0)
                return head[i].name;
        
    }
    return NULL;
}

//查找卡主人
const int Find_With_FP_ID(const person_info_t * head, int FP_ID)
{
    for(int i = 0; i < sizeof(head)/sizeof(person_info_t); i++)
    {
        if(head->fp_id == FP_ID)
            return head->fp_id;
    }
    return -1;
}