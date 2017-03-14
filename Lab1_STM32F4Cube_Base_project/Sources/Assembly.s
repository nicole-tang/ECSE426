	AREA text, CODE, READONLY
	EXPORT FIR_asm

; function FIR_asm
; R0: pointer to input
; R1: pointer to output
; R2: coefficient
; R3: length
; R4: order

FIR_asm
	VLDR.f32	S0, [R2, #0]	; load coefficient 1 into S0
	VLDR.f32	S1, [R2, #4]	; load coefficient 2 into S1
	VLDR.f32	S2, [R2, #8]	; load coefficient 3 into S2
	VLDR.f32	S3, [R2, #12]	; load coefficient 4 into S3
	VLDR.f32	S4, [R2, #16]	; load coefficient 5 into S4
	SUB		R7, R3, #4

loop
	CMP		R7, #0		; loop through input array
	BEQ		exit		; go to exit input - 4 = 0
	
	VLDR.f32	S5, [R0, #0]	; load first element of input array into S5
	VMUL.f32	S6, S5, S0	; multiply first element by first coefficient
	VADD.f32	S7, S7, S6	; add S6 to S7

	VLDR.f32	S5, [R0, #4]	; load second element of input array into S5
	VMUL.f32	S6, S5, S1	; multiply second element by corresponding coefficient
	VADD.f32	S7, S7, S6	; add S6 to S7

	VLDR.f32	S5, [R0, #8]	; load third element of input array into S5
	VMUL.f32	S6, S5, S2	; multiply third element by corresponding coefficient
	VADD.f32	S7, S7, S6	; add S6 to S7

	VLDR.f32	S5, [R0, #12]	; load fourth element of input array into S5
	VMUL.f32	S6, S5, S3	; multiply fourth element by corresponding coefficient
	VADD.f32	S7, S7, S6	; add S6 to S7

	VLDR.f32	S5, [R0, #16]	; load fifth element of input array into S5
	VMUL.f32	S6, S5, S4	; multiply fifth element by corresponding coefficient
	VADD.f32	S7, S7, S6	; add S6 to S7
;	VSTR.f32  S7, R1
	ADD		R0, R0, #4	; increment input array memory address 
	ADD		R1, R1, #4	; increment output array memory address
	SUB		R7, R7, #1	; decrease counter

	B		loop		; return to the beginning of the loop
	
exit
	
	BX		LR
	END