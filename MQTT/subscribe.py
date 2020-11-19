import paho.mqtt.client as mqtt
import time


def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc)) #打印连接状态
    client.subscribe("stm32/#") #订阅2222


def on_message(client, userdata, message):
    topic = "%s" %(message.topic) #打印接受的消息
    msg = "%s" %(message.payload)
    msg = msg[2:-1]
    print("Topic:%s\tMessage:%s" %(topic, msg))




client_id = time.strftime('%Y%m%d%H%M%S',time.localtime(time.time()))
client = mqtt.Client(client_id)    # ClientId不能重复，所以使用当前时间
client.on_connect = on_connect
client.on_message = on_message
client.connect("127.0.0.1", 1883, 60)
client.loop_forever()

