#include "Keyboard.h"
#include "Arch/x86/asm.h"
#include "utils/ErrorCodes.h"

Keyboard* Keyboard::current_instance = nullptr;

Keyboard::Keyboard() :
    DeviceNode{"keyboard", DeviceNode::Type::Keyboard},
    lock{},
    m_wait_queue{},
    m_buffer{1024},
    pressed_keys{false, false, false}
{
	ASSERT(current_instance == nullptr);
	current_instance = this;
	PIC::enable_irq(PIC_KEYBOARD);
	ISR::register_isr_handler(keyboard_driver_handler, PIC_KEYBOARD + PIC1_IDT_OFFSET);
}

Keyboard::~Keyboard()
{
}

Result<void> Keyboard::receive(void* buffer, size_t count)
{
	if (m_buffer.size() < count) {
		Thread::current->wait_on(m_wait_queue);
	}
	char* _buf = static_cast<char*>(buffer);
	for (size_t i = 0; i < count; i++) {
		_buf[i] = m_buffer.dequeue();
	}
	m_wait_queue.wake_up_all();

	return ResultError(ERROR_SUCCESS);
}

Result<void> Keyboard::send(void* buffer, size_t count)
{
	UNUSED(buffer);
	UNUSED(count);
	return ResultError(ERROR_INVALID_OPERATION);
}

Result<void> Keyboard::can_read()
{
	return ResultError(ERROR_INVALID_OPERATION);
}

Result<void> Keyboard::can_write()
{
	return ResultError(ERROR_INVALID_OPERATION);
}

Result<void> Keyboard::control()
{
	return ResultError(ERROR_INVALID_OPERATION);
}

void Keyboard::enqueue_keystoke(unsigned char data)
{
	if (data == 0x2A) // SHIFT Pressed
	{
		pressed_keys[0] = 1;
	} else if (data == 0xAA) { // SHIFT Released
		pressed_keys[0] = 0;
	} else if ((data & 0x80) == 0) {
		if (m_buffer.is_full()) {
			m_buffer.dequeue(); // memory is full get rid of old keyboard strokes.
		}
		m_buffer.queue(asccode[data][pressed_keys[0]]);
		m_wait_queue.wake_up_all();
	}
}

void Keyboard::keyboard_driver_handler(ISRContextFrame* frame)
{
	UNUSED(frame);
	current_instance->enqueue_keystoke(in8(KBD_DATA_PORT));
	PIC::acknowledge_pic(PIC_PIT);
}