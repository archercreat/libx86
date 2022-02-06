#include "libx86/emulator.hpp"
#include "libx86/disasm.hpp"

namespace x86
{
static constexpr uint64_t default_stack_base = 0x20000;

emulator::emulator( triton::arch::architecture_e arch )
{
    api = std::make_unique<triton::API>();
    api->setArchitecture( arch );
    api->setMode( triton::modes::ALIGNED_MEMORY, true );
    api->setMode( triton::modes::AST_OPTIMIZATIONS, true );
    api->setMode( triton::modes::TAINT_THROUGH_POINTERS, true );

    api->enableTaintEngine( true );

    api->concretizeAllMemory();
    api->concretizeAllRegister();

    api->setConcreteRegisterValue( sp_r(), default_stack_base );
}

void emulator::map( uint64_t va, const std::vector<uint8_t>& bytes )
{
    api->setConcreteMemoryAreaValue( va, bytes );
}

void emulator::execute( triton::arch::Instruction& ins )
{
    if ( !api->buildSemantics( ins ) )
    {
        std::cout << "Failed to execute " << ins << std::endl;
        throw -1;
    }
}

void emulator::execute( std::vector<triton::arch::Instruction>& stream )
{
    for ( auto& ins : stream )
        execute( ins );
}

void emulator::execute( std::vector<instruction>& stream )
{
    for ( auto& ins : stream )
        execute( ins );
}

// This deletes symbolic, taint, astcontext, irbuilder and solver.
//
void emulator::soft_reset() const
{
    api->removeEngines();
    api->initEngines();
    api->concretizeAllRegister();
    api->concretizeAllMemory();
}

uint64_t emulator::pc() const
{
    return api->getConcreteRegisterValue( api->getCpuInstance()->getProgramCounter() ).convert_to<uint64_t>();
}

uint64_t emulator::sp() const
{
    return api->getConcreteRegisterValue( api->getCpuInstance()->getStackPointer() ).convert_to<uint64_t>();
}

uint64_t emulator::value( const triton::arch::Register& r ) const
{
    return api->getConcreteRegisterValue( r ).convert_to<uint64_t>();
}

uint64_t emulator::value( const triton::arch::register_e r ) const
{
    return value( api->getRegister( r ) );
}

const triton::arch::Register& emulator::pc_r() const
{
    return api->getCpuInstance()->getProgramCounter();
}

const triton::arch::Register& emulator::sp_r() const
{
    return api->getCpuInstance()->getStackPointer();
}

emulator emulator::fork() const
{
    using namespace triton::arch;
    using namespace triton::arch::x86;

    emulator out( api->getArchitecture() );
    // Copy CPU instance.
    //
    if ( api->getArchitecture() == ARCH_X86_64 )
        *(x8664Cpu*)out.api->getCpuInstance() = *(x8664Cpu*)api->getCpuInstance();
    else
        *(x86Cpu*)out.api->getCpuInstance() = *(x86Cpu*)api->getCpuInstance();
    return out;
}

void emulator::dump_regs() const
{
    for ( const auto& [r_e, r] : api->getAllRegisters() )
    {
        if ( r.getParent() == r_e )
            std::printf( "[%s] - 0x%lx\n", r.getName().c_str(), value( r_e ) );
    }
}
};