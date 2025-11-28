# WARNING: This Code is still testing

# HAL Configuration
The STM32 I used for driving WS2812 matrix screen is STM32F103C8T6.

## Clock Configuration

HCLK: 64MHz

## Pinout & Configuration

**SPI2 MODE**: 	Full-Duplex Master

**Hardware NSS Signal**: 	Disable (CS_Pin: PB12, GPIO_Output)

## Parameter Settings 

**Frame Format**: Motorola

**Data Size**: 8 Bits

**First Bit**: MSB first

**Prescaler**: 16

**CPOL**: LOW

**CPHA**: 1 Edge

