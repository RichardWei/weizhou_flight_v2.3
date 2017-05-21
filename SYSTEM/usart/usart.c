#include "sys.h"
#include "usart.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
//ʹucos,ͷļ.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos ʹ	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//ֻѧϰʹãδɣκ;
//ALIENTEK STM32
//1ʼ		   
//ԭ@ALIENTEK
//̳:www.openedv.com
//޸:2012/8/18
//汾V1.5
//ȨУؾ
//Copyright(C) ӿƼ޹˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3޸˵ 
//֧ӦͬƵµĴڲ.
//˶printf֧
//˴ڽ.
//printfһַʧbug
//V1.4޸˵
//1,޸ĴڳʼIObug
//2,޸USART_RX_STA,ʹôֽΪ214η
//3,USART_REC_LEN,ڶ崮յֽ(214η)
//4,޸EN_USART1_RXʹܷʽ
//V1.5޸˵
//1,˶UCOSII֧
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//´,֧printf,Ҫѡuse MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//׼Ҫֺ֧                 
struct __FILE 
{ 
	int handle; 

}; 

FILE* __stdout;       
//_sys_exit()Աʹðģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//ضfputc 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ,ֱ   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*ʹmicroLibķ*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
#if EN_USART1_RX   //ʹ˽
//1жϷ
//ע,ȡUSARTx->SRܱĪĴ   	
u8 USART_RX_BUF[USART_REC_LEN];     //ջ,USART_REC_LENֽ.
//״̬
//bit15	ɱ־
//bit14	յ0x0d
//bit13~0	յЧֽĿ
u16 USART_RX_STA=0;       //״̬	  
  
void uart_init(u32 bound){
    //GPIO˿
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹUSART1GPIOAʱ
     //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

   //Usart1 NVIC 

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//ռȼ3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//ȼ3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨʹ
	NVIC_Init(&NVIC_InitStructure);	//ָĲʼVICĴ
  
   //USART ʼ

	USART_InitStructure.USART_BaudRate = bound;//һΪ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//ֳΪ8λݸʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//żУλ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//Ӳ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//շģʽ

    USART_Init(USART1, &USART_InitStructure); //ʼ
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//ж
    USART_Cmd(USART1, ENABLE);                    //ʹܴ 

}

void USART1_IRQHandler(void)                	//1жϷ
	{
	u8 Res;
#ifdef OS_TICKS_PER_SEC	 	//ʱӽ,˵ҪʹucosII.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //ж(յݱ0x0d 0x0aβ)
		{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//ȡյ
		
		if((USART_RX_STA&0x8000)==0)//δ
			{
			if(USART_RX_STA&0x4000)//յ0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//մ,¿ʼ
				else USART_RX_STA|=0x8000;	// 
				}
			else //ûյ0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//ݴ,¿ʼ	  
					}		 
				}
			}   		 
     } 
#ifdef OS_TICKS_PER_SEC	 	//ʱӽ,˵ҪʹucosII.
	OSIntExit();  											 
#endif
} 
#endif	

