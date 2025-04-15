/*----------------------------------------------------------------------------
 * Name:    Secure_Functions.h
 * Purpose: Function and Typedef Declarations to include into NonSecure Application.
 *----------------------------------------------------------------------------*/

#ifndef SECURE_FUNCTIONS_H_
#define SECURE_FUNCTIONS_H_

#include "RTE_Components.h" 
#include CMSIS_device_header
#include <arm_cmse.h>
#include "cfa.h"
#include "alcata_definitions.h"
#include "Secure_Functions_CFA.h"


#define NSENTRY __attribute__((cmse_nonsecure_entry))
// #define NSENTRY_NAKED __attribute__((cmse_nonsecure_entry, naked))
#define NSENTRY_NAKED __attribute__((cmse_nonsecure_entry))



/* Define typedef for NonSecure callback function */ 
typedef int32_t (*NonSecure_funcptr)(uint32_t);

/* typedef for NonSecure callback functions */
typedef int32_t (*NonSecure_fpParam)(uint32_t) __attribute__((cmse_nonsecure_call));
typedef void (*NonSecure_fpVoid)(void) __attribute__((cmse_nonsecure_call));


/* Function declarations for Secure functions called from NonSecure application */
int32_t Secure_LED_On (uint32_t);
int32_t Secure_LED_Off(uint32_t);
int32_t Secure_LED_On_callback (NonSecure_funcptr);
int32_t Secure_LED_Off_callback(NonSecure_funcptr);

extern void    Secure_printf (char*);



void SECURE_log_loop_cond();
void SECURE_log_cond_br_not_taken();
void SECURE_log_cond_br_taken();
void SECURE_log_call(uint32_t addr);

void SECURE_register_callback(void *);
void SECURE_start_cfa(CFReport *);

void SECURE_NOPE(void);

#endif /* SECURE_FUNCTIONS_H_ */