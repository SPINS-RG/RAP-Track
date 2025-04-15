#include "mtb.h"
#include "RTE_Components.h" 
#include <stdint.h>
#include <stdio.h>
#include CMSIS_device_header
#include "core_cm33.h"
#include "stdio.h"
#include "stdout_USART.h"
#include "alcata_definitions.h"



SCB_Type * SCB_ = ((SCB_Type *) SCB_BASE);

MTB_struct *mtb = (MTB_struct *) MTB_BASE_addr;
CoreDebug_Type * CoreDebug_ = ((CoreDebug_Type *)     DCB_BASE         );
DWT_Type * DWT_ = ((DWT_Type       *)     DWT_BASE         );
ITM_Type * ITM_ = ((ITM_Type       *)     ITM_BASE         ) ;

#define DWT_FUNCTION_ACTION_VALUE 0b10 << DWT_FUNCTION_ACTION_OFFSET       // Generate a data trace match
#define DWT_FUNCTION_DATAVSIZE_VALUE 0b10 << DWT_FUNCTION_DATAVSIZE_OFFSET // word size
#define DWT_FUNCTION_MATCH_VALUE 0b0011 << DWT_FUNCTION_MATCH_OFFSET       // Generate a match on the data value
#define DWT_FUNCTION_MODIFY_MASK (DWT_FUNCTION_MATCH_MASK | DWT_FUNCTION_DATAVSIZE_MASK | DWT_FUNCTION_ACTION_MASK)
#define DWT_FUNCTION_MODIFY_VALUE (DWT_FUNCTION_MATCH_VALUE | DWT_FUNCTION_DATAVSIZE_VALUE | DWT_FUNCTION_ACTION_VALUE)


#define MTBAR_START_ADDRESS  0x00380000
#define MTBAR_END_ADDRESS    0x00400000

#define MTBDR_START_ADDRESS   0x00300000
#define MTBDR_END_ADDRESS     0x00360000


// #define MTBAR_START_ADDRESS  0x00300000
// #define MTBDR_START_ADDRESS   0x00280000
// #define MTBDR_END_ADDRESS     0x00300000

// compile this function with -O3 flag
#pragma GCC push_options
#pragma GCC optimize ("-O3")

void mtb_setup_DWT()
{

    // Disable Halt Mode
    // SET_BITS(CoreDebug_->DHCSR, 16, 31, DHCSR_DEBUG_KEY);
    // CoreDebug_->DHCSR &= ~(DHCSR_HALT_ENABLED_MASK);

    // Enable DWT
    CoreDebug_->DEMCR |= (DEMCR_TRCENA | DEMCR_MON_EN ); // Enable Trace and Debug
    

    ITM_->TCR |= (ITM_TCR_TXENA|ITM_TCR_ITMENA);

    // DWT_->COMP0 = (uint32_t) matmul;  // Initial Address
    DWT_->COMP0 = (uint32_t) MTBAR_START_ADDRESS;
    SET_BITS(DWT->COMP0,0,0,0b0);

    // DWT->COMP1 = (uint32_t) matmul2; // Final Address
    DWT_->COMP1 = (uint32_t) MTBAR_END_ADDRESS;
    SET_BITS(DWT->COMP1,0,0,0b0);     
    
    // DWT->COMP2 = (uint32_t) run;  // Initial Address
    DWT_->COMP2 = (uint32_t) MTBDR_START_ADDRESS;
    SET_BITS(DWT->COMP2,0,0,0b0);
    
    // DWT->COMP3 = (uint32_t) setup_DWT; // Final Address
    DWT_->COMP3 = (uint32_t) MTBDR_END_ADDRESS;
    SET_BITS(DWT->COMP3,0,0,0b0);
    
    // START SIGNAL
    // CMP0
    SET_BITS(DWT->FUNCTION0,10,11,0b00); // DATAVSIZE
    SET_BITS(DWT->FUNCTION0,4,5,0b00); // ACTION
    SET_BITS(DWT->FUNCTION0,0,3,0b0010); // MATCH

    // CMP1
    SET_BITS(DWT->FUNCTION1,10,11,0b00); // DATAVSIZE
    SET_BITS(DWT->FUNCTION1,4,5,0b11); // ACTION
    SET_BITS(DWT->FUNCTION1,0,3,0b0011); // MATCH

    // STOP SIGNAL
    // CMP2
    SET_BITS(DWT->FUNCTION2,10,11,0b00); // DATAVSIZE
    SET_BITS(DWT->FUNCTION2,4,5,0b00); // ACTION
    SET_BITS(DWT->FUNCTION2,0,3,0b0010); // MATCH

    // CMP3
    SET_BITS(DWT->FUNCTION3,10,11,0b00); // DATAVSIZE
    SET_BITS(DWT->FUNCTION3,4,5,0b11); // ACTION
    SET_BITS(DWT->FUNCTION3,0,3,0b0011); // MATCH
    
    printf("[LOG] CoreDebug_->DHCSR : %x\n", (uint32_t) CoreDebug_->DHCSR);
    printf("[LOG] CoreDebug_->DEMCR : %x\n", (uint32_t) CoreDebug_->DEMCR);
    

    return;
}

void mtb_cleanMTB(){
    uint32_t * ptr = (uint32_t *) mtb->MTB_BASE;
    for (int i = 0; i < MTB_BUFFER_SIZE; i++){
        ptr[i] = 0;
    }
}

#define USE_PRINTF 1

uint32_t log_counter;

void vMTB_sendBuffer(){

        printf("[LOG] Sending MTB Buffer\n");
    // stdout_init();
    #if USE_PRINTF == 1
    #endif
    log_counter+=mtb->MTB_POSITION;
    printf("[LOG] Log Counter : %d\n", log_counter);
    printf("%d\n", mtb->MTB_POSITION);

    uint32_t * ptr = (uint32_t *) mtb->MTB_BASE;
#if USE_PRINTF == 1
    for (int i = 0; i < mtb->MTB_POSITION/4; i++){
        printf("%x;", ptr[i]);

    }
#else
    stdout_putbuffer(ptr, mtb->MTB_POSITION/4, 2);

#endif
    #if USE_PRINTF == 1
    #endif
printf("\n[LOG] MTB Buffer Sent\n");

    return;
}

void mtb_debugMonitorHandler(){

    printf("[LOG] Debug Monitor Handler\n");

    // Deactivate halt mode
    mtb->MTB_MASTER &= ~( 1U << 9 );


#if ALCATA_PP == TRUE

#else
    vMTB_sendBuffer();
    mtb->MTB_POSITION = 0;

#endif

    return;
}

void mtb_debugMonitorHandlerEmpty(){
    while(1){};
}

void mtb_remove_debugMonitor(){
    // setup VTOR 
    uint32_t * VTOR = (uint32_t *) SCB_->VTOR;
    VTOR[12] = (uint32_t) mtb_debugMonitorHandlerEmpty;
    return;
}


void pendsv_handler(){
    printf("[LOG] PendSV\n");
    vMTB_sendBuffer();
    while(1){};
    return;
}

void hardfault_handler(){
    printf("[ERROR] Hardfault\n");
    vMTB_sendBuffer();

    // set pendsv
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;

    return;
}


void config_hardfault_handler(){
    uint32_t * VTOR = (uint32_t *) SCB->VTOR;
    
    NVIC_SetPriority(PendSV_IRQn, (1 << __NVIC_PRIO_BITS) - 1);

    // for (int i = 0; i < 16 ; i++)
    //     VTOR[i] = (uint32_t) hardfault_handler;

    // set hardfault handler to be hardfault_handler
    VTOR[3] = (uint32_t) hardfault_handler;

    // set pendsv handler to be pendsv_handler
    VTOR[14] = (uint32_t) pendsv_handler;




    return;
}

void mtb_config_interrupthandlers(){
    // Setting PendSV handler
    uint32_t * VTOR = (uint32_t *) SCB->VTOR;

    // Setting Debug Monitor handler
    VTOR[12] = (uint32_t) mtb_debugMonitorHandler;

    // Dont do this in real life :). This is just to bypass a uart problem with interupt priority
    NVIC_SetPriority(DebugMonitor_IRQn, (1 << __NVIC_PRIO_BITS) - 1);
    return;
}


void mtb_setup_MTB(){
    // SCB_NS->VTOR = (uint32_t) VTOR;
    printf("[LOG] Setting up MTB\n");
    mtb_cleanMTB();
    mtb->MTB_TSTART |= 0b10;  // Set to use DWT_COMP1
    mtb->MTB_TSTOP  |= 0b1000;  // Set to use DWT_COMP3
    mtb->MTB_FLOW = MTB_WATERMARK_A;
    mtb->MTB_POSITION = 0;
    mtb->MTB_MASTER |= MTB_MASTER_TSTARTEN_MASK;
    mtb->MTB_MASTER |= MTB_MASTER_MASK_MASK;
    return;
}

#pragma GCC pop_options

void mtb_init(){
    printf("[LOG] Setting up hardfaultHandler\n");
    config_hardfault_handler();
    log_counter = 0;
    printf("[LOG] Setting up DWT\n");
	mtb_setup_DWT();
    printf("[LOG] Setting up MTB\n");
	mtb_setup_MTB();
    printf("[LOG] Setting up Interrupt Handlers\n");
    mtb_config_interrupthandlers();
	return;
}

void mtb_exit(){

    __disable_irq();

    // clean mtb buffer
    mtb_cleanMTB();
    
    // Remove the debug monitor function handler
    mtb_remove_debugMonitor();

    // deactivate MTB
    mtb->MTB_MASTER &= ~(1U << 5);
    mtb->MTB_MASTER &= ~(1U << 9);
    mtb->MTB_MASTER &= ~(1U << 31);

    // deactivate DWT
    DWT_->COMP0 = 0;
    DWT_->COMP1 = 0;
    DWT_->COMP2 = 0;
    DWT_->COMP3 = 0;
    DWT_->FUNCTION0 = 0;
    DWT_->FUNCTION1 = 0;
    DWT_->FUNCTION2 = 0;
    DWT_->FUNCTION3 = 0;
    
    __enable_irq();

    return;
}



void __debug_MTB_Registers(){
    printf("[LOG] Entering Debug MTB Registers\n");
    printf("[LOG] MTB_BASE : %x\n", (uint32_t) mtb->MTB_BASE);
    printf("[LOG] MTB_POSITION : %x\n", (uint32_t) mtb->MTB_POSITION);
    printf("[LOG] MTB_MASTER : %x\n", (uint32_t) mtb->MTB_MASTER);
    printf("[LOG] MTB_FLOW : %x\n", (uint32_t) mtb->MTB_FLOW);
    printf("[LOG] MTB_TSTART : %x\n", (uint32_t) mtb->MTB_TSTART);
    printf("[LOG] MTB_TSTOP : %x\n", (uint32_t) mtb->MTB_TSTOP);
    printf("[LOG] MTB_SECURE : %x\n", (uint32_t) mtb->MTB_SECURE);
    return;
}

void __debug_DWT_Registers(){
    printf("[LOG] Entering Debug DWT Registers\n");
    printf("[LOG] DWT_COMP0 : %x\n", (uint32_t) DWT_->COMP0);
    printf("[LOG] DWT_COMP1 : %x\n", (uint32_t) DWT_->COMP1);
    printf("[LOG] DWT_COMP2 : %x\n", (uint32_t) DWT_->COMP2);
    printf("[LOG] DWT_COMP3 : %x\n", (uint32_t) DWT_->COMP3);
    printf("[LOG] DWT_FUNCTION0 : %x\n", (uint32_t) DWT_->FUNCTION0);
    printf("[LOG] DWT_FUNCTION1 : %x\n", (uint32_t) DWT_->FUNCTION1);
    printf("[LOG] DWT_FUNCTION2 : %x\n", (uint32_t) DWT_->FUNCTION2);
    printf("[LOG] DWT_FUNCTION3 : %x\n", (uint32_t) DWT_->FUNCTION3);
    return;
}