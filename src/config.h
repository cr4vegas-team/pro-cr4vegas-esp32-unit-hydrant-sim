#include <Arduino.h>

#define GSM_PIN ""
const char apn[] = "movistar.es";
const char gprsUser[] = "MOVISTAR";
const char gprsPass[] = "MOVISTAR";
const char wifiSSID[] = "";
const char wifiPass[] = "";
const char *broker = "emqx.rubenfgr.com";
const char *topicSub = "s/u/h/vi/172";
const char *topicPub = "n/u/h/vi/172";
const uint16_t MQTT_RECONNECT_ATTEMPTS_MAX = 3;
const uint32_t RECCONECT_TIME = 10000;
long PUBLISH_DATA_SPEED = 1000 * 60 * 24;