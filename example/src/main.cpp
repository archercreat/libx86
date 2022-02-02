#include <iostream>
#include <libx86/disasm.hpp>

int test()
{
    int x = 0;
    return x ^ 5 * 10 ^ 1337;
}

int main() 
{
    std::vector<uint8_t> raw((const uint8_t*)test, (const uint8_t*)test + 100);
    triton::API api;
    api.setArchitecture(triton::arch::ARCH_X86_64);
    api.setConcreteMemoryAreaValue(0, raw);
    auto r = x86::unroll(&api, 0);
    r.dump();
    return EXIT_SUCCESS;
}
