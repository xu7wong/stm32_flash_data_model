/*
 * main.c
 *
 *  Created on: 10/11/2023
 *      Author: carl
 *      STM32F103RCT6
 */
#include "data_struct.h"
#include "date_time.h"
#include "logging.h"
#include "stm32f1xx.h"

volatile bool SecondsUpdated = false;
int led = 0;
// struct __data_model_SUNSPEC SUNSPEC;
void Init_SysClock_72M(void);

int main(void) {
  Init_SysClock_72M();

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD |
                             RCC_APB2Periph_USART1,
                         ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_ResetBits(GPIOD, GPIO_Pin_2);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  USART_InitTypeDef USART_InitStructure;
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl =
      USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART1, &USART_InitStructure);
  USART_Cmd(USART1, ENABLE);
  SystemCoreClockUpdate();
  Logging("System Clock updated: %d\n", SystemCoreClock);

  uint32_t r_data = Init_Data_Model();
  
//   Logging("dm SUNSPEC.struct_version = 0x%X, SUNSPEC data size = %d\n",
//           SUNSPEC.struct_version, sizeof(SUNSPEC));
  Logging("dm init = %d, addr = 0x%X, SUNSPEC.crc32 = 0x%X\n", r_data,
          SUNSPEC.addr, SUNSPEC.crc32);
  // uint32_t *a = (uint32_t*)0x08020000;
  //(*((volatile uint32_t *)(0x08020000)))
  Logging("dm bank1 modified_date = %d, SUNSPEC size = %d\n",
         SUNSPEC.modified_date, sizeof(SUNSPEC));
  Logging("dm SUNSPEC.version2 = %d,%d\n", SUNSPEC.version2[0], SUNSPEC.version2[1]);
  Logging("dm SUNSPEC.lcd_control.day = %d, led_control.month = %d,%d\n", SUNSPEC.lcd_control.day, SUNSPEC.led_control.month[0], SUNSPEC.led_control.month[1]);
  
  Init_RTC();
  while (1) {
    if (SecondsUpdated) {
      SecondsUpdated = false;
      if (led > 0) {
        led = 0;
        GPIO_ResetBits(GPIOD, GPIO_Pin_2);
      } else {
        led = 1;
        GPIO_SetBits(GPIOD, GPIO_Pin_2);
      }
      // Logging("rtc=%d, %d\n",77,88);
      Logging_DateTime_Now();
    }

    //
  }
  return 0;
}

void Init_SysClock_72M(void) {
  // NVIC_InitTypeDef NVIC_InitStructure;

  // /* Enable and configure RCC global IRQ channel */
  // NVIC_InitStructure.NVIC_IRQChannel = RCC_IRQn;
  // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  // NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  // NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  // NVIC_Init(&NVIC_InitStructure);

  ErrorStatus HSEStartUpStatus;
  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS) {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);

    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1);

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

#ifdef STM32F10X_CL
    /* Configure PLLs *********************************************************/
    /* PLL2 configuration: PLL2CLK = (HSE / 5) * 8 = 40 MHz */
    RCC_PREDIV2Config(RCC_PREDIV2_Div5);
    RCC_PLL2Config(RCC_PLL2Mul_8);

    /* Enable PLL2 */
    RCC_PLL2Cmd(ENABLE);

    /* Wait till PLL2 is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLL2RDY) == RESET) {
    }

    /* PLL configuration: PLLCLK = (PLL2 / 5) * 9 = 72 MHz */
    RCC_PREDIV1Config(RCC_PREDIV1_Source_PLL2, RCC_PREDIV1_Div5);
    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_9);
#else
    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
#endif

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while (RCC_GetSYSCLKSource() != 0x08) {
    }
  }
  // RCC_ClocksTypeDef RCC_ClockFreq;
  /* This function fills the RCC_ClockFreq structure with the current
   frequencies of different on chip clocks (for debug purpose) */
  //   RCC_GetClocksFreq(&RCC_ClockFreq);

  /* Enable Clock Security System(CSS): this will generate an NMI exception
     when HSE clock fails */
  RCC_ClockSecuritySystemCmd(ENABLE);
}

void RTC_IRQHandler(void) {
  if (RTC_GetITStatus(RTC_IT_SEC) != RESET) {
    /* Clear the RTC Second interrupt */
    RTC_ClearITPendingBit(RTC_IT_SEC);

    SecondsUpdated = true;
    RTC_WaitForLastTask();
  }
}

// /**
//   * @brief  This function handles RCC interrupt request.
//   * @param  None
//   * @retval None
//   */
// void RCC_IRQHandler(void)
// {
//   if(RCC_GetITStatus(RCC_IT_HSERDY) != RESET)
//   {
//     /* Clear HSERDY interrupt pending bit */
//     RCC_ClearITPendingBit(RCC_IT_HSERDY);

//     /* Check if the HSE clock is still available */
//     if (RCC_GetFlagStatus(RCC_FLAG_HSERDY) != RESET)
//     {
// #ifdef SYSCLK_HSE
//       /* Select HSE as system clock source */
//       RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);
// #else
//  #ifdef STM32F10X_CL
//       /* Enable PLL2 */
//       RCC_PLL2Cmd(ENABLE);
//  #else
//       /* Enable PLL: once the PLL is ready the PLLRDY interrupt is generated
//       */ RCC_PLLCmd(ENABLE);
//  #endif	/* STM32F10X_CL */
// #endif /* SYSCLK_HSE */
//     }
//   }

// #ifdef STM32F10X_CL
//   if(RCC_GetITStatus(RCC_IT_PLL2RDY) != RESET)
//   {
//     /* Clear PLL2RDY interrupt pending bit */
//     RCC_ClearITPendingBit(RCC_IT_PLL2RDY);

//     /* Enable PLL: once the PLL is ready the PLLRDY interrupt is generated */
//     RCC_PLLCmd(ENABLE);
//   }
// #endif /* STM32F10X_CL */

//   if(RCC_GetITStatus(RCC_IT_PLLRDY) != RESET)
//   {
//     /* Clear PLLRDY interrupt pending bit */
//     RCC_ClearITPendingBit(RCC_IT_PLLRDY);

//     /* Check if the PLL is still locked */
//     if (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) != RESET)
//     {
//       /* Select PLL as system clock source */
//       RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
//     }
//   }
// }

/**
 * @brief  This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler(void) {
  /* This interrupt is generated when HSE clock fails */

  if (RCC_GetITStatus(RCC_IT_CSS) !=
      RESET) { /* At this stage: HSE, PLL are disabled (but no change on PLL
                  config) and HSI is selected as system clock source */

    /* Enable HSE */
    RCC_HSEConfig(RCC_HSE_ON);

    /* Enable HSE Ready interrupt */
    RCC_ITConfig(RCC_IT_HSERDY, ENABLE);

#ifndef SYSCLK_HSE
#ifdef STM32F10X_CL
    /* Enable PLL and PLL2 Ready interrupts */
    RCC_ITConfig(RCC_IT_PLLRDY | RCC_IT_PLL2RDY, ENABLE);
#else
    /* Enable PLL Ready interrupt */
    RCC_ITConfig(RCC_IT_PLLRDY, ENABLE);
#endif /* STM32F10X_CL */
#endif /* SYSCLK_HSE */

    /* Clear Clock Security System interrupt pending bit */
    RCC_ClearITPendingBit(RCC_IT_CSS);

    /* Once HSE clock recover, the HSERDY interrupt is generated and in the RCC
       ISR routine the system clock will be reconfigured to its previous state
       (before HSE clock failure) */
  }
}