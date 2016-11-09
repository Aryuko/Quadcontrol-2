  # labwork.S
  # Written 2015 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1 2015-2016.
  # The macros PUSH and POP are in the public domain.

.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm

.data
.align 2
.text

# Converts a given value into an Ascii character, from 0-9 and A-F.
# arguments:
# 	$a0: The 4 least significant bits specify the number to convert.
#returns:
#	$v0: Ascii character
.global hexasc
hexasc:
	#mask $a0 to remove everything except 4 least significant bits
	andi $a0, $a0, 0xF

	#branch to A-F converter if $a0 >= 10
	sge $t0, $a0, 10
	bne $t0, $zero, convert_AF
	nop

	#convert to ascii '0'-'9'
	addi $v0, $a0, 0x30	#Ascii '0' starts at 0x30
	jr $ra
	nop

	#convert to ascii 'A'-'F'
	convert_AF:
	addi $v0, $a0, 0x37  #Ascii '0' starts at 0x41, input starts at 0xA, 0x41-0xA = 0x37
	jr $ra
	nop

#Delays the program for a specified amount of milliseconds.
#arguments:
#	$a0: The amount of milliseconds to delay for
#returns:
#	nothing
.global delay
delay:
	li $t0, 11397		#constant for delay_loop
	# 39889 /10*35
	delay_counter_loop:
		sgt $t2, $a0, $zero
		beq $t2, $zero, delay_counter_loop_end
		nop

		addi $a0, $a0, -1	#decrement ms
		add $t1, $zero, $zero	#i = 0;
		delay_loop:
			slt $t2, $t1, $t0	#$t2 = i < constant
			beq $t2, $zero, delay_counter_loop
			nop

			addi $t1, $t1, 1	#increment i
			j delay_loop
		delay_loop_end:
	delay_counter_loop_end:
	jr  $ra
	nop

#time2string
#arguments:
#	$a0: The address to a suitably large area in memory.
#	$a1: The 16 least significant bits of $a1 contains time-info organized as four NBCD-coded digits of 4 bits each. All other bits are ignored.
#returns:
#	nothing
.global time2string
time2string:
	#store registers that will change in stack
	PUSH $s0
	PUSH $s1
	PUSH $ra

	add $s0, $a0, $zero		#save $a0 to $s1
	andi $s1, $a1, 0xFFFF	#mask $a1 to remove everything except 16 least significant bits, save to $s1

	#4 most significant bits converted and stored
	srl $a0, $s1, 12		#shift 12 bits to the right to move the 4 wanted bits to the least significant positions
	jal hexasc
	nop
	sb $v0, 0($s0)

	#4 more significant bits converted and stored
	srl $a0, $s1, 8			#shift 8 bits to the right to move the 4 wanted bits to the least significant positions
	jal hexasc
	nop
	sb $v0, 1($s0)

	#store colon character
	addi $t0, $zero, 0x3A		#Ascii code for ':' is 0x3A
	sb $t0, 2($s0)

	#4 less significant bits converted and stored
	srl $a0, $s1, 4			#shift 4 bits to the right to move the 4 wanted bits to the least significant positions
	jal hexasc
	nop
	sb $v0, 3($s0)

	#4 least significant bits converted and stored
	srl $a0, $s1, 0			#shift 0 bits to the right to move the 4 wanted bits to the least significant positions

	#andi $t0, $a0, 0xF
	#beq	$t0, 9, time2string_write_nine	#branch for "NINE" case
	#nop

	jal hexasc
	nop
	sb $v0, 4($s0)

	#store null character in default case
	addi $t0, $zero, 0x00		#Ascii code for null character is 0x00
	sb $t0, 5($s0)

	j time2string_end
	nop

	time2string_write_nine:
	#store letters for "NINE"
	addi $t0, $zero, 0x4E
	sb $t0, 4($s0)

	addi $t0, $zero, 0x49
	sb $t0, 5($s0)

	addi $t0, $zero, 0x4E
	sb $t0, 6($s0)

	addi $t0, $zero, 0x45
	sb $t0, 7($s0)

	#store null character
	addi $t0, $zero, 0x00		#Ascii code for null character is 0x00
	sb $t0, 8($s0)

	time2string_end:
	#retrieve saved registers from the stack
	POP $ra
	POP $s1
	POP $s0

	#finish subroutine
	jr $ra
	nop
