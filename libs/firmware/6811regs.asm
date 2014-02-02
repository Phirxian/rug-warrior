* file of standard 6811 register declarations

*********************************************************************
* Control Registers

BASE	EQU	$1000

PORTA	EQU	$1000	; Port A data register
RESV1	EQU	$1001	; Reserved
PIOC	EQU	$1002	; Parallel I/O Control register
PORTC	EQU	$1003	; Port C latched data register
PORTB	EQU	$1004	; Port B data register
PORTCL	EQU	$1005	;
DDRC	EQU	$1007	; Data Direction register for port C
PORTD   EQU     $1008	; Port D data register
DDRD    EQU     $1009	; Data Direction register for port D
PORTE	EQU	$100A	; Port E data register
CFORC	EQU	$100B	; Timer Compare Force Register
OC1M	EQU	$100C	; Output Compare 1 Mask register
OC1D	EQU	$100D	; Output Compare 1 Data register

* Two-Byte Registers (High,Low -- Use Load & Store Double to access)
TCNT	EQU	$100E	; Timer Count Register
TIC1	EQU	$1010	; Timer Input Capture register 1
TIC2	EQU	$1012	; Timer Input Capture register 2
TIC3	EQU	$1014	; Timer Input Capture register 3
TOC1	EQU	$1016	; Timer Output Compare register 1
TOC2	EQU	$1018	; Timer Output Compare register 2
TOC3	EQU	$101A	; Timer Output Compare register 3
TOC4	EQU	$101C	; Timer Output Compare register 4
TI4O5	EQU	$101E	; Timer Input compare 4 or Output compare 5 register

TCTL1	EQU	$1020	; Timer Control register 1
TCTL2	EQU	$1021	; Timer Control register 2
TMSK1	EQU	$1022	; main Timer interrupt Mask register 1
TFLG1	EQU	$1023	; main Timer interrupt Flag register 1
TMSK2	EQU	$1024	; misc Timer interrupt Mask register 2
TFLG2	EQU	$1025	; misc Timer interrupt Flag register 2
PACTL	EQU	$1026	; Pulse Accumulator Control register
PACNT	EQU	$1027	; Pulse Accumulator Count register
SPCR	EQU	$1028	; SPI Control Register
SPSR	EQU	$1029	; SPI Status Register
SPDR	EQU	$102A	; SPI Data Register
BAUD	EQU	$102B	; SCI Baud Rate Control Register
SCCR1	EQU	$102C	; SCI Control Register 1
SCCR2	EQU	$102D	; SCI Control Register 2
SCSR	EQU     $102E	; SCI Status Register
SCDR	EQU     $102F	; SCI Data Register
ADCTL	EQU	$1030	; A/D Control/status Register
ADR1	EQU	$1031	; A/D Result Register 1
ADR2	EQU	$1032	; A/D Result Register 2
ADR3	EQU	$1033	; A/D Result Register 3
ADR4	EQU	$1034	; A/D Result Register 4
BPROT	EQU	$1035	; Block Protect register
RESV2	EQU	$1036	; Reserved
RESV3	EQU	$1037	; Reserved
RESV4	EQU	$1038	; Reserved
OPTION	EQU	$1039	; system configuration Options
COPRST	EQU	$103A	; Arm/Reset COP timer circuitry
PPROG	EQU	$103B	; EEPROM Programming register
HPRIO	EQU	$103C	; Highest Priority Interrupt and misc.
INIT	EQU	$103D	; RAM and I/O Mapping Register
TEST1	EQU	$103E	; factory Test register
CONFIG	EQU	$103F	; Configuration Control Register


* Interrupt Vector locations

SCIINT	EQU	$D6	; SCI serial system
SPIINT	EQU	$D8	; SPI serial system
PAIINT EQU	$DA	; Pulse Accumulator Input Edge
PAOVINT EQU	$DC	; Pulse Accumulator Overflow
TOINT	EQU	$DE	; Timer Overflow
TOC5INT	EQU	$E0	; Timer Output Compare 5
TOC4INT	EQU	$E2	; Timer Output Compare 4
TOC3INT	EQU	$E4	; Timer Output Compare 3
TOC2INT	EQU	$E6	; Timer Output Compare 2
TOC1INT	EQU	$E8	; Timer Output Compare 1
TIC3INT	EQU	$EA	; Timer Input Capture 3
TIC2INT	EQU	$EC	; Timer Input Capture 2
TIC1INT	EQU	$EE	; Timer Input Capture 1
RTIINT	EQU	$F0	; Real Time Interrupt
IRQINT	EQU	$F2	; IRQ External Interrupt
XIRQINT	EQU	$F4	; XIRQ External Interrupt
SWIINT	EQU	$F6	; Software Interrupt
BADOPINT EQU	$F8	; Illegal Opcode Trap Interrupt
NOCOPINT EQU	$FA	; COP Failure (Reset)
CMEINT	EQU	$FC	; COP Clock Monitor Fail (Reset)
RESETINT EQU	$FE	; RESET Interrupt


