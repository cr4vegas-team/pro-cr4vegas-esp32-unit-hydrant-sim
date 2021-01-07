#include <EEPROM.h>
#include "../debugger/debugger.h"
#include "../sensors/sensors.h"

// ==================================================
//  Memoria FLASH
// ==================================================
int FLASH_SIZE = 50;
int POS_LECTURA = 0;
int POS_BOYA_BAJA = 10;
int POS_BOYA_MEDIA = 20;
int POS_BOYA_ALTA = 30;

void readDataFromFlash();

void setupSave()
{
    EEPROM.begin(FLASH_SIZE);
    readDataFromFlash();
}

void readDataFromFlash()
{
    setLectura(EEPROM.readLong(POS_LECTURA));
    setBoyaBaja(EEPROM.readInt(POS_BOYA_BAJA));
    setBoyaMedia(EEPROM.readInt(POS_BOYA_MEDIA));
    setBoyaAlta(EEPROM.readInt(POS_BOYA_ALTA));
}

void saveDataOnFlash()
{
    EEPROM.writeLong(POS_LECTURA, getLectura());
    EEPROM.writeInt(POS_BOYA_BAJA, getBoyaBaja());
    EEPROM.writeInt(POS_BOYA_MEDIA, getBoyaMedia());
    EEPROM.writeInt(POS_BOYA_ALTA, getBoyaAlta());
    EEPROM.commit();
}
