
#ifndef ARMIN_H
#define ARMIN_H

#ifndef $CPU_TYPE
#define $CPU_TYPE 0 /* Generic */
#endif


/* ARM Registers */

#define $R0  0x00
#define $R1  0x01
#define $R2  0x02
#define $R3  0x03
#define $R4  0x04
#define $R5  0x05
#define $R6  0x06
#define $R7  0x07
#define $R8  0x08
#define $R9  0x09
#define $R10 0x0A
#define $R11 0x0B
#define $R12 0x0C
#define $R13 0x0D
#define $R14 0x0E
#define $R15 0x0F
#define $SP  0x0D
#define $LR  0x0E
#define $PC  0x0F

/* Mask Bits of Registers */

#define $BR0  0x0001
#define $BR1  0x0002
#define $BR2  0x0004
#define $BR3  0x0008
#define $BR4  0x0010
#define $BR5  0x0020
#define $BR6  0x0040
#define $BR7  0x0080
#define $BR8  0x0100
#define $BR9  0x0200
#define $BR10 0x0400
#define $BR11 0x0800
#define $BR12 0x1000
#define $BR13 0x2000
#define $BR14 0x4000
#define $BR15 0x8000
#define $BSP  0x2000
#define $BLR  0x4000
#define $BPC  0x8000

/* ARM Conditions */
#define $EQ (0x00000000^0xE0000000) /* Z==1       (Equal)              */
#define $NE (0x10000000^0xE0000000) /* Z==0       (Not equal)          */
#define $CS (0x20000000^0xE0000000) /* C==1       (Uns higher or same) */
#define $CC (0x30000000^0xE0000000) /* C==0       (Uns lower)          */
#define $MI (0x40000000^0xE0000000) /* N==1       (Negative)           */
#define $PL (0x50000000^0xE0000000) /* N==0       (Positive or zero)   */
#define $VS (0x60000000^0xE0000000) /* V==1       (Overflow)           */
#define $VC (0x70000000^0xE0000000) /* V==0       (No overflow)        */
#define $HI (0x80000000^0xE0000000) /* C==1&&Z==0 (Uns higher)         */
#define $LS (0x90000000^0xE0000000) /* C==0||Z==1 (Uns lower or same)  */
#define $GE (0xA0000000^0xE0000000) /* N==V       (Greater or equal)   */
#define $LT (0xB0000000^0xE0000000) /* N!=V       (Less than)          */
#define $GT (0xC0000000^0xE0000000) /* Z==0&&N==V (Greater than)       */
#define $LE (0xD0000000^0xE0000000) /* Z==0||N!=V (Less than or equal) */
#define $AL (0xE0000000^0xE0000000) /* Ignored    (Always)             */
#define $NV (0xF0000000^0xE0000000) /* Ignored    (Never)              */

/* Shift Types
LSL=0
LSR=1
ASR=2
ROR=3
*/

/* Operand2 Constructor */

#define $IMM(IMM,ROT) (0x02000000|(((ROT)&15)<<8)|((IMM)&255))
#define $LSL(SHIFT)   ((((SHIFT)&31)<<7)|(0x00))
#define $LSR(SHIFT)   ((((SHIFT)&31)<<7)|(0x20))
#define $ASR(SHIFT)   ((((SHIFT)&31)<<7)|(0x40))
#define $ROR(SHIFT)   ((((SHIFT)&31)<<7)|(0x60))
#define $LSLR(SREG)   ((((SREG)&15)<<8)|(0x10))
#define $LSRR(SREG)   ((((SREG)&15)<<8)|(0x30))
#define $ASRR(SREG)   ((((SREG)&15)<<8)|(0x50))
#define $RORR(SREG)   ((((SREG)&15)<<8)|(0x70))

#define $IMM12(IMM)   ((0x02000000)|((IMM)&0x0FFF))

/* ------------------------ */
/* | ARM CPU Instructions | */
/* ------------------------ */

/*
   Branch

-----------------------------------------------------------------
|31  28|27 25| 24 |23                                          0| 
-----------------------------------------------------------------
| Cond | 101 | L  |                   Offset                    |
-----------------------------------------------------------------
L: 0 - Branch, 1 - Branch with Link (Save R15 to R14)
*/

#define $B(OFT)  (0xEA000000|((OFT)&0x00FFFFFF))
#define $BL(OFT) (0xEB000000|((OFT)&0x00FFFFFF))

/*
   Data Processing

-----------------------------------------------------------------
|31  28|27  26| 25 |24    21| 20 |19  16|15  12|11             0|
-----------------------------------------------------------------
| Cond |  00  | I  | OpCode | S  |  Rn  |  Rd  |    Operand2    |
-----------------------------------------------------------------
I: 1 - Op2 is immediate, 0 - Op2 is register;
S: 1 - Set cond in CPRS, 0 - Do not;
OpCodes:
0000 = AND - Rd = Rn & Op2
0001 = EOR - Rd = Rn ^ Op2
0010 = SUB - Rd = Rn - Op2
0011 = RSB - Rd = Op2 - Rn
0100 = ADD - Rd = Rn + Op2
0101 = ADC - Rd = Rn + Op2 + C
0110 = SBC - Rd = Rn - Op2 + C
0111 = RSC - Rd = Op2 - Rn + C
1000 = TST - Set CPSR on Rn & Op2
1001 = TEQ - Set CPSR on Rn ^ Op2
1010 = CMP - Set CPSR on Rn - Op2
1011 = CMN - Set CPSR on Rn + Op2
1100 = ORR - Rd = Rn | Op2
1101 = MOV - Rd = Op2
1110 = BIC - Rd = Rn & (~Op2)
1111 = MVN - Rd = ~Op2
*/

#define $AND(Rd,Rn,Op2) ((0xE0000000)|((Rn&15)<<16)| \
                           ((Rd&15)<<12)|((Op2)&0x02000FFF))

#define $EOR(Rd,Rn,Op2) ((0xE0200000)|((Rn&15)<<16)| \
                           ((Rd&15)<<12)|((Op2)&0x02000FFF))

#define $SUB(Rd,Rn,Op2) ((0xE0400000)|((Rn&15)<<16)| \
                           ((Rd&15)<<12)|((Op2)&0x02000FFF))

#define $RSB(Rd,Rn,Op2) ((0xE0600000)|((Rn&15)<<16)| \
                           ((Rd&15)<<12)|((Op2)&0x02000FFF))

#define $ADD(Rd,Rn,Op2) ((0xE0800000)|((Rn&15)<<16)| \
                           ((Rd&15)<<12)|((Op2)&0x02000FFF))

#define $ADC(Rd,Rn,Op2) ((0xE0A00000)|((Rn&15)<<16)| \
                           ((Rd&15)<<12)|((Op2)&0x02000FFF))

#define $SBC(Rd,Rn,Op2) ((0xE0C00000)|((Rn&15)<<16)| \
                           ((Rd&15)<<12)|((Op2)&0x02000FFF))

#define $RSC(Rd,Rn,Op2) ((0xE0E00000)|((Rn&15)<<16)| \
                           ((Rd&15)<<12)|((Op2)&0x02000FFF))

#define $TST(Rd,Rn,Op2) ((0xE1100000)|((Rn&15)<<16)| \
                           ((Rd&15)<<12)|((Op2)&0x02000FFF))

#define $TEQ(Rd,Rn,Op2) ((0xE1300000)|((Rn&15)<<16)| \
                           ((Rd&15)<<12)|((Op2)&0x02000FFF))

#define $CMP(Rd,Rn,Op2) ((0xE1500000)|((Rn&15)<<16)| \
                           ((Rd&15)<<12)|((Op2)&0x02000FFF))

#define $CMN(Rd,Rn,Op2) ((0xE1700000)|((Rn&15)<<16)| \
                           ((Rd&15)<<12)|((Op2)&0x02000FFF))

#define $ORR(Rd,Rn,Op2) ((0xE1800000)|((Rn&15)<<16)| \
                           ((Rd&15)<<12)|((Op2)&0x02000FFF))

#define $MOV(Rd,Op2)    ((0xE1A00000)|((0)<<16)| \
                           ((Rd&15)<<12)|((Op2)&0x02000FFF))

#define $BIC(Rd,Rn,Op2) ((0xE1C00000)|((Rn&15)<<16)| \
                           ((Rd&15)<<12)|((Op2)&0x02000FFF))

#define $MVN(Rd,Op2)    ((0xE1E00000)|((0)<<16)| \
                           ((Rd&15)<<12)|((Op2)&0x02000FFF))

#define $ANDS(Rd,Rn,Op2) ((0xE0100000)|((Rn&15)<<16)| \
                           ((Rd&15)<<12)|((Op2)&0x02000FFF))

#define $EORS(Rd,Rn,Op2) ((0xE0300000)|((Rn&15)<<16)| \
                           ((Rd&15)<<12)|((Op2)&0x02000FFF))

#define $SUBS(Rd,Rn,Op2) ((0xE0500000)|((Rn&15)<<16)| \
                           ((Rd&15)<<12)|((Op2)&0x02000FFF))

#define $RSBS(Rd,Rn,Op2) ((0xE0700000)|((Rn&15)<<16)| \
                           ((Rd&15)<<12)|((Op2)&0x02000FFF))

#define $ADDS(Rd,Rn,Op2) ((0xE0900000)|((Rn&15)<<16)| \
                           ((Rd&15)<<12)|((Op2)&0x02000FFF))

#define $ADCS(Rd,Rn,Op2) ((0xE0B00000)|((Rn&15)<<16)| \
                           ((Rd&15)<<12)|((Op2)&0x02000FFF))

#define $SBCS(Rd,Rn,Op2) ((0xE0D00000)|((Rn&15)<<16)| \
                           ((Rd&15)<<12)|((Op2)&0x02000FFF))

#define $RSCS(Rd,Rn,Op2) ((0xE0F00000)|((Rn&15)<<16)| \
                           ((Rd&15)<<12)|((Op2)&0x02000FFF))

#define $TSTS(Rd,Rn,Op2) ((0xE1100000)|((Rn&15)<<16)| \
                           ((Rd&15)<<12)|((Op2)&0x02000FFF))

#define $TEQS(Rd,Rn,Op2) ((0xE1300000)|((Rn&15)<<16)| \
                           ((Rd&15)<<12)|((Op2)&0x02000FFF))

#define $CMPS(Rd,Rn,Op2) ((0xE1500000)|((Rn&15)<<16)| \
                           ((Rd&15)<<12)|((Op2)&0x02000FFF))

#define $CMNS(Rd,Rn,Op2) ((0xE1700000)|((Rn&15)<<16)| \
                           ((Rd&15)<<12)|((Op2)&0x02000FFF))

#define $ORRS(Rd,Rn,Op2) ((0xE1900000)|((Rn&15)<<16)| \
                           ((Rd&15)<<12)|((Op2)&0x02000FFF))

#define $MOVS(Rd,Op2)    ((0xE1B00000)|((0)<<16)| \
                           ((Rd&15)<<12)|((Op2)&0x02000FFF))

#define $BICS(Rd,Rn,Op2) ((0xE1D00000)|((Rn&15)<<16)| \
                           ((Rd&15)<<12)|((Op2)&0x02000FFF))

#define $MVNS(Rd,Op2)    ((0xE1F00000)|((0)<<16)| \
                           ((Rd&15)<<12)|((Op2)&0x02000FFF))

/*
  Program State Register (PSR) Transfer 

MRS (transfer PSR contents to a register)
-----------------------------------------------------------------
|31  28|27            23| 22  |21    16|15  12|11              0|
-----------------------------------------------------------------
| Cond | 00010          | PSR | 001111 |  Rd  |  000000000000   |
-----------------------------------------------------------------

MSR (transfer register contents to PSR)
-----------------------------------------------------------------
|31  28|27            23| 22  |21        12|11          4|3    0|
-----------------------------------------------------------------
| Cond | 00010          | PSR | 1010011111 |  00000000   |  Rm  |
-----------------------------------------------------------------

MSR (transfer register contents or immdiate to PSR flags only)
-----------------------------------------------------------------
|31  28|27 26| 25 |24 23| 22  |21        12|11                 0|
-----------------------------------------------------------------
| Cond | 00  | I  | 10  | PSR | 1010001111 |   Source Operand   |
-----------------------------------------------------------------
PSR: 0 - CPSR, 1 - SPSR;
I:   0 - Source Op is register, 1 - Source Op is immediate;

*/

#define $SPSR      0x00410000
#define $CPSR      0x00010000
#define $SPSR_flg  0x00400000
#define $CPSR_flg  0x00000000

#define $MRS(PSR,REG) ((0xE10F0000)|((PSR)&0x00400000)|(((REG)&15)<<12))
#define $MSR(PSR,REG) ((0xE128F000)|((PSR)&0x00410000)|((REG)&0x02000FFF))

/*

Multiply and Multiply-Accumulate (MUL, MLA)

-----------------------------------------------------------------
|31  28|27     22| 21 | 20 |19  16|15  12|11   8|7      4|3    0|
-----------------------------------------------------------------
| Cond | 000000  | A  | S  |  Rd  |  Rn  |  Rs  |  1001  |  Rm  |
-----------------------------------------------------------------
A: 0 - Multiply only, 1 - Multiply and accumulate;
S: 1 - Set cond in CPRS, 0 - Do not;
The multiply (MUL) form of the instruction gives Rd=Rm*Rs.
Rn is ignored, and should be set to zero;
The multiply-accumulate (MLA) form gives Rd=Rm*Rs+Rn.

*/

#define $MUL(Rd,Rm,Rs)     ((0xE0000090)|(((Rd)&15)<<16)|\
                           (((Rs)&15)<<8)|((Rm)&15))
#define $MULS(Rd,Rm,Rs)    ((0xE0100090)|(((Rd)&15)<<16)|\
                           (((Rs)&15)<<8)|((Rm)&15))
#define $MLA(Rd,Rm,Rs,Rn)  ((0xE0200090)|(((Rd)&15)<<16)|\
                           (((Rn)&15)<<12)|(((Rs)&15)<<8)|((Rm)&15))
#define $MLAS(Rd,Rm,Rs,Rn) ((0xE0300090)|(((Rd)&15)<<16)|\
                           (((Rn)&15)<<12)|(((Rs)&15)<<8)|((Rm)&15))


/*

Multiply Long and Multiply-Accumulate Long (MULL,MLAL)

-----------------------------------------------------------------
|31  28|27   23| 22 | 21 | 20 |19  16|15  12|11   8|7    4|3   0|
-----------------------------------------------------------------
| Cond | 00001 | U  | A  | S  | RdHi | RdLo |  Rs  | 1001 | Rm  |
-----------------------------------------------------------------
A: 0 - Multiply only, 1 - Multiply and accumulate;
S: 1 - Set cond in CPRS, 0 - Do not;
U: 1 - Signed, 0 - Unsigned;
Action: RdHi:RdLo=Rm*Rs.

*/

#define $UMULL(RdHi,RdLo,Rm,Rs)  ((0xE0800090)|(((RdHi)&15)<<16)|\
                                 (((RdLo)&15)<<12)|(((Rs)&15)<<8)|\
                                 ((Rm)&15))
#define $UMULLS(RdHi,RdLo,Rm,Rs) ((0xE0900090)|(((RdHi)&15)<<16)|\
                                 (((RdLo)&15)<<12)|(((Rs)&15)<<8)|\
                                 ((Rm)&15))
#define $UMLAL(RdHi,RdLo,Rm,Rs)  ((0xE0A00090)|(((RdHi)&15)<<16)|\
                                 (((RdLo)&15)<<12)|(((Rs)&15)<<8)|\
                                 ((Rm)&15))
#define $UMLALS(RdHi,RdLo,Rm,Rs) ((0xE0B00090)|(((RdHi)&15)<<16)|\
                                 (((RdLo)&15)<<12)|(((Rs)&15)<<8)|\
                                 ((Rm)&15))
#define $SMULL(RdHi,RdLo,Rm,Rs)  ((0xE0C00090)|(((RdHi)&15)<<16)|\
                                 (((RdLo)&15)<<12)|(((Rs)&15)<<8)|\
                                 ((Rm)&15))
#define $SMULLS(RdHi,RdLo,Rm,Rs) ((0xE0D00090)|(((RdHi)&15)<<16)|\
                                 (((RdLo)&15)<<12)|(((Rs)&15)<<8)|\
                                 ((Rm)&15))
#define $SMLAL(RdHi,RdLo,Rm,Rs)  ((0xE0E00090)|(((RdHi)&15)<<16)|\
                                 (((RdLo)&15)<<12)|(((Rs)&15)<<8)|\
                                 ((Rm)&15))
#define $SMLALS(RdHi,RdLo,Rm,Rs) ((0xE0F00090)|(((RdHi)&15)<<16)|\
                                 (((RdLo)&15)<<12)|(((Rs)&15)<<8)|\
                                 ((Rm)&15))


/*

Single Data Transfer (LDR, STR)

-----------------------------------------------------------------
|31  28|27 26| 25 | 24 | 23 | 22 | 21 | 20 |19  16|15  12|11   0|
-----------------------------------------------------------------
| Cond | 0 1 | I  | P  | U  | B  | W  | L  |  Rn  |  Rd  |Offset|
-----------------------------------------------------------------
I:   0 - Offset is register, 1 - Offset is immediate;
P:   0 - Post-indexing, 1 - Pre-indexing;
U:   0 - Down (substrackt offset), 1 - Up (Add offset);
B:   0 - Word transfer, 1 - Byte transfer;
W:   0 - No write-back, 1 - Write-back address into base;
L:   0 - Store to memory, 1 - Load from memory.

Action: [Rn{+/-}Offset]=Rd or Rd=[Rn{+/-}Offset].


STR[B]/LDR[B] Flags:

$STP - Pre-indexing mode;
$STU - Add offset (Up index);
$STW - Write-back bit.

Use IMM12(Offset) macro for immediate offset.
*/

#define $STP 0x01000000
#define $STU 0x00800000
#define $STW 0x00200000


#define STR(BASE,REG,OFFSET,FLAGS)          \
	((0xE4000000)|(((BASE)&15)<<16)|        \
    (((REG)&15)<<12)|((OFFSET&)0x02000FFF)| \
    ((FLAGS)&0x01A00000))

#define STRB(BASE,REG,OFFSET,FLAGS)         \
	((0xE4400000)|(((BASE)&15)<<16)|        \
    (((REG)&15)<<12)|((OFFSET)&0x02000FFF)| \
    ((FLAGS)&0x01A00000))

#define LDR(BASE,REG,OFFSET,FLAGS)          \
	((0xE4100000)|(((BASE)&15)<<16)|        \
    (((REG)&15)<<12)|((OFFSET)&0x02000FFF)| \
    ((FLAGS)&0x01A00000))

#define LDRB(BASE,REG,OFFSET,FLAGS)         \
	((0xE4500000)|(((BASE)&15)<<16)|        \
    (((REG)&15)<<12)|((OFFSET)&0x02000FFF)| \
    ((FLAGS)&0x01A00000))


/*

Halfword and Signed Data Transfer(LDRH/STRH/LDRSB/LDRSH)

-----------------------------------------------------------------
|31  28|27 25|24|23|22|21|20|19  16|15  12|11 8|7|6|5|4|3      0|
-----------------------------------------------------------------
| Cond | 000 |P |U |0 |W |L |  Rn  |  Rd  |0000|1|S|H|1|   Rm   |
-----------------------------------------------------------------

Halfword and Signed Data Transfer With Immediate Offset

-----------------------------------------------------------------
|31  28|27 25|24|23|22|21|20|19  16|15  12|11   8|7|6|5|4|3    0|
-----------------------------------------------------------------
| Cond | 000 |P |U |1 |W |L |  Rn  |  Rd  |OffsHi|1|S|H|1|OffsLo|
-----------------------------------------------------------------
P:   0 - Post-indexing, 1 - Pre-indexing;
U:   0 - Down (substrackt offset), 1 - Up (Add offset);
W:   0 - No write-back, 1 - Write-back address into base;
L:   0 - Store to memory, 1 - Load from memory;
SH:
00 - SWP instruction;
01 - Unsigned halfwords;
10 - Signed byte;
11 - Signed halfwords;

Rm - Offset in register.

Action: [Rn{+/-}Offset]=Rd or Rd=[Rn{+/-}Offset].


Flags:

$HSTP - Pre-indexing mode;
$HSTU - Add offset (Up index);
$HSTW - Write-back bit.

Use HSTIMM(Offset) macro for immediate offset.

*/

#define $HSTP 0x01000000
#define $HSTU 0x00800000
#define $HSTW 0x00200000

#define $HSTIMM(IMM) ((0x00400000)|(((IMM)&0xF0)<<8)|((IMM)&0x0F))

#define $STRH(BASE,REG,OFFSET,FLAGS)        \
	((0xE00000B0)|(((BASE)&15)<<16)|        \
    (((REG)&15)<<12)|((OFFSET)&0x00400F0F)| \
    ((FLAGS)&0x01A00000))

#define $LDRH(BASE,REG,OFFSET,FLAGS)        \
	((0xE01000B0)|(((BASE)&15)<<16)|        \
    (((REG)&15)<<12)|((OFFSET)&0x00400F0F)| \
    ((FLAGS)&0x01A00000))

#define $LDRSB(BASE,REG,OFFSET,FLAGS)       \
	((0xE01000D0)|(((BASE)&15)<<16)|        \
    (((REG)&15)<<12)|((OFFSET)&0x00400F0F)| \
    ((FLAGS)&0x01A00000))

#define $LDRSH(BASE,REG,OFFSET,FLAGS)       \
	((0xE01000F0)|(((BASE)&15)<<16)|        \
    (((REG)&15)<<12)|((OFFSET)&0x00400F0F)| \
    ((FLAGS)&0x01A00000))


/*

Block Data Transfer (LDM, STM)

-----------------------------------------------------------------
|31  28|27 25| 24 | 23 | 22 | 21 | 20 |19  16|15               0|
-----------------------------------------------------------------
| Cond | 100 | P  | U  | S  | W  | L  |  Rn  |  Register List   |
-----------------------------------------------------------------
P:   0 - Post-indexing, 1 - Pre-indexing;
U:   0 - Down (substrackt offset), 1 - Up (Add offset);
S:   0 - Do not force user mode, 1 - Forse user mode;
W:   0 - No write-back, 1 - Write-back address into base;
L:   0 - Store to memory, 1 - Load from memory;

*/

#define $MTP 0x01000000
#define $MTU 0x00800000
#define $MTS 0x00400000
#define $MTW 0x00200000

#define $STM(BASE,FLAGS,REGLIST)       \
	((0xE8000000)|(((BASE)&15)<<16)|   \
    ((REGLIST)&0x0000FFFF)|            \
    ((FLAGS)&0x01E))

#define $STMIB(BASE,REGLIST)           \
	((0xE9A00000)|(((BASE)&15)<<16)|   \
    ((REGLIST)&0x0000FFFF))

#define $STMIA(BASE,REGLIST)           \
	((0xE8A00000)|(((BASE)&15)<<16)|   \
    ((REGLIST)&0x0000FFFF))

#define $STMDB(BASE,REGLIST)           \
	((0xE9200000)|(((BASE)&15)<<16)|   \
    ((REGLIST)&0x0000FFFF))

#define $STMDA(BASE,REGLIST)           \
	((0xE8200000)|(((BASE)&15)<<16)|   \
    ((REGLIST)&0x0000FFFF))

#define $LDM(BASE,FLAGS,REGLIST)       \
	((0xE8100000)|(((BASE)&15)<<16)|   \
    ((REGLIST)&0x0000FFFF)|            \
    ((FLAGS)&0x01E))

#define $LDMIB(BASE,REGLIST)           \
	((0xE9B00000)|(((BASE)&15)<<16)|   \
    ((REGLIST)&0x0000FFFF))

#define $LDMIA(BASE,REGLIST)           \
	((0xE8B00000)|(((BASE)&15)<<16)|   \
    ((REGLIST)&0x0000FFFF))

#define $LDMDB(BASE,REGLIST)           \
	((0xE9300000)|(((BASE)&15)<<16)|   \
    ((REGLIST)&0x0000FFFF))

#define $LDMDA(BASE,REGLIST)           \
	((0xE8300000)|(((BASE)&15)<<16)|   \
    ((REGLIST)&0x0000FFFF))


/*

Single Data Swap (SWP)

-----------------------------------------------------------------
|31  28|27   23| 22 |21   20|19  16|15  12|11           4|3    0|
-----------------------------------------------------------------
| Cond | 00010 | B  |  0 0  |  Rn  |  Rd  |   00001001   |  Rm  |
-----------------------------------------------------------------
B:   0 - Swap word quantity, 1 - Swap byte quantity;
Rn:  Base register;
Rd:  Destination register;
Rm:  Source Register.

*/

#define $SWP(BASER,SRCR,DSTR)          \
	((0xE1000090)|(((BASER)&15)<<16)|  \
    (((DSTR)&15)<<12)|((SRCR)&15))

#define $SWPB(BASER,SRCR,DSTR)         \
	((0xE1400090)|(((BASER)&15)<<16)|  \
    (((DSTR)&15)<<12)|((SRCR)&15))

/*

Software Interrupt (SWI)

-----------------------------------------------------------------
|31  28|27  24|23                                              0|
-----------------------------------------------------------------
| Cond | 1111 |       Comment field (IIIgnored by CPU)          |
-----------------------------------------------------------------
*/

#define $SWI(DATA) ((0xEF000000)|((DATA)&0x00FFFFFF))


#endif  /* ARMIN_H */

