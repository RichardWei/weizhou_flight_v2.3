#ifndef __DMA_H
#define __DMA_H	 
#include "sys.h"


void MYDMA_Config(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar,u16 cndtr);//����DMA1_CHx

void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx);//ʹ��DMA1_CHx

void DMA1_USART1_SEND(u32 SendBuff,u16 len);//DMA---USART1����
		 				    
#endif
