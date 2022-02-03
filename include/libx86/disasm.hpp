#pragma once
#include <vector>
#include <functional>
#include <triton/api.hpp>
#include <triton/x86Specifications.hpp>

namespace x86
{
    using ins_filter_t = std::function<bool(const triton::arch::Instruction& ins)>;

    struct instruction : public triton::arch::Instruction
    {
        using triton::arch::Instruction::Instruction;

        instruction(const triton::arch::Instruction& ins);
        [[nodiscard]] bool is(triton::arch::x86::instruction_e mnem, const std::vector<triton::arch::operand_e>& ops) const;
        [[nodiscard]] bool is(const ins_filter_t& filter) const;
    };
    
    struct routine
    {
        std::vector<instruction> stream;

        [[nodiscard]] int next(const ins_filter_t& filter, int from =  0) const;
        [[nodiscard]] int prev(const ins_filter_t& filter, int from = -1) const;
        [[nodiscard]] int next(triton::arch::x86::instruction_e mnem, const std::vector<triton::arch::operand_e>& ops, int from = -1) const;
        [[nodiscard]] int prev(triton::arch::x86::instruction_e mnem, const std::vector<triton::arch::operand_e>& ops, int from = -1) const;

        void dump() const;

        [[nodiscard]] auto begin() { return stream.begin(); }
        [[nodiscard]] auto end()   { return stream.end(); }
        [[nodiscard]] auto begin() const { return stream.begin(); }
        [[nodiscard]] auto end()   const { return stream.end(); }
        [[nodiscard]] auto size()  const { return stream.size(); }
        [[nodiscard]] auto operator[](size_t n) const { return stream[n]; }
        [[nodiscard]] auto operator[](size_t n) { return stream[n]; }
    };

    [[nodiscard]] routine& unroll(triton::API* api, uint64_t address);
};
