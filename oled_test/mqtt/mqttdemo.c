#include "mqttdemo.h"
#include "debug.h"
#include "deal_response.h"
#include "mem.h"
#include "config.h"
#include "simple_ui.h"

MQTT_Client mqttClient;

/******************************************************************************
 * FunctionName : mqttConnectedCb
 * Description  : 
 * Parameters   : none
 * Returns      : none
 *******************************************************************************/
void ICACHE_FLASH_ATTR
mqttConnectedCb(uint32_t *args)
{
	MQTT_Client* client = (MQTT_Client*)args;
	INFO("MQTT: Connected\r\n");
	UI_UpdateSysBar("[MQTT]Connected");
	MQTT_Subscribe(client, "/topic/0", 0);
	MQTT_Subscribe(client, "/rgb/1", 0);
//	MQTT_Subscribe(client, "/mqtt/topic/1", 0);
//	MQTT_Publish(client, "/mqtt/topic/0", "hello0", 6, 0, 0);
}

/******************************************************************************
 * FunctionName : mqttDisconnectedCb
 * Description  : 
 * Parameters   : none
 * Returns      : none
 *******************************************************************************/
void ICACHE_FLASH_ATTR
mqttDisconnectedCb(uint32_t *args)
{
	MQTT_Client* client = (MQTT_Client*)args;
	INFO("MQTT: Disconnected\r\n");
	UI_UpdateSysBar("[MQTT]Disconnected");
}

/******************************************************************************
 * FunctionName : mqttPublishedCb
 * Description  : 
 * Parameters   : none
 * Returns      : none
 *******************************************************************************/
void ICACHE_FLASH_ATTR
mqttPublishedCb(uint32_t *args)
{
	MQTT_Client* client = (MQTT_Client*)args;
	INFO("MQTT: Published\r\n");
	UI_UpdateSysBar("[MQTT]Published");
}

/******************************************************************************
 * FunctionName : mqttDataCb
 * Description  : 
 * Parameters   : none
 * Returns      : none
 *******************************************************************************/
void ICACHE_FLASH_ATTR
mqttDataCb(uint32_t *args, const char* topic, uint32_t topic_len, const char *data, uint32_t data_len)
{
	char *topicBuf = (char*)os_zalloc(topic_len+1),
			*dataBuf = (char*)os_zalloc(data_len+1);

	MQTT_Client* client = (MQTT_Client*)args;

	os_memcpy(topicBuf, topic, topic_len);
	topicBuf[topic_len] = 0;

	os_memcpy(dataBuf, data, data_len);
	dataBuf[data_len] = 0;

	INFO("Receive topic: %s, data: %s \r\n", topicBuf, dataBuf);
	UI_UpdateSysBar("[MQTT]Received");
	#ifdef ALLOW_DEAL_MQTT_DATA
	deal_response(topicBuf,dataBuf);
	#endif
	os_free(topicBuf);
	os_free(dataBuf);
}

/******************************************************************************
 * FunctionName : MQTTDemo_InitConnection
 * Description  : 
 * Parameters   : none
 * Returns      : none
 *******************************************************************************/
void ICACHE_FLASH_ATTR
MQTTDemo_InitConnection(void)
{
	UI_UpdateSysBar("[MQTT]Initializing");
	MQTT_InitConnection(&mqttClient, sysCfg.mqtt_host, sysCfg.mqtt_port, sysCfg.security);
	//MQTT_InitConnection(&mqttClient, "192.168.11.122", 1880, 0);
	MQTT_InitClient(&mqttClient, sysCfg.device_id, sysCfg.mqtt_user, sysCfg.mqtt_pass, sysCfg.mqtt_keepalive, 1);
	//MQTT_InitClient(&mqttClient, "client_id", "user", "pass", 120, 1);
	MQTT_InitLWT(&mqttClient, "/lwt", "offline", 0, 0);//lastwill topic&message
	MQTT_OnConnected(&mqttClient, mqttConnectedCb);
	MQTT_OnDisconnected(&mqttClient, mqttDisconnectedCb);
	MQTT_OnPublished(&mqttClient, mqttPublishedCb);
	MQTT_OnData(&mqttClient, mqttDataCb);
}

void ICACHE_FLASH_ATTR
MQTTDemo_Publish(const u8* topic,const u8* message,int qos,int retain)
{
	u8 len=0,i;
	for(i=0;message[i]!='\0';i++)
	{
		len++;
	}
	MQTT_Publish(&mqttClient, topic, message, len, qos, retain);
}