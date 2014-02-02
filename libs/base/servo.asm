* icb file:  "servo.asm"
*
*   Anne Wright and PK Oberoi
*   Sat Jan 12 05:12:13 1992
*   Sun Jan 13 03:31:55 1992 added sweeping capabilities -- anarch

#include <6811regs.asm>
* program equates
SERVO_ADDRESS	EQU	PORTA		/* Using PortA for Output */
SERVO_MASK	EQU	%10000000	/* D7 is the control Signal */
REPEAT_PERIOD	EQU	20		/* number of interrupts between pulses */

	ORG	MAIN_START

* variables

/* C variables */
variable_servo_pulse_wavetime	FDB	3000
variable_servo_enable		FDB	0

/* internal variable */
servo_count			FCB	0
	
subroutine_initialize_module:
#include <ldxibase.asm>
* X now has base pointer to interrupt vectors ($FF00 or $BF00)

* get current vector; poke such that when we finish, we go there
	LDD	TOC4INT,X		; SystemInt on TOC4
	STD	interrupt_code_exit+1

* install ourself as new vector
	LDD	#interrupt_code_start
	STD	TOC4INT,X
	
	LDX	#BASE
* set up A7 for output
	BSET	PACTL,X $80
	BSET	OC1M,X $80		/* set OC1 to control A7 */

*initialize Servo Parameters
	LDD	#3000
	STD	variable_servo_pulse_wavetime	/* Initialize  servo 
						period to 2000 */	
* fall through to servo_off subroutine

/************************************************************************/
subroutine_asm_servo_off:
	LDX	#BASE

	LDD	#0
	STD	variable_servo_enable	/* disable servo */
	BCLR	OC1D,X $80		/* set OC1 to clear on match */
	
	RTS
/************************************************************************/


/************************************************************************/
subroutine_asm_servo_on:
	LDX	#BASE
	LDD 	#1			/* load 1 into D */
	STD	variable_servo_enable	/* enable servo */

	LDAA	#REPEAT_PERIOD 		/* Reset Cycle Counter */
	STAA	servo_count
	
	RTS

interrupt_code_start:
	LDD	variable_servo_enable	/* check if servo is on */
	BEQ	interrupt_code_exit

	LDAA	servo_count		/* check if at beginning of cycle */
	BEQ	SetPulse

	ADDA	#-1			/* No: decrement cycle counter */
	STAA	servo_count
	
	BRA	interrupt_code_exit

SetPulse:
/*#include "r21_beep.asm"*/
	LDX	#BASE

	LDD	TCNT,X			/* Get current timer counts */
	ADDD	variable_servo_pulse_wavetime	/* Add the servo period and */
	STD	TOC1			/* Place in TOC1 */

	BSET	OC1D,X $80		/* set OC1 to set on match */
	BSET	CFORC,X $80             /* force a match now */

	BCLR	OC1D,X $80		/* set OC1 to clear on match */

	LDAA	#REPEAT_PERIOD 		/* Reset Cycle Counter */
	STAA	servo_count

interrupt_code_exit:
	JMP	$0000	; this value poked in by init routine

