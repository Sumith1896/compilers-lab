
	.text 			# The .text assembler directive indicates
	.globl main		# The following is the code (as oppose to data)
main:				# .globl makes main know to the 
				# outside of the program.
# Prologue begins 
	sw $ra, -4($sp)		# Save the return address
	sw $fp, -8($sp)		# Save the frame pointer
	sub $fp, $sp, 8		# Update the frame pointer

	sub $sp, $sp, 16		# Make space for the locals
	sw $v0, -12($sp)
	sw $a0, -20($sp)
	sw $a1, -24($sp)
	sw $a2, -28($sp)
	sw $a3, -32($sp)
	sw $t0, -36($sp)
	sw $t1, -40($sp)
	sw $t2, -44($sp)
	sw $t3, -48($sp)
	sw $t4, -52($sp)
	sw $t5, -56($sp)
	sw $t6, -60($sp)
	sw $t7, -64($sp)
	sw $t8, -68($sp)
	sw $t9, -72($sp)
	sw $s0, -76($sp)
	sw $s1, -80($sp)
	sw $s2, -84($sp)
	sw $s3, -88($sp)
	sw $s4, -92($sp)
	sw $s5, -96($sp)
	sw $s6, -100($sp)
	sw $s7, -104($sp)
	s.d $f2, -120($sp)
	s.d $f4, -128($sp)
	s.d $f6, -136($sp)
	s.d $f8, -144($sp)
	s.d $f10, -152($sp)
	s.d $f12, -160($sp)
	s.d $f14, -168($sp)
	s.d $f16, -176($sp)
	s.d $f18, -184($sp)
	s.d $f20, -192($sp)
	s.d $f22, -200($sp)
	s.d $f24, -208($sp)
	s.d $f26, -216($sp)
	s.d $f28, -224($sp)
	s.d $f30, -232($sp)
	sub $sp, $sp, 232		# Update the stack pointer
# Prologue ends

	lw $v0, -8($fp)
	sw $v0, -4($sp)
	sub $sp, $sp, 4
	jal mc91_0
	add $sp, $sp, 4
	move $v0, $v1
	sw $v0, -4($fp)
	lw $v0, -4($fp)
	addi $sp, $sp, -4
	sw $v0, 0($sp)
	addi $sp, $sp, -4
	sw $a0, 0($sp)
	addi $sp, $sp, -8
	s.d $f12, 0($sp)
	move $a0, $v0
	li $v0, 1
	syscall 
	l.d $f12, 0($sp)
	addi $sp, $sp, 8
	lw $a0, 0($sp)
	addi $sp, $sp, 4
	lw $v0, 0($sp)
	addi $sp, $sp, 4
	j epilogue_main

# Epilogue Begins
epilogue_main:
	add $sp, $sp, 232		# Update the stack pointer
	l.d $f30, -232($sp)
	l.d $f28, -224($sp)
	l.d $f26, -216($sp)
	l.d $f24, -208($sp)
	l.d $f22, -200($sp)
	l.d $f20, -192($sp)
	l.d $f18, -184($sp)
	l.d $f16, -176($sp)
	l.d $f14, -168($sp)
	l.d $f12, -160($sp)
	l.d $f10, -152($sp)
	l.d $f8, -144($sp)
	l.d $f6, -136($sp)
	l.d $f4, -128($sp)
	l.d $f2, -120($sp)
	lw $s7, -104($sp)
	lw $s6, -100($sp)
	lw $s5, -96($sp)
	lw $s4, -92($sp)
	lw $s3, -88($sp)
	lw $s2, -84($sp)
	lw $s1, -80($sp)
	lw $s0, -76($sp)
	lw $t9, -72($sp)
	lw $t8, -68($sp)
	lw $t7, -64($sp)
	lw $t6, -60($sp)
	lw $t5, -56($sp)
	lw $t4, -52($sp)
	lw $t3, -48($sp)
	lw $t2, -44($sp)
	lw $t1, -40($sp)
	lw $t0, -36($sp)
	lw $a3, -32($sp)
	lw $a2, -28($sp)
	lw $a1, -24($sp)
	lw $a0, -20($sp)
	lw $v0, -12($sp)
	add $sp, $sp, 16
	lw $fp, -8($sp)  
	lw $ra, -4($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends


	.text 			# The .text assembler directive indicates
	.globl mc91_0		# The following is the code (as oppose to data)
mc91_0:				# .globl makes main know to the 
				# outside of the program.
# Prologue begins 
	sw $ra, -4($sp)		# Save the return address
	sw $fp, -8($sp)		# Save the frame pointer
	sub $fp, $sp, 8		# Update the frame pointer

	sub $sp, $sp, 8		# Make space for the locals
	sw $v0, -12($sp)
	sw $a0, -20($sp)
	sw $a1, -24($sp)
	sw $a2, -28($sp)
	sw $a3, -32($sp)
	sw $t0, -36($sp)
	sw $t1, -40($sp)
	sw $t2, -44($sp)
	sw $t3, -48($sp)
	sw $t4, -52($sp)
	sw $t5, -56($sp)
	sw $t6, -60($sp)
	sw $t7, -64($sp)
	sw $t8, -68($sp)
	sw $t9, -72($sp)
	sw $s0, -76($sp)
	sw $s1, -80($sp)
	sw $s2, -84($sp)
	sw $s3, -88($sp)
	sw $s4, -92($sp)
	sw $s5, -96($sp)
	sw $s6, -100($sp)
	sw $s7, -104($sp)
	s.d $f2, -120($sp)
	s.d $f4, -128($sp)
	s.d $f6, -136($sp)
	s.d $f8, -144($sp)
	s.d $f10, -152($sp)
	s.d $f12, -160($sp)
	s.d $f14, -168($sp)
	s.d $f16, -176($sp)
	s.d $f18, -184($sp)
	s.d $f20, -192($sp)
	s.d $f22, -200($sp)
	s.d $f24, -208($sp)
	s.d $f26, -216($sp)
	s.d $f28, -224($sp)
	s.d $f30, -232($sp)
	sub $sp, $sp, 232		# Update the stack pointer
# Prologue ends

	lw $v0, 8($fp)
	li $t0, 100
	sgt $t1, $v0, $t0
	beq $t1, $zero, label0 
	lw $v0, 8($fp)
	li $t0, 10
	sub $t2, $v0, $t0
	move $v1, $t2
	j epilogue_mc91_0
	j label1

label0:    	

label1:    	
	lw $v0, 8($fp)
	li $t0, 11
	add $t1, $v0, $t0
	sw $t1, -4($sp)
	sub $sp, $sp, 4
	jal mc91_0
	add $sp, $sp, 4
	move $v0, $v1
	sw $v0, -4($sp)
	sub $sp, $sp, 4
	jal mc91_0
	add $sp, $sp, 4
	move $v0, $v1
	move $v1, $v0
	j epilogue_mc91_0

# Epilogue Begins
epilogue_mc91_0:
	add $sp, $sp, 232		# Update the stack pointer
	l.d $f30, -232($sp)
	l.d $f28, -224($sp)
	l.d $f26, -216($sp)
	l.d $f24, -208($sp)
	l.d $f22, -200($sp)
	l.d $f20, -192($sp)
	l.d $f18, -184($sp)
	l.d $f16, -176($sp)
	l.d $f14, -168($sp)
	l.d $f12, -160($sp)
	l.d $f10, -152($sp)
	l.d $f8, -144($sp)
	l.d $f6, -136($sp)
	l.d $f4, -128($sp)
	l.d $f2, -120($sp)
	lw $s7, -104($sp)
	lw $s6, -100($sp)
	lw $s5, -96($sp)
	lw $s4, -92($sp)
	lw $s3, -88($sp)
	lw $s2, -84($sp)
	lw $s1, -80($sp)
	lw $s0, -76($sp)
	lw $t9, -72($sp)
	lw $t8, -68($sp)
	lw $t7, -64($sp)
	lw $t6, -60($sp)
	lw $t5, -56($sp)
	lw $t4, -52($sp)
	lw $t3, -48($sp)
	lw $t2, -44($sp)
	lw $t1, -40($sp)
	lw $t0, -36($sp)
	lw $a3, -32($sp)
	lw $a2, -28($sp)
	lw $a1, -24($sp)
	lw $a0, -20($sp)
	lw $v0, -12($sp)
	add $sp, $sp, 8
	lw $fp, -8($sp)  
	lw $ra, -4($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends

