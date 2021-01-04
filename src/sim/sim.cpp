#include "../sensors/sensors.h"
#include "../main.h"

// Select your modem:
// #define TINY_GSM_MODEM_SIM800
// #define TINY_GSM_MODEM_SIM808
// #define TINY_GSM_MODEM_SIM868
#define TINY_GSM_MODEM_SIM900
// #define TINY_GSM_MODEM_SIM7000
// #define TINY_GSM_MODEM_SIM5360
// #define TINY_GSM_MODEM_SIM7600
// #define TINY_GSM_MODEM_UBLOX
// #define TINY_GSM_MODEM_SARAR4
// #define TINY_GSM_MODEM_M95
// #define TINY_GSM_MODEM_BG96
// #define TINY_GSM_MODEM_A6
// #define TINY_GSM_MODEM_A7
// #define TINY_GSM_MODEM_M590
// #define TINY_GSM_MODEM_MC60
// #define TINY_GSM_MODEM_MC60E
// #define TINY_GSM_MODEM_ESP8266
// #define TINY_GSM_MODEM_XBEE
// #define TINY_GSM_MODEM_SEQUANS_MONARCH

#define PIN_RESET_SIM 21
#define PIN_LED 25

// Set serial for AT commands (to the module)
// Use Hardware Serial on Mega, Leonardo, Micro
//#define SerialAT Serial1

// or Software Serial on Uno, Nano
#include <SoftwareSerial.h>
SoftwareSerial SerialAT(18, 19); // RX, TX
//HardwareSerial SerialAT(1);

// See all AT commands, if wanted
// #define DUMP_AT_COMMANDS

// Define the serial console for debug prints, if needed
#define TINY_GSM_DEBUG SerialMon

// Range to attempt to autobaud
#define GSM_AUTOBAUD_MIN 9600
#define GSM_AUTOBAUD_MAX 115200

// Add a reception delay - may be needed for a fast processor at a slow baud rate
/* #define TINY_GSM_YIELD() \
    {                    \
        delay(2);        \
    } */

// Define how you're planning to connect to the internet
#define TINY_GSM_USE_GPRS true
#define TINY_GSM_USE_WIFI false

// set GSM PIN, if any
#define GSM_PIN ""

// Your GPRS credentials, if any
const char apn[] = "movistar.es";
const char gprsUser[] = "MOVISTAR";
const char gprsPass[] = "MOVISTAR";

// Your WiFi connection credentials, if applicable
const char wifiSSID[] = "";
const char wifiPass[] = "";

#include <TinyGsmClient.h>
#include <PubSubClient.h>

// Just in case someone defined the wrong thing..
#if TINY_GSM_USE_GPRS && not defined TINY_GSM_MODEM_HAS_GPRS
#undef TINY_GSM_USE_GPRS
#undef TINY_GSM_USE_WIFI
#define TINY_GSM_USE_GPRS false
#define TINY_GSM_USE_WIFI true
#endif
#if TINY_GSM_USE_WIFI && not defined TINY_GSM_MODEM_HAS_WIFI
#undef TINY_GSM_USE_GPRS
#undef TINY_GSM_USE_WIFI
#define TINY_GSM_USE_GPRS true
#define TINY_GSM_USE_WIFI false
#endif

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif
TinyGsmClient client(modem);
PubSubClient mqtt(client);

// ==================================================
// TODO MQTT. Constantes
//      - Dirección del servidor MQTT
//      - Tópico para subscripción
//      - Tópico para publicación
//      - ID cliente en el servidor MQTT
// ==================================================
const char *broker = "cr4v.rubenfgr.com";
const char *topicSub = "s/u/g/1"; // server/unit/generic/id
const char *topicPub = "n/u/g/1"; // node/unit/generic/id
const char *UNIT_GENERIC_ID = "ug1";

// ==================================================
// TODO Constantes
//      - Intentos de conexión a MQTT antes de reiniciar la SIM
//      - Velocidad de envio de datos
//      - Tiempo de espera para volver a intentar conectar
// ==================================================
const uint8_t NET_GPRS_ATTEMPTS_MAX = 3;
const uint16_t MQTT_RECONNECT_ATTEMPTS_MAX = 5;
const uint32_t RECCONECT_TIME = 10000;
uint32_t PUBLISH_DATA_SPEED = 600000;

// ==================================================
//  TODO Varaibles de tiempos
//      - Tiempo del último intento de conexión MQTT
//      - Número de intentos de conexión MQTT
//      - Tiempo de la última publicación de datos
//      - Estado del led de conexión MQTT
// ==================================================
uint32_t lastMQTTReconnectAttempt = 0;
uint8_t mqttReconnectAttempts = 0;
long lastPublishData = 0;
int ledStatus = LOW;

// ==================================================
//  TODO Variables de publicación
//      Si se ponen en "false" se enviaran los datos
// ==================================================
bool publishedCommunication = true;
bool publishedData = true;
bool publishedSIMData = true;
bool publishedSendSpeed = true;

// ==================================================
//  Tiempo de sincronización para el xTaskDelayUntil
// ==================================================
TickType_t xLastWakeTime;

// ==================================================
// TODO SIM. Declaración de funciones
// ==================================================
void initSIM();
void resetSIM();
void setupSIM(TickType_t &xLastWakeTimeP);
void loopSIM();

// ==================================================
// TODO MQTT. Declaración de funciones
// ==================================================
void subscribeOrder(String payload[]);
void mqttCallback(char *topic, byte *payload, unsigned int len);
void publish();
boolean mqttConnect();
void readOrders(String payload[]);
void readConfiguration(String payload[]);
void readSendSpeed(String payload[]);
void publishCommunication();
void publishData();
void publishSIMData();
void publishSendSpeed();

// ==================================================
//  TODO "setup" del módulo SIM
// ==================================================
void setupSIM(TickType_t &xLastWakeTimeP)
{
    xLastWakeTime = xLastWakeTimeP;
    SerialMon.println("setupSim()");
    pinMode(PIN_RESET_SIM, OUTPUT);
    pinMode(PIN_LED, OUTPUT);

    mqtt.setServer(broker, 1883);
    mqtt.setCallback(mqttCallback);
}

// ==================================================
//  TODO "loop" del módulo SIM
// ==================================================
void loopSIM()
{
    if (!mqtt.connected())
    {
        if (!modem.isNetworkConnected())
        {
            initSIM();
        }
        SerialMon.println("=== MQTT NOT CONNECTED ===");
        // Reconnect every 10 seconds
        uint32_t t = millis();
        if (t - lastMQTTReconnectAttempt > RECCONECT_TIME)
        {
            lastMQTTReconnectAttempt = t;
            if (mqttConnect())
            {
                lastMQTTReconnectAttempt = 0;
            }
        }
        return;
    }
    else
    {
        publish();
    }

    mqtt.loop();
}

// ==================================================
//  TODO Inicio de la SIM
// ==================================================
void initSIM()
{
    {
        SerialAT.end();

        digitalWrite(PIN_LED, LOW);
        SerialMon.println("initSIM() --> wait...");

        digitalWrite(PIN_RESET_SIM, LOW);
        vTaskDelayUntil(&xLastWakeTime, 2000);

        // Set GSM module baud rate
        //TinyGsmAutoBaud(SerialAT, GSM_AUTOBAUD_MIN, GSM_AUTOBAUD_MAX);
        //SerialAT.begin(115200, SERIAL_8N1, 16, 17, false);
        SerialAT.begin(9600);
        vTaskDelayUntil(&xLastWakeTime, 10000);

        digitalWrite(PIN_RESET_SIM, HIGH);

        SerialMon.println("modem.init()...");

        if (!modem.init())
        {
            SerialMon.println("ERROR!");
            setupSIM(xLastWakeTime);
        }
        SerialMon.println("OK!");
        vTaskDelayUntil(&xLastWakeTime, 100);

#if TINY_GSM_USE_GPRS
        // Unlock your SIM card with a PIN if needed
        if (GSM_PIN && modem.getSimStatus() != 3)
        {
            modem.simUnlock(GSM_PIN);
        }
#endif

#if TINY_GSM_USE_WIFI
        // Wifi connection parameters must be set before waiting for the network
        SerialMon.print(F("Setting SSID/password..."));
        if (!modem.networkConnect(wifiSSID, wifiPass))
        {
            SerialMon.println(" fail");
            vTaskDelayUntil(&xLastWakeTime, 10000);
            return;
        }
        SerialMon.println(" success");
#endif

#if TINY_GSM_USE_GPRS && defined TINY_GSM_MODEM_XBEE
        // The XBee must run the gprsConnect function BEFORE waiting for network!
        modem.gprsConnect(apn, gprsUser, gprsPass);
#endif

        SerialMon.println("Waiting for network...");
        uint8_t triesNetwork = 0;
        while (!modem.waitForNetwork())
        {
            SerialMon.println(" fail!");
            vTaskDelayUntil(&xLastWakeTime, 10000);
            if (triesNetwork == NET_GPRS_ATTEMPTS_MAX)
            {
                initSIM();
            }
            triesNetwork++;
        }
        SerialMon.println(" success!");
        vTaskDelayUntil(&xLastWakeTime, 100);

        if (modem.isNetworkConnected())
        {
            SerialMon.println("Network connected!");
        }
        else
        {
            initSIM();
        }

#if TINY_GSM_USE_GPRS
        // GPRS connection parameters are usually set after network registration
        SerialMon.println(("Connecting GPRS to " + (String)apn + "..."));
        uint8_t triesGPRS = 0;
        while (!modem.gprsConnect(apn, gprsUser, gprsPass))
        {
            SerialMon.println(" fail!");
            vTaskDelayUntil(&xLastWakeTime, 10000);
            if (triesGPRS == NET_GPRS_ATTEMPTS_MAX)
            {
                initSIM();
            }
            triesGPRS++;
        }
        SerialMon.println(" success!");

        if (modem.isGprsConnected())
        {
            SerialMon.println("GPRS connected!");
        }
        else
        {
            initSIM();
        }
#endif
        vTaskDelayUntil(&xLastWakeTime, 100);

        digitalWrite(PIN_LED, HIGH);
    }
}

// ==================================================
//  TODO MQTT. Conexión
// ==================================================
boolean mqttConnect()
{
    if (mqttReconnectAttempts == MQTT_RECONNECT_ATTEMPTS_MAX)
    {
        mqttReconnectAttempts = 0;
        initSIM();
    }
    mqttReconnectAttempts++;

    SerialMon.println("mqttConnect()...");

    boolean status = mqtt.connect(UNIT_GENERIC_ID);

    // Con autenticación
    //boolean status = mqtt.connect("GsmClientName", "mqtt_user", "mqtt_pass");

    if (status == false)
    {
        SerialMon.println("mqttConnect() return false");
        return false;
    }
    SerialMon.println("mqttConnect() return true");

    mqtt.subscribe(topicSub);
    return mqtt.connected();
}

// ==================================================
//  TODO MQTT. Recepción de paquetes
// ==================================================
void mqttCallback(char *topic, byte *payload, unsigned int len)
{
    SerialMon.println("mqttCallback()");

    unsigned int commas = 1;

    for (uint8_t i = 0; i < len; i++)
    {
        if ((char)payload[i] == ',')
        {
            commas++;
        }
    }

    String payloadS[commas + 1];
    int pos = 0;

    for (uint8_t i = 0; i < len; i++)
    {
        if ((char)payload[i] != ',')
        {
            payloadS[pos] += (char)payload[i];
        }
        else
        {
            pos++;
        }
    }

    for (uint8_t i = 0; i < commas; i++)
    {
        SerialMon.println(payloadS[i]);
    }

    subscribeOrder(payloadS);
}

// ==================================================
//  TODO MQTT. subscripción
// ==================================================
void subscribeOrder(String payloadS[])
{
    if (payloadS[0] == "1")
    {
        publishedCommunication = false;
    }
    if (payloadS[0] == "2")
    {
        publishedData = false;
    }
    if (payloadS[0] == "3")
    {
        publishedSIMData = false;
    }
    if (payloadS[0] == "5")
    {
        readOrders(payloadS);
    }
    if (payloadS[0] == "8")
    {
        readSendSpeed(payloadS);
        publishedSendSpeed = false;
    }
    if (payloadS[0] == "9")
    {
        readConfiguration(payloadS);
    }
}

// ==================================================
//  TODO MQTT. publicación
//  Si el tiempo actual menos el tiempo de la ultima publicación es mayor o igual a la velocidad
//  de envío, y si ha ocurrido un evento, se envían los datos
// ==================================================
void publish()
{
    long t = millis();

    if (t - lastPublishData >= PUBLISH_DATA_SPEED && getEvent() == 1)
    {
        lastPublishData = t;
        setEvent(0);
        publishedData = false;
    }
    if (!publishedCommunication)
    {
        publishCommunication();
        publishedCommunication = true;
    }
    if (!publishedData)
    {
        publishData();
        publishedData = true;
    }
    if (!publishedSIMData)
    {
        publishSIMData();
        publishedSIMData = true;
    }
    if (!publishedSendSpeed)
    {
        publishSendSpeed();
        publishedSendSpeed = true;
    }
}

// ==================================================
//  TODO 1 ==> publish 1
// ==================================================
void publishCommunication()
{
    const char *msg = "1";
    mqtt.publish(topicPub, msg);
    SerialMon.println("¡publicado!");
}

// ==================================================
//  TODO 2 ==> publish 2,lectura,caudal
// ==================================================
void publishData()
{
    String payloadString = "2," + (String)getLectura() + "," + (String)getCaudal();
    char payloadCharArray[payloadString.length()];
    payloadString.toCharArray(payloadCharArray, payloadString.length() + 1);
    mqtt.publish(topicPub, payloadCharArray);
    publishedData = true;
}

// ==================================================
//  TODO 3 ==> publish
// ==================================================
void publishSIMData()
{
    if (mqtt.connected())
    {
        String simOperator = modem.getOperator();
        String simSignal = (String)modem.getSignalQuality();
        String payloadString = "3," + simOperator + "," + simSignal;
        SerialMon.println(payloadString);
        char payload[payloadString.length()];
        payloadString.toCharArray(payload, payloadString.length() + 1);
        mqtt.publish(topicPub, payload);
    }
}

// ==================================================
//  TODO 5 ==> subscribe orden1,orden2,orden3,orden4,orden5
// ==================================================
void readOrders(String payload[])
{
    /* if (payload[1])
    {
        String order1 = payload[1];
    } */
    SerialMon.println("¡ordenes recibidas!");
}

// ==================================================
//  TODO 8 ==> subscribe 8,sendspeed
// ==================================================
void readSendSpeed(String payload[])
{
    if (payload[1] != nullptr && payload[1] != "")
    {
        PUBLISH_DATA_SPEED = payload[1].toInt();
        SerialMon.println("¡configuración de tiempo de envio recibida!");
    }
    else
    {
        SerialMon.println("¡configuración de tiempo de envio recibida INCORRECTA!");
    }
    // guardar en EEPROM
}

// ==================================================
//  TODO 8 ==> publish 8,sendspeed
// ==================================================
void publishSendSpeed()
{
    String payloadString = "8," + (String)PUBLISH_DATA_SPEED;
    char payloadCharArray[payloadString.length()];
    payloadString.toCharArray(payloadCharArray, payloadString.length() + 1);
    mqtt.publish(topicPub, payloadCharArray);
    publishedData = true;
}

// ==================================================
//  TODO 9 ==> subscribe 9,lectura
// ==================================================
void readConfiguration(String payload[])
{
    if (payload[1] != nullptr && payload[1] != "")
    {
        setLectura(payload[1].toInt());
        SerialMon.println("¡configuración recibida!");
    }
    else
    {
        SerialMon.println("¡configuración recibida INCORRECTA!");
    }
}
