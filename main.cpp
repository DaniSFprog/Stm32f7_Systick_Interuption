#include <stdio.h>
#include <STM32F7xx.h>
#include <math.h> 
short estado = 0;
int BCD [17] = {0X01,0X4F,0x12,0x06,0x4C,0X24,0X20,0X0F,0X00,0X04,0x08,0X60,0X31,0X42,0X30,0x38,0X7E};
int cont = 0;
int delay_10ms = 0;
int delay_1s = 0;
int delay_1200ms =0;
bool sentido = 0;
short polling_display=0; // Contador con el que hace el polling del display
int display;
int digY [4];
int digX [4];
int digZ [4];
int aleatorioX;
int aleatorioZ;
//Funcion mostrar X	
	void mostrar_X(){	
		digX[0]=display/10; 																//Digito decena
		digX[1]=(display-digX[0]*10); 											//Digito u
		switch(polling_display){ //switch polling del display
				case 0:
					GPIOD->ODR =0x0E;
					GPIOB->ODR =BCD[digX[1]];
				break;
			case 1:
					GPIOD->ODR =0x0D;
					GPIOB->ODR =BCD[digX[0]];
				break;
			case 2:
					GPIOD->ODR =0x0B;
					GPIOB->ODR =BCD[16];
				break;
			case 3:
					GPIOD->ODR =0x07;
					GPIOB->ODR =BCD[10];
				break;			
			}//fin switch polling del display	
	}//fin Funcion visualizacion 
//Funcion mostrar Y
	void mostrar_Y(){	
		digY[0]=display/1000; 																//Digito mil
		digY[1]=(display-digY[0]*1000)/100; 									//Digito centena
		digY[2]=(display-digY[0]*1000-digY[1]*100)/10; 				//Digito decena
		digY[3]=(display-digY[0]*1000-digY[1]*100-digY[2]*10);//Digito unidad	
		switch(polling_display){ //switch polling del display
				case 0:
					GPIOD->ODR =0x0E;
					GPIOB->ODR =BCD[digY[3]];
				break;
			case 1:
					GPIOD->ODR =0x0D;
					GPIOB->ODR =BCD[digY[2]];
				break;
			case 2:
					GPIOD->ODR =0x0B;
					GPIOB->ODR =BCD[digY[1]];
				break;
			case 3:
					GPIOD->ODR =0x07;
					GPIOB->ODR =BCD[digY[0]];
				break;			
			}//fin switch polling del display
	}//fin Funcion visualizacion 
//Funcion mostrar Z
	void mostrar_Z(){	
		digZ[1]=display/100; 																	//Digito centena							
		digZ[2]=(display-digZ[1]*100)/10; 									  //Digito decena
		digZ[3]=(display-digZ[1]*100-digZ[2]*10); 						//Digito unidad
		switch(polling_display){ //switch polling del display
			case 0:
					GPIOD->ODR =0x0E;
					GPIOB->ODR =BCD[15];
				break;
			case 1:
					GPIOD->ODR =0x0D;
					GPIOB->ODR =BCD[digZ[3]];
				break;
			case 2:
					GPIOD->ODR =0x0B;
					GPIOB->ODR =BCD[digZ[2]];
				break;
			case 3:
					GPIOD->ODR =0x07;
					GPIOB->ODR =BCD[digZ[1]];
				break;			
			}//fin switch polling del display	
	}//fin Funcion visualizacion 	
extern "C" {
			void EXTI15_10_IRQHandler(void){
			EXTI->PR |= 0x2000;      //limpia la bandera de la interrupcion	de sentido
				sentido ^=1;
//				if (sentido == 0){sentido = 1;}
//				else{sentido = 0;}	
			}
			void EXTI0_IRQHandler(void){
			EXTI->PR |= 0x1;			//limpia la bandera de la interrupcion Pulsador X	
				estado=1;					
			}
			void EXTI1_IRQHandler(void){
			EXTI->PR |= 0x2;      //limpia la bandera de la interrupcion Pulsador Y	
				estado=2;
			}
			void EXTI2_IRQHandler(void){
			EXTI->PR |= 0x4;      //limpia la bandera de la interrupcion Pulsador Z	
				estado=3;	
			}			
		 void SysTick_Handler(void){
//*****POLLING DISPLAY***************************
			 	polling_display++;
				if(polling_display>3){
				polling_display=0; 
				}
//*****FIN POLLING DISPLAY************************		
//******ESTADO 1****ALEATORIO 20-50 Y A********************************************				 
			 if(estado==1){				 
				display=aleatorioX;
				delay_1s++;
				if(delay_1s==500){ // Divisor de frecuencia 1s	
				aleatorioX=20+rand()%(50-20+1);			
				delay_1s=0;
				}// fin if divisor de freccuencia 1s		
			 }// fin if estado 1
//******FIN ESTADO 1****ALEATORIO 20-50 Y A********************************************
//******ESTADO 2***CONTADOR 0-9999****************************
			 if(estado==2){
				 display=cont;
				 delay_10ms++;		 
				 if(delay_10ms==10){ // Divisor de frecuencia 10ms	
						if(sentido==0){
							if (cont<9999 && cont>=0 ){ //conteo hasta 9999 y que vuelva a 0
								cont++;
							}//if comparacion y suma 
								else{
								cont=0;
								}//fin else sentido ascendente
							}//sentido ascendente
						if(sentido==1){
							if (cont<=9999 && cont>0){  //conteo hasta 0 y que vuelva a 9999
								cont--;
							}//if comparacion y resta
								else{
								cont=9999;
								}//fin else sentido descendente					
						}//fin sentido descendente 
						delay_10ms=0;		
					}// fin if divisor de freccuencia 10ms
				}// fin if estado 2
//******FIN ESTADO 2***ALEATORIO 20-50 Y A*************************
//******ESTADO 3************************************************		
 			 if(estado==3){	
				display=aleatorioZ;				 
				delay_1200ms++;
				if(delay_1200ms==1200){ // Divisor de frecuencia 1200ms	
				aleatorioZ=1+rand()%(999-1+1);						
					delay_1200ms=0;
				}// fin if divisor de freccuencia 1200ms		
			 }// fin if estado 3
//******FIN ESTADO 3***F Y ALEATORIO 0-999 Y A*************************								 
		 }// fin systick		 
		}// fin extern
  int main(void)	{
		//CONFIGURACION "CLOCK"
		RCC->AHB1ENR |= 0x0000002E;    //PRENDER EL CLOCK DEL PB  PC PD Y PF
		RCC->APB2ENR |= 0x4000;          //System configuration controller clock enable
		//CONFIGURACION PINES B
		GPIOB->MODER =  0x00001555;        //Pines del PB0 al PB7 como salida  7 segmentos
		GPIOB->OTYPER = 0;
		GPIOB->OSPEEDR = 0x00001555;       //medium speed
		GPIOB->PUPDR =  0x00001555;        //pull up
		//CONFIGURACION PINES D
		//HABILITADORES
		GPIOD->MODER |=  0x00000055;        //Pines del PD0 al PD3 como salida  4 habilitadores
		GPIOD->OTYPER |= 0;
		GPIOD->OSPEEDR |= 0x000000AA;       //alta velocidad
		GPIOD->PUPDR |=  0x00000000;        //no pull up no pull down
		//CONFIGURACION PINES C		
		//BOTON DE USUARIO
		GPIOC -> MODER  |=  0x00000000;
		GPIOC->PUPDR 		|=  0x00000000;		
		//BOTONES DE CASO
		GPIOF->MODER |=  0x00000000;
		GPIOF->PUPDR |=  0x00000000;	
		//CONFIGURACION SYSTICK 		
		SystemCoreClockUpdate();
		SysTick_Config(SystemCoreClock/1000);         //1ms  		
		//CONFIGURACION EXTI
		SYSCFG->EXTICR[0] =0x0555; //PF0-1-2
		SYSCFG->EXTICR[3] =0x0020; //PC13					
		EXTI->IMR |= 0X2007;
		EXTI->FTSR |= 0X2005;
		EXTI->RTSR |= 0X0002;		
		NVIC_EnableIRQ(EXTI0_IRQn);							//Habilitar interrupcion 0
		NVIC_EnableIRQ(EXTI1_IRQn); 						//Habilitar interrupcion 1
		NVIC_EnableIRQ(EXTI2_IRQn);             //Habilitar interrupcion 2			
		NVIC_EnableIRQ(EXTI15_10_IRQn);         //Habilitar la interrupción	13
	while(true){		//bucle infinito
		if(estado==0){
			GPIOB->ODR |= BCD[16]; // Visualiza el led G
		}
		if(estado==1){
			mostrar_X();		
		}
		if(estado==2){
			mostrar_Y();
		}	
		if(estado==3){
			mostrar_Z();
		}			
}//main
	}//while