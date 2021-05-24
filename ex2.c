//========================================
// Load AccCom program to memory
// - return start address of program
//========================================
UINT loadProgram() {
	// reset whole memory
	memset(mem, 0, MEM_SIZE);

	/*
		S = A + (A+1) + (A+2) + ... + (B-1) + B

		0100: A = -2
		0102: B = 3
		0104: S

		r1 = A	// counter
		r2 = B + 1
		r3 = 0	// sum
		#loop_begin
		if (r1 == r2) goto loop_end	// !(A <= B) => (A > B) => (A == B + 1)
		r3 = r3 + r1
		r1 = r1 + 1
		goto loop_begin
		#loop_end
		S = r3

		sub	r0, r0, r0		// r0 = 0
			0000 000 000 000 011		=> 0000 0000 0000 0011 => 0003
		addi r0, r0, 0x10	// r0 = 0x10
			1010 000 000 010000			=> 1010 0000 0001 0000 => A010
		mul	r0, r0, r0		// r0 = r0*r0 = 0x0100
			0000 000 000 000 100		=> 0000 0010 0000 0100 => 0004
		lw	r1, 0(r0)		// r1 = A
			0100 000 001 000000 (100h)	=> 0100 0000 0100 0000 => 4040
		lw	r2,	1(r0)		// r2 = B
			0100 000 010 000001 (102h)	=> 0100 0000 1000 0001 => 4081
		addi r2, r2, 1
			1010 010 010 000001			=> 1010 0100 1000 0001 => A481
		sub r3, r3, r3		// r3 = 0
			0000 011 011 011 011		=> 0000 0110 1101 1011 => 06DB
		#loop_begin
		beq r1, r2, loop_end(+3)
			0001 001 010 000011			=> 0001 0010 1000 0011 => 1283
		add r3, r3, r1		// r3 = r3 + r1
			0000 011 001 011 010		=> 0000 0110 0101 1010 => 065A
		addi r1, r1, 1		// r1 = r1 + 1
			1010 001 001 000001			=> 1010 0010 0100 0001 => A241
		j loop_begin(-4)
			0011 111111111100			=> 0011 1111 1111 1100 => 3FFC
		#loop_end
		sw r3, 2(r0)		// S = r3
			0101 000 011 000002			=> 0101 0000 1100 0001 => 50C2
		halt
			1111 0000 0000 0000								   => F000
	*/

	// DATA section ----------------------------------------

	data_end = writeWords(data_bgn =
			0x0100,		0xFFFE, //	0100:	A = -2
						0x0003, //	0102:	B = 3
						0x0000, //	0104:	S
						END_OF_ARG);

	// CODE section ----------------------------------------

	code_end = writeWords(code_bgn =
			0x0200,		0x0003,
						0xA010,
						0x0004,
						0x4040,
						0x4081,
						0xA481,
						0x06DB,
						0x1283,
						0x065A,
						0xA241,
						0x3FFC,
						0x50C2,
						0xF000,
						END_OF_ARG);

	// -----------------------------------------------------

	// print memory for verify
	printMemory("DATA", data_bgn, data_end);
	printMemory("CODE", code_bgn, code_end);

	return 0x0200;	// return start address of program
}