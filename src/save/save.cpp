#include <EEPROM.h>
#include "../debugger/debugger.h"
#include "../sensors/sensors.h"

// ==================================================
//  Memoria FLASH
// ==================================================
int FLASH_SIZE = 50;
int POS_LECTURA = 0;
int POS_CAPTADOR = 10;

void readDataFromFlash();

void setupSave()
{
    EEPROM.begin(FLASH_SIZE);
    readDataFromFlash();
}

void readDataFromFlash()
{
    setLectura(EEPROM.readLong(POS_LECTURA));
    setCaptador(EEPROM.readInt(POS_CAPTADOR));
}

void saveDataOnFlash()
{
    EEPROM.writeLong(POS_LECTURA, getLectura());
    EEPROM.writeInt(POS_CAPTADOR, getCaptador());
    EEPROM.commit();
}
