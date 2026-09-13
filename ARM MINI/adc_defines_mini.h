/*------------------------------------------------------------
File    : adc_defines_mini.h
Purpose : Define clock constants, ADC register bit positions,
          analog input pin masks, and channel numbers for LPC2129.
------------------------------------------------------------*/
#define FOSC 12000000									// Main oscillator frequency used in the system

#define PCLK (CCLK/4)									// Peripheral clock runs at 1/4 of CPU clock
#define ADCCLK 3000000								// Target ADC clock frequency = 3MHz for sensor conversion
#define CLKDIV ((PCLK/ADCCLK)-1)			// ADC clock divider value calculated from PCLK and ADCCLK	

//------------------------------------------------------------
// ADCR (ADC Control Register) bit definitions
//------------------------------------------------------------
#define CLKDIV_BITS        8 					// Bit position for CLKDIV field start in ADCR (bits 8-15)
#define PDN_BIT            21					// ADC Power-down control bit position in ADCR
#define ADC_CONV_START_BIT 24					// ADC conversion start bit position in ADCR

//------------------------------------------------------------
// ADDR (ADC Data Register) bit definitions
//------------------------------------------------------------
#define DIGITAL_DATA_BITS 6						// ADC result data bit start position in ADDR (bits 6-15)
#define DONE_BIT          31					// ADC conversion complete status bit in ADDR	

//------------------------------------------------------------
// Analog input pin masks mapped in PINSEL1 register
//------------------------------------------------------------
#define AIN0_PIN_0_27 0x00400000			// Mask to configure P0.27 as ADC channel 0 input		
#define AIN1_PIN_0_28 0x01000000			// Mask to configure P0.28 as ADC channel 1 input	
#define AIN2_PIN_0_29 0x04000000			// Mask to configure P0.29 as ADC channel 2 input	
#define AIN3_PIN_0_30 0x10000000			// Mask to configure P0.30 as ADC channel 3 input	

//------------------------------------------------------------
// Channel number aliases for readability
//------------------------------------------------------------
#define CH0 0													// Alias for ADC channel 0
#define CH1 1													// Alias for ADC channel 1
#define CH2 2													// Alias for ADC channel 2
#define CH3 3													// Alias for ADC channel 3	
//add mores defines as & when required
