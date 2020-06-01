#include "../application/uart.h"

#include <em_usart.h>
#include <em_cmu.h>
#include <em_gpio.h>

volatile char newchar = '0'; // Soros porton érkezõ karakter
volatile uint8_t position = 0; // Kosár pozició
volatile uint8_t gameIsRunning = 0; // Játékengedélyezõ flag
volatile uint8_t delayTimeFactor = 10;	// Játék sebesség szabályozó
volatile uint8_t newPiece = 0;
volatile uint8_t reDrawLCD = 1;
volatile uint8_t map[10] = {3,2,0,1,1,0,3,3,3,0};
volatile uint8_t mapIndex = 0;

void UART0_RX_IRQHandler(void) // UART INT HANDLER
{
	newchar = (char) USART_RxDataGet(UART0);
	if(newchar=='d' && position < 3){
		position++;
		reDrawLCD = 1;
		newchar = 'q';
	}

	if(newchar=='a' && position > 0){
		position--;
		newchar = 'q';
		reDrawLCD = 1;
  	}
	if(newchar=='s'){
		gameIsRunning = 1;
		newchar = 'q';
		reDrawLCD = 1;
  	}
	if(newchar=='+'){
		if(delayTimeFactor != 1){delayTimeFactor--;}
		newchar = 'q';
	}
	if(newchar=='-'){
		if(delayTimeFactor != 9){delayTimeFactor++;}
		newchar = 'q';
 	}
	if( ((newchar-0x30)<4) && ((newchar-0x30)>=0) ){
		if(mapIndex==10){mapIndex = 0;}
		map[mapIndex] = newchar-0x30;
		mapIndex++;
	}
	else{
		newchar = 'q';
	}
}

void uart_init()
{
//UART0 konfigurálása a 4 gyakorlat kódja alapján:
  //Configure UATR0: 11520 Baud, Frame format 81N
    //Location 1 routing

       //Enable CLK for UATRT0
       CMU_ClockEnable(cmuClock_UART0, true);

       //Actual configuration
       USART_InitAsync_TypeDef uinit;

       uinit.autoCsEnable = false;
       uinit.baudrate = 115200;
       uinit.databits = usartDatabits8;
       uinit.enable = usartEnable;
       uinit.mvdis = false;
       uinit.oversampling = usartOVS16;
       uinit.parity = usartNoParity;
       uinit.prsRxCh = usartPrsRxCh0;
       uinit.prsRxEnable = false;
       uinit.refFreq = 0;
       uinit.stopbits = usartStopbits1;

       USART_InitAsync(UART0, &uinit);

       UART0->ROUTE |= (USART_ROUTE_TXPEN | USART_ROUTE_RXPEN);
       UART0->ROUTE |= (USART_ROUTE_LOCATION_LOC1);

       CMU_ClockEnable(cmuClock_GPIO, true);
       GPIO_PinModeSet(gpioPortE, 0, gpioModePushPull, 1); // TX
       GPIO_PinModeSet(gpioPortE, 1, gpioModeInput, 0);    // RX
       GPIO_PinModeSet(gpioPortF, 7, gpioModePushPull, 1); // Enable to debugger

       USART_IntEnable(UART0,UART_IF_RXDATAV);
       //Interrupt enable
       NVIC_EnableIRQ(UART0_RX_IRQn);


}
