#pragma once
#include <type_traits>

#include <triton/api.hpp>
#include <triton/x86Specifications.hpp>
#include <triton/ast.hpp>
#include <triton/symbolicEngine.hpp>
#include <triton/taintEngine.hpp>
#include <triton/x8664Cpu.hpp>
#include <triton/x86Cpu.hpp>

namespace x86
{
    struct emulator
    {
        emulator() = default;
        emulator(triton::arch::architecture_e arch);

        void map(uint64_t va, const std::vector<uint8_t>& bytes);
        void execute(triton::arch::Instruction& ins);
        void execute(std::vector<triton::arch::Instruction>& stream);
        void soft_reset() const;

        [[nodiscard]] uint64_t pc() const;
        [[nodiscard]] uint64_t sp() const;
        [[nodiscard]] uint64_t value(const triton::arch::Register& r) const;
        [[nodiscard]] uint64_t value(const triton::arch::register_e r) const;

        [[nodiscard]] const triton::arch::Register& pc_r() const;
        [[nodiscard]] const triton::arch::Register& sp_r() const;

        template<typename T = uint64_t>
        void write_mem(uint64_t addr, T value)
        {
            api->setConcreteMemoryValue(triton::arch::MemoryAccess(addr, sizeof(T)), value);
        }

        template<typename T = uint64_t>
        [[nodiscard]] T read_mem(uint64_t addr) const
        {
            return api->getConcreteMemoryValue(triton::arch::MemoryAccess(addr, sizeof(T))).convert_to<T>();
        }
        
        [[nodiscard]] emulator fork() const;
        void dump_regs() const;

        std::unique_ptr<triton::API> api;
    };
};
