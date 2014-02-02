*
* PWM Motor driver code for Rev 2.1 Expansion Board
*
* by Fred Martin
* Thu Jan  9 12:36:37 1992
*

#include <6811regs.asm>

* required statement to start code
	ORG	MAIN_START


* declare variables

* high byte of this word controls motors and 2 transistors
variable_exp_motor_bits:
	FDB	0

* high byte is motor 5 bits, low byte is motor 4 bits
variable_exp_motor_speeds:
	FDB	$FFFF

* initialization routine 
* (1) install into System Interrupt structure
* (2) zero out 374 chip
* (3) initialize subroutine variables

subroutine_initialize_module:
#include <ldxibase.asm>
	LDD	TOC4INT,X		; SystemInt on TOC4
	STD	interrupt_code_exit+1
	LDD	#interrupt_code_start
	STD	TOC4INT,X

	CLR	$4000			; turn off expansion bits

	LDD	#$3C00
	STD	variable_exp_motor_bits
	LDD	#$FFFF
	STD	variable_exp_motor_speeds

	RTS

* main PWM routine
interrupt_code_start:

	LDAB	variable_exp_motor_bits
	ANDB	#%11000000		; keep motor on data only
	EORB	#%11000000		; flip speed bits

* motor 5
        LDAA	variable_exp_motor_speeds     ; rotate bits in motor speed
	ASLA
	BCC	SPDoffA
	ADDA	#1
	BRA	SPDsetA
SPDoffA	ORAB	#%01000000
SPDsetA	STAA	variable_exp_motor_speeds

* motor 4
        LDAA	variable_exp_motor_speeds+1     ; rotate bits in motor speed
	ASLA
	BCC	SPDoffB
	ADDA	#1
	BRA	SPDsetB
SPDoffB	ORAB	#%10000000
SPDsetB	STAA	variable_exp_motor_speeds+1     ; rotate bits in motor speed

	EORB	#%11000000			; flip motor speed bits

	LDAA	variable_exp_motor_bits
	ANDA	#$3F		; turn off speed bits
	ABA			; add in speed bits

SPDexit STAA	$4000

interrupt_code_exit:
	JMP	$0000
