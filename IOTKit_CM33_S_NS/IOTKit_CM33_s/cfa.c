#include "IOTKit_CM33_FP.h"
# include "RTE_Components.h"
# include "cfa.h"
# include "mtb.h"
# include <stdint.h>
# include <string.h>
# include <stdio.h>
# include "stdout_USART.h"

CFReport report_s;
extern MTB_struct *mtb;
typedef void (*NonSecure_fpVoid)(void) __attribute__((cmse_nonsecure_call));











uint8_t attestationKey[KEY_SIZE] = {0};

extern NonSecure_fpVoid fNSFunc;

error_t vCFA_copy_report(CFReport*report_ns){
    
    if( memcpy(report_ns, &report_s, sizeof(CFReport)) == NULL)
    {
        return CFA_STATUS_ERROR;
    }
    return CFA_STATUS_SUCCESS;
}

void vDebug_send_report_to_UART(){
    // todo
}

void vCFA_add_log(){
    //todo
}


#define USE_MTB 1
#define DEBUG_REGISTERS 1

void cfa_cflog_init(){
    report_s.num_CF_Log_size = 0;
    return;
}

uint32_t log_size_counter = 0;

error_t eCFA_init_cfa(CFReport *report_ns){

    printf("[LOG] CFA funtion entry.\n");

    report_ns->status = CFA_STATUS_STARTED;

#if USE_MTB != 1
    report_ns->log_counter = 0;
#endif

    if(memcmp(report_ns->challenge, report_s.challenge, sizeof(uint32_t) * CFA_CHALLANGE_SIZE))
    {
        report_ns->status = CFA_STATUS_FAILURE;
        return CFA_STATUS_FAILURE;
    }

    // clean the report
    memset(report_ns, 0, sizeof(CFReport));

    // hash the memory
    // todo
    
#if USE_MTB == 1
    // configure the MTB
    mtb_init();
#else
    cfa_cflog_init();

#endif

    printf("[LOG] CFA Calling Non Secure Function : %x\n",(uint32_t)fNSFunc);
    
    // run the NSfunction

	// IOTKIT_SECURE_TIMER0->CTRL = 0x00000000;
	// IOTKIT_SECURE_TIMER0->RELOAD = 0x00000000;

	IOTKIT_SECURE_DUALTIMER1->CTRL = 0b10000011;
	IOTKIT_SECURE_DUALTIMER1->LOAD = 0xFFFFFFFF;

    fNSFunc();

	uint32_t t = IOTKIT_SECURE_DUALTIMER1->VALUE;
	printf("Secure Timer Value: %u \n", 0xFFFFFFFF - t);

	printf(" Log Counter: %u \n", 4*log_size_counter);


    printf("[LOG] CFA Returned from Non Secure Function\n");

#if DEBUG_REGISTERS == 1
    __debug_MTB_Registers();
    __debug_DWT_Registers();
#endif

    // sign the report
    // todo

    // // copy the report
    // if(vCFA_copy_report(report_ns) == CFA_STATUS_ERROR)
    // {
    //     report_ns->status = CFA_STATUS_ERROR;
    //     return CFA_STATUS_ERROR;
    // }
	

    report_ns->status = CFA_STATUS_SUCCESS;

#if USE_MTB == 1
    vMTB_sendBuffer();

    mtb_exit();
#endif


    printf("[LOG] CFA funtion exit\n");
    return CFA_STATUS_SUCCESS;
}



uint8_t loop_detect = 0;
uint16_t loop_counter = 1;
uint32_t prev_entry;
uint32_t overflow = 0;
#define CFLOG_TYPE CFLOG_RAM
// VERBATIM STYLE
/**/


void _send_report(){
    
    // hash
    // todo
    
    // send the report
    stdout_putbuffer((uint32_t *)report_s.CFLog, report_s.num_CF_Log_size, 1);

    // reset log size
    report_s.num_CF_Log_size = 0;
    
    // repo
	return;
}

#define REMOVE_LOG 1


void CFA_ENGINE_new_log_entry(uint32_t value){
	log_size_counter++;
	printf("[LOG] New Log Entry\n");
	return;

	if(report_s.num_CF_Log_size >= MAX_CF_LOG_SIZE){
		// cfa_engine_conf.attestation_status = WAITING_PARTIAL;
		report_s.num_CF_Log_size = MAX_CF_LOG_SIZE; // might point over with loop overflow
		_send_report();

		if(overflow != 0){
			#if CFLOG_TYPE == CFLOG_RAM
			report_s.CFLog[report_s.num_CF_Log_size] = overflow;
			#else
			uint32_t addr = (uint32_t)(&FLASH_CFLog[report_secure.num_CF_Log_size]);
			//		update_flash(addr, value);
			FLASH_CFLog[report_secure.num_CF_Log_size] = overflow;
			#endif
			overflow = 0;

			#ifndef REMOVE_LOG
			report_s.num_CF_Log_size++;
			#endif
		}

		#if CFLOG_TYPE == CFLOG_RAM
		report_s.CFLog[report_s.num_CF_Log_size] = value;
		#else
		uint32_t addr = (uint32_t)(&FLASH_CFLog[report_secure.num_CF_Log_size]);
//		update_flash(addr, value);
		FLASH_CFLog[report_secure.num_CF_Log_size] = value;
		#endif

		#ifndef REMOVE_LOG
		report_s.num_CF_Log_size++;
		#endif

		// _read_serial_loop();
		// start = HAL_GetTick();
	}
	else{
//
//		if(report_secure.num_CF_Log_size == MAX_CF_LOG_SIZE)
//			loop_detect = loop_detect;

		// compare current value to previous, if equal, replace with counter
		#if CFLOG_TYPE == CFLOG_RAM
		prev_entry = report_s.CFLog[report_s.num_CF_Log_size - 1];
		if(report_s.num_CF_Log_size != 0 && prev_entry == value){

		#else
		prev_entry = FLASH_CFLog[report_secure.num_CF_Log_size - 1];
		if(report_secure.num_CF_Log_size != 0 && prev_entry == value){

		#endif
			if (loop_detect == 0){
				// since first instance of repeat, set flag
				loop_detect ^= 1;
			} else if (loop_detect == 1){
				// if more than one instance, increment counter
				loop_counter++;
			}
		}
		else{ // enter this block either because 1) not a loop or 2) loop exit
			if(loop_detect == 1){
				// if loop exit, clear flag and increment log size for next entry
                report_s.CFLog[report_s.num_CF_Log_size] = (0xffff0000 + loop_counter);
				

				loop_detect = 0;
				#ifndef REMOVE_LOG
				report_s.log_counter++;
				#endif
				report_s.num_CF_Log_size++;
				loop_counter = 1;

				// Need to catch case when the counter is the last entry in the cflog
				if(report_s.num_CF_Log_size == MAX_CF_LOG_SIZE){
					overflow = value;
				}
			}

			 if (overflow == 0){
				#if CFLOG_TYPE == CFLOG_RAM
				report_s.CFLog[report_s.num_CF_Log_size] = value;
				#else
				uint32_t addr = (uint32_t)(&FLASH_CFLog[report_secure.num_CF_Log_size]);
	//			update_flash(addr, value);
				FLASH_CFLog[report_secure.num_CF_Log_size] = value;
				prev_entry = value;
				#endif
				#ifndef REMOVE_LOG
				report_s.log_counter++;
				report_s.num_CF_Log_size++;
				#endif
			}

		}
	}
	return;
}