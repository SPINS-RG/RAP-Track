
#ifndef MTB_H
#define MTB_H

#include <stdint.h>

#define MTB_BASE_addr 						0xE0043000
#define MTB_MASTER_EN_MASK 				1<<31
#define MTB_MASTER_NSEN_MASK 			1<<30
#define MTB_MASTER_HALTREQ_MASK 	1<<9
#define MTB_MASTER_RAMPRIV_MASK 	1<<8
#define MTB_MASTER_TSTOPEN_MASK 	1<<6
#define MTB_MASTER_TSTARTEN_MASK 	1<<5
#define MTB_MASTER_HALTREQ_MASK 	1<<9
#define MTB_MASTER_MASK_MASK 			0b1111
#define MTB_FLOW_WATERMARK_MASK 	~(0b1111)
#define MTB_FLOW_AUTOHALT_MASK 	0b10
#define MTB_FLOW_AUTOSTOP_MASK 	0b01

#define MTB_BUFFER_SIZE 1024

#define MTB_BASE_MASK 	~(0b11111)

typedef struct MTB_struct{
	uint32_t MTB_POSITION;
	uint32_t MTB_MASTER;
	uint32_t MTB_FLOW;
	uint32_t MTB_BASE;
	uint32_t MTB_TSTART;		
	uint32_t MTB_TSTOP;
	uint32_t MTB_SECURE;	
} MTB_struct;


#define DWT_BASE_addr 0xE0001000
#define ITM_TCR_addr  0xE0000E80
#define DEMCR_addr    0xE000EDFC

#define ITM_TCR_TXENA_OFFSET 			3
#define ITM_TCR_TXENA 					1 << ITM_TCR_TXENA_OFFSET

#define ITM_TCR_ITMENA_OFFSET 			0
#define ITM_TCR_ITMENA					1 << ITM_TCR_ITMENA_OFFSET

#define DEMCR_TRCENA_OFFSET 			24
#define DEMCR_MON_EN_OFFSET 			16
#define DEMCR_SDME_OFFSET 				20

#define DEMCR_TRCENA 					1 << DEMCR_TRCENA_OFFSET
#define DEMCR_MON_EN 					1 << DEMCR_MON_EN_OFFSET
#define DEMCR_SDME 						1 << DEMCR_SDME_OFFSET


#define DHCSR_HALT_ENABLED_OFFSET 		0
#define DHCSR_HALT_ENABLED_MASK 		1 << DHCSR_HALT_ENABLED_OFFSET

#define DHCSR_DEBUG_KEY	 				0xA05F

#define DWT_FUNCTION_DATAVSIZE_OFFSET   10
#define DWT_FUNCTION_ACTION_OFFSET      4
#define DWT_FUNCTION_MATCHED_OFFSET     24
#define DWT_FUNCTION_MATCH_OFFSET       0

#define DWT_FUNCTION_DATAVSIZE_MASK     0b11 << DWT_FUNCTION_DATAVSIZE_OFFSET
#define DWT_FUNCTION_ACTION_MASK        0b00 << DWT_FUNCTION_ACTION_OFFSET
#define DWT_FUNCTION_MATCHED_MASK       0b1 << DWT_FUNCTION_MATCHED_OFFSET
#define DWT_FUNCTION_MATCH_MASK         0b1111 <<  DWT_FUNCTION_MATCH_OFFSET

#define DWT_A_SIZE 						63
#define DWT_B_SIZE 						5

#define SET_BITS(D, X, Y, Z) \
    ((D) = ((D) & ~(((1U << ((Y) - (X) + 1)) - 1) << (X))) | ((Z) << (X))) // set a specific range of bits in a variable (or register) D to a value Z, starting at bit position X and ending at bit position Y

#define MTB_WATERMARK_A (( (sizeof(uint32_t)* 2 * DWT_A_SIZE) << 3 ) | MTB_FLOW_AUTOHALT_MASK | MTB_FLOW_AUTOSTOP_MASK)
#define MTB_WATERMARK_B (( (sizeof(uint32_t)* 2 * DWT_A_SIZE * 2) << 3 ) | MTB_FLOW_AUTOHALT_MASK | MTB_FLOW_AUTOSTOP_MASK)

void mtb_init();
void mtb_exit();
void mtb_cleanMTB();
void vMTB_sendBuffer();

void __debug_MTB_Registers();
void __debug_DWT_Registers();

#endif

