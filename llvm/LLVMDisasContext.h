#ifndef LLVMDISASCONTEXT_H
#define LLVMDISASCONTEXT_H
#include <memory>
#include <vector>
#include <unordered_map>
#include "cpu.h"
#include "translate.h"
#include "CompilerState.h"
#include "Output.h"
#include "DisasContextBase.h"

namespace jit {

class LLVMDisasContext : public DisasContextBase {
public:
    explicit LLVMDisasContext(ExecutableMemoryAllocator* allocator, void* dispDirect, void* dispInDirect);
    ~LLVMDisasContext();
    inline Output* output() { return m_output.get(); }
    inline CompilerState* state() { return m_state.get(); }
    template <typename Type>
    Type allocateTcg();
    LBasicBlock labelToBB(int n);
    int newLabel();
    template <typename TCGType>
    TCGType wrap(LValue v);

    template <typename TCGType>
    TCGType wrapMem(LValue v);
    template <typename TCGType>
    LValue unwrap(TCGType v);
    template <typename TCGType>
    void storeToTCG(LValue v, TCGType ret);

    void extract_64_32(LValue my64, LValue thirtytwo, TCGv_i32 rl, TCGv_i32 rh);
    LValue tcgPointerToLLVM(TCGMemOp op, TCGv pointer);
    LValue tcgMemCastTo32(TCGMemOp op, LValue val);

    virtual void compile() override;
    virtual void link() override;
    virtual int gen_new_label() override;
    virtual void gen_set_label(int n) override;
    virtual TCGv_i64 global_mem_new_i64(int reg, intptr_t offset, const char* name) override;
    virtual TCGv_i32 global_mem_new_i32(int reg, intptr_t offset, const char* name) override;
    virtual TCGv_ptr global_reg_new_ptr(int reg, const char* name) override;

    virtual TCGv_i32 const_i32(int32_t val) override;
    virtual TCGv_ptr const_ptr(const void* val) override;
    virtual TCGv_i64 const_i64(int64_t val) override;

    virtual void gen_add2_i32(TCGv_i32 rl, TCGv_i32 rh, TCGv_i32 al,
        TCGv_i32 ah, TCGv_i32 bl, TCGv_i32 bh) override;

    virtual void gen_add_i32(TCGv_i32 ret, TCGv_i32 arg1, TCGv_i32 arg2) override;
    virtual void gen_add_i64(TCGv_i64 ret, TCGv_i64 arg1, TCGv_i64 arg2) override;
    virtual void gen_addi_i32(TCGv_i32 ret, TCGv_i32 arg1, int32_t arg2) override;
    virtual void gen_addi_ptr(TCGv_ptr ret, TCGv_ptr arg1, int32_t arg2) override;
    virtual void gen_addi_i64(TCGv_i64 ret, TCGv_i64 arg1, int64_t arg2) override;
    virtual void gen_andc_i32(TCGv_i32 ret, TCGv_i32 arg1, TCGv_i32 arg2) override;
    virtual void gen_and_i32(TCGv_i32 ret, TCGv_i32 arg1, TCGv_i32 arg2) override;
    virtual void gen_and_i64(TCGv_i64 ret, TCGv_i64 arg1, TCGv_i64 arg2) override;
    virtual void gen_andi_i32(TCGv_i32 ret, TCGv_i32 arg1, uint32_t arg2) override;
    virtual void gen_andi_i64(TCGv_i64 ret, TCGv_i64 arg1, int64_t arg2) override;
    virtual void gen_brcondi_i32(TCGCond cond, TCGv_i32 arg1,
        int32_t arg2, int label_index) override;
    virtual void gen_bswap16_i32(TCGv_i32 ret, TCGv_i32 arg) override;
    virtual void gen_bswap32_i32(TCGv_i32 ret, TCGv_i32 arg) override;
    virtual void gen_concat_i32_i64(TCGv_i64 dest, TCGv_i32 low,
        TCGv_i32 high) override;

    virtual void gen_deposit_i32(TCGv_i32 ret, TCGv_i32 arg1,
        TCGv_i32 arg2, unsigned int ofs,
        unsigned int len) override;
    virtual void gen_mov_i32(TCGv_i32 ret, TCGv_i32 arg) override;
    virtual void gen_exit_tb(int direct) override;
    virtual void gen_ext16s_i32(TCGv_i32 ret, TCGv_i32 arg) override;
    virtual void gen_ext16u_i32(TCGv_i32 ret, TCGv_i32 arg) override;
    virtual void gen_ext32u_i64(TCGv_i64 ret, TCGv_i64 arg) override;
    virtual void gen_ext8s_i32(TCGv_i32 ret, TCGv_i32 arg) override;
    virtual void gen_ext8u_i32(TCGv_i32 ret, TCGv_i32 arg) override;
    virtual void gen_ext_i32_i64(TCGv_i64 ret, TCGv_i32 arg) override;
    virtual void gen_extu_i32_i64(TCGv_i64 ret, TCGv_i32 arg) override;
    virtual void gen_ld_i32(TCGv_i32 ret, TCGv_ptr arg2, tcg_target_long offset) override;
    virtual void gen_ld_i64(TCGv_i64 ret, TCGv_ptr arg2,
        target_long offset) override;
    virtual void gen_movcond_i32(TCGCond cond, TCGv_i32 ret,
        TCGv_i32 c1, TCGv_i32 c2,
        TCGv_i32 v1, TCGv_i32 v2) override;
    virtual void gen_movcond_i64(TCGCond cond, TCGv_i64 ret,
        TCGv_i64 c1, TCGv_i64 c2,
        TCGv_i64 v1, TCGv_i64 v2) override;
    virtual void gen_mov_i64(TCGv_i64 ret, TCGv_i64 arg) override;
    virtual void gen_movi_i32(TCGv_i32 ret, int32_t arg) override;
    virtual void gen_movi_i64(TCGv_i64 ret, int64_t arg) override;
    virtual void gen_mul_i32(TCGv_i32 ret, TCGv_i32 arg1, TCGv_i32 arg2) override;
    virtual void gen_muls2_i32(TCGv_i32 rl, TCGv_i32 rh,
        TCGv_i32 arg1, TCGv_i32 arg2) override;
    virtual void gen_mulu2_i32(TCGv_i32 rl, TCGv_i32 rh,
        TCGv_i32 arg1, TCGv_i32 arg2) override;
    virtual void gen_neg_i32(TCGv_i32 ret, TCGv_i32 arg) override;
    virtual void gen_neg_i64(TCGv_i64 ret, TCGv_i64 arg) override;
    virtual void gen_not_i32(TCGv_i32 ret, TCGv_i32 arg) override;
    virtual void gen_orc_i32(TCGv_i32 ret, TCGv_i32 arg1, TCGv_i32 arg2) override;
    virtual void gen_or_i32(TCGv_i32 ret, TCGv_i32 arg1, TCGv_i32 arg2) override;
    virtual void gen_or_i64(TCGv_i64 ret, TCGv_i64 arg1, TCGv_i64 arg2) override;
    virtual void gen_ori_i32(TCGv_i32 ret, TCGv_i32 arg1, int32_t arg2) override;
    virtual void gen_qemu_ld_i32(TCGv_i32 val, TCGv addr, TCGArg idx, TCGMemOp memop) override;
    virtual void gen_qemu_ld_i64(TCGv_i64 val, TCGv addr, TCGArg idx, TCGMemOp memop) override;
    virtual void gen_qemu_st_i32(TCGv_i32 val, TCGv addr, TCGArg idx, TCGMemOp memop) override;
    virtual void gen_qemu_st_i64(TCGv_i64 val, TCGv addr, TCGArg idx, TCGMemOp memop) override;
    virtual void gen_rotr_i32(TCGv_i32 ret, TCGv_i32 arg1, TCGv_i32 arg2) override;
    virtual void gen_rotri_i32(TCGv_i32 ret, TCGv_i32 arg1, int32_t arg2) override;
    virtual void gen_sar_i32(TCGv_i32 ret, TCGv_i32 arg1, TCGv_i32 arg2) override;
    virtual void gen_sari_i32(TCGv_i32 ret, TCGv_i32 arg1, int32_t arg2) override;
    virtual void gen_setcond_i32(TCGCond cond, TCGv_i32 ret,
        TCGv_i32 arg1, TCGv_i32 arg2) override;
    virtual void gen_shl_i32(TCGv_i32 ret, TCGv_i32 arg1, TCGv_i32 arg2) override;
    virtual void gen_shli_i32(TCGv_i32 ret, TCGv_i32 arg1, int32_t arg2) override;
    virtual void gen_shli_i64(TCGv_i64 ret, TCGv_i64 arg1, int64_t arg2) override;
    virtual void gen_shr_i32(TCGv_i32 ret, TCGv_i32 arg1, TCGv_i32 arg2) override;
    virtual void gen_shri_i32(TCGv_i32 ret, TCGv_i32 arg1, int32_t arg2) override;
    virtual void gen_shri_i64(TCGv_i64 ret, TCGv_i64 arg1, int64_t arg2) override;
    virtual void gen_st_i32(TCGv_i32 arg1, TCGv_ptr arg2, tcg_target_long offset) override;
    virtual void gen_st_i64(TCGv_i64 arg1, TCGv_ptr arg2,
        target_long offset) override;
    virtual void gen_sub_i32(TCGv_i32 ret, TCGv_i32 arg1, TCGv_i32 arg2) override;
    virtual void gen_sub_i64(TCGv_i64 ret, TCGv_i64 arg1, TCGv_i64 arg2) override;
    virtual void gen_subi_i32(TCGv_i32 ret, TCGv_i32 arg1, int32_t arg2) override;
    virtual void gen_trunc_i64_i32(TCGv_i32 ret, TCGv_i64 arg) override;
    virtual void gen_xor_i32(TCGv_i32 ret, TCGv_i32 arg1, TCGv_i32 arg2) override;
    virtual void gen_xor_i64(TCGv_i64 ret, TCGv_i64 arg1, TCGv_i64 arg2) override;
    virtual void gen_xori_i32(TCGv_i32 ret, TCGv_i32 arg1, int32_t arg2) override;
    virtual TCGv_i32 temp_local_new_i32() override;
    virtual TCGv_i32 temp_new_i32() override;
    virtual TCGv_ptr temp_new_ptr() override;
    virtual TCGv_i64 temp_new_i64() override;
    virtual void temp_free_i32(TCGv_i32 a) override;
    virtual void temp_free_i64(TCGv_i64 a) override;
    virtual void temp_free_ptr(TCGv_ptr a) override;
    virtual void gen_callN(void* func, TCGArg ret,
        int nargs, TCGArg* args) override;
    virtual void func_start() override;
    virtual bool should_continue() override;

private:
    LValue myhandleCallRet(void* func, TCGArg ret,
        int nargs, TCGArg* args);
    void myhandleCallRetNone(void* func, int nargs, TCGArg* args);
    uint8_t* m_currentBufferPointer;
    uint8_t* m_currentBufferEnd;
    const static size_t allocate_unit = 4096 * 16;
    typedef std::vector<void*> TcgBufferList;
    TcgBufferList m_bufferList;
    std::unique_ptr<CompilerState> m_state;
    std::unique_ptr<Output> m_output;
    typedef std::unordered_map<int, LBasicBlock> LabelMap;
    LabelMap m_labelMap;
    int m_labelCount;
    void* m_dispDirect;
    void* m_dispIndirect;
};
}
#endif /* LLVMDISASCONTEXT_H */
