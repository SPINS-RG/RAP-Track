
#include "Secure_Functions.h"
#include "Board_LED.h"
#include "cfa.h"
#include <stdint.h>
#include <stdio.h>
#include "core_cm33.h"
#include "Secure_Functions_CFA.h"

// define callback functions
NonSecure_fpVoid fNSFunc = (NonSecure_fpVoid)NULL;

#define MEMORY_REGION_CFA_START 0x20000000U
#define MEMORY_REGION_CFA_END   0x20010000U
#define TRUE 1
#define FALSE 0


void SECURE_register_callback(void* callback) NSENTRY;
void SECURE_start_cfa(CFReport * report) NSENTRY;
void SECURE_NOPE(void) NSENTRY;

void SECURE_NOPE(void){
    return;
}


 void SECURE_register_callback(void* callback) 
{
    printf("[LOG] Entered Secure register_callback\n");
    // check if callback is a valid address
    // if (callback == NULL | callback < MEMORY_REGION_CFA_START | callback > MEMORY_REGION_CFA_END)
    // {
    //     return;
    // }

    fNSFunc = (NonSecure_fpVoid) callback;
    printf("[LOG] Callback function registered\n");
    return;
}


uint32_t CFA_stat = FALSE;

uint8_t _set_stat(){
    // disable interrupt
    __disable_irq();
    if (CFA_stat == TRUE)
    {
        __enable_irq();
        return 1;
    }
    // set the status
    CFA_stat = TRUE;
    // enable interrupt
    __enable_irq();
    return 0;
}

void _clear_stat(){
    // disable interrupt
    __disable_irq();
    // clear the status
    CFA_stat = FALSE;
    // enable interrupt
    __enable_irq();
}

void SECURE_start_cfa(CFReport * report){
    printf("[LOG] Entered Secure start_cfa\n");	

    __disable_irq();
    // check if callback function is defined
    if (fNSFunc == NULL)
    {
        printf("[ERROR] Callback function not defined\n");
        report->status = CFA_STATUS_ERROR;
        __enable_irq();
        return;
    }
    __enable_irq();

    // initialize cfa process
    if(_set_stat())return; 

    printf("[LOG] CFA initialization\n");	

    eCFA_init_cfa(report);

    _clear_stat();
    
    return;
}



// typedef int32_t (*NonSecure_fpParam)(uint32_t) __attribute__((cmse_nonsecure_call));
// typedef void (*NonSecure_fpVoid)(void) __attribute__((cmse_nonsecure_call));


char text[] = "Hello World (secure)\r\n";

/*----------------------------------------------------------------------------
  NonSecure callback functions
 *----------------------------------------------------------------------------*/
extern NonSecure_fpParam pfNonSecure_LED_On;
NonSecure_fpParam pfNonSecure_LED_On = (NonSecure_fpParam)NULL;
extern NonSecure_fpParam pfNonSecure_LED_Off;
NonSecure_fpParam pfNonSecure_LED_Off = (NonSecure_fpParam)NULL;

/*----------------------------------------------------------------------------
  Secure functions exported to NonSecure application
 *----------------------------------------------------------------------------*/
int32_t Secure_LED_On(uint32_t num) __attribute__((cmse_nonsecure_entry));
int32_t Secure_LED_On(uint32_t num) { return LED_On(num); }

int32_t Secure_LED_Off(uint32_t num) __attribute__((cmse_nonsecure_entry));
int32_t Secure_LED_Off(uint32_t num) { return LED_Off(num); }

void Secure_printf(char *pString) __attribute__((cmse_nonsecure_entry));
void Secure_printf(char *pString) { printf("%s", pString); }

/*----------------------------------------------------------------------------
  Secure function for NonSecure callbacks exported to NonSecure application
 *----------------------------------------------------------------------------*/
int32_t Secure_LED_On_callback(NonSecure_fpParam callback)
    __attribute__((cmse_nonsecure_entry));
int32_t Secure_LED_On_callback(NonSecure_fpParam callback)
{
    pfNonSecure_LED_On = callback;
    return 0;
}

int32_t Secure_LED_Off_callback(NonSecure_fpParam callback)
    __attribute__((cmse_nonsecure_entry));
int32_t Secure_LED_Off_callback(NonSecure_fpParam callback)
{
    pfNonSecure_LED_Off = callback;
    return 0;
}

