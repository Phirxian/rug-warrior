* Encoders.asm -- assembler control for encoders for 6.270 rev 2.2 board 
* Encoders on digital ports 0 and 1 are interrupt driven and are efficient
* Encoders on digital ports 2 and 3 require constant servicing and should 
*    only be used after digitals 0 and 1 are used up.

* Anne Wright Jan 19, 1993

#include <6811regs.asm>
* program equates
port_address			EQU	$7000
port2_mask			EQU	%00000100
port3_mask			EQU	%00001000

* variables

	ORG	MAIN_START

variable_port0_shaft_count	FDB	0
variable_port1_shaft_count	FDB	0
variable_port2_shaft_count	FDB	0
variable_port3_shaft_count	FDB	0

port2_enabled		FCB	0
port3_enabled		FCB	0
interrupts_installed	FCB	0

port2_state		FCB	0
port3_state		FCB	0

subroutine_initialize_module:

#include <ldxibase.asm>
* X now has base pointer to interrupt vectors ($FF00 or $BF00)

	LDD	#Port0_ShaftInt
	STD	TIC3INT,X
	
	LDD	#Port1_ShaftInt
	STD	TIC2INT,X	


* Setup Interrupt for Encoders *
	LDX	#BASE			
	BSET	TCTL2,X %00001111	/* Interrupts TIC3,TIC2 rising edge*/
	BCLR	TFLG1,X %00000011	/* Clear IC3,IC2 Flags */
	BCLR	TMSK1,X	%00000011	/* Clear Mask For Interrupt */

	LDAA	#0
	STAA	interrupts_installed	/* default interrupts not intalled */

	RTS
	

/************************************************************************/
subroutine_enable_cheesy_encoders:   /* B should have 2 or 3 in it */
	PSHB
	LDAA	interrupts_installed
	BNE	already_installed
	BSR	install_interrupt_driver	/* install on interrupt */
	LDAA	#1
	STAA	interrupts_installed

already_installed:
	PULB
	CMPB	#2		/* see if enabling port2 */
	BNE	install_port3
	LDAA	#1
	STAA	port2_enabled
	LDD	#0
	STD	variable_port2_shaft_count
	RTS

install_port3:
	LDAA	#1
	STAA	port3_enabled
	LDD	#0
	STD	variable_port3_shaft_count
	RTS

subroutine_disable_cheesy_encoders:   /* B should have 2 or 3 in it */
	CMPB	#2		/* see if enabling port2 */
	BNE	disable_port3
	CLR	port2_enabled
	RTS

disable_port3:
	CLR	port3_enabled
	RTS

Port0_ShaftInt:
	LDX	variable_port0_shaft_count
	INX
	STX	variable_port0_shaft_count
	LDX	#BASE
	BCLR	TFLG1,X %11111110
	RTI

Port1_ShaftInt:
	LDX	variable_port1_shaft_count
	INX
	STX	variable_port1_shaft_count
	LDX	#BASE
	BCLR	TFLG1,X %11111101
	RTI

interrupt_code_start:
	LDAA	port2_enabled
	BEQ	port3_start

	LDAA	port_address
	ANDA	#port2_mask	
	CMPA	port2_state
	BEQ	port3_start

	STAA	port2_state
	LDX	variable_port2_shaft_count
	INX
	STX	variable_port2_shaft_count
	
port3_start:
	LDAA	port3_enabled
	BEQ	interrupt_code_exit	

	LDAA	port_address
	ANDA	#port3_mask	
	CMPA	port3_state
	BEQ	interrupt_code_exit

	STAA	port3_state
	LDX	variable_port3_shaft_count
	INX
	STX	variable_port3_shaft_count

interrupt_code_exit:
	JMP	$0000	; this value poked in by init routine

install_interrupt_driver:
#include <ldxibase.asm>
* X now has base pointer to interrupt vectors ($FF00 or $BF00)

* get current vector; poke such that when we finish, we go there
	LDD	TOC4INT,X		; SystemInt on TOC4
	STD	interrupt_code_exit+1

* install ourself as new vector
	LDD	#interrupt_code_start
	STD	TOC4INT,X
	RTS
