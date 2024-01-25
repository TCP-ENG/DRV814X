#ifndef _DRV814X_H_
#define _DRV814X_H_

#include "Arduino.h"

#define DEVICE_ID 0x00
#define FAULT_SUMMARY 0x01
#define STATUS1 0x02
#define STATUS2 0x03
#define COMMAND 0x08
#define SPI_IN  0x09
#define CONFIG1 0x0A
#define CONFIG2 0x0B
#define CONFIG3 0x0C
#define CONFIG4 0x0D

#define DRV8143S    0xBA
#define DRV8144S    0xCA
#define DRV8145S    0xDA
#define DRV8143P    0xBE
#define DRV8145P    0xDE


typedef union _command_byte_
{
    uint16_t d16;
    struct 
    {
        uint8_t data:8;
        uint8_t address:6;
        uint8_t readWrite:1;
        uint8_t NOT_USED:1;
        
    }command_Byte;
};
typedef union _stat_Byte_
{
    uint16_t d16; 
    struct 
    {
        uint8_t DATA:8;
        uint8_t SPI_E_RR:1;
        uint8_t TSD:1;
        uint8_t OCP:1;
        uint8_t VMUV:1;
        uint8_t VMOV:1;
        uint8_t FAULT:1;
        uint8_t NOT_USED:2;
    }stat_Byte;
};
typedef union _device_ID_
{
    uint8_t d8;
    struct 
    {
        uint8_t REV_ID:2;
        uint8_t DEV_ID:6;
    }device_ID;
};
typedef union _fault_sum_
{
    uint8_t d8;
    struct
    {
        uint8_t VCPUV:1;
        uint8_t TSD:1;
        uint8_t OCP:1;
        uint8_t VMUV:1;
        uint8_t VMOV:1;
        uint8_t FAULT:1;
        uint8_t POR:1;
        uint8_t SPI_ERR:1;
    }fault_sum;   
};
typedef union _status1_
{
    uint8_t d8;
    struct
    {
        uint8_t OCP_L_2:1;
        uint8_t OCP_H_2:1;
        uint8_t OCP_L:1;
        uint8_t OCP_H:1;
        uint8_t ACTIVE:1;
        uint8_t ITRIP_CMP:1;
        uint8_t OLA:2;
    }status1;
};
typedef union _status2_
{    
    uint8_t d8;
    struct
    {
        uint8_t OLP_CMP:1;
        uint8_t NOT_USED2:3;
        uint8_t ACTIVE:1;
        uint8_t NOT_USED:2;
        uint8_t DRVOFF_STAT:1;
    }status2;
};
typedef union _command_
{
    uint8_t d8;
    struct 
    {
        uint8_t REG_LOCK:2;
        uint8_t NOT_USED2:1;
        uint8_t SPI_IN_LOCK:2;
        uint8_t NOT_USED:2;
        uint8_t CLR_FLT:1;
    }command;
};
typedef union _spi_in_
{
    uint8_t d8;
    struct 
    {
        uint8_t S_IN:1;
        uint8_t SPARE:2;
        uint8_t S_DRVOFF:1;
        uint8_t NOT_USED:4;
    }spi_in;  
};
typedef union _config1_
{
    uint8_t d8;
    struct 
    {
        uint8_t OLA_RETRY:1;
        uint8_t VMOV_RETRY:1;
        uint8_t TSD_RETRY:1;
        uint8_t OCP_RETRY:1;
        uint8_t SSC_DIS:1;
        uint8_t VMOV_SEL:2;
        uint8_t EN_OLA:1;
    }config1;  
};
typedef union _config2_
{
    uint8_t d8;
    struct 
    {
        uint8_t S_ITRIP:3;
        uint8_t NOT_USED:2;
        uint8_t S_DIAG:2;
        uint8_t SPARE:1;
    }config2;
};
typedef union _config3_
{
    uint8_t d8;
    struct
    {
        uint8_t SPARE:2;
        uint8_t S_SR:3;
        uint8_t NOT_USED:1;
        uint8_t TOFF:2;
    }config3;
};
typedef union _config4_
{
    uint8_t d8;
    struct
    {
        uint8_t IN_SEL:1;
        uint8_t SPARE:1;
        uint8_t DRVOFF_SEL:1;
        uint8_t OCP_SEL:2;
        uint8_t NOT_USED:1;
        uint8_t TOCP_SEL:2;
    }config4;
};   
class DRV814X
{
private:
    /* data */
    uint8_t drive_pin;
    uint8_t cs_pin;
    int RIPROP;
    uint32_t AIPROP;
    float VTrip[8] = {0,1.18,1.41,1.65,1.98,2.31,2.64,2.97};
    _stat_Byte_ readReg(uint8_t reg);
    _stat_Byte_ writeReg(uint8_t reg, uint8_t value);
public:
    DRV814X(uint8_t dpin, uint8_t cspin,int rp);
    DRV814X();
    ~DRV814X();
    _device_ID_ getID(void);
    _fault_sum_ getFaultSum(void);
    _status1_ getStatus1(void);
    _status2_ getStatus2(void);
    _command_ getCommand(void);
    void setCommand(_command_ cmd);
    _spi_in_ getSPIin(void);
    void setSPIin(_spi_in_ spiin);
    _config1_ getConfig1(void);
    void setConfig1(_config1_ cfg1);
    _config2_ getConfig2(void);
    void setConfig2(_config2_ cfg2);
    _config3_ getConfig3(void);
    void setConfig3(_config3_ cfg3);
    _config4_ getConfig4(void);
    void setConfig4(_config4_ cfg4);
    void setPWM(uint8_t duty);
    float current(float volts);
    uint16_t getAIPROP(void);
    uint16_t getAIPROP(uint8_t dID);
    void setS_ITRIP(uint8_t s_itrip);
    void setS_ITRIP(float cur, uint8_t hiLow);
    void setDrivePin(uint8_t dPin);
    void setCSpin(uint8_t csPin);
    void setRIProp(int riprop);

};



#endif
