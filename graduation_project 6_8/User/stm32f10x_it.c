/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "adc.h"
#include "usart2.h"
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.I/O线中断，中断线为PA0
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0)!=RESET)
 {
	 GPIO_WriteBit(GPIOD, GPIO_Pin_8, (BitAction)(1-GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_8)));
	 EXTI_ClearITPendingBit(EXTI_Line0);
 }
}
/**
  * @brief  This function handles USART DMA interrupt request.
  * @param  None
  * @retval None
  */
void DMA1_Channel7_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA1_FLAG_TC7)!=RESET)
 {
//	 GPIO_ResetBits(GPIOD, GPIO_Pin_11);
	
	 DMA_ClearFlag(DMA1_FLAG_TC7);
 }
}

/**
  * @brief  This function handles ADC DMA interrupt request.
  * @param  None
  * @retval None
  */
extern uint16_t ADC_ConvertedValue;
extern uint16_t flag1,flag2,flag3;
volatile uint16_t Preiod_value=1;
void DMA1_Channel1_IRQHandler(void)//中断服务子程序运行时间不要大于中断响应周期
{                                  //否则会漏掉中断程序
	static uint8_t div_count=0;
	if(DMA_GetFlagStatus(DMA1_FLAG_GL1)!=RESET)
	{ 
//		printf("%d ",ADC_ConvertedValue);
//	  Data_Send_Senser(ADC_ConvertedValue,flag1,flag2,flag3);//串口绘制波形
		if(++div_count==150)
		{
			Preiod_value=preiod(2600,ADC_ConvertedValue);
		  div_count=0;
		}
		DMA_ClearFlag(DMA1_FLAG_GL1);
	}		    
}

/**
  * @brief  This function handles BASIC_TIM6 interrupt request.
  * @param  None
  * @retval None
  */
uint16_t DDS_step=0,DDSM=1;
uint8_t mode_step=0;
void TIM6_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6,TIM_IT_Update)!=RESET)
	{
		DDS_step=DDS_step+DDSM;
		if(DDS_step>255)
		 {
			DDS_step=0;
			if(mode_step==0)
			mode_step++;
			else if(mode_step==1)
			mode_step=0;	
		 }
		TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
	}
		
}

/**
  * @brief  This function handles BASIC_TIM7 interrupt request.
  * @param  None
  * @retval None
  */
unsigned int tim7_counter1=0,tim7_counter2=0;
extern char time_interval;//引用变量要与源变量声明的类型相同
uint8_t stop_stage=0;
void TIM7_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM7,TIM_IT_Update)!=RESET)
	{
		
		if(++tim7_counter1>=1000)
		{
			tim7_counter2++;
			tim7_counter1=0;
			
		}
		if(tim7_counter2==time_interval)
			{
				stop_stage=0;
			}
			if(tim7_counter2==time_interval*2)
			{
				stop_stage=1;
			  tim7_counter2=0;
			}	
		TIM_ClearITPendingBit(TIM7,TIM_IT_Update);
	}
		
}

/**
  * @brief  This function handles USART2_RX interrupt request.
  * @param  None
  * @retval None
  */
extern char syn_state;
static char u2recdat;
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{ u2recdat=USART_ReceiveData(USART2);
		if(u2recdat==0X4F)
		{
		syn_state=0;//idle
		}
	}  
}
/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

