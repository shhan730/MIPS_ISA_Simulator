#include <stdio.h>

#include "util.h"
#include "run.h"

/***************************************************************/
/*                                                             */
/* Procedure: get_inst_info                                    */
/*                                                             */
/* Purpose: Read insturction information                       */
/*                                                             */
/***************************************************************/
instruction* get_inst_info(uint32_t pc)
{
    return &INST_INFO[(pc - MEM_TEXT_START) >> 2];
}

/***************************************************************/
/*                                                             */
/* Procedure: process_instruction                              */
/*                                                             */
/* Purpose: Process one instrction                             */
/*                                                             */
/***************************************************************/
void process_instruction()
{
        if (CURRENT_STATE.PC <= 0x400000 + (NUM_INST * 4) && CURRENT_STATE.PC >=0x400000){

            instruction* tempInst = get_inst_info(CURRENT_STATE.PC);
            //printf("\n-%d-\n", tempInst->opcode);

            if(tempInst->opcode == 0){ // R format
                //printf("\nR format\n");
                //printf("\nR - %d %d %d %d %d %x\n", tempInst->opcode, tempInst->r_t.r_i.rs, tempInst->r_t.r_i.rt, tempInst->r_t.r_i.r_i.r.rd, tempInst->r_t.r_i.r_i.r.shamt, tempInst->func_code);

                if(tempInst->func_code == fromBinary("100001")){ //addu
                    CURRENT_STATE.REGS[tempInst->r_t.r_i.r_i.r.rd] = CURRENT_STATE.REGS[tempInst->r_t.r_i.rs] + CURRENT_STATE.REGS[tempInst->r_t.r_i.rt];
                } else if(tempInst->func_code == fromBinary("100100")){ //and
                    CURRENT_STATE.REGS[tempInst->r_t.r_i.r_i.r.rd] = CURRENT_STATE.REGS[tempInst->r_t.r_i.rs] & CURRENT_STATE.REGS[tempInst->r_t.r_i.rt];
                } else if(tempInst->func_code == fromBinary("001000")){ // jr
                    CURRENT_STATE.PC = CURRENT_STATE.REGS[tempInst->r_t.r_i.rs] - 4;
                } else if(tempInst->func_code == fromBinary("100111")){ // nor
                    CURRENT_STATE.REGS[tempInst->r_t.r_i.r_i.r.rd] = ~(CURRENT_STATE.REGS[tempInst->r_t.r_i.rs] | CURRENT_STATE.REGS[tempInst->r_t.r_i.rt]);
                } else if(tempInst->func_code == fromBinary("100101")){ // or
                    CURRENT_STATE.REGS[tempInst->r_t.r_i.r_i.r.rd] = (CURRENT_STATE.REGS[tempInst->r_t.r_i.rs] | CURRENT_STATE.REGS[tempInst->r_t.r_i.rt]);
                } else if(tempInst->func_code == fromBinary("101011")){ // sltu
                    CURRENT_STATE.REGS[tempInst->r_t.r_i.r_i.r.rd] = (CURRENT_STATE.REGS[tempInst->r_t.r_i.rs] < CURRENT_STATE.REGS[tempInst->r_t.r_i.rt]) ? 1 : 0;
                } else if(tempInst->func_code == fromBinary("000000")){ // sll
                    CURRENT_STATE.REGS[tempInst->r_t.r_i.r_i.r.rd] = CURRENT_STATE.REGS[tempInst->r_t.r_i.rt] << tempInst->r_t.r_i.r_i.r.shamt;
                } else if(tempInst->func_code == fromBinary("000010")){ // srl
                    CURRENT_STATE.REGS[tempInst->r_t.r_i.r_i.r.rd] = CURRENT_STATE.REGS[tempInst->r_t.r_i.rt] >> tempInst->r_t.r_i.r_i.r.shamt;
                } else if(tempInst->func_code == fromBinary("100011")){ // subu
                    CURRENT_STATE.REGS[tempInst->r_t.r_i.r_i.r.rd] = CURRENT_STATE.REGS[tempInst->r_t.r_i.rs] - CURRENT_STATE.REGS[tempInst->r_t.r_i.rt];
                }

            } else if(tempInst->opcode == fromBinary("000010") ){ // J foramt - j
                //printf("\nJ format\n");
                CURRENT_STATE.PC = (tempInst->r_t.target * 4) - 4;
            } else if(tempInst->opcode == fromBinary("000011")){ // J foramt - jal
                //printf("\nJ format\n");
                CURRENT_STATE.REGS[31] = CURRENT_STATE.PC + 8;
                CURRENT_STATE.PC = (tempInst->r_t.target * 4) - 4;
            } else{ // I format
                //printf("\nI format\n");

                if(tempInst->opcode == fromBinary("001001")){ // addiu
                    CURRENT_STATE.REGS[tempInst->r_t.r_i.rt] = CURRENT_STATE.REGS[tempInst->r_t.r_i.rs] + tempInst->r_t.r_i.r_i.imm;
                } else if(tempInst->opcode == fromBinary("001100")){ // andi
                    CURRENT_STATE.REGS[tempInst->r_t.r_i.rt] = CURRENT_STATE.REGS[tempInst->r_t.r_i.rs] & tempInst->r_t.r_i.r_i.imm;
                } else if(tempInst->opcode == fromBinary("000100")){ // beq
                    if(CURRENT_STATE.REGS[tempInst->r_t.r_i.rs] == CURRENT_STATE.REGS[tempInst->r_t.r_i.rt]) CURRENT_STATE.PC = CURRENT_STATE.PC + (tempInst->r_t.r_i.r_i.imm * 4);
                } else if(tempInst->opcode == fromBinary("000101")){ // bne
                    if(CURRENT_STATE.REGS[tempInst->r_t.r_i.rs] != CURRENT_STATE.REGS[tempInst->r_t.r_i.rt]) CURRENT_STATE.PC = CURRENT_STATE.PC + (tempInst->r_t.r_i.r_i.imm * 4);
                } else if(tempInst->opcode == fromBinary("001111")){ // lui
                    CURRENT_STATE.REGS[tempInst->r_t.r_i.rt] = tempInst->r_t.r_i.r_i.imm << 16;
                } else if(tempInst->opcode == fromBinary("100011")){ // lw
                    CURRENT_STATE.REGS[tempInst->r_t.r_i.rt] = mem_read_32( CURRENT_STATE.REGS[tempInst->r_t.r_i.rs] + tempInst->r_t.r_i.r_i.imm );
                } else if(tempInst->opcode == fromBinary("001101")){ // ori
                    CURRENT_STATE.REGS[tempInst->r_t.r_i.rt] = CURRENT_STATE.REGS[tempInst->r_t.r_i.rs] | tempInst->r_t.r_i.r_i.imm;
                } else if(tempInst->opcode == fromBinary("001011")){ // sltiu
                    CURRENT_STATE.REGS[tempInst->r_t.r_i.rt] = (CURRENT_STATE.REGS[tempInst->r_t.r_i.rs] < tempInst->r_t.r_i.r_i.imm) ? 1 : 0;
                } else if(tempInst->opcode == fromBinary("101011")){ // sw
                    mem_write_32(CURRENT_STATE.REGS[tempInst->r_t.r_i.rs] + tempInst->r_t.r_i.r_i.imm, CURRENT_STATE.REGS[tempInst->r_t.r_i.rt]);
                }

            }

            CURRENT_STATE.PC += 4;
        }

    if(CURRENT_STATE.PC >= (0x400000 + (NUM_INST * 4))) RUN_BIT = FALSE;

}
