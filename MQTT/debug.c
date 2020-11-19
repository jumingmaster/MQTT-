#define libmosq_EXPORT
#include "mosquitto.h"

#include <stdio.h>
#include <string.h>

char topic_list[6][16] = {{"state"}, {"fp-nums"}, {"fp-name"}, {"rfid-id"}, {"rfid-name"}, {"mqtt_debug"}};

void on_message(struct mosquitto *, void *, const struct mosquitto_message *);


int main (void)
{
    enum mosq_err_t mosq_err;
    struct mosquitto_message message;
    struct mosquitto *client = NULL;

    mosq_err = mosquitto_lib_init();
    if(mosq_err != MOSQ_ERR_SUCCESS)
    {
        printf("MQTT初始化失败:%s\n",mosquitto_strerror(mosq_err));
        return -1;
    }
    client =  mosquitto_new(NULL, true, NULL);
    if(client == NULL)
    {
        printf("创建实例失败%s\n",mosquitto_strerror(mosq_err));
        return -1;
    }

    mosq_err = mosquitto_connect(client, "127.0.0.1", 1883, 600);
    if(mosq_err != MOSQ_ERR_SUCCESS)
    {
        printf("MQTT连接失败:%s\n",mosquitto_strerror(mosq_err));
        return -1;
    }

    for(int i = 0; i < 6; i++)
    {
        mosq_err = mosquitto_subscribe(client, NULL, topic_list[i], 0);
        if(mosq_err != MOSQ_ERR_SUCCESS)
        {
            printf("订阅失败:%s\n",mosquitto_strerror(mosq_err));
            return -1;
        }
    }
    mosquitto_message_callback_set(client, on_message);
    mosq_err = mosquitto_loop_forever(client, 3, 1);
    printf("%s\n",mosquitto_strerror(mosq_err));

    return 0;
}


void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
{
    printf("MID:%d\t",message->mid);
    printf("TOPIC:%s\t",message->topic);
    printf("QoS:%d\n",message->qos);
    printf("MSG:%s\n",message->payload);
    
}

