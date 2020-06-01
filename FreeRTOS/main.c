#include "em_device.h"
#include "em_chip.h"

#include <stdint.h>
#include "segmentlcd.h"
#include <udelay.h>

#include "FreeRTOS.h"
#include "task.h"
#include "../application/lcdDraw.h"
#include "../application/uart.h"

extern uint8_t position;
extern uint8_t delayTimeFactor;
extern uint8_t gameIsRunning;
extern uint8_t reDrawLCD;

#define  mainTASK_HI_PRIORITY    ( tskIDLE_PRIORITY + 3 )
#define  mainTASK_MID_PRIORITY    ( tskIDLE_PRIORITY + 2 )
#define  mainTASK_LO_PRIORITY    ( tskIDLE_PRIORITY + 1 )
#define  mainTASK_LO_STACK_SIZE  configMINIMAL_STACK_SIZE

#define LIFE_TO_LOSE 10

//Globális változók
TaskHandle_t LCDHandle;
TaskHandle_t gameHandle;
TaskHandle_t gameOverHandle;

uint8_t score = 0; //elért pontszám
uint8_t bananaY = 0; //banán függõleges helyzete
uint8_t bananaX = 0;
uint8_t round = 0; //körök száma
uint8_t life = LIFE_TO_LOSE;
extern uint8_t map[10];


void LCDTask(){

	while(1){
		if(reDrawLCD){
			lcdClearLowerSegment();
			lcdDrawBasket(position);
			lcdDrawBanana(bananaX,bananaY);
			lcdDrawScore(score,life);
			reDrawLCD = 0;
		}

		vTaskDelay(20/portTICK_PERIOD_MS);
	}
}

void gameTask(){

	while (1) {
		if((gameIsRunning) && (life!=0)){
		  reDrawLCD = 1;

	      //Banán érés
	      if(bananaY<4){bananaY++;}
	      else{bananaY=0;}

	      //Elkapás
	      if(4==bananaY){ (position==bananaX) ? score++ : life--; }

	      //Új banán rajzolás
	      if(4==bananaY){
	    	  round++;
	    	  bananaX = map[(round%10)];
	      }

	      vTaskDelay(delayTimeFactor*50/ portTICK_PERIOD_MS);
		}

	vTaskDelay(250/ portTICK_PERIOD_MS);
	}
}

void gameOverTask(){
	while(1){
		if((life==0) || (score==99)){
			if(score>9){
				USART_Tx(UART0,(score/10)+0x30);
				USART_Tx(UART0,(score%10)+0x30);
			}
			else{
				USART_Tx(UART0,0+0x30);
				USART_Tx(UART0,score+0x30);
			}
			score = 0;
			round = 0;
			delayTimeFactor = 10;
			life = LIFE_TO_LOSE;
			gameIsRunning = 0;
			for(int i = 0;i<10;i++){map[i] = i%4;}
		}
		vTaskDelay(1000/portTICK_PERIOD_MS);

	}
}


int main(void){
  /* Chip errata */
  CHIP_Init();

  SegmentLCD_Init(false);
  uart_init();

  xTaskCreate(LCDTask,"",mainTASK_LO_STACK_SIZE,NULL,mainTASK_LO_PRIORITY,&LCDHandle);
  xTaskCreate(gameTask,"",mainTASK_LO_STACK_SIZE,NULL,mainTASK_MID_PRIORITY,&gameHandle);
  xTaskCreate(gameOverTask,"",mainTASK_LO_STACK_SIZE,NULL,mainTASK_HI_PRIORITY,&gameOverHandle);

  vTaskStartScheduler();

  return 0;
}
