#ifndef nrf24l01_h
#define nrf24l01_h
#include "Arduino.h"

/*
------------------------------------------------------------------------------------------
nrf24l01.h                                                                    
Author: Junon M.                                                               
Date: 2022/07/17 
Version: 1.0.2                                                             
Arduino 1.8.16                                                     
------------------------------------------------------------------------------------------
*/

//----------------------------------------------------------------------------------------
#define  RX_DR  6
#define  TX_DS  5
#define  MAX_RT 4
//----------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------
// SPI(nrf24l01) commands
#define READ_REG        0x00  // Define read command to register
#define WRITE_REG       0x20  // Define write command to register
#define RD_RX_PLOAD     0x61  // Define RX payload register address
#define WR_TX_PLOAD     0xA0  // Define TX payload register address
#define FLUSH_TX        0xE1  // Define flush TX register command
#define FLUSH_RX        0xE2  // Define flush RX register command
#define REUSE_TX_PL     0xE3  // Define reuse TX payload register command
#define NOP             0xFF  // Define No Operation, might be used to read status register
//----------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------
// SPI(nrf24l01) registers(addresses)
#define CONFIG          0x00  // 'Config' register address
#define EN_AA           0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR       0x02  // 'Enabled RX addresses' register address
#define SETUP_AW        0x03  // 'Setup address width' register address
#define SETUP_RETR      0x04  // 'Setup Auto. Retrans' register address
#define RF_CH           0x05  // 'RF channel' register address
#define RF_SETUP        0x06  // 'RF setup' register address
#define STATUS          0x07  // 'Status' register address
#define OBSERVE_TX      0x08  // 'Observe TX' register address
#define CD              0x09  // 'Carrier Detect' register address
#define RX_ADDR_P0      0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1      0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2      0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3      0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4      0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5      0x0F  // 'RX address pipe5' register address
#define TX_ADDR         0x10  // 'TX address' register address
#define RX_PW_P0        0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1        0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2        0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3        0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4        0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5        0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS     0x17  // 'FIFO Status Register' register address
//----------------------------------------------------------------------------------------



class nrf24l01
{
  private:

    int 
    _Pin_CE, 
    _Pin_CSN, 
    _Pin_SCK, 
    _Pin_MOSI, 
    _Pin_MISO, 
    _Pin_IRQ;

    int _lenBuff;
    int _Channel; 
    int _txAddr; 
    int _rxAddr; 

    unsigned int _wait_ack;
    uint8_t  _ACK;
    uint8_t * _rxBuff;
    uint8_t * _txBuff;

  public:


    nrf24l01();

    void set_pinout(int CE, int CSN, int SCK, int MOSI, int MISO, int IRQ);

    void set_config(int lenBuff, int ch, int txAddr, int rxAddr);

    uint8_t spi_rw(uint8_t in_data);

    uint8_t rw_reg(uint8_t Addr, uint8_t D);

    void send_cmd(uint8_t cmd);

    uint8_t send_data();

    uint8_t receive_data();

    void write_data(uint8_t * data);

    uint8_t * read_data();

    void set_channel(int channel);

    uint8_t get_irq();
};

#endif /* nrf24l01_h */
