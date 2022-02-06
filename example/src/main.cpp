#include <iostream>
#include <libx86/disasm.hpp>
#include <libx86/emulator.hpp>

int test()
{
    int x = 0;
    return x ^ 1337;
}

int main()
{
    auto emu = x86::emulator( triton::arch::ARCH_X86_64 );
    emu.map( 0, { (const uint8_t*)test, (const uint8_t*)test + 100 } );
    auto r = x86::unroll( emu.api.get(), 0 );
    r.dump();
    emu.execute( r.stream );
    auto ret = emu.value( triton::arch::ID_REG_X86_RAX );
    std::printf( "rax: %lu\n", ret );
    assert( ret == 1337 );
    return EXIT_SUCCESS;
}
