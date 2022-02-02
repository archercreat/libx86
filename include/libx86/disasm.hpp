#pragma once
#include <vector>
#include <functional>
#include <triton/api.hpp>
#include <triton/x86Specifications.hpp>

namespace x86
{
    using ins_filter_t = std::function<bool(const triton::arch::Instruction& ins)>;
    
    struct routine
    {
        std::vector<triton::arch::Instruction> stream;

        [[nodiscard]] int next(const ins_filter_t& filter, int from =  0) const;
        [[nodiscard]] int prev(const ins_filter_t& filter, int from = -1) const;

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
