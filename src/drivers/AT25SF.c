
#include <AT25SF.h>
#include <SaLPort.h>
#include <SaLBitBang.h>


uint8_t AT25SFGetByte(uint32_t address) {

    SaLDigitalOut(AT25SF_SLAVE_SELECT_PIN,false);
    byteOut(AT25SF_SCK_PIN,AT25SF_MOSI_PIN,OPCODE_SLOWREAD_ARRAY);
    byteOut(AT25SF_SCK_PIN,AT25SF_MOSI_PIN,((address & 0xFF0000) >> 16));
    byteOut(AT25SF_SCK_PIN,AT25SF_MOSI_PIN,((address & 0x00FF00) >>  8));
    byteOut(AT25SF_SCK_PIN,AT25SF_MOSI_PIN,((address & 0x0000FF) >>  0));

    uint8_t _byte = getByte(AT25SF_SCK_PIN,AT25SF_MISO_PIN);
    SaLDigitalOut(AT25SF_SLAVE_SELECT_PIN,true);

    return _byte;
}

void AT25SFWriteByte(uint32_t address, uint8_t byteToWrite) {

    SaLDigitalOut(AT25SF_SLAVE_SELECT_PIN,false);
    byteOut(AT25SF_SCK_PIN,AT25SF_MOSI_PIN,OPCODE_WRITEENABLE);
    SaLDigitalOut(AT25SF_SLAVE_SELECT_PIN,true);

    SaLDigitalOut(AT25SF_SLAVE_SELECT_PIN,false);
    byteOut(AT25SF_SCK_PIN,AT25SF_MOSI_PIN,OPCODE_PROGRAM);
    byteOut(AT25SF_SCK_PIN,AT25SF_MOSI_PIN,((address & 0xFF0000) >> 16));
    byteOut(AT25SF_SCK_PIN,AT25SF_MOSI_PIN,((address & 0x00FF00) >>  8));
    byteOut(AT25SF_SCK_PIN,AT25SF_MOSI_PIN,((address & 0x0000FF) >>  0));

    byteOut(AT25SF_SCK_PIN,AT25SF_MOSI_PIN,byteToWrite);
    SaLDigitalOut(AT25SF_SLAVE_SELECT_PIN,true);

}

void AT25SFErace32KBlock(uint8_t blockToErace) {

    uint32_t address = 0x008000*blockToErace;

    SaLDigitalOut(AT25SF_SLAVE_SELECT_PIN,false);
    byteOut(AT25SF_SCK_PIN,AT25SF_MOSI_PIN,OPCODE_WRITEENABLE);
    SaLDigitalOut(AT25SF_SLAVE_SELECT_PIN,true);

    SaLDigitalOut(AT25SF_SLAVE_SELECT_PIN,false);
    byteOut(AT25SF_SCK_PIN,AT25SF_MOSI_PIN,OPCODE_32K_ERASE);
    byteOut(AT25SF_SCK_PIN,AT25SF_MOSI_PIN,((address & 0xFF0000) >> 16));
    byteOut(AT25SF_SCK_PIN,AT25SF_MOSI_PIN,((address & 0x00FF00) >>  8));
    byteOut(AT25SF_SCK_PIN,AT25SF_MOSI_PIN,((address & 0x0000FF) >>  0));
    SaLDigitalOut(AT25SF_SLAVE_SELECT_PIN,true);

    delay_ms(300);
}

void AT25SFErace4KBlock(uint8_t blockToErace) {

    uint32_t address = 0x001000*blockToErace;

    SaLDigitalOut(AT25SF_SLAVE_SELECT_PIN,false);
    byteOut(AT25SF_SCK_PIN,AT25SF_MOSI_PIN,OPCODE_WRITEENABLE);
    SaLDigitalOut(AT25SF_SLAVE_SELECT_PIN,true);

    SaLDigitalOut(AT25SF_SLAVE_SELECT_PIN,false);
    byteOut(AT25SF_SCK_PIN,AT25SF_MOSI_PIN,OPCODE_04K_ERASE);
    byteOut(AT25SF_SCK_PIN,AT25SF_MOSI_PIN,((address & 0xFF0000) >> 16));
    byteOut(AT25SF_SCK_PIN,AT25SF_MOSI_PIN,((address & 0x00FF00) >>  8));
    byteOut(AT25SF_SCK_PIN,AT25SF_MOSI_PIN,((address & 0x0000FF) >>  0));
    SaLDigitalOut(AT25SF_SLAVE_SELECT_PIN,true);

    delay_ms(60);

}