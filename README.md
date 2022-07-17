# nrf24l01 Arduino Library

# Functions:

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

# Configurations: 

Channel(1-126)
lenBuff(1-32)
txAddr(1-255)
rxAddr(1-255) 
                                                                               
OBS: Os canais devem ser o mesmo nos módulos que se comunicarão, o buffer 
tambem deve ser o mesmo válidos de 1 a 32 Bytes, txAddr e rxAddr tambem 
devem ser os mesmos(endereço), a potencia de transmissão foi configurada 
para 0dbm(maxima), taxa de transferencia em 250Kbps, autoACK ativado, e 
nenhuma retransmissão de pacotes, para outras configurações consulte o 
datasheet.

# Tested on

* Arduino Uno (OK)

# Changes

`1.0.0` - Commit Inicial

`1.0.1` - Mudança de nome de library

# Example

See `examples`.

# References

Datasheet NORDIC nrf24l01+

Baseada no código fonte para microcontroladores Pic de Ezequiel Donhauser
https://github.com/ezequieldonhauser/NRF24L01P
