* file of equates to control rev 2.2 board

* Anne Wright
* Sun Jan  3 07:22:16 EST 1993


* following devices are on 6811 Port A
IR_XMIT	EQU	$40
BEEP	EQU	$08

* equates for Sharp IR sensors -- start at port 4
IR_MASK_ZERO	EQU	%00010000
IR_ADDR_ZERO	EQU	$7000

IR_MASK_ONE	EQU	%00100000
IR_ADDR_ONE	EQU	$7000

IR_MASK_TWO	EQU	%01000000
IR_ADDR_TWO	EQU	$7000

IR_MASK_THREE	EQU	%10000000
IR_ADDR_THREE	EQU	$7000
