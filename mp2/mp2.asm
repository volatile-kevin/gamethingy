; Joe Lopez, Kevin Hu, Daniel Ao
; joseal2, kwh2, dao3
;
; This program evaluates postix expressions using a stack
; The solution will be output as a hexadecimal
; We implemented different subroutines for each arithmetic operation
; as well as a subroutine to evaluate the operation the user Inputs
; and a subroutine to print in hex, based on our MP1 code

.ORIG x3000

LD R6, STACK_TOP        ; STACK_TOP points to x4000 at the beginning

MAIN
	GETC
	OUT
	JSR EVALUATE
	BRnzp MAIN

;Evaluate the input before it is pushed to the stack
;If operator: go to operator subroutine
;If number: push to stack
EVALUATE
	ST R7, EVAL_SAVER7

	CHECK_SPACE
		LD R4,SPACE	  ;Check for spaces, don't add to stack if true
		NOT R4,R4
		ADD R4,R4,#1
		ADD R4,R0,R4
		BRz DONE_EVAL

	CHECK_PLUS
		LD R4,PLUS_SIGN	;Check for plus operator to call ADDITION
		NOT R4,R4
		ADD R4,R4,#1
		ADD R4,R0,R4
		BRnp CHECK_MINUS
		JSR ADDITION
		BRnzp DONE_EVAL

	CHECK_MINUS
		LD R4, MINUS_SIGN	;Check for minus operator to call STACK_NOTADD
		NOT R4,R4
		ADD R4,R4,#1
		ADD R4,R0,R4
		BRnp CHECK_DIVIDE
		JSR SUBTRACT
		BRnzp DONE_EVAL

	CHECK_DIVIDE
		LD R4,DIVIDE_SIGN	;Check for minus operator to call STACK_NOTADD
		NOT R4,R4
		ADD R4,R4,#1
		ADD R4,R0,R4
		BRnp CHECK_MULTIPLY
		JSR DIVIDE
		BRnzp DONE_EVAL

	CHECK_MULTIPLY
  	LD R4, MULTIPLY_SIGN ;Check for multiply operator to call STACK_MULT
  	NOT R4, R4
		ADD R4,R4,#1
  	ADD R4,R0,R4
		BRnp CHECK_EQUALS
		JSR MULTIPLY
		BRnzp DONE_EVAL

	CHECK_EQUALS
		LD R4, EQUALS_SIGN	;Check for equals to call PRINT_HEX
		ADD R4,R0,R4
		BRnp NUM_INPUT
		JSR PRINT_HEX
		BRnzp EXIT

	;Converts numbers to be pushed to the stack to hex
	NUM_INPUT
		LD R4,FOURTY_EIGHT	;Check if input is below 0 in hex
		ADD R4,R0,R4
		BRn INVALID_IN

		ADD R4,R4,#-9		;Check if input is above 9 in hex
		BRp INVALID_IN

		LD R4,FOURTY_EIGHT	;convert the ascii character to hex
		ADD R0,R0,R4
		JSR PUSH
		BRnzp DONE_EVAL

DONE_EVAL
	LD R7,EVAL_SAVER7
	RET

;Error message to be printed if invalid input
INVALID_IN
	LEA R0, ERRORMSG
	PUTS
	BRnzp EXIT

; Addition operation
; IN: 2 number inputs in R4+R0
; OUT: 1 output R0
; R5: indicates pop success
; R4: first number input
; R0: value in stack
ADDITION
	ST R7,ADD_SAVER7
	JSR POP
	ADD R5, R5, #0		; check if pop is successful
	BRp INVALID_IN		; print error message and HALT if underflow
	ADD R4,R0,#0			; store first value into R4
	JSR POP
	ADD R5, R5, #0		; check if pop is successful
	BRp INVALID_IN		; print error message and HALT if underflow
	ADD R0, R0, R4		; perform ‘+’ operation: R0 <- R0 + R1
	JSR PUSH
	LD R7,ADD_SAVER7
	RET

; subtraction operation
; IN: 2 number inputs in R4+R0
; OUT: 1 output R0
; R5: indicates pop success
; R4: first number input
; R0: value in stack
SUBTRACT
	ST R7,SUB_SAVER7	;Save R7 at this location
	JSR POP
	ADD R5, R5, #0	 ; check if pop is successful
	BRp INVALID_IN	 ; print error message and HALT if underflow
	NOT R4,R0		 		 ; make the top number negative
	ADD R4,R4,#1
	JSR POP
	ADD R5, R5, #0	 ; check if pop is successful
	BRp INVALID_IN	 ; print error message and HALT if underflow
	ADD R0,R0,R4
	JSR PUSH
	LD R7, SUB_SAVER7	; restore R7
	RET


; Multiplication operation
; IN: 2 number inputs in R1+R0
; OUT: 1 output R0
; R5: indicates pop success
; R1: first number input
; R0: value in stack
; R3: second number input as counter
MULTIPLY
		ST R7,MULT_SAVER7 ; save R7 at this location
    JSR POP
    ADD R5, R5, #0    ; check if pop is successful
	  BRp INVALID_IN	  ; print error message and HALT if underflow
    ADD R1, R0, #0    ; put first value into R1
    JSR POP
    ADD R5, R5, #0    ; check if pop is successful
	  BRp INVALID_IN	  ; print error message and HALT if underflow
    ADD R3, R0, #0    ; put second value into a counter
    BRnzp MULT_LOOP
MULT_LOOP
    ADD R0, R0, R3
    ADD R1, R1, #-1  ; decrement the counter
    BRp MULT_LOOP
    NOT R3, R3
    ADD R3, R3, #1
    ADD R0, R0, R3
    JSR PUSH
    LD R7,MULT_SAVER7
		RET

; Division operation
DIVIDE
;   Description: divides two numbers (R0 = R3 / R4)
;   Inputs: R3 - numerator
;	    R4 - denominator
;   Outputs: R0 - quotient
	ST R7,DIV_SAVER7 ; save R7 at this location
	JSR POP
	ADD R5, R5, #0	;CHECKS IF THE POP IS SUCCESSFUL
	BRp INVALID_IN	; print error message and HALT if underflow
	ADD R4, R0, #0	;CHANGES THE INPUT INTO THE INTEGER FORM
	JSR POP
	ADD R5, R5, #0	;CHECKS IF THE POP IS SUCCESSFUL
	BRp INVALID_IN	; print error message and HALT if underflow

	ADD R3, R0, #0
	NOT R4, R4
	ADD R4, R4, #1  ;NEGATES R4
	AND R0, R0, #0	;CLEAR R0
DIVLOOP
	ADD R0, R0, #1; INCREMENT QUOTIENT
	ADD R3, R3, R4; R1 = R1 - R4
	BRzp DIVLOOP
	;undo one subtraction in case we took away too much
	ADD R0, R0, #-1
	NOT R4, R4
	ADD R4, R4, #1 ; RETURN R4 TO ORIGINAL STATE
	ADD R3, R3, R4; R1 = R1 + R4
	JSR PUSH
	LD R7,DIV_SAVER7
	RET

EXIT
	HALT

; PUSH subroutine
; IN: R0 (value)
; OUT: R5 (0 – success, 1 – fail)
; R3: STACK_END
; R6: STACK_TOP
;
PUSH
;
; save registers that will be modified in PUSH subroutine
;
        ST R3, PUSH_SaveR3 ; save R3
        ST R6, PUSH_SaveR6 ; save R6
        AND R5, R5, #0 ; clear R5, indicates success
        LD R3, STACK_END
        LD R6, STACK_TOP
;
; check for overflow (when stack is full)
; STACK_TOP < STACK_END (STACK_TOP = STACK_END - 1)
        ADD R3, R3, #-1
        NOT R3, R3
        ADD R3, R3, #1
        ADD R3, R3, R6
        BRz OVERFLOW ; stack is full
;
; store value in the stack
;
        STR R0, R6, #0 ; push onto the stack
        ADD R6, R6, #-1 ; move top of the stack
        ST R6, STACK_TOP ; store top of stack pointer
        BRnzp DONE_PUSH
;
; indicate the overflow condition on return
;
OVERFLOW ADD R5, R5, #1
;
; restore modified registers and return
;
DONE_PUSH
        LD R3, PUSH_SaveR3
        LD R6, PUSH_SaveR6
        RET

; POP Subroutine
; OUT: R0 (value)
; OUT: R5 (0 – success, 1 – fail)
; R3: STACK_START
; R6: STACK_TOP
;
POP
;
; save registers that will be modified in POP subroutine
;
        ST R3, POP_SaveR3 ; save R3
        ST R6, POP_SaveR6 ; save R6
        AND R5, R5, #0 ; clear R5, indicates success
        LD R3, STACK_START
        LD R6, STACK_TOP
;
; check for underflow (when stack is empty)
; STACK_TOP = STACK_START
        NOT R3, R3
        ADD R3, R3, #1
        ADD R3, R3, R6
        BRz UNDERFLOW ; stack is empty, nothing to pop
;
; "remove" value from the stack
;
        ADD R6, R6, #1 ; move top of the stack
        LDR R0, R6, #0 ; read value from the stack
        ST R6, STACK_TOP ; store top of stack pointer
        BRnzp DONE_POP
;
; indicate the underflow condition on return
;
UNDERFLOW ADD R5, R5, #1
;
; restore modified registers and return
;
DONE_POP
        LD R3, POP_SaveR3
        LD R6, POP_SaveR6
        RET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
PRINT_HEX
	ST R7,PRINT_SAVER7 ; store R7 at this location
	JSR POP
	ADD R3,R0,#0	;Put popped value into R3
	AND R1,R1,#0;	Initialize digit counter to 4
	ADD R1,R1,#4
	LD R5,FOURTY_EIGHT_NEG 	;Initialize R5 to 48

LOOP
	ADD R1,R1,#0
	BRnz MAIN

	AND R2, R2, #0	;Initialize bit counter to 4
	ADD R2,R2,#4
	AND R0, R0, #0	;Initialize R0 to 0, this is the current digit

CHECK_MSB
	ADD R2,R2,#0	;Check if the bit counter is at 0 yet
	BRnz OPTIMIZE
	ADD R0,R0,R0	;Left-shift the digit
	ADD R3,R3,#0
	BRzp SHIFT_R3
	ADD R0,R0,#1	;ADD 1 to R0 if there is a '1' at the MSB of R3

SHIFT_R3
	ADD R3,R3,R3	;Double R3 to left-shift
	ADD R2,R2,#-1	;Decrement bit counter
	BRnzp CHECK_MSB

OPTIMIZE
	AND R4,R4,#0	;Initialize R4 to check R0
	ADD R4,R0,#-9	;Check if digit is <= 9
	BRnz PRINT
	ADD R0,R0,#7 	;If >9 add 7 to get to A-F

PRINT
	ADD R0,R0,R5
	OUT
	ADD R1,R1,#-1	;Decrement digit counter
	BRnzp LOOP
	ADD R5,R0,#0	;store answer in R5

DONE_PRINT
	LD R7,PRINT_SAVER7
	RET
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

PUSH_SaveR3 .BLKW #1
PUSH_SaveR6 .BLKW #1

POP_SaveR3 .BLKW #1
POP_SaveR6 .BLKW #1

ADD_SAVER7   .BLKW #1
SUB_SAVER7   .BLKW #1
MULT_SAVER7  .BLKW #1
DIV_SAVER7   .BLKW #1
EVAL_SAVER7  .BLKW #1
PRINT_SAVER7 .BLKW #1

STACK_START .FILL x4000
STACK_END   .FILL x3FF0
STACK_TOP   .FILL x4000

FOURTY_EIGHT_NEG .FILL #48
FOURTY_EIGHT 	 .FILL #-48
EQUALS_SIGN 	 .FILL #-61
SPACE 			 .FILL #32
ERRORMSG .STRINGZ "Invalid Input"

PLUS_SIGN	     .FILL #43
MINUS_SIGN 		 .FILL #45
MULTIPLY_SIGN	 .FILL #42
DIVIDE_SIGN		 .FILL #47

.END

