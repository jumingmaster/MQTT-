/*********************************/
/*
/*       作者：曾健恒
/*               
/*
/*       2020年6月17日
/*********************************/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <string.h>
#define D0 3
#define RX 3
#define D1 1
#define TX 1
#define D2 16
#define D3 5
#define SCL 5
#define D4 4
#define SDA 4
#define D5 14
#define SCK 14
#define D6 12
#define MISO 12
#define D7 13
#define MOSI 13
#define D8 0
#define D9 2
#define TX1 2
#define D10 15
#define D11 13
#define D12 12
#define D13 14
#define D14 4
#define D15 5
#define SS 15
#define A0 17

#define MQ2 D2
#define DELAY D14

#define     DOOR_STATE      "state"
#define     FP_NUMS         "fp-nums"
#define     FP_NAME         "fp-name"
#define     RFID_ID         "rfid-id"
#define     RFID_NAME       "rfid-name"
#define     MQTT_DEBUG      "mqtt_debug"
#define     PERSON_IN       "person-in"

const char* ssid = "MQTTBroker";                    //WIFI-SSID
//const char* password = "h990611.";                //WIFI-PASSWD
const char* mqttServer = "192.168.188.3";         //Host
const long mqttPort = 1883;                         //PORT
const char* client_id = "Stm32-TCP/IP Module";      //DEVICE-ID
char comdata[1024];
char msg[1024];
int mark = 0;
int state = true;
WiFiClient espClient;                              //声明类WIFIClien的对象，该对象允许建立到特点ip和端口的连接
PubSubClient client(espClient);                    //声明类PubSubClient的对象，接受先前定义的WiFiClient 作为构造函数的输入



void callback(char* topic, byte* payload, unsigned int length);  //MQTT消息回调函数
void reconnect();
void send_packet(char * packet);

void setup() 
{
  // put your setup code here, to run once:
     Serial.begin(115200);                                         //设置串口频率
     pinMode(LED_BUILTIN, OUTPUT);
     pinMode(DELAY, OUTPUT);
     digitalWrite(LED_BUILTIN, LOW);    
     digitalWrite(DELAY, HIGH);    
     WiFi.begin(ssid);            //连接WIFI
     while (WiFi.status() != WL_CONNECTED)
     {
          delay(500);
          Serial.print("Connecting to WiFi…");
     }
     Serial.println("Connected to the WiFi network");
     
     client.setServer(mqttServer, mqttPort);    //指定MQTT服务器的IP和端口
     client.setCallback(callback);              //初始化消息回调

     while (!client.connect("Stm32-TCP/IP Module"))              //连接MQTT
     {
     Serial.println("Connecting to MQTT...");
 
     if (client.connect("ESP8266Client"))
     {
         Serial.println("connected");  
   
     }
     else
     {
   
        Serial.print("failed with state ");
        Serial.print(client.state());
        delay(2000);
   
     }

    
    }
    Serial.println("MQTT Connected");
    client.subscribe("iPhone8");                  //订阅消息
    
}
void loop()
{
  // put your main code here, to run repeatedly:


     if (!client.connect("Stm32-TCP/IP Module")) 
     {
        reconnect();
     }
     client.loop();


     
      if(Serial.available())
      {
          memset(comdata, '0' , sizeof(comdata));
          digitalWrite(LED_BUILTIN, HIGH);
          int index = 0;
          memset(comdata, 0, sizeof(comdata));//缓存清零
          while (Serial.available() > 0 && index < 1024)//循环串口是否有数据
          {
            comdata[index] = char(Serial.read());//叠加数据到comdata
            if(comdata[index] == ':')
            {
              send_packet(comdata);
              break;
            }
            index++;
            delay(5);//延时等待响应
          }
          
          digitalWrite(LED_BUILTIN, LOW);
       }
       
    

     
}


void reconnect() 
{
  while (!client.connect("Stm32-TCP/IP Module")) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("Stm32-TCP/IP Module")) 
    {
      Serial.println("connected");
      
        
    } 
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  char msg[length];
  for (int i = 0; i < length; i++) 
  {
    msg[i] = payload[i];
    Serial.print((char)payload[i]);
  }//串口打印出收到的信息
  Serial.println();
  if(strncmp(msg, "open", strlen("open")) == 0)
    digitalWrite(DELAY, LOW);
  else if(strncmp(msg, "close", strlen("close")) == 0)
    digitalWrite(DELAY, HIGH);
}


void send_packet(char * packet)
{
    memset(msg, 0, sizeof(msg));
    char *p;
    String str = "";
    if(strncmp(packet, DOOR_STATE, strlen(DOOR_STATE)) == 0)
    {
        
        for(int i = strlen(DOOR_STATE); i < strlen(packet); i++)
          str += packet[i];
        strcpy(msg, str.c_str());
        strtok(msg, ":");
        client.publish(DOOR_STATE, msg);
        
        if(strncmp(msg, "close", strlen("close")) == 0)
          digitalWrite(DELAY, HIGH);
        else if(strncmp(msg, "open", strlen("open")) == 0)
          digitalWrite(DELAY, LOW);
        
    }
    
    else if(strncmp(packet, FP_NUMS, strlen(FP_NUMS)) == 0)
    {
        for(int i = strlen(FP_NUMS); i < strlen(packet); i++)
          str += packet[i];
        strcpy(msg, str.c_str());
        strtok(msg, ":");
        client.publish(FP_NUMS, msg);
    }
    
    else if(strncmp(packet, FP_NAME, strlen(FP_NAME)) == 0)
    {
        for(int i = strlen(FP_NAME); i < strlen(packet); i++)
          str += packet[i];
        strcpy(msg, str.c_str());
        strtok(msg, ":");
        client.publish(FP_NAME, msg);
    }

    else if(strncmp(packet, RFID_ID, strlen(RFID_ID)) == 0)
    {       
        for(int i = strlen(RFID_ID); i < strlen(packet); i++)
          str += packet[i];
        strcpy(msg, str.c_str());
        strtok(msg, ":");
        client.publish(RFID_ID, msg);
    }

    else if(strncmp(packet, RFID_NAME, strlen(RFID_NAME)) == 0)
    { 
        for(int i = strlen(RFID_NAME); i < strlen(packet); i++)
          str += packet[i];
        strcpy(msg, str.c_str());
        strtok(msg, ":");
        client.publish(RFID_NAME, msg);
    }
    
    else if(strncmp(packet, MQTT_DEBUG, strlen(MQTT_DEBUG)) == 0)
    {
        for(int i = strlen(MQTT_DEBUG); i < strlen(packet); i++)
          str += packet[i];
        strcpy(msg, str.c_str());
        strtok(msg, ":");
        client.publish(MQTT_DEBUG, msg);
    }

    else if(strncmp(packet, PERSON_IN, strlen(PERSON_IN)) == 0)
    {
        state != state;
        digitalWrite(DELAY, state);
    }
}
