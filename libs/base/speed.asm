/* This section starts the 6811 assembler routines  */

TFLG1    EQU     $1023

  ORG MAIN_START             

subroutine_initialize_module:        /* This module runs whenever a reset occurs */

     ldd #IC3_interrupt_handler      /* Get 16-bit address of interrupt handler */
     std $FFEA                       /* Store in IC3 interrupt vector location */
     cli                             /* Enable interrupts globally */
     rts                             /* Return from initialzie_module subroutine */


variable_left_clicks:		     /* Create right_clicks, a variable accessible by IC */
     fdb 0                           /* Fill double byte (16 bits).  right_clicks = 0 */

IC3_interrupt_handler:       
     ldd variable_left_clicks	     /* Get the value in right_clicks */
     addd #1                         /* Add one more encoder count */
     std variable_right_clicks       /* Store back to right_clicks */
     ldaa #%00000001                 /* Clear the IC3 flag by writing a one */
     staa TFLG1                      /* Store in TFLG1 to clear IC3 flag */
     rti                             /* Return from interrupt */

;; S11C8020 CC 802A FD FFEA 0E 39 0000   FC 8028 C3 0001 FD 8028 86 01 B7 1023 3B E7
;; S9030000FC
;; S11C872B CC 8735 FD FFEA 0E 39 0000   FC 8733 C3 0001 FD 8733 86 01 B7 1023 3B 9F
;; S9030000FC

;; In special test mode FFEA becomds BFEA:
;; S11C872B CC 8735 FD BFEA 0E 39 0000   FC 8733 C3 0001 FD 8733 86 01 B7 1023 3B 9F
;; S9030000FC