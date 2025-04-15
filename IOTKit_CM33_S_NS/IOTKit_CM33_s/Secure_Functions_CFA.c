#include "Secure_Functions.h"
#include "cfa.h"


NSENTRY_NAKED void SECURE_log_loop_cond() {
    __asm__ volatile(
        "push    {r0, r1, r2, r3, r4, r7, r12}     \n"
        "sub     sp, sp, #28                       \n"
        "add     r7, sp, #0                        \n"
        "push    {r7, lr}                          \n"
        "sub     sp, sp, #8                        \n"
        "add     r7, sp, #0                        \n"

        // Load loop-taken destination address from r10
        "mov     r0, r10                           \n"
        "bl      CFA_ENGINE_new_log_entry          \n"

        // Load loop condition from r11, apply mask, and log it
        "mov     r1, r11                           \n"
        "ldr     r2, =0xffff0000               \n"  // Load immediate into r2
        "orr     r1, r1, r2                    \n"
        "mov     r0, r1                            \n"
        "bl      CFA_ENGINE_new_log_entry          \n"

        "nop                                       \n"
        "adds    r7, r7, #8                        \n"
        "mov     sp, r7                            \n"
        "pop     {r7, lr}                          \n"
        "mov     r0, lr                            \n"
        "mov     r1, lr                            \n"
        "mov     r2, lr                            \n"
        "mov     r3, lr                            \n"

        // Floating-point register setup
        "vmov.f32 s0, #1.0e+0                      \n"
        "vmov.f32 s1, #1.0e+0                      \n"
        "vmov.f32 s2, #1.0e+0                      \n"
        "vmov.f32 s3, #1.0e+0                      \n"
        "vmov.f32 s4, #1.0e+0                      \n"
        "vmov.f32 s5, #1.0e+0                      \n"
        "vmov.f32 s6, #1.0e+0                      \n"
        "vmov.f32 s7, #1.0e+0                      \n"
        "vmov.f32 s8, #1.0e+0                      \n"
        "vmov.f32 s9, #1.0e+0                      \n"
        "vmov.f32 s10, #1.0e+0                     \n"
        "vmov.f32 s11, #1.0e+0                     \n"
        "vmov.f32 s12, #1.0e+0                     \n"
        "vmov.f32 s13, #1.0e+0                     \n"
        "vmov.f32 s14, #1.0e+0                     \n"
        "vmov.f32 s15, #1.0e+0                     \n"

        // APSR and FPSCR setup
        "msr     APSR_nzcvqg, lr                   \n"
        "push    {r4}                              \n"
        "vmrs    ip, fpscr                         \n"
        "movw    r4, #65376                        \n"
        "movt    r4, #4095                         \n"
        "and     ip, r4                            \n"
        "vmsr    fpscr, ip                         \n"
        "pop     {r4}                              \n"

        "mov     ip, lr                            \n"
        "adds    r7, r7, #28                       \n"
        "mov     sp, r7                            \n"
        "pop     {r0, r1, r2, r3, r4, r7, r12}     \n"
        "bics    lr, #1                            \n"
        "bxns    lr                                \n"
    );
}


NSENTRY_NAKED void SECURE_log_cond_br_taken() {
    __asm__ volatile(
        "push    {r0, r1, r2, r3, r4, r7, r12}     \n"
        "sub     sp, sp, #28                       \n"
        "add     r7, sp, #0                        \n"
        "push    {r7, lr}                          \n"
        "sub     sp, sp, #8                        \n"
        "add     r7, sp, #0                        \n"

        // Save the address in LR to a register for passing to another function.
        "mov     r0, lr                            \n"
        "sub     r0, r0, #4                        \n" // Adjust for instruction address

        // Call a C function to log the instruction address
        "bl      CFA_ENGINE_new_log_entry          \n"

        "nop                                       \n"
        "adds    r7, r7, #8                        \n"
        "mov     sp, r7                            \n"
        "pop     {r7, lr}                          \n"
        "mov     r0, lr                            \n"
        "mov     r1, lr                            \n"
        "mov     r2, lr                            \n"
        "mov     r3, lr                            \n"

        // Floating-point register cleaning
        "vmov.f32 s0, #1.0e+0                      \n"
        "vmov.f32 s1, #1.0e+0                      \n"
        "vmov.f32 s2, #1.0e+0                      \n"
        "vmov.f32 s3, #1.0e+0                      \n"
        "vmov.f32 s4, #1.0e+0                      \n"
        "vmov.f32 s5, #1.0e+0                      \n"
        "vmov.f32 s6, #1.0e+0                      \n"
        "vmov.f32 s7, #1.0e+0                      \n"
        "vmov.f32 s8, #1.0e+0                      \n"
        "vmov.f32 s9, #1.0e+0                      \n"
        "vmov.f32 s10, #1.0e+0                     \n"
        "vmov.f32 s11, #1.0e+0                     \n"
        "vmov.f32 s12, #1.0e+0                     \n"
        "vmov.f32 s13, #1.0e+0                     \n"
        "vmov.f32 s14, #1.0e+0                     \n"
        "vmov.f32 s15, #1.0e+0                     \n"

        // APSR and FPSCR setup
        "msr     APSR_nzcvqg, lr                   \n"
        "push    {r4}                              \n"
        "vmrs    ip, fpscr                         \n"
        "movw    r4, #65376                        \n"
        "movt    r4, #4095                         \n"
        "and     ip, r4                            \n"
        "vmsr    fpscr, ip                         \n"
        "pop     {r4}                              \n"

        "mov     ip, lr                            \n"
        "adds    r7, r7, #28                       \n"
        "mov     sp, r7                            \n"
        "pop     {r0, r1, r2, r3, r4, r7, r12}     \n"
        "bics    lr, #1                            \n"
        "bxns    lr                                \n"
    );
}

NSENTRY_NAKED void SECURE_log_cond_br_not_taken() {
    __asm__ volatile(
        "push    {r0, r1, r2, r3, r4, r7, r12}     \n"
        "sub     sp, sp, #28                       \n"
        "add     r7, sp, #0                        \n"
        "push    {r7, lr}                          \n"
        "sub     sp, sp, #8                        \n"
        "add     r7, sp, #0                        \n"

        // Save the address in LR to a register for passing to another function.
        "mov     r0, lr                            \n"
        "sub     r0, r0, #4                        \n" // Adjust for instruction address

        // Call a C function to log the instruction address
        "bl      CFA_ENGINE_new_log_entry          \n"

        "nop                                       \n"
        "adds    r7, r7, #8                        \n"
        "mov     sp, r7                            \n"
        "pop     {r7, lr}                          \n"
        "mov     r0, lr                            \n"
        "mov     r1, lr                            \n"
        "mov     r2, lr                            \n"
        "mov     r3, lr                            \n"

        // Floating-point register cleaning
        "vmov.f32 s0, #1.0e+0                      \n"
        "vmov.f32 s1, #1.0e+0                      \n"
        "vmov.f32 s2, #1.0e+0                      \n"
        "vmov.f32 s3, #1.0e+0                      \n"
        "vmov.f32 s4, #1.0e+0                      \n"
        "vmov.f32 s5, #1.0e+0                      \n"
        "vmov.f32 s6, #1.0e+0                      \n"
        "vmov.f32 s7, #1.0e+0                      \n"
        "vmov.f32 s8, #1.0e+0                      \n"
        "vmov.f32 s9, #1.0e+0                      \n"
        "vmov.f32 s10, #1.0e+0                     \n"
        "vmov.f32 s11, #1.0e+0                     \n"
        "vmov.f32 s12, #1.0e+0                     \n"
        "vmov.f32 s13, #1.0e+0                     \n"
        "vmov.f32 s14, #1.0e+0                     \n"
        "vmov.f32 s15, #1.0e+0                     \n"

        // APSR and FPSCR setup
        "msr     APSR_nzcvqg, lr                   \n"
        "push    {r4}                              \n"
        "vmrs    ip, fpscr                         \n"
        "movw    r4, #65376                        \n"
        "movt    r4, #4095                         \n"
        "and     ip, r4                            \n"
        "vmsr    fpscr, ip                         \n"
        "pop     {r4}                              \n"

        "mov     ip, lr                            \n"
        "adds    r7, r7, #28                       \n"
        "mov     sp, r7                            \n"
        "pop     {r0, r1, r2, r3, r4, r7, r12}     \n"
        "bics    lr, #1                            \n"
        "bxns    lr                                \n"
    );
}

NSENTRY_NAKED void SECURE_log_ret() {
    __asm__ volatile(
        "push    {r0, r1, r2, r3, r4, r7, r12}     \n"
        "sub     sp, sp, #28                       \n"
        "add     r7, sp, #0                        \n"
        "push    {r7, lr}                          \n"
        "sub     sp, sp, #8                        \n"
        "add     r7, sp, #0                        \n"

        // Move the return address (in LR) into r0 and call the logging function
        "mov     r0, lr                            \n"
        "bl      CFA_ENGINE_new_log_entry          \n"

        "nop                                       \n"
        "adds    r7, r7, #8                        \n"
        "mov     sp, r7                            \n"
        "pop     {r7, lr}                          \n"
        "mov     r0, lr                            \n"
        "mov     r1, lr                            \n"
        "mov     r2, lr                            \n"
        "mov     r3, lr                            \n"

        // Floating-point register setup
        "vmov.f32 s0, #1.0e+0                      \n"
        "vmov.f32 s1, #1.0e+0                      \n"
        "vmov.f32 s2, #1.0e+0                      \n"
        "vmov.f32 s3, #1.0e+0                      \n"
        "vmov.f32 s4, #1.0e+0                      \n"
        "vmov.f32 s5, #1.0e+0                      \n"
        "vmov.f32 s6, #1.0e+0                      \n"
        "vmov.f32 s7, #1.0e+0                      \n"
        "vmov.f32 s8, #1.0e+0                      \n"
        "vmov.f32 s9, #1.0e+0                      \n"
        "vmov.f32 s10, #1.0e+0                     \n"
        "vmov.f32 s11, #1.0e+0                     \n"
        "vmov.f32 s12, #1.0e+0                     \n"
        "vmov.f32 s13, #1.0e+0                     \n"
        "vmov.f32 s14, #1.0e+0                     \n"
        "vmov.f32 s15, #1.0e+0                     \n"

        // APSR and FPSCR setup
        "msr     APSR_nzcvqg, lr                   \n"
        "push    {r4}                              \n"
        "vmrs    ip, fpscr                         \n"
        "movw    r4, #65376                        \n"
        "movt    r4, #4095                         \n"
        "and     ip, r4                            \n"
        "vmsr    fpscr, ip                         \n"
        "pop     {r4}                              \n"

        "mov     ip, lr                            \n"
        "adds    r7, r7, #28                       \n"
        "mov     sp, r7                            \n"
        "pop     {r0, r1, r2, r3, r4, r7, r12}     \n"
        "bics    lr, #1                            \n"
        "bxns    lr                                \n"
    );
}


NSENTRY_NAKED void SECURE_log_call(uint32_t addr) {
    __asm__ volatile(
        "push    {r0, r1, r2, r3, r4, r7, r10}     \n"
        "sub     sp, sp, #28                       \n"
        "add     r7, sp, #0                        \n"
        "push    {r7, lr}                          \n"
        "sub     sp, sp, #8                        \n"
        "add     r7, sp, #0                        \n"

        // Decrement the address (assumed in r10) and call logging function
        "sub     r0, r10, #1                       \n"
        "bl      CFA_ENGINE_new_log_entry          \n"

        "nop                                       \n"
        "adds    r7, r7, #8                        \n"
        "mov     sp, r7                            \n"
        "pop     {r7, lr}                          \n"
        "mov     r0, lr                            \n"
        "mov     r1, lr                            \n"
        "mov     r2, lr                            \n"
        "mov     r3, lr                            \n"

        // Floating-point register setup
        "vmov.f32 s0, #1.0e+0                      \n"
        "vmov.f32 s1, #1.0e+0                      \n"
        "vmov.f32 s2, #1.0e+0                      \n"
        "vmov.f32 s3, #1.0e+0                      \n"
        "vmov.f32 s4, #1.0e+0                      \n"
        "vmov.f32 s5, #1.0e+0                      \n"
        "vmov.f32 s6, #1.0e+0                      \n"
        "vmov.f32 s7, #1.0e+0                      \n"
        "vmov.f32 s8, #1.0e+0                      \n"
        "vmov.f32 s9, #1.0e+0                      \n"
        "vmov.f32 s10, #1.0e+0                     \n"
        "vmov.f32 s11, #1.0e+0                     \n"
        "vmov.f32 s12, #1.0e+0                     \n"
        "vmov.f32 s13, #1.0e+0                     \n"
        "vmov.f32 s14, #1.0e+0                     \n"
        "vmov.f32 s15, #1.0e+0                     \n"

        // APSR and FPSCR setup
        "msr     APSR_nzcvqg, lr                   \n"
        "vmrs    ip, fpscr                         \n"
        "movw    r4, #65376                        \n"
        "movt    r4, #4095                         \n"
        "and     ip, r4                            \n"
        "vmsr    fpscr, ip                         \n"

        "mov     ip, lr                            \n"
        "adds    r7, r7, #28                       \n"
        "mov     sp, r7                            \n"
        "pop     {r0, r1, r2, r3, r4, r7, r10}     \n"
        "bics    r10, #1                           \n"
        "bxns    r10                               \n"
    );
}
