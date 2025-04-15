#ifndef SECURE_FUNCTIONS_CFA_H_
#define SECURE_FUNCTIONS_CFA_H_

#include "stdint.h"

void SECURE_log_loop_cond();
void SECURE_log_cond_br_not_taken();
void SECURE_log_cond_br_taken();
void SECURE_log_call(uint32_t addr);

#endif