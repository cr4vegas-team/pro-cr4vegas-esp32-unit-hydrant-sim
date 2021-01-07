# Unidad de control "Hidrante"

**Placa: ESP32 WOROOM 32**
**Autor:** Rubén Francisco Gazquez Rosales
**Descripción:** Nodo final para las unidades de control de tipo hidrante.
   Su uso es exclusivo para GPRS (NO WIFI).
   Existen 4 tipos de publicación: comunicación, datos, datos de SIM y velocidad de envío. Solo los
   son considerados de vital importancia por lo que se realiza un envío "retenido (retained)" hacia
   el servidor para que persistan y el servidor pueda obtener en cada momento el último estado de los
   sensores y actuadores. Es la única publicación que se comprueba en el envío y si no fue exitoso,
   en cada iteración se intenta volver a enviar.
   El resto de publicaciónes se consideran "en tiempo de solicitud" y su importancia esta en la 
   comunicación cliente/servidor/nodo en el momento.

## Librerías

- https://github.com/knolleary/pubsubclient
- http://librarymanager/all#PubSubClient
- https://tiny.cc/tinygsm-readme

## Configuración MQTT

| MQTT                    | Valor                                 |
| ----------------------- | ------------------------------------- |
| Tópico de subscripción: | "server/test" --> "s/u/h/x"           |
| Tópico de publicación:  | "client/test" --> "n/u/h/x"           |
| Server MQTT:            | "mqtt.rubenfgr.com"                   |
| APN Things-Mobile:      | "TM"                                  |
| APN Movistar            | "movistar.es", "MOVISTAR", "MOVISTAR" |

>**p** = propiedad
>**o** = orden

### Arrays de publicación soportados

| Array                                                       | Descripción                    |
| ----------------------------------------------------------- | ------------------------------ |
| 1                                                           | comunicación                   |
| 2,electrovalvula,lectura,caudal,boyaBaja,boyaMedia,boyaAlta | datos de sensores y actuadores |
| 3,operador,señal,ip                                         | datos de la sim                |
| 8,ss                                                        | velocidad de envio de datos    |

### Arrays de subscripción soportados

| Array            | Descripción                                  |
| ---------------- | -------------------------------------------- |
| 1                | comunicación                                 |
| 2                | datos de sensores                            |
| 3                | datos de la sim                              |
| 5,electrovalvula | ordenes a actuadores                         |
| 7,ss             | configuración de velocidad de envio de datos |
| 9,lectura        | configuración                                |

## Pines

### ENTRADAS (inputs)

| PIN | Destino      |
| --- | ------------ |
| D18 | SIM900L RX   |
| D19 | SIM900L TX   |
| D21 | SIM900 RESET |
| D13 | contador     |
| D12 | boyaBaja     |
| D14 | boyaMedia    |
| D27 | boyaAlta     |

### SALIDAS (outputs)

| PIN | Origen         |
| --- | -------------- |
| D34 | electrovalvula |
| D35 | sensor2        |
| D32 | sensor3        |
| D33 | sensor4        |
| D25 | sensor5        |

## Diagrama de flujo

Este diagrama es adaptable a todas las unidades de control. La lectura y cálculos sobre los sensores siempre se realizan sobre el núcleo 0 y el envío y recepción de datos sobre el servidor MQTT desde el núcleo 1. La memoria flash es compartida a los dos núcleos por lo que se debe tener especial precaución a la hora de realizar escrituras sobre los datos.

![Diagrama de flujo](cr4v-esp32-diagrama-de-flujo.jpg)