/*
  This file is modified by liuyufanlyf in 02/08/2021 to make it
  suitable for GD32F30x_hd series
*/ 

/**
  ******************************************************************************
  * @file      startup_gd32f30x_hd.s
  * @author    MCD Application Team
  * @brief     STM32F411xExx Devices vector table for GCC based toolchains. 
  *            This module performs:
  *                - Set the initial SP
  *                - Set the initial PC == Reset_Handler,
  *                - Set the vector table entries with the exceptions ISR address
  *                - Branches to main in the C library (which eventually
  *                  calls main()).
  *            After Reset the Cortex-M4 processor is in Thread mode,
  *            priority is Privileged, and the Stack is set to Main.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
    
  .syntax unified
  .cpu cortex-m4
  .fpu softvfp
  .thumb

.global  g_pfnVectors
.global  Default_Handler

/* start address for the initialization values of the .data section. 
defined in linker script */
.word  _sidata
/* start address for the .data section. defined in linker script */  
.word  _sdata
/* end address for the .data section. defined in linker script */
.word  _edata
/* start address for the .bss section. defined in linker script */
.word  _sbss
/* end address for the .bss section. defined in linker script */
.word  _ebss
/* stack used for SystemInit_ExtMemCtl; always internal RAM used */

/**
 * @brief  This is the code that gets called when the processor first
 *          starts execution following a reset event. Only the absolutely
 *          necessary set is performed, after which the application
 *          supplied main() routine is called. 
 * @param  None
 * @retval : None
*/

    .section  .text.Reset_Handler
  .weak  Reset_Handler
  .type  Reset_Handler, %function
Reset_Handler:  
  ldr   sp, =_estack    		 /* set stack pointer */

/* Copy the data segment initializers from flash to SRAM */  
  movs  r1, #0
  b  LoopCopyDataInit

CopyDataInit:
  ldr  r3, =_sidata
  ldr  r3, [r3, r1]
  str  r3, [r0, r1]
  adds  r1, r1, #4
    
LoopCopyDataInit:
  ldr  r0, =_sdata
  ldr  r3, =_edata
  adds  r2, r0, r1
  cmp  r2, r3
  bcc  CopyDataInit
  ldr  r2, =_sbss
  b  LoopFillZerobss
/* Zero fill the bss segment. */  
FillZerobss:
  movs  r3, #0
  str  r3, [r2], #4
    
LoopFillZerobss:
  ldr  r3, = _ebss
  cmp  r2, r3
  bcc  FillZerobss

/* Call the clock system intitialization function.*/
  bl  SystemInit   
/* Call static constructors */
    bl __libc_init_array
/* Call the application's entry point.*/
  bl  main
  bx  lr    
.size  Reset_Handler, .-Reset_Handler

/**
 * @brief  This is the code that gets called when the processor receives an 
 *         unexpected interrupt.  This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
 * @param  None     
 * @retval None       
*/
    .section  .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
  b  Infinite_Loop
  .size  Default_Handler, .-Default_Handler
/******************************************************************************
*
* The minimal vector table for a Cortex M3. Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
* 
*******************************************************************************/
   .section  .isr_vector,"a",%progbits
  .type  g_pfnVectors, %object
  .size  g_pfnVectors, .-g_pfnVectors
    
g_pfnVectors:
  .word  _estack
  .word  Reset_Handler
  .word  NMI_Handler
  .word  HardFault_Handler
  .word  MemManage_Handler
  .word  BusFault_Handler
  .word  UsageFault_Handler
  .word  0
  .word  0
  .word  0
  .word  0
  .word  SVC_Handler
  .word  DebugMon_Handler
  .word  0
  .word  PendSV_Handler
  .word  SysTick_Handler
  
  /* External Interrupts */
  .word     WWDGT_IRQHandler                  /* 16:Window WatchDog                        */
  .word     LVD_IRQHandler                    /* 17:LVD through EXTI Line detect           */
  .word     TAMPER_IRQHandler                 /* 18:Tamper through the EXTI line detect    */
  .word     RTC_IRQHandler                    /* 19:RTC through the EXTI line              */
  .word     FMC_IRQHandler                    /* 20:FMC                                    */
  .word     RCU_CTC_IRQHandler                /* 21:RCU and CTC                            */
  .word     EXTI0_IRQHandler                  /* 22:EXTI Line0                             */
  .word     EXTI1_IRQHandler                  /* 23:EXTI Line1                             */
  .word     EXTI2_IRQHandler                  /* 24:EXTI Line2                             */
  .word     EXTI3_IRQHandler                  /* 25:EXTI Line3                             */
  .word     EXTI4_IRQHandler                  /* 26:EXTI Line4                             */
  .word     DMA0_Channel0_IRQHandler          /* 27:DMA0 Channel0                          */
  .word     DMA0_Channel1_IRQHandler          /* 28:DMA0 Channel1                          */
  .word     DMA0_Channel2_IRQHandler          /* 29:DMA0 Channel2                          */
  .word     DMA0_Channel3_IRQHandler          /* 30:DMA0 Channel3                          */
  .word     DMA0_Channel4_IRQHandler          /* 31:DMA0 Channel4                          */
  .word     DMA0_Channel5_IRQHandler          /* 32:DMA0 Channel5                          */
  .word     DMA0_Channel6_IRQHandler          /* 33:DMA0 Channel6                          */
  .word     ADC0_1_IRQHandler                 /* 34:ADC0, ADC1                             */
  .word     USBD_HP_CAN0_TX_IRQHandler        /* 35:USBD HP and CAN0 TX                    */
  .word     USBD_LP_CAN0_RX0_IRQHandler       /* 36:USBD LP and CAN0 RX0                   */
  .word     CAN0_RX1_IRQHandler               /* 37:CAN0 RX1                               */
  .word     CAN0_EWMC_IRQHandler              /* 38:CAN0 EWMC                              */
  .word     EXTI5_9_IRQHandler                /* 39:EXTI5 to EXTI9                         */
  .word     TIMER0_BRK_IRQHandler             /* 40:TIMER0 Break                           */
  .word     TIMER0_UP_IRQHandler              /* 41:TIMER0 Update                          */
  .word     TIMER0_TRG_CMT_IRQHandler         /* 42:TIMER0 Trigger and Commutation         */
  .word     TIMER0_Channel_IRQHandler         /* 43:TIMER0 Channel Capture Compare         */
  .word     TIMER1_IRQHandler                 /* 44:TIMER1                                 */
  .word     TIMER2_IRQHandler                 /* 45:TIMER2                                 */
  .word     TIMER3_IRQHandler                 /* 46:TIMER3                                 */
  .word     I2C0_EV_IRQHandler                /* 47:I2C0 Event                             */
  .word     I2C0_ER_IRQHandler                /* 48:I2C0 Error                             */
  .word     I2C1_EV_IRQHandler                /* 49:I2C1 Event                             */
  .word     I2C1_ER_IRQHandler                /* 50:I2C1 Error                             */
  .word     SPI0_IRQHandler                   /* 51:SPI0                                   */
  .word     SPI1_IRQHandler                   /* 52:SPI1                                   */
  .word     USART0_IRQHandler                 /* 53:USART0                                 */
  .word     USART1_IRQHandler                 /* 54:USART1                                 */
  .word     USART2_IRQHandler                 /* 55:USART2                                 */
  .word     EXTI10_15_IRQHandler              /* 56:EXTI10 to EXTI15                       */
  .word     RTC_Alarm_IRQHandler              /* 57:RTC Alarm                              */
  .word     USBD_WKUP_IRQHandler              /* 58:USBD Wakeup                            */
  .word     TIMER7_BRK_IRQHandler             /* 59:TIMER7 Break                           */
  .word     TIMER7_UP_IRQHandler              /* 60:TIMER7 Update                          */
  .word     TIMER7_TRG_CMT_IRQHandler         /* 61:TIMER7 Trigger and Commutation         */
  .word     TIMER7_Channel_IRQHandler         /* 62:TIMER7 Channel Capture Compare         */
  .word     ADC2_IRQHandler                   /* 63:ADC2                                   */
  .word     EXMC_IRQHandler                   /* 64:EXMC                                   */
  .word     SDIO_IRQHandler                   /* 65:SDIO                                   */
  .word     TIMER4_IRQHandler                 /* 66:TIMER4                                 */
  .word     SPI2_IRQHandler                   /* 67:SPI2                                   */
  .word     UART3_IRQHandler                  /* 68:UART3                                  */
  .word     UART4_IRQHandler                  /* 69:UART4                                  */
  .word     TIMER5_IRQHandler                 /* 70:TIMER5                                 */
  .word     TIMER6_IRQHandler                 /* 71:TIMER6                                 */
  .word     DMA1_Channel0_IRQHandler          /* 72:DMA1 Channel0                          */
  .word     DMA1_Channel1_IRQHandler          /* 73:DMA1 Channel0                          */
  .word     DMA1_Channel2_IRQHandler          /* 74:DMA1 Channel0                          */
  .word     DMA1_Channel3_4_IRQHandler        /* 75:DMA1 Channel3 and Channel4             */
  
                    
/*******************************************************************************
*
* Provide weak aliases for each Exception handler to the Default_Handler. 
* As they are weak aliases, any function with the same name will override 
* this definition.
* 
*******************************************************************************/
   .weak      NMI_Handler
   .thumb_set NMI_Handler,Default_Handler
  
   .weak      HardFault_Handler
   .thumb_set HardFault_Handler,Default_Handler
  
   .weak      MemManage_Handler
   .thumb_set MemManage_Handler,Default_Handler
  
   .weak      BusFault_Handler
   .thumb_set BusFault_Handler,Default_Handler

   .weak      UsageFault_Handler
   .thumb_set UsageFault_Handler,Default_Handler

   .weak      SVC_Handler
   .thumb_set SVC_Handler,Default_Handler

   .weak      DebugMon_Handler
   .thumb_set DebugMon_Handler,Default_Handler

   .weak      PendSV_Handler
   .thumb_set PendSV_Handler,Default_Handler

   .weak      SysTick_Handler
   .thumb_set SysTick_Handler,Default_Handler              
  
   .weak      WWDGT_IRQHandler
   .thumb_set WWDGT_IRQHandler,Default_Handler
                  
   .weak      LVD_IRQHandler      
   .thumb_set LVD_IRQHandler,Default_Handler
               
   .weak      TAMPER_IRQHandler            
   .thumb_set TAMPER_IRQHandler,Default_Handler
            
   .weak      RTC_IRQHandler                  
   .thumb_set RTC_IRQHandler,Default_Handler
            
   .weak      FMC_IRQHandler         
   .thumb_set FMC_IRQHandler,Default_Handler
                  
   .weak      RCU_CTC_IRQHandler      
   .thumb_set RCU_CTC_IRQHandler,Default_Handler
                  
   .weak      EXTI0_IRQHandler         
   .thumb_set EXTI0_IRQHandler,Default_Handler
                  
   .weak      EXTI1_IRQHandler         
   .thumb_set EXTI1_IRQHandler,Default_Handler
                     
   .weak      EXTI2_IRQHandler         
   .thumb_set EXTI2_IRQHandler,Default_Handler 
                 
   .weak      EXTI3_IRQHandler         
   .thumb_set EXTI3_IRQHandler,Default_Handler
                        
   .weak      EXTI4_IRQHandler         
   .thumb_set EXTI4_IRQHandler,Default_Handler
                  
   .weak      DMA0_Channel0_IRQHandler               
   .thumb_set DMA0_Channel0_IRQHandler,Default_Handler
         
   .weak      DMA0_Channel1_IRQHandler               
   .thumb_set DMA0_Channel1_IRQHandler,Default_Handler
                  
   .weak      DMA0_Channel2_IRQHandler               
   .thumb_set DMA0_Channel2_IRQHandler,Default_Handler
                  
   .weak      DMA0_Channel3_IRQHandler               
   .thumb_set DMA0_Channel3_IRQHandler,Default_Handler 
                 
   .weak      DMA0_Channel4_IRQHandler              
   .thumb_set DMA0_Channel4_IRQHandler,Default_Handler
                  
   .weak      DMA0_Channel5_IRQHandler               
   .thumb_set DMA0_Channel5_IRQHandler,Default_Handler
                  
   .weak      DMA0_Channel6_IRQHandler               
   .thumb_set DMA0_Channel6_IRQHandler,Default_Handler
                  
   .weak      ADC0_1_IRQHandler      
   .thumb_set ADC0_1_IRQHandler,Default_Handler
            
   .weak      USBD_HP_CAN0_TX_IRQHandler   
   .thumb_set USBD_HP_CAN0_TX_IRQHandler,Default_Handler
            
   .weak      USBD_LP_CAN0_RX0_IRQHandler            
   .thumb_set USBD_LP_CAN0_RX0_IRQHandler,Default_Handler
            
   .weak      CAN0_RX1_IRQHandler            
   .thumb_set CAN0_RX1_IRQHandler,Default_Handler
      
   .weak      CAN0_EWMC_IRQHandler      
   .thumb_set CAN0_EWMC_IRQHandler,Default_Handler
      
   .weak      EXTI5_9_IRQHandler   
   .thumb_set EXTI5_9_IRQHandler,Default_Handler
                  
   .weak      TIMER0_BRK_IRQHandler            
   .thumb_set TIMER0_BRK_IRQHandler,Default_Handler
                  
   .weak      TIMER0_UP_IRQHandler            
   .thumb_set TIMER0_UP_IRQHandler,Default_Handler
                  
   .weak      TIMER0_TRG_CMT_IRQHandler            
   .thumb_set TIMER0_TRG_CMT_IRQHandler,Default_Handler

   .weak      TIMER0_Channel_IRQHandler            
   .thumb_set TIMER0_Channel_IRQHandler,Default_Handler

   .weak      TIMER1_IRQHandler            
   .thumb_set TIMER1_IRQHandler,Default_Handler

   .weak      TIMER2_IRQHandler            
   .thumb_set TIMER2_IRQHandler,Default_Handler

   .weak      TIMER3_IRQHandler            
   .thumb_set TIMER3_IRQHandler,Default_Handler
                  
   .weak      I2C0_EV_IRQHandler   
   .thumb_set I2C0_EV_IRQHandler,Default_Handler
                     
   .weak      I2C0_ER_IRQHandler   
   .thumb_set I2C0_ER_IRQHandler,Default_Handler
                     
   .weak      I2C1_EV_IRQHandler   
   .thumb_set I2C1_EV_IRQHandler,Default_Handler
                  
   .weak      I2C1_ER_IRQHandler   
   .thumb_set I2C1_ER_IRQHandler,Default_Handler
                           
   .weak      SPI0_IRQHandler            
   .thumb_set SPI0_IRQHandler,Default_Handler
                        
   .weak      SPI1_IRQHandler            
   .thumb_set SPI1_IRQHandler,Default_Handler
                  
   .weak      USART0_IRQHandler      
   .thumb_set USART0_IRQHandler,Default_Handler
                     
   .weak      USART1_IRQHandler      
   .thumb_set USART1_IRQHandler,Default_Handler

   .weak      USART2_IRQHandler      
   .thumb_set USART2_IRQHandler,Default_Handler
                                  
   .weak      EXTI10_15_IRQHandler               
   .thumb_set EXTI10_15_IRQHandler,Default_Handler
               
   .weak      RTC_Alarm_IRQHandler               
   .thumb_set RTC_Alarm_IRQHandler,Default_Handler
            
   .weak      USBD_WKUP_IRQHandler         
   .thumb_set USBD_WKUP_IRQHandler,Default_Handler
            
   .weak      TIMER7_BRK_IRQHandler               
   .thumb_set TIMER7_BRK_IRQHandler,Default_Handler

      .weak      TIMER7_UP_IRQHandler               
   .thumb_set TIMER7_UP_IRQHandler,Default_Handler

      .weak      TIMER7_TRG_CMT_IRQHandler               
   .thumb_set TIMER7_TRG_CMT_IRQHandler,Default_Handler

      .weak      TIMER7_Channel_IRQHandler               
   .thumb_set TIMER7_Channel_IRQHandler,Default_Handler

      .weak      ADC2_IRQHandler               
   .thumb_set ADC2_IRQHandler,Default_Handler

      .weak      EXMC_IRQHandler               
   .thumb_set EXMC_IRQHandler,Default_Handler
                     
   .weak      SDIO_IRQHandler            
   .thumb_set SDIO_IRQHandler,Default_Handler
                     
   .weak      TIMER4_IRQHandler            
   .thumb_set TIMER4_IRQHandler,Default_Handler
                     
   .weak      SPI2_IRQHandler            
   .thumb_set SPI2_IRQHandler,Default_Handler

      .weak      UART3_IRQHandler            
   .thumb_set UART3_IRQHandler,Default_Handler

      .weak      UART4_IRQHandler            
   .thumb_set UART4_IRQHandler,Default_Handler

      .weak      TIMER5_IRQHandler            
   .thumb_set TIMER5_IRQHandler,Default_Handler

      .weak      TIMER6_IRQHandler            
   .thumb_set TIMER6_IRQHandler,Default_Handler
                     
   .weak      DMA1_Channel0_IRQHandler               
   .thumb_set DMA1_Channel0_IRQHandler,Default_Handler
               
   .weak      DMA1_Channel1_IRQHandler               
   .thumb_set DMA1_Channel1_IRQHandler,Default_Handler
                  
   .weak      DMA1_Channel2_IRQHandler               
   .thumb_set DMA1_Channel2_IRQHandler,Default_Handler
            
   .weak      DMA1_Channel3_4_IRQHandler               
   .thumb_set DMA1_Channel3_4_IRQHandler,Default_Handler
            

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

