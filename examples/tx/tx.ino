/*
------------------------------------------------------------------------------------------
Example: tx.ino                                                                    
Author: Junon M.                                                               
Date: 2022/07/17 
Version: 1.0.2                                                             
Arduino 1.8.16                                                     
------------------------------------------------------------------------------------------
*/

#include "nrf24l01.h"

nrf24l01 nRF24;

void setup()
{
  Serial.begin(115200);
  
  //nRF24.set_pinout(CE, CSN, SCK, MOSI, MISO, IRQ);
  nRF24.set_pinout(/*CE=*/9, /*CSN=*/10, /*SCK=*/13, /*MOSI=*/11, /*MISO=*/12, /*IRQ=*/8);

  //nRF24.set_config(int lenBuff, int ch, int txAddr, int rxAddr)
  nRF24.set_config(/*lenBuff=*/ 11, /*ch=*/ 50, /*txAddr=*/ 75, /*rxAddr=*/ 75);
  
  ReadStatus();
}

void loop()
{
  unsigned int idx = 0;
  uint8_t dadoEnviado = 0;  

    Serial.println(" ");
    Serial.println("Modo transmissor ativado!");
    Serial.println(" ");
    nRF24.send_cmd(FLUSH_RX);
    nRF24.send_cmd(FLUSH_TX);
    ReadStatus();
       
    idx = 0;   
    while (1){
           
      String S = "Pack " + String(idx);

      nRF24.write_data((uint8_t *)S.c_str());
           
      dadoEnviado = nRF24.send_data();
 
      if (dadoEnviado == 1)
      {
        Serial.print(">");
        Serial.print(S);
      }
      else 
      {
        Serial.println("Erro ao enviar os dados!");
      }
      
      Serial.println(" ");
      idx++;
       
      delay(20);     
    } 
}



void ReadStatus(){
  
   uint8_t S = nRF24.rw_reg(READ_REG|STATUS, NOP);
   Serial.println(" ");
   Serial.print("Status = ");   
   Serial.println(S);
   if (S == 14)
   {
     Serial.println("O sistema foi iniciado com sucesso.");
   }
   Serial.println(" "); 
} 
