#include "nrf24l01.h"

/*
------------------------------------------------------------------------------------------
nrf24l01.cpp                                                                    
Autor: Junon M.                                                               
Data: 17/07/2022 
Version: 1.0.1                                                             
Compilador: Arduino 1.8.16                                                     
------------------------------------------------------------------------------------------
*/

//----------------------------------------------------------------------------------------
nrf24l01::nrf24l01() {

  // Default Values
  //
  _lenBuff = 32; // 1 a 32
  _Channel = 50;  // 1 a 126
  _txAddr =  75;  // 1 a 255
  _rxAddr =  75;  // 1 a 255
}
//----------------------------------------------------------------------------------------
void nrf24l01::set_pinout(int CE, int CSN, int SCK, int MOSI, int MISO, int IRQ){

   _Pin_CE = CE;
   _Pin_CSN = CSN; 
   _Pin_SCK = SCK; 
   _Pin_MOSI = MOSI; 
   _Pin_MISO = MISO; 
   _Pin_IRQ = IRQ;  
  
   pinMode(_Pin_IRQ, INPUT);
   pinMode(_Pin_MISO, INPUT);
   
   pinMode(_Pin_CSN, OUTPUT);
   pinMode(_Pin_CE, OUTPUT);
   pinMode(_Pin_SCK, OUTPUT);
   pinMode(_Pin_MOSI, OUTPUT);
   
   digitalWrite(_Pin_CSN, HIGH);
   digitalWrite(_Pin_CE, HIGH); // LOW 
   digitalWrite(_Pin_SCK, LOW);
   digitalWrite(_Pin_MOSI, LOW);
}
//----------------------------------------------------------------------------------------
void nrf24l01::set_config(int lenBuff, int ch, int txAddr, int rxAddr){

   _lenBuff = lenBuff;
   _Channel = ch; 
   _txAddr =  txAddr; 
   _rxAddr =  rxAddr; 

   _rxBuff = (uint8_t*) realloc(_rxBuff, (_lenBuff) * sizeof(uint8_t));
   _txBuff = (uint8_t*) realloc(_txBuff, (_lenBuff) * sizeof(uint8_t));

   for (int i = 0; i < _lenBuff; i++)
   {
      _rxBuff[i] = 0;
      _txBuff[i] = 0;
   }  

   delay(5);

   //configura modulo SPI
   //setup_spi(SPI_MASTER|SPI_L_TO_H|SPI_XMIT_L_TO_H
   //|SPI_CLK_DIV_4|SPI_SAMPLE_AT_END);
   
   digitalWrite(_Pin_CE, LOW);

   //RX_ADDR_P0 - configura endereço de recepção PIPE0
   digitalWrite(_Pin_CSN, LOW);
   spi_rw(0x2A);
   spi_rw(_rxAddr);
   spi_rw(0xC2);
   spi_rw(0xC2);
   spi_rw(0xC2);
   spi_rw(0xC2);
   digitalWrite(_Pin_CSN, HIGH);
   
   //TX_ADDR - configura endereço de transmissão
   digitalWrite(_Pin_CSN, LOW);
   spi_rw(0x30);
   spi_rw(_txAddr);
   spi_rw(0xC2);
   spi_rw(0xC2);
   spi_rw(0xC2);
   spi_rw(0xC2);
   digitalWrite(_Pin_CSN, HIGH);

   //EN_AA - habilita autoACK no PIPE0
   rw_reg(WRITE_REG|EN_AA, 0x01);

   //EN_RXADDR - ativa o PIPE0
   rw_reg(WRITE_REG|EN_RXADDR, 0x01);

   //SETUP_AW - define o endereço com tamanho de 5 Bytes
   rw_reg(WRITE_REG|SETUP_AW, 0x03);

   //SETUP_RETR - configura para nao retransmitir pacotes
   rw_reg(WRITE_REG|SETUP_RETR, 0x00);

   //RF_CH - define o canal do modulo (TX e RX devem ser iguais)
   rw_reg(WRITE_REG|RF_CH, _Channel);

   //RF_SETUP - ativa LNA, taxa em 250K, e maxima potencia 0dbm
   rw_reg(WRITE_REG|RF_SETUP, 0b00100110);
   
   rw_reg(WRITE_REG|STATUS, 0x70);//Status Reset

   //RX_PW_P0 - tamanho do buffer PIPE0
   rw_reg(WRITE_REG|RX_PW_P0, _lenBuff);

   //CONFIG - coloca em modo de recepção, e define CRC de 2 Bytes
   rw_reg(WRITE_REG|CONFIG, 0x0F);
   
   send_cmd(FLUSH_RX);
   send_cmd(FLUSH_TX);
   
   //tempo para sair do modo standby entrar em modo de recepção
   delay(2);
   digitalWrite(_Pin_CE, HIGH);
   delayMicroseconds(150);//delay_us(150);
}
//----------------------------------------------------------------------------------------
uint8_t nrf24l01::spi_rw(uint8_t in_data)
{
  uint8_t bit_ctr = 0, D = 0;
  D = in_data;

  // Arduino
  for(bit_ctr = 0; bit_ctr < 8; bit_ctr++ ) // output 8-bit
  {
     // output 'byte', MSB to MOSI
     if (D & 0x80) digitalWrite(_Pin_MOSI, HIGH);
     else digitalWrite(_Pin_MOSI, LOW);
     
     D = (D << 1); // shift next bit into MSB..
     
     digitalWrite(_Pin_SCK, HIGH); // Set SCK high..
     
     if (digitalRead(_Pin_MISO)) bitSet(D, 0); //capture current MISO bit
     else bitClear(D, 0);
     
     digitalWrite(_Pin_SCK, LOW);  // ..then set SCK low again
  }
  
  /* //Microchip Pic
  for(bit_ctr = 0; bit_ctr < 8; bit_ctr++ ) // output 8-bit
  {
     Pin_MOSI = (D & 0x80); // output 'byte', MSB to MOSI
     D = (D << 1);      // shift next bit into MSB..
     Pin_SCK = 1;           // Set SCK high..
     D |= Pin_MISO;         // capture current MISO bit
     Pin_SCK = 0;           // ..then set SCK low again
  }
  */
  return(D);            // return read byte
}
//----------------------------------------------------------------------------------------
uint8_t nrf24l01::rw_reg(uint8_t Addr, uint8_t D)
{
   uint8_t byte_Read;
   digitalWrite(_Pin_CSN, LOW);
   spi_rw(Addr);
   byte_Read = spi_rw(D);
   digitalWrite(_Pin_CSN, HIGH);
   return(byte_Read); 
}
//----------------------------------------------------------------------------------------
void nrf24l01::send_cmd(uint8_t cmd)
{
  digitalWrite(_Pin_CSN, LOW);
  spi_rw(cmd);
  digitalWrite(_Pin_CSN, HIGH);
}
//----------------------------------------------------------------------------------------
uint8_t nrf24l01::send_data()
{   
   uint8_t i, nRF_Status;
   
   digitalWrite(_Pin_CE, LOW);

   rw_reg(WRITE_REG|STATUS, 0x70);//Status Reset

   // W_TX_PAYLOAD - envia os dados para o buffer FIFO TX 
   digitalWrite(_Pin_CSN, LOW);
   spi_rw(WR_TX_PLOAD);

   for (i=0; i < _lenBuff; i++) spi_rw(_txBuff[i]);
   
   digitalWrite(_Pin_CSN, HIGH);

   //CONFIG - ativa modo de transmissão
   rw_reg(WRITE_REG|CONFIG, 0x0E);

   //pulso para transmitir os dados
   digitalWrite(_Pin_CE, HIGH);
   delayMicroseconds(15);
   digitalWrite(_Pin_CE, LOW);

   _wait_ack = 0;

   while(digitalRead(_Pin_IRQ) != 0){
      _wait_ack++;
      delayMicroseconds(12);
      //espera 5ms, pela recepção do pacote ACK
      if(_wait_ack > 400){
      break;
      }
   }

   nRF_Status = rw_reg(READ_REG|STATUS, NOP);
   
   rw_reg(WRITE_REG|STATUS, 0x70);//Status Reset

   //limpa os buffers
   send_cmd(FLUSH_RX);
   send_cmd(FLUSH_TX);

   //CONFIG - configura para modo de recepção
   rw_reg(WRITE_REG|CONFIG, 0x0F);

   digitalWrite(_Pin_CE, HIGH);
   
   delayMicroseconds(150);

   //se não recebeu ACK em 5ms retorna 0
   if(_wait_ack > 400){
     return(0);
   }
   else
   { //se recebeu ACK retorna 1
     return(1);
   }
}
//----------------------------------------------------------------------------------------
uint8_t nrf24l01::receive_data()
{
   uint8_t i, nRF_Status;

   nRF_Status = rw_reg(READ_REG|STATUS, NOP);
   rw_reg(WRITE_REG|STATUS, 0x70);//Status Reset
   
   //verifica o bit de recepção de dados
   if (bitRead(nRF_Status, RX_DR) == 0)
   {
     return(0); 
   }
   
   //RD_RX_PAYLOAD - recebe os dados do buffer FIFO RX
   digitalWrite(_Pin_CSN, LOW);

   spi_rw(RD_RX_PLOAD);
   
   for(i = 0; i < _lenBuff; i++) _rxBuff[i] = spi_rw(0);   

   digitalWrite(_Pin_CSN, HIGH);
      
   return(1);
}
//----------------------------------------------------------------------------------------
void nrf24l01::write_data(uint8_t * data){
  memcpy(_txBuff, data, _lenBuff);
}
//----------------------------------------------------------------------------------------
uint8_t * nrf24l01::read_data(){
  return _rxBuff;
}
//----------------------------------------------------------------------------------------
void nrf24l01::set_channel(int channel){
  _Channel = channel;
  //RF_CH - define o canal do modulo (TX e RX devem ser iguais)
  rw_reg(WRITE_REG|RF_CH, _Channel);
} 
//----------------------------------------------------------------------------------------
uint8_t nrf24l01::get_irq(){
  return digitalRead(_Pin_IRQ);
}
