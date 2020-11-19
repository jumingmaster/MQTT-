import test as influx
import paho.mqtt.client as mqtt
import time


database = 'stm32'
retention_policy = 'autogen'
# 订阅主题列表
topic_list = ['state', 'fp-nums', 'fp-name', 'rfid-id', 'rfid-name']
# 订阅主题对应数据库键值的字典
topic_dict = {'state':'stm32/door-state', 'fp-nums':'stm32/figerueprint-lib',
              'fp-name':'stm32/figerueprint-lib', 'rfid-id':'stm32/rfid', 
              'rfid-name':'stm32/rfid'}


def mqtt_debug( msg):
    print("Message:%s\n" %(msg))

def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc)) #打印连接状态
    for Topic in topic_list:
        client.subscribe(Topic) #订阅
        print("%10s\t已订阅" %(Topic));


def on_message(client, userdata, message):
    topic = "%s" %(message.topic) #打印接受的消息
    msg = "%s" %(message.payload)
    msg = msg[2:-1]
    print("Topic:%s\tMessage:%s" %(topic, msg))
    point = influx.get_point(topic_dict[topic], 'Devices', 'Stm32F407', topic, msg)
    influx.write_data_to_DB(point, write_api, bucket)


# 必须先初始化时序数据库
bucket = influx.create_bucket(database, retention_policy)
client = influx.influxDB_new(bucket)
write_api = influx.create_writeOBJ(client)


# 然后才能连接(订阅)
client_id = time.strftime('%Y%m%d%H%M%S',time.localtime(time.time()))
client = mqtt.Client(client_id)    # ClientId不能重复，所以使用当前时间
client.on_connect = on_connect
client.on_message = on_message
client.connect("127.0.0.1", 1883, 60)
client.loop_forever()

influx.destory_write_api(write_api)
influx.destory_client(client)
