#ifndef TRANSLATE_H
#define TRANSLATE_H
#include "tgtypes.h"
#include "compatglib.h"
#include "tb.h"
#ifdef __cplusplus
extern "C" {
#endif
#define DISAS_NEXT    0 /* next instruction can be analyzed */
#define DISAS_JUMP    1 /* only pc was modified dynamically */
#define DISAS_UPDATE  2 /* cpu state was modified dynamically */
#define DISAS_TB_JUMP 3 /* only pc was modified statically */
#define DISAS_WFI 4
#define DISAS_SWI 5
#define DISAS_EXC 6
/* WFE */
#define DISAS_WFE 7
#define DISAS_HVC 8
#define DISAS_SMC 9

typedef struct DisasContext {
    target_ulong pc;
    uint32_t insn;
    int is_jmp;
    /* Nonzero if this instruction has been conditionally skipped.  */
    int condjmp;
    /* The label that will be jumped to when the instruction is skipped.  */
    int condlabel;
    /* Thumb-2 conditional execution bits.  */
    int condexec_mask;
    int condexec_cond;
    struct TranslationBlock *tb;
    int singlestep_enabled;
    int thumb;
    int bswap_code;
    bool cpacr_fpen; /* FP enabled via CPACR.FPEN */
    bool vfp_enabled; /* FP enabled via FPSCR.EN */
    int vec_len;
    int vec_stride;
    /* Immediate value in AArch32 SVC insn; must be set if is_jmp == DISAS_SWI
     * so that top level loop can generate correct syndrome information.
     */
    uint32_t svc_imm;
    int aarch64;
    int current_el;
    GHashTable *cp_regs;
    uint64_t features; /* CPU features bits */
    /* Because unallocated encodings generate different exception syndrome
     * information from traps due to FP being disabled, we can't do a single
     * "is fp access disabled" check at a high level in the decode tree.
     * To help in catching bugs where the access check was forgotten in some
     * code path, we set this flag when the access check is done, and assert
     * that it is set at the point where we actually touch the FP regs.
     */
    bool fp_access_checked;
    /* ARMv8 single-step state (this is distinct from the QEMU gdbstub
     * single-step support).
     */
    bool ss_active;
    bool pstate_ss;
    /* True if the insn just emitted was a load-exclusive instruction
     * (necessary for syndrome information for single step exceptions),
     * ie A64 LDX*, LDAX*, A32/T32 LDREX*, LDAEX*.
     */
    bool is_ldex;
    /* True if a single-step exception will be taken to the current EL */
    bool ss_same_el;
    /* Bottom two bits of XScale c15_cpar coprocessor access control reg */
    int c15_cpar;
#define TMP_A64_MAX 16
    int tmp_a64_count;
    TCGv_i64 tmp_a64[TMP_A64_MAX];
    TCGv_ptr __cpu_env;
    /* We reuse the same 64-bit temporaries for efficiency.  */
    TCGv_i64 __cpu_V0, __cpu_V1, __cpu_M0;
    TCGv_i32 __cpu_R[16];
    TCGv_i32 __cpu_CF, __cpu_NF, __cpu_VF, __cpu_ZF;
    TCGv_i64 __cpu_exclusive_addr;
    TCGv_i64 __cpu_exclusive_val;
    TCGv_i64 __cpu_exclusive_test;
    TCGv_i32 __cpu_exclusive_info;

    /* FIXME:  These should be removed.  */
    TCGv_i32 __cpu_F0s, __cpu_F1s;
    TCGv_i64 __cpu_F0d, __cpu_F1d;
} DisasContext;

extern const ARMCPRegInfo *get_arm_cp_reginfo(GHashTable *cpregs, uint32_t encoded_cp);

#define OPC_BUF_SIZE 640
#define MAX_OP_PER_INSTR 266
#define OPC_MAX_SIZE (OPC_BUF_SIZE - MAX_OP_PER_INSTR)


void gen_intermediate_code_internal(ARMCPU* cpu, TranslationBlock *tb, DisasContext* dc);
                                                  

#ifdef __cplusplus
}
#endif
#endif /* TRANSLATE_H */
