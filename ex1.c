//========================================
// Load AccCom program to memory
// - return start address of program
//========================================
UINT loadProgram() {
	// reset whole memory
	memset(mem, 0, MEM_SIZE);

	/*
		Y = A*A + B*B

		0100: A = -10
		0102: B = 20
		0104: Y

		sub	r0, r0, r0		// r0 = 0
			0000 000 000 000 011		=> 0000 0000 0000 0011 => 0003
		addi r0, r0, 0x10	// r0 = 0x10
			1010 000 000 010000			=> 1010 0000 0001 0000 => A010
		mul	r0, r0, r0		// r0 = r0*r0 = 0x0100
			0000 000 000 000 100		=> 0000 0010 0000 0100 => 0004
		lw	r1, 0(r0)		// r1 = A (0x0100 + 0*2 = 0x0100)
			0100 000 001 000000 (100h)	=> 0100 0000 0100 0000 => 4040
		lw	r2,	1(r0)		// r2 = B (0x0100 + 1*2 = 0x0102)
			0100 000 010 000001 (102h)	=> 0100 0000 1000 0001 => 4081
		mul	r3, r1, r1		// r3 = r1*r1
			0000 001 001 011 100		=> 0000 0010 0101 1100 => 025C
		mul	r4, r2, r2		// r4 = r2*r2
			0000 010 010 100 100		=> 0000 0100 1010 0100 => 04A4
		add	r5, r3, r4		// r5 = r3 + r4
			0000 011 100 101 010		=> 0000 0111 0010 1010 => 072A
		sw	r5, 02(r0)		// Y = r5 (0x0100 + 2*2 = 0x0104)
			0101 000 101 000010 (104h)	=> 0101 0001 0100 0010 => 5142
		halt
			1111 0000 0000 0000								   => F000
	*/

	// DATA section ----------------------------------------

	data_end = writeWords(data_bgn =
			0x0100,		0xFFF6, //	0100:	A = -10
						0x0014, //	0102:	B = 20
						0x0000, //	0104:	Y
						END_OF_ARG);

	// CODE section ----------------------------------------

	code_end = writeWords(code_bgn =
			0x0200,		0x0003,
						0xA010,
						0x0004,
						0x4040,
						0x4081,
						0x025C,
						0x04A4,
						0x072A,
						0x5142,
						0xF000,
						END_OF_ARG);

	// -----------------------------------------------------

	// print memory for verify
	printMemory("DATA", data_bgn, data_end);
	printMemory("CODE", code_bgn, code_end);

	return 0x0200;	// return start address of program
}