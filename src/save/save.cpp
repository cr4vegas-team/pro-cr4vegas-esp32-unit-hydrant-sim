#include <EEPROM.h>
#include "../main.h"

// ==================================================
//  Memoria FLASH
// ==================================================
uint8_t FLASH_SIZE = 50;
uint8_t P1_FLASH_POSITION = 0;

uint32_t readLectura()
{
    EEPROM.begin(FLASH_SIZE);
    uint32_t lectura = EEPROM.readLong(0);
    return lectura;
}

void saveLectura(uint32_t lectura)
{
    SerialMon.println("save core: " + (String)xPortGetCoreID());
    EEPROM.begin(FLASH_SIZE);
    EEPROM.writeLong(P1_FLASH_POSITION, lectura);
    EEPROM.commit();
    EEPROM.end();
}
