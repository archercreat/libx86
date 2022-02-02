#pragma once
#include <vector>
#include <functional>
#include <triton/api.hpp>
#include <triton/x86Specifications.hpp>

namespace x86
{
    using ins_filter_t = std::function<bool(const triton::arch::Instruction& ins)>;
    
    struct handler
    {
        std::vector<triton::arch::Instruction> stream;

        int next(const ins_filter_t& filter, int from =  0) const;
	    int prev(const ins_filter_t& filter, int from = -1) const;

        void dump() const;

        auto begin() { return stream.begin(); }
        auto end()   { return stream.end(); }
        auto begin() const { return stream.begin(); }
        auto end()   const { return stream.end(); }
        auto size()  const { return stream.size(); }
        auto operator[](size_t n) const { return stream[n]; }
        auto operator[](size_t n) { return stream[n]; }
    };

    handler& unroll(triton::API* api, uint64_t address);
};
