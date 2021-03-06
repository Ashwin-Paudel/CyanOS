#include "Context.h"

uint32_t Context::setup_task_stack_context(ContextType type, ContextInformation& info)
{
	UNUSED(type);

	InitialTaskContext* context =
	    reinterpret_cast<InitialTaskContext*>(uintptr_t(info.stack) + info.stack_size - sizeof(InitialTaskContext));
	context->return_address = info.return_function;
	context->argument = info.argument;
	context->isr_frame.eip = info.start_function;
	context->isr_frame.cs = KCS_SELECTOR;
	context->isr_frame.registers.ds = KDS_SELECTOR;
	context->isr_frame.registers.es = KDS_SELECTOR;
	context->isr_frame.registers.fs = TIB_SELECTOR;
	context->isr_frame.registers.gs = PIB_SELECTOR;

	context->isr_frame.eflags = EFLAGS_IF_ENABLE;
	return uint32_t(&context->isr_frame) + 4;
}

void Context::switch_task_stack(uint32_t task_stack_start, uint32_t pib, uint32_t tib)
{
	GDT::set_tss_stack(task_stack_start);
	GDT::load_gs(pib);
	GDT::load_fs(tib);
}

void Context::enter_usermode(uintptr_t address, uintptr_t stack)
{
	asm volatile("mov %0, %%ds;"
	             "mov %0, %%es;"
	             "pushl %0;"
	             "pushl %1;"
	             "pushl %2;"
	             "pushl %3;"
	             "pushl %4;"
	             "iret;"
	             :
	             : "r"(uint32_t{UDS_SELECTOR}), "r"(uint32_t{stack}), "i"(uint32_t{EFLAGS_IF_ENABLE}),
	               "r"(uint32_t{UCS_SELECTOR}), "r"(uint32_t{address}));
}

void Context::set_return_value(ISRContextFrame& frame, uint32_t value)
{
	frame.registers.eax = value;
}

void Context::set_return_arg1(ISRContextFrame& frame, uint32_t value)
{
	frame.registers.ecx = value;
}

void Context::set_return_arg2(ISRContextFrame& frame, uint32_t value)
{
	frame.registers.edx = value;
}

void Context::set_return_arg3(ISRContextFrame& frame, uint32_t value)
{
	frame.registers.ebx = value;
}

uint32_t Context::syscall_num(ISRContextFrame& frame)
{
	return frame.registers.eax;
}

uint32_t Context::syscall_param1(ISRContextFrame& frame)
{
	return frame.registers.ecx;
}

uint32_t Context::syscall_param2(ISRContextFrame& frame)
{
	return frame.registers.edx;
}

uint32_t Context::syscall_param3(ISRContextFrame& frame)
{
	return frame.registers.ebx;
}

uint32_t Context::syscall_param4(ISRContextFrame& frame)
{
	return frame.registers.esi;
}

uint32_t Context::syscall_param5(ISRContextFrame& frame)
{
	return frame.registers.edi;
}