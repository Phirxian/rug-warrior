*
* "sbirxmit.asm" for the 6.270 Rev 2.2 board
*
* by Fred Martin
* Mon Oct 28 10:25:45 1991
*
* Modified by Anne Wright
* Sun Jan  3 07:22:16 EST 1993
*
* Performs function of broadcasting on s-bot's IR transmitters
* (choice of two frequencies),
* and decoding choice of frequency present on any of the 4 Sharp IR
* receivers.
*
* Note that with the current implementation you can only choose one of
* two desired reception frequencies; you cannot be set up to detect
* both those frequencies at the same time.
*

#include <6811regs.asm>
#include <r22_eq.asm>

	ORG	MAIN_START

* input variable determines transmission frequency
* should be 10000 for 100 Hz tone,
* or         8000 for 125 Hz tone.
variable_ir_xmit_period		FDB	8000

* input variable controls selection of frequency to be decoded
* value should be 125 or 100
variable_ir_freq_rcv_select	FDB	125

* 0 for routine off; other value for routine on.
variable_num_ir_sensors		FDB	0

* offsets to these state variables (from pointer to _counts variable)
IR_COUNTS	EQU	1		; use low byte of variable
IR_PHASE	EQU	9
IR_ERROR	EQU	17

* output variables count for each consecutive time that IR square
* wave was successfully decoded
variable_ir_counts_zero		FDB	0
variable_ir_counts_one		FDB	0
variable_ir_counts_two		FDB	0
variable_ir_counts_three	FDB	0

ir_phase_zero			FDB	0
ir_phase_one			FDB	0
ir_phase_two			FDB	0
ir_phase_three			FDB	0

ir_error_zero			FDB	0
ir_error_one			FDB	0
ir_error_two			FDB	0
ir_error_three			FDB	0

* routine to transmit a square wave on the IR emitters
* 
* uses TOC2 interrupt
toc2_code_start:

* setup for next interrupt
	LDX	#BASE		* point to register base
	LDD	variable_ir_xmit_period
	ADDD	TOC2,X		* add TOC2 to D
	STD	TOC2,X		* store back
	BCLR	TFLG1,X %10111111	* clear OC2 for next compare
	LDAA	PORTA,X
	EORA	#IR_XMIT
	STAA	PORTA,X		; toggle IR output pin

	RTI

* IR decoding routine starts here
* (installed on 1000Hz system interrupt)
*

interrupt_code_start:

* test if software is enabled
	LDAA	variable_num_ir_sensors+1
	BEQ	interrupt_code_exit

* determine reception frequency
	LDY	#IR_jumptable125
	LDD	variable_ir_freq_rcv_select
	CMPD	#125
	BEQ	IR_go
	LDY	#IR_jumptable100

* do decoding at selected frequency
IR_go:
	LDAA	IR_ADDR_ZERO
	ANDA	#IR_MASK_ZERO
	LDX	#variable_ir_counts_zero
	PSHY
	BSR	IR_doitbaby
	PULY

	LDAA	IR_ADDR_ONE
	ANDA	#IR_MASK_ONE
	INX
	INX			; to ir_counts_one
	PSHY
	BSR	IR_doitbaby
	PULY

	LDAA	IR_ADDR_TWO
	ANDA	#IR_MASK_TWO
	INX
	INX			; to ir_counts_two
	PSHY
	BSR	IR_doitbaby
	PULY

	LDAA	IR_ADDR_THREE
	ANDA	#IR_MASK_THREE
	INX
	INX			; to ir_counts_three
	PSHY
	BSR	IR_doitbaby
	PULY

interrupt_code_exit:
	JMP	$0000		/* this value poked in by init routine */


*
*
* IR phase locked loop code
*
* algorithm:  compare incoming waveform to a "waveform" generated
* internally.  sum the # of errors (when the two waves don't match)
* over the course of one waveform.  If the error count is large,
* then we have not "found" the desired frequency.  If the error count
* is small, then we have.
*
* at rising edge of internal wave, make phase corrections by jumping
* ahead one notch or waiting one notch.
*
* incoming wave:
*
*  +           +-- -- -- --+           +-- -- -- --+
*  |           |           |           |           |
*  +-- -- -- --+           +-- -- -- --+           +--
*
* internal wave: (for 125 Hz detection)
*
*            +-- -- -- --+           +-- -- -- --+
*            |           |           |           |
* -- -- -- --+           +-- -- -- --+           +--
*  0  1  2  3  4  6  7  8  0  1  2  3
*              5 (state 4 after wait)
*
* at state 3, if incoming wave is high, we are behind it.
*             jump to state 6 (catch up).
* at state 4, if incoming wave is low, we are ahead of it.
*	      go to state 5 (wait for it to catch up).
* all other states:  proceed to next state.
* all states:  accumulate error if internal wave != incoming wave.
*
* internal wave for 100 Hz detection
*
*               +-- -- -- -- --+
*               |              |
* -- -- -- -- --+              +-- -- -- -- --
*  0  1  2  3  4  5  7  8  9 10
*                 6 (state 5 after wait)
*
*
* register usage:
*   A==0 if sensor was 0, not zero otherwise
*   X has ptr to table of IRdetect, IRphase, and IRerror
*   Y has ptr to jumptable for either 100Hz or 125Hz detection
*      (destroyed on exit)
*

IR_DETECT_LIMIT	EQU	2	; max 2 errors per cycle for detect
IR_DETECT_THRESHOLD EQU 5	; # of detects for LCD status indicator

IR_doitbaby:
* check if phase is > 10; if so, reset to zero
	LDAB	IR_PHASE,X
	CMPB	#10
	BLS	IR_phaseok
	CLRB
	STAB	IR_PHASE,X
IR_phaseok:
* multiply phase by 2 and calc jump
	ASLB
	ABY
	LDY	0,Y		; fetch jump
	JMP	0,Y

IR_jumptable100
	FDB	IR_start_cycle	; phase 0
	FDB	IR_low_phase	; phase 1
	FDB	IR_low_phase	; phase 2
	FDB	IR_low_phase	; phase 3
	FDB	IR_pre_edge	; phase 4
	FDB	IR_post_edge	; phase 5
	FDB	IR_high_phase	; phase 6
	FDB	IR_high_phase	; phase 7
	FDB	IR_high_phase	; phase 8
	FDB	IR_high_phase	; phase 9
	FDB	IR_end_cycle	; phase 10

IR_jumptable125
	FDB	IR_start_cycle	; phase 0
	FDB	IR_low_phase	; phase 1
	FDB	IR_low_phase	; phase 2
	FDB	IR_pre_edge	; phase 3
	FDB	IR_post_edge	; phase 4
	FDB	IR_high_phase	; phase 5
	FDB	IR_high_phase	; phase 6
	FDB	IR_high_phase	; phase 7
	FDB	IR_end_cycle	; phase 8
* the following should get called only when user switches from 125hz to
* 100hz detection on the fly
	FDB	IR_start_cycle	; phase 0
	FDB	IR_start_cycle	; phase 0


* start cycle	calc IRdetect based on previous error; set new error
*              	based on current sensor val.
IR_start_cycle:
	LDAB	IR_ERROR,X
	CMPB	#IR_DETECT_LIMIT
	BLS	IR_detect_true
* no detect
	CLR	IR_COUNTS,X
	BRA	IR_check_phase0
IR_detect_true
* increment byte until it reaches 0xFF
	LDAB	IR_COUNTS,X
	INCB
	BNE	IR_detect_ok
	DECB
IR_detect_ok
	STAB	IR_COUNTS,X
IR_check_phase0
	TSTA
	BEQ	IR_phase0_noerr
* error
	LDAB	#1
	STAB	IR_ERROR,X
	BRA	IR_setphase1
IR_phase0_noerr
	CLR	IR_ERROR,X
	LDAB	#1
IR_setphase1
	STAB	IR_PHASE,X
	RTS

* IR_low_phase	increment error count if incoming wave is high
*               increment phase
IR_low_phase:
	INC	IR_PHASE,X
	TSTA
	BNE	IR_incerrorreturn
	RTS
IR_incerrorreturn
	INC	IR_ERROR,X
	RTS

* IR_pre_edge	if wave is high, inc error and goto phase n+3;
*		otherwise, inc phase & return
IR_pre_edge:
	TSTA
	BNE	IR_catchup
* no prob; inc phase & return
IR_incphasereturn
	INC	IR_PHASE,X
	RTS
* inc error, goto phase n+3
IR_catchup:
	INC	IR_ERROR,X
	LDAB	IR_PHASE,X
	ADDB	#3
	STAB	IR_PHASE,X
	RTS

* IR_post_edge	if wave is low; inc error and goto phase n+1;
*		otherwise, proceed normally to phase n+2 and return
IR_post_edge:
	TSTA
	BEQ	IR_wait
* normal exit: phase += 2
	LDAB	IR_PHASE,X
	ADDB	#2
	STAB	IR_PHASE,X
	RTS
* inc error, inc phase
IR_wait
	INC	IR_ERROR,X
	INC	IR_PHASE,X
	RTS

* IR_high_phase	inc phase
*               if wave is high; return; otherwise inc error & return
IR_high_phase:
	INC	IR_PHASE,X
	TSTA
	BEQ	IR_incerrorreturn
	RTS

* IR_end_cycle	set phase to zero
*		if wave is low incr. error
IR_end_cycle:
	CLR	IR_PHASE,X
	TSTA
	BEQ	IR_incerrorreturn
	RTS

subroutine_initialize_module:
#include <ldxibase.asm>
/* X now has base pointer to interrupt vectors ($FF00 or $BF00) */
	BCLR	PORTA,X $40		; shut off ir's by default

	LDD	#toc2_code_start
	STD	TOC2INT,X		; TOC2 interrupt

/*  get current vector; poke such that when we finish, we go there */
	LDD	TOC4INT,X		/* SystemInt on TOC4 */
	STD	interrupt_code_exit+1

* install ourself as new vector
	LDD	#interrupt_code_start
	STD	TOC4INT,X

* disable decoding routine
	CLR	variable_num_ir_sensors+1

* turn on TOC2 interrupt
  	LDX	#BASE
	BSET	TFLG1,X $40		; enable interrupt

	RTS

