#ifndef _DATA_H
#define _DATA_H

typedef struct 
{
    char name[128];
    unsigned char rfid_id[2][64];
    int fp_id;
}person_info_t;

typedef struct
{
    int figureprint_nums;
    int rfid_id_nums;
    
}state_info_t;

//车间信息数据初始化
void Info_Init(person_info_t * info);
//查找卡主人
const char * Find_With_CardID(const person_info_t * head, const  char * CARD_ID);
const int Find_With_FP_ID(const person_info_t * head, int FP_ID);
#endif
