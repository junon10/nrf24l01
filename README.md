# nrf24l01 Arduino Library

## Description

- Library used to control Nordic nrf24l01 radios, in a simple way and with low memory usage on the Arduino board.

## Requirements

- **Arduino Compatibility**: Works with most Arduino boards. Tested on Arduino Uno/Nano.

## Installation

To install the library:

1. Download the repository from GitHub:
   [https://github.com/junon10/nrf24l01](https://github.com/junon10/nrf24l01)

2. In Arduino IDE, go to Sketch > Include Library > Add ZIP File...

3. Select the downloaded zip file.

## Example

See the examples folder.

## Usage

Channel(1-126)
lenBuff(1-32)
txAddr(1-255)
rxAddr(1-255) 
                                                                               
NOTE: The channels must be the same in the modules that will communicate, the buffer 
must also be the same valid from 1 to 32 Bytes, txAddr and rxAddr too 
must be the same (address), the transmission power has been configured 
to 0dbm (maximum), transfer rate at 250Kbps, autoACK activated, and 
no packet retransmission, for other configurations consult the 
datasheet.

```c++
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
```

## Author

- **Junon M.**  
  Contact: [junon10@tutamail.com](mailto:junon10@tutamail.com)

## Contributing

Contributions are welcome! Please fork the repository and send a pull request.

## Repository

- [https://github.com/junon10/nrf24l01](https://github.com/junon10/nrf24l01)

## References

Datasheet NORDIC nrf24l01+

Based on the source code for Pic microcontrollers by Ezequiel Donhauser
https://github.com/ezequieldonhauser/NRF24L01P

## Changelog

- **v1.0.0 (2022/07/17)**: Initial commit.
- **v1.0.1 (2022/07/17)**: Changed the library name.
- **v1.0.2 (2023/05/22)**: Minor bug fixes.


