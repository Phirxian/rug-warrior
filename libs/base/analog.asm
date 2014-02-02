* analog.asm for the rev 2.2 6.270 board
* 
* written by Anne Wright, December 3, 1992
* This file is incompatible with the rev 2.1 and earlier boards and 
* compatible with rev 2.2 and 2.21 as of March 5, 1994

* Performs atomic operation of setting analog multiplexers and
* returning value of an analog port 

* Argument is integer from 0 to 31 corresponding to the 
* consecutive selectable ports on the three expansion board 
* 4051s.  The expectation is that this code will normally be called by 
* the analog() function in lib_r22.c which does limit checking

#include <6811regs.asm>

	ORG	MAIN_START
subroutine_analog_header_port:	
*				  to increase efficiency of header port access
*				  accum B contains port number (8-11).
*				  actual 6811 port numbers are (4-7), 
*				  so just subtract 4 and jump...
	SEI		; turn off system interrupts so that
*			  we're not confused by system analog calls
	LDX	#BASE
	ADDB	#-4		; subtract 4 to normalize to hardware
*				  go to store value in ADCTL, read, and return
	BRA	read_analog_direct
subroutine_read_analog:		
*			; Accum B contains analog port number, 12-35
	SEI		; turn off system interrupts so that
*			  we're not confused by system analog calls
	LDX	#BASE
	ADDB	#-12		; Subtract 12 to normalize to start at 0
	PSHB		; push the value so it can be used later
* Set the select bits on the analog multiplexer
	LSLB
	LSLB
	LSLB		; shift select bits into position
	ANDB	#%00111000	; mask off the select bits
	LDAA	PORTD,X		; load A with the contents of portD
	ANDA	#%11000111	; clear the select bits	
	ABA			; put the result of A or B into A
	STAA	PORTD,X		; Now the select bits of portD are modified!
	PULB			; B now contains the original value

	LSRB			; now set the 6811 analog port to read
	LSRB
	LSRB			; shift the analog port bits into position
	ADDB	#4		; add 4 for offset (0->4, 1->5, etc...)
read_analog_direct:
	STAB	ADCTL,X		; store the port select value into the ctl reg
	BRCLR	ADCTL,X $80 *	; loop till analogs ready
	LDAB	ADR1,X		; load B with result
	CLRA			; clear A

	CLI			; enable other interrupts
	RTS			; return



