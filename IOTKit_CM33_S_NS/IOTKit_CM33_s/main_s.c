/*----------------------------------------------------------------------------
 * Name:    main_s.c
 * Purpose: Main function secure mode
 *----------------------------------------------------------------------------*/

#include "RTE_Components.h" /* Component selection */
#include <arm_cmse.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
// #include "Driver_USART.h" /* ::CMSIS Driver:USART */
#include CMSIS_device_header
#include "Board_GLCD.h"  /* ::Board Support:Graphic LCD */
#include "Board_LED.h"   /* ::Board Support:LED */
#include "GLCD_Config.h" /* Keil.V2M-MPS2 IOT-Kit::Board Support:Graphic LCD */
#include "mtb.h"
#include "core_cm33.h"
#import "Secure_Functions.h"
/* Start address of non-secure application */
#define NONSECURE_START (0x00200000u)

extern GLCD_FONT GLCD_Font_16x24;

extern int stdout_init(void);

/* typedef for NonSecure callback functions */
typedef int32_t (*NonSecure_fpParam)(uint32_t) __attribute__((cmse_nonsecure_call));
typedef void (*NonSecure_fpVoid)(void) __attribute__((cmse_nonsecure_call));

/*----------------------------------------------------------------------------
  SysTick IRQ Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void);
void SysTick_Handler(void)
{
    static uint32_t ticks = 0;
    static uint32_t ticks_printf = 0;

    ticks++;
    printf("Hello Secure\n");
}


// //
// #define USART_DRV_NUM           0
 
// //   <o>Baudrate
// #define USART_BAUDRATE          115200
 
// // </h>
  
// #define _USART_Driver_(n)  Driver_USART##n
// #define  USART_Driver_(n) _USART_Driver_(n)
 
// extern ARM_DRIVER_USART  USART_Driver_(USART_DRV_NUM);
// #define ptrUSART       (&USART_Driver_(USART_DRV_NUM))

// /**
//   Put a character to the stdout
 
//   \param[in]   ch  Character to output
//   \return          The character written, or -1 on write error.
// */
// int stdout_putchar_ (int ch) {
//   uint8_t buf[1];
 
//   buf[0] = (uint8_t)ch;
//   if (ptrUSART->Send(buf, 1) != ARM_DRIVER_OK) {
//     return (-1);
//   }
//   while (ptrUSART->GetTxCount() != 1);
//   return (ch);
// }

static uint32_t x;
/*----------------------------------------------------------------------------
  Main function
 *----------------------------------------------------------------------------*/
int main(void)
{
    uint32_t NonSecure_StackPointer = (*((uint32_t *)(NONSECURE_START + 0u)));
    NonSecure_fpVoid NonSecure_ResetHandler =
        (NonSecure_fpVoid)(*((uint32_t *)(NONSECURE_START + 4u)));

    /* exercise some floating point instructions from Secure Mode */
    volatile uint32_t fpuType = SCB_GetFPUType();
    volatile float x1 = 12.4567f;
    volatile float x2 = 0.6637967f;
    volatile float x3 = 24.1111118f;

    x3 = x3 * (x1 / x2);

    /* exercise some core register from Secure Mode */
    x = __get_MSP();
    x = __get_PSP();
    __TZ_set_MSP_NS(NonSecure_StackPointer);
    x = __TZ_get_MSP_NS();
    __TZ_set_PSP_NS(0x22000000u);
    x = __TZ_get_PSP_NS();

    SystemCoreClockUpdate();

    stdout_init(); /* Initialize Serial interface */
    // LED_Initialize();
    // GLCD_Initialize();

    // /* display initial screen */
    //  GLCD_SetFont(&GLCD_Font_16x24);
    //  GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
    //  GLCD_ClearScreen();
    //  GLCD_SetBackgroundColor(GLCD_COLOR_BLUE);
    //  GLCD_SetForegroundColor(GLCD_COLOR_RED);
    //  GLCD_DrawString(0 * 16, 0 * 24, "   V2M-MPS2+ Demo   ");
    //  GLCD_DrawString(0 * 16, 1 * 24, " Secure/Non-Secure  ");
    //  GLCD_DrawString(0 * 16, 2 * 24, "   www.keil.com     ");

    // GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
    // GLCD_SetForegroundColor(GLCD_COLOR_BLACK);
    // switch ((SCB->CPUID >> 4) & 0xFFF)
    // {
    // case 0xD20:
    //     GLCD_DrawString(0 * 16, 4 * 24, "  Cortex-M23        ");
    //     break;
    // case 0xD21:
    //     GLCD_DrawString(0 * 16, 4 * 24, "  Cortex-M33        ");
    //     break;
    // default:
    //     GLCD_DrawString(0 * 16, 4 * 24, "  unknown Cortex-M  ");
    //     break;
    // }

    // SysTick->CTRL = 0;                      /* Disable SysTick IRQ and SysTick Timer */

    stdout_init(); /* Initialize Serial interface */

    // SysTick_Config(SystemCoreClock / 100); /* Generate interrupt each 10 ms */

		// while(1){
		// 	stdout_putchar_('A');
		// }

    NonSecure_ResetHandler();
}





