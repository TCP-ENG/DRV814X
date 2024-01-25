#include <Arduino.h>
#include <SPI.h>
#include "drv814x.h"

_stat_Byte_ DRV814X::readReg(uint8_t reg)
{

    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    _command_byte_ commandByte;
    _stat_Byte_ statByte;
    commandByte.command_Byte.address=reg;
    commandByte.command_Byte.readWrite=0x01;
    commandByte.command_Byte.data=0x00;
    SerialUSB.println(commandByte.d16);
    digitalWrite(cs_pin, LOW);
    statByte.d16 = SPI.transfer16(commandByte.d16);
    digitalWrite(cs_pin,HIGH);
    SPI.endTransaction();
    return statByte;
}
_stat_Byte_ DRV814X::writeReg(uint8_t reg,uint8_t val)
{
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    _command_byte_ commandByte;
    _stat_Byte_ statByte;
    commandByte.command_Byte.address=reg;
    commandByte.command_Byte.readWrite=0x01;
    commandByte.command_Byte.data=val;
    digitalWrite(cs_pin, LOW);
    statByte.d16 = SPI.transfer16(commandByte.d16);
    digitalWrite(cs_pin,HIGH);
    SPI.endTransaction();
    return statByte;
}
_device_ID_ DRV814X::getID(void)
{
    _stat_Byte_ temp = readReg(DEVICE_ID);
    _device_ID_ dID;
    dID.d8 = temp.stat_Byte.DATA;
    return dID;
}

_fault_sum_ DRV814X::getFaultSum(void)
{
    _stat_Byte_ temp = readReg(FAULT_SUMMARY);
    _fault_sum_ fSum;
    fSum.d8 = temp.stat_Byte.DATA;
    return fSum;
}

_status1_ DRV814X::getStatus1(void)
{
    _stat_Byte_ temp = readReg(STATUS1);
    _status1_ stat;
    stat.d8 = temp.stat_Byte.DATA;
    return stat;
}

_status2_ DRV814X::getStatus2(void)
{
    _stat_Byte_ temp = readReg(STATUS2);
    _status2_ stat;
    stat.d8 = temp.stat_Byte.DATA;
    return stat;
}

_command_ DRV814X::getCommand(void)
{
    _stat_Byte_ temp = readReg(COMMAND);
    _command_ cmd;
    cmd.d8 = temp.stat_Byte.DATA;
    return cmd;
}

void DRV814X::setCommand(_command_ cmd)
{
    writeReg(COMMAND, cmd.d8);
}

_spi_in_ DRV814X::getSPIin(void)
{
    _stat_Byte_ temp = readReg(SPI_IN);
    _spi_in_ spi_in;
    spi_in.d8 = temp.stat_Byte.DATA;
    return spi_in;
}

void DRV814X::setSPIin(_spi_in_ sIn)
{
    writeReg(SPI_IN, sIn.d8);
}

_config1_ DRV814X::getConfig1(void)
{
    _stat_Byte_ temp = readReg(CONFIG1);
    _config1_ cfg;
    cfg.d8 = temp.stat_Byte.DATA;
    return cfg;
}

void DRV814X::setConfig1(_config1_ cfg)
{
    writeReg(CONFIG1, cfg.d8);
}

_config2_ DRV814X::getConfig2(void)
{
    _stat_Byte_ temp = readReg(CONFIG2);
    _config2_ cfg;
    cfg.d8 = temp.stat_Byte.DATA;
    return cfg;
}

void DRV814X::setConfig2(_config2_ cfg)
{
    writeReg(CONFIG2, cfg.d8);
}

_config3_ DRV814X::getConfig3(void)
{
    _stat_Byte_ temp = readReg(CONFIG3);
    _config3_ cfg;
    cfg.d8 = temp.stat_Byte.DATA;
    return cfg;
}

void DRV814X::setConfig3(_config3_ cfg)
{
    writeReg(CONFIG3, cfg.d8);
}

_config4_ DRV814X::getConfig4(void)
{
    _stat_Byte_ temp = readReg(CONFIG4);
    _config4_ cfg;
    cfg.d8 = temp.stat_Byte.DATA;
    return cfg;
}

void DRV814X::setConfig4(_config4_ cfg)
{
    writeReg(CONFIG4, cfg.d8);
}

void DRV814X::setPWM(uint8_t duty)
{
    analogWrite(drive_pin,duty);
}
uint16_t DRV814X::getAIPROP(void)
{
    _device_ID_ temp = getID();
    return getAIPROP(temp.d8);
}
uint16_t DRV814X::getAIPROP(uint8_t dID)
{
    switch (dID)
    {
    case DRV8143S:
        return 3050;
        break;
    case DRV8144S:
        return 4630;
        break;
    case DRV8145S:
        return 6230;
        break;
    case DRV8143P:
        return 3040;
        break;
    case DRV8145P:
        return 5980;
        break;
    
    default:
        break;
    }
}
float DRV814X::current(float volts)
{
    float IPROPI = volts/RIPROP;
    return IPROPI * AIPROP;
}
void DRV814X::setS_ITRIP(uint8_t s_itrip)
{
    _config2_ cfg;
    cfg.config2.S_ITRIP = s_itrip;
    writeReg(CONFIG2,cfg.d8);
}
void DRV814X::setS_ITRIP(float cur, uint8_t hiLow)
{
    float ITrip[8];
    for(int x = 1; x < 8; x++)
    {
        ITrip[x]=current(VTrip[x]);
        if(ITrip[x] >= cur)
        {
            if(hiLow == 1)
            {
                setS_ITRIP(x);
                x = 9;
            }
            else
            {
                x--;
                if(x<1)
                {
                    x=1;
                }
                setS_ITRIP(x);
                x = 9;
            }
        }
    }
}

DRV814X::DRV814X()
{

}

DRV814X::DRV814X(uint8_t dpin, uint8_t cspin, int rp)
{
    drive_pin = dpin;
    cs_pin = cspin;
    RIPROP = rp;
    AIPROP = getAIPROP();
}

DRV814X::~DRV814X()
{
}

void DRV814X::setDrivePin(uint8_t dPin)
{
    drive_pin = dPin;
}
void DRV814X::setCSpin(uint8_t csPin)
{
    cs_pin=csPin;
}
void DRV814X::setRIProp(int riprop)
{
    RIPROP=riprop;
}