#include "disasm.h"
#include <algorithm>
#include <set>
#include <mutex>

namespace x86
{
    int routine::next(const ins_filter_t& filter, int from) const
    {
        if (from >= stream.size()) return -1;
        for (int i = from; i < stream.size(); i++)
            if (filter(stream[i])) return i;
        return -1;
    }

    int routine::prev(const ins_filter_t& filter, int from) const
    {
        if (from == -1) from = (int)stream.size() - 1;
        if (from >= stream.size()) return -1;
        for (int i = from; i >= 0; i--)
            if (filter(stream[i])) return i;
        return -1;
    }

    void routine::dump() const
    {
        for (const auto& i : stream)
            std::printf("0x%lx %s\n", i.getAddress(), i.getDisassembly().c_str());
    }

    routine& unroll(triton::API* api, uint64_t address)
    {
        static std::map<uint64_t, routine> cache;
        static std::mutex cache_mutex;

        std::lock_guard g(cache_mutex);
        auto& out = cache[address];
        if (out.stream.size()) return out;

        while (true)
        {
            auto ss = api->disassembly(address);
            if (ss.empty()) break;
            
            out.stream.insert(out.stream.end(), ss.begin(), ss.end());

            const auto& ins = ss.back();
            if (ins.isBranch() || ins.getType() == triton::arch::x86::ID_INS_CALL)
            {
                if (ins.operands[0].getType() == triton::arch::OP_IMM)
                    address = ins.operands[0].getConstImmediate().getValue();
                else
                    break;
            }
            else
                break;
        }
        return out;
    }
};