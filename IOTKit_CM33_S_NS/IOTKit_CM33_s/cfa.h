#ifndef CFA_H
#define CFA_H

#include <stdint.h>

#define CFA_CHALLANGE_SIZE 28
#define CFA_SIGNATURE_SIZE 28
#define CFA_REPORT_SIZE 512
#define KEY_SIZE 32 // bytes

// ERROR MESSAGEs
#define CFA_STATUS_ERROR    (uint8_t) 100
#define CFA_STATUS_FAILURE  (uint8_t) 101
#define CFA_STATUS_STARTED  (uint8_t) 1
#define CFA_STATUS_SUCCESS  (uint8_t) 2

// typedef struct CFReport{
//     uint8_t     status;
//     uint8_t     challenge[CFA_CHALLANGE_SIZE];
//     uint8_t     signature[CFA_SIGNATURE_SIZE];
//     uint16_t    report_size;
//     uint8_t     report [CFA_REPORT_SIZE]; 
// } CFReport;


#define MAX_CF_LOG_SIZE CFA_REPORT_SIZE
#define HASH_SIZE_BYTES CFA_CHALLANGE_SIZE
#define SIGNATURE_SIZE_BYTES CFA_SIGNATURE_SIZE

typedef struct CFReport{
    
        uint8_t     status;
        uint32_t    log_counter;
        uint8_t     challenge[CFA_CHALLANGE_SIZE];
		uint8_t     signature[SIGNATURE_SIZE_BYTES];
		uint16_t	isFinal;
		uint8_t 	mem_hash [HASH_SIZE_BYTES];
		uint16_t	num_CF_Log_size;
		uint32_t 	CFLog[MAX_CF_LOG_SIZE];
		uint16_t	number_of_logs_sent; // init = 0
}CFReport;

typedef uint8_t error_t;

error_t  eCFA_init_cfa(CFReport *report);
void CFA_ENGINE_new_log_entry(uint32_t value);


#endif