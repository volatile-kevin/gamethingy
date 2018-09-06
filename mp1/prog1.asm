; Kevin Hu, Joe Lopez, Daniel Ao
; kwh2, joseal2, dao3
; The code we added is similar to lab 1, in that it prints the hex values
; at a register. It prints the entire histogram, including labels, spaces,
; and newlines. It also handles the loop control over the bins.
;
; The code given to you here implements the histogram calculation that
; we developed in class.  In programming lab, we will add code that
; prints a number in hexadecimal to the monitor.
;
; Your assignment for this program is to combine these two pieces of
; code to print the histogram to the monitor.
;
; If you finish your program,
;    ** commit a working version to your repository  **
;    ** (and make a note of the repository version)! **


	.ORIG	x3000		; starting address is x3000


;
; Count the occurrences of each letter (A to Z) in an ASCII string
; terminated by a NUL character.  Lower case and upper case should
; be counted together, and a count also kept of all non-alphabetic
; characters (not counting the terminal NUL).
;
; The string starts at x4000.
;
; The resulting histogram (which will NOT be initialized in advance)
; should be stored starting at x3F00, with the non-alphabetic count
; at x3F00, and the count for each letter in x3F01 (A) through x3F1A (Z).
;
; table of register use in this part of the code
;    R0 holds a pointer to the histogram (x3F00)
;    R1 holds a pointer to the current position in the string
;       and as the loop count during histogram initialization
;    R2 holds the current character being counted
;       and is also used to point to the histogram entry
;    R3 holds the additive inverse of ASCII '@' (xFFC0)
;    R4 holds the difference between ASCII '@' and 'Z' (xFFE6)
;    R5 holds the difference between ASCII '@' and '`' (xFFE0)
;    R6 is used as a temporary register
;

	LD R0,HIST_ADDR      	; point R0 to the start of the histogram

	; fill the histogram with zeroes
	AND R6,R6,#0		; put a zero into R6
	LD R1,NUM_BINS		; initialize loop count to 27
	ADD R2,R0,#0		; copy start of histogram into R2

	; loop to fill histogram starts here
HFLOOP	STR R6,R2,#0		; write a zero into histogram
	ADD R2,R2,#1		; point to next histogram entry
	ADD R1,R1,#-1		; decrement loop count
	BRp HFLOOP		; continue until loop count reaches zero

	; initialize R1, R3, R4, and R5 from memory
	LD R3,NEG_AT		; set R3 to additive inverse of ASCII '@'
	LD R4,AT_MIN_Z		; set R4 to difference between ASCII '@' and 'Z'
	LD R5,AT_MIN_BQ		; set R5 to difference between ASCII '@' and '`'
	LD R1,STR_START		; point R1 to start of string

	; the counting loop starts here
COUNTLOOP
	LDR R2,R1,#0		; read the next character from the string
	BRz PRINT_HIST		; found the end of the string

	ADD R2,R2,R3		; subtract '@' from the character
	BRp AT_LEAST_A		; branch if > '@', i.e., >= 'A'
NON_ALPHA
	LDR R6,R0,#0		; load the non-alpha count
	ADD R6,R6,#1		; add one to it
	STR R6,R0,#0		; store the new non-alpha count
	BRnzp GET_NEXT		; branch to end of conditional structure
AT_LEAST_A
	ADD R6,R2,R4		; compare with 'Z'
	BRp MORE_THAN_Z         ; branch if > 'Z'

; note that we no longer need the current character
; so we can reuse R2 for the pointer to the correct
; histogram entry for incrementing
ALPHA
	ADD R2,R2,R0		; point to correct histogram entry
	LDR R6,R2,#0		; load the count
	ADD R6,R6,#1		; add one to it
	STR R6,R2,#0		; store the new count
	BRnzp GET_NEXT		; branch to end of conditional structure

; subtracting as below yields the original character minus '`'
MORE_THAN_Z
	ADD R2,R2,R5		; subtract '`' - '@' from the character
	BRnz NON_ALPHA		; if <= '`', i.e., < 'a', go increment non-alpha
	ADD R6,R2,R4		; compare with 'z'
	BRnz ALPHA		; if <= 'z', go increment alpha count
	BRnzp NON_ALPHA		; otherwise, go increment non-alpha

GET_NEXT
	ADD R1,R1,#1		; point to next character in string
	BRnzp COUNTLOOP		; go to start of counting loop


; Our added code starts here:
; R0 is the current digit, used to print characters
; R1 is our digit counter
; R2 is our bit counter
; R3 is where the hex number is stored to be displayed
; R4 is a temporary register to check if each hex value is between 1 and 9
; R5 is a counter for alphabetic characters
; R6 is a temporary register
PRINT_HIST
	LD R5,A_TO_Z_START	;Set R5 as a counter for alphas

LOAD_R3
	AND R1,R1,#0	;Initialize digit counter to 4
	ADD R1,R1,#4
	AND R6,R6,#0	;Initialize R6 to zero

	LD R6, NEG_SIXTY_FIVE 	;Load -65 to be added to content in R5
	ADD R3,R5,R6	;Subtract 65 from current a-z count
	LD R6,HIST_ADDR	;Load content at first histogram location into R3
	ADD R3,R3,R6	;R3 is now address of histogram
	ADD R3,R3,#1	; Adds one to R3 to offset
	LDR R3,R3,#0	;R3 is now the content at the current histogram location

PRINT_LOCATION
	AND R0,R0,#0	;load R5 into R0
	ADD R0,R5,#0
	OUT
	LD R0,EQUALS	;Load equals sign hex value into R0
	OUT

LOOP
	ADD R1,R1,#0
	BRnz FINISH_ROW

	AND R2, R2, #0	;Initialize bit counter to 4
	ADD R2,R2,#4
	AND R0, R0, #0	;Initialize R0 to 0, this is the current digit

CHECK_MSB
	ADD R2,R2,#0	;Check if the bit counter is at 0 yet
	BRnz CHECK_ALPHA
	ADD R0,R0,R0	;Left-shift the digit
	ADD R3,R3,#0
	BRzp SHIFT_R3
	ADD R0,R0,#1	;ADD 1 to R0 if there is a '1' at the MSB of R3

SHIFT_R3
	ADD R3,R3,R3	;Double R3 to left-shift
	ADD R2,R2,#-1	;Decrement bit counter
	BRnzp CHECK_MSB

CHECK_ALPHA
	LD R6,FORTY_EIGHT ;Initialize R6 to 48
	AND R4,R4,#0	;Initialize R4 to check R0
	ADD R4,R0,#-9	;Check if digit is <= 9
	BRnz PRINT
	ADD R0,R0,#7	;If >9, add 7 to get to A-F

PRINT
	ADD R0,R0,R6
	OUT
	ADD R1,R1,#-1	;Decrement digit counter
	BRnzp LOOP

FINISH_ROW
	ADD R5,R5,#1	;Increment R5 (a-z counter)
	LD R0,NEG_Z		;Check if R5 has passed 'Z' in the count
	AND R6,R6,#0
	ADD R6,R5,R0
	BRp DONE
	LD R0,NEW_LINE	;Print a new line
	OUT
	BRnzp LOAD_R3



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


DONE
	HALT	; done


; the data needed by the program
NEW_LINE		.FILL #10	;Newline char
EQUALS 			.FILL #32	;space sign hex value
A_TO_Z_START 	.FILL #64	;Starting location of a-z counter ('@' char for non-alphas)
NEG_SIXTY_FIVE 	.FILL #-65 	;Number to subtract from a-z counter to get current hist location
Z				.FILL #90	;Z in hex, used to find end of a-z counter
NUM_BINS		.FILL #27	;27 loop iterations
NEG_Z			.FILL #-90	;Difference between 'Z' and x0000
FORTY_EIGHT		.FILL #48	;48
NEG_AT		.FILL xFFC0	; the additive inverse of ASCII '@'
AT_MIN_Z	.FILL xFFE6	; the difference between ASCII '@' and 'Z'
AT_MIN_BQ	.FILL xFFE0	; the difference between ASCII '@' and '`'
HIST_ADDR	.FILL x3F00 ; histogram starting address
STR_START	.FILL x4000	; string starting address


; for testing, you can use the lines below to include the string in this
; program...
;STR_START	.FILL STRING	; string starting address
;STRING		.STRINGZ "the quick brown fox jumps over the lazy dog"



	; the directive below tells the assembler that the program is done
	; (so do not write any code below it!)

.END
