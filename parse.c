/***************************************************************/
/*                                                             */
/*   MIPS-32 Instruction Level Simulator                       */
/*                                                             */
/*   SCE212 Ajou University                                    */
/*   parse.c                                                   */
/*   Adapted from CS311@KAIST                                  */
/*                                                             */
/***************************************************************/

#include <stdio.h>
#include <string.h>

#include "util.h"
#include "parse.h"

int text_size;
int data_size;

instruction parsing_instr(const char *buffer, const int index)
{
    instruction instr;

    char op[7];
    strncpy(op, buffer, 6);
    op[6] = '\0';

    if(!strcmp(op, "000000")){ // R format including sll srl(R-S format)
        instr.opcode = 0;

        char rs[6]; strncpy(rs, buffer+6, 5);
        instr.r_t.r_i.rs = fromBinary(rs);
        rs[5] = '\0';

        char rt[6]; strncpy(rt, buffer+11, 5);
        instr.r_t.r_i.rt = fromBinary(rt);
        rt[5] = '\0';

        char rd[6]; strncpy(rd, buffer+16, 5);
        instr.r_t.r_i.r_i.r.rd = fromBinary(rd);
        rd[5] = '\0';


        char shamt[6]; strncpy(shamt, buffer+21, 5);
        instr.r_t.r_i.r_i.r.shamt = fromBinary(shamt);
        shamt[5] = '\0';

        char funct[7]; strncpy(funct, buffer+26, 6);
        instr.func_code = fromBinary(funct);
        funct[6] = '\0';

        //printf("\n R - %s %s %s %s %s %s\n",op, rs, rt, rd, shamt, funct);

    } else if(!strcmp(op, "000010") || !strcmp(op, "000011")){ // J format

        instr.opcode = fromBinary(op);

        char add[27]; strncpy(add, buffer+6, 26);
        instr.r_t.target = fromBinary(add);
        add[26] = '\0';

        //printf("\nJ - %s %s\n", op, add);

    } else { // I format

        instr.opcode = fromBinary(op);

        char rs[6]; strncpy(rs, buffer+6, 5);
        instr.r_t.r_i.rs = fromBinary(rs);
        rs[5] = '\0';

        char rt[6]; strncpy(rt, buffer+11, 5);
        instr.r_t.r_i.rt = fromBinary(rt);
        rt[5] = '\0';

        char add[17]; strncpy(add, buffer+16, 16);
        instr.r_t.r_i.r_i.imm = fromBinary(add);
        add[16] = '\0';

        //printf("\nI - %s %s %s %s\n",op, rs, rt, add);

    }

    return instr;
}

void parsing_data(const char *buffer, const int index)
{
	mem_write_32(0x10000000 + index, fromBinary(buffer));
	//printf("\n%d %d %d\n", index, fromBinary(buffer), mem_read_32(index));
}

void print_parse_result()
{
    int i;
    printf("Instruction Information\n");

    for(i = 0; i < text_size/4; i++)
    {
        printf("INST_INFO[%d].value : %x\n",i, INST_INFO[i].value);
        printf("INST_INFO[%d].opcode : %d\n",i, INST_INFO[i].opcode);

	    switch(INST_INFO[i].opcode)
        {
            //Type I
            case 0x9:		//(0x001001)ADDIU
            case 0xc:		//(0x001100)ANDI
            case 0xf:		//(0x001111)LUI	
            case 0xd:		//(0x001101)ORI
            case 0xb:		//(0x001011)SLTIU
            case 0x23:		//(0x100011)LW
            case 0x2b:		//(0x101011)SW
            case 0x4:		//(0x000100)BEQ
            case 0x5:		//(0x000101)BNE
                printf("INST_INFO[%d].rs : %d\n",i, INST_INFO[i].r_t.r_i.rs);
                printf("INST_INFO[%d].rt : %d\n",i, INST_INFO[i].r_t.r_i.rt);
                printf("INST_INFO[%d].imm : %d\n",i, INST_INFO[i].r_t.r_i.r_i.imm);
                break;

            //TYPE R
            case 0x0:		//(0x000000)ADDU, AND, NOR, OR, SLTU, SLL, SRL, SUBU  if JR
                printf("INST_INFO[%d].func_code : %d\n",i, INST_INFO[i].func_code);
                printf("INST_INFO[%d].rs : %d\n",i, INST_INFO[i].r_t.r_i.rs);
                printf("INST_INFO[%d].rt : %d\n",i, INST_INFO[i].r_t.r_i.rt);
                printf("INST_INFO[%d].rd : %d\n",i, INST_INFO[i].r_t.r_i.r_i.r.rd);
                printf("INST_INFO[%d].shamt : %d\n",i, INST_INFO[i].r_t.r_i.r_i.r.shamt);
                break;

            //TYPE J
            case 0x2:		//(0x000010)J
            case 0x3:		//(0x000011)JAL
                printf("INST_INFO[%d].target : %d\n",i, INST_INFO[i].r_t.target);
                break;

            default:
                printf("Not available instruction\n");
                assert(0);
        }
    }

    printf("Memory Dump - Text Segment\n");
    for(i = 0; i < text_size; i+=4)
        printf("text_seg[%d] : %x\n", i, mem_read_32(MEM_TEXT_START + i));
    for(i = 0; i < data_size; i+=4)
        printf("data_seg[%d] : %x\n", i, mem_read_32(MEM_DATA_START + i));
    printf("Current PC: %x\n", CURRENT_STATE.PC);
}
