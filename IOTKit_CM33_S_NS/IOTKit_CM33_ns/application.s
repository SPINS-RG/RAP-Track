	.cpu cortex-m33
	.arch armv8-m.main
	.fpu fpv5-sp-d16
	.eabi_attribute 27, 1
	.eabi_attribute 28, 1
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 34, 1
	.eabi_attribute 18, 2
	.text
	.section	.MTBDR_MEM,"ax",%progbits
	.align	1
	.global	application
	.syntax unified
	.thumb
	.thumb_func
	.type	application, %function
application:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	sub	sp, sp, #8
	add	r7, sp, #0
	movs	r3, #0
	str	r3, [r7, #4]
	bl	getUltrasonicReading
	str	r0, [r7, #4]
	ldr	r2, .L2
	ldr	r3, [r7, #4]
	str	r3, [r2]
	adds	r7, r7, #8
	mov	sp, r7
	@ sp needed
	pop	{r7, lr}
	b	SECURE_log_ret
.L3:
	.align	2
.L2:
	.word	read_val
	.size	application, .-application
	.align	1
	.global	application_entry
	.syntax unified
	.thumb
	.thumb_func
	.type	application_entry, %function
application_entry:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	add	r7, sp, #0
	bl	application
	pop	{r7, lr}
	b	SECURE_log_ret
	.size	application_entry, .-application_entry
	.global	read_val
	.section	.bss.read_val,"aw",%nobits
	.align	2
	.type	read_val, %object
	.size	read_val, 4
read_val:
	.space	4
	.global	GPIOA
	.section	.data.GPIOA,"aw"
	.align	2
	.type	GPIOA, %object
	.size	GPIOA, 4
GPIOA:
	.word	1073872896
	.section	.MTBDR_MEM
	.align	1
	.global	delay
	.syntax unified
	.thumb
	.thumb_func
	.type	delay, %function
delay:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r7, lr}
	sub	sp, sp, #20
	add	r7, sp, #0
	str	r0, [r7, #4]
	movs	r3, #0
	str	r3, [r7, #12]
	b	.L7
.L8:
	bl	SECURE_log_cond_br_taken
	ldr	r3, [r7, #12]
	adds	r3, r3, #1
	str	r3, [r7, #12]
.L7:
	ldr	r3, [r7, #12]
	ldr	r2, [r7, #4]
	cmp	r2, r3
	bhi	.L8
	bl	SECURE_log_cond_br_not_taken
	adds	r7, r7, #20
	mov	sp, r7
	@ sp needed
	pop	{r7, lr}
	b	SECURE_log_ret
	.size	delay, .-delay
	.align	1
	.global	pulseIn
	.syntax unified
	.thumb
	.thumb_func
	.type	pulseIn, %function
pulseIn:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r7, lr}
	sub	sp, sp, #12
	add	r7, sp, #0
	movs	r3, #0
	str	r3, [r7, #4]
	movs	r3, #0
	str	r3, [r7]
	b	.L10
.L11:
	bl	SECURE_log_cond_br_taken
	ldr	r3, .L13
	ldr	r3, [r3]
	ldr	r3, [r3, #16]
	lsrs	r3, r3, #8
	and	r3, r3, #1
	ldr	r2, [r7, #4]
	add	r3, r3, r2
	str	r3, [r7, #4]
	ldr	r3, [r7]
	adds	r3, r3, #1
	str	r3, [r7]
.L10:
	ldr	r3, [r7]
	cmp	r3, #1000
	blt	.L11
	bl	SECURE_log_cond_br_not_taken
	ldr	r3, [r7, #4]
	mov	r0, r3
	adds	r7, r7, #12
	mov	sp, r7
	@ sp needed
	pop	{r7, lr}
	b	SECURE_log_ret
.L14:
	.align	2
.L13:
	.word	GPIOA
	.size	pulseIn, .-pulseIn
	.align	1
	.global	getUltrasonicReading
	.syntax unified
	.thumb
	.thumb_func
	.type	getUltrasonicReading, %function
getUltrasonicReading:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	sub	sp, sp, #8
	add	r7, sp, #0
	ldr	r3, .L17
	ldr	r3, [r3]
	mov	r2, #256
	str	r2, [r3, #24]
	movs	r0, #2
	bl	delay
	ldr	r3, .L17
	ldr	r3, [r3]
	mov	r2, #256
	str	r2, [r3, #40]
	movs	r0, #5
	bl	delay
	ldr	r3, .L17
	ldr	r3, [r3]
	mov	r2, #256
	str	r2, [r3, #24]
	bl	pulseIn
	str	r0, [r7, #4]
	ldr	r3, [r7, #4]
	mov	r0, r3
	adds	r7, r7, #8
	mov	sp, r7
	@ sp needed
	pop	{r7, lr}
	b	SECURE_log_ret
.L18:
	.align	2
.L17:
	.word	GPIOA
	.size	getUltrasonicReading, .-getUltrasonicReading
	.ident	"GCC: (GNU Tools for STM32 12.3.rel1.20240612-1315) 12.3.1 20230626"
