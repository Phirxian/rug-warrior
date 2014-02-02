* 
* File "ldxibase.asm"
*
* Fred Martin Thu Oct 10 19:49:38 1991
*
*
* The following code loads the X register with a base pointer to
* the 6811 interrupt vectors:  $FF00 if the 6811 is in normal mode,
* and $BF00 if the 6811 is in special mode.
*
* The file "6811regs.asm" must be loaded first for this to work.
*
*

	LDAA	HPRIO
	ANDA	#$40			; test SMOD bit
	BNE	*+7
	LDX	#$FF00			; normal mode interrupts
	BRA	*+5
	LDX	#$BF00			; special mode interrupts

