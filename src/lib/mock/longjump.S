.section .text
.globl set_long_jump
.globl long_jump

#u64 set_long_jump(JumpBuff_t env)
#env : pointer to buffer to save registers
#returns 0 on initial call, nonzero after long_jump
set_long_jump:
    #a0 : env(JumpBuff_t pointer)
    #Save callee - saved registers : sp, s0 - s11, ra
    sd sp, 0(a0)
    sd s0, 8(a0)
    sd s1, 16(a0)
    sd s2, 24(a0)
    sd s3, 32(a0)
    sd s4, 40(a0)
    sd s5, 48(a0)
    sd s6, 56(a0)
    sd s7, 64(a0)
    sd s8, 72(a0)
    sd s9, 80(a0)
    sd s10, 88(a0)
    sd s11, 96(a0)
    sd ra, 104(a0) #Save return address
    li a0, 0 #Return 0 on initial call
    ret

#void long_jump(JumpBuff_t env, int val)
#env : pointer to buffer with saved registers
#val : value to return from setjump(if 0, return 1)
long_jump:
    #a0 : env(JumpBuff_t pointer)
    #a1 : val
    ld sp, 0(a0)
    ld s0, 8(a0)
    ld s1, 16(a0)
    ld s2, 24(a0)
    ld s3, 32(a0)
    ld s4, 40(a0)
    ld s5, 48(a0)
    ld s6, 56(a0)
    ld s7, 64(a0)
    ld s8, 72(a0)
    ld s9, 80(a0)
    ld s10, 88(a0)
    ld s11, 96(a0)
    ld ra, 104(a0) #Restore return address
    mv a0, a1 #Set return value to val
    beqz a0, .return_one #If val == 0,
    li a0, 1
    ret
.return_one: li a0, 1
    ret
