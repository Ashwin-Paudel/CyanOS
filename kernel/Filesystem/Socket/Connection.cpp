#include "Connection.h"

Connection::Connection(const StringView& name) :
    FSNode{name, 0, 0, NodeType::Connection, 0},
    m_server_buffer{BUFFER_SIZE},
    m_client_buffer{BUFFER_SIZE},
    m_server_wait_queue{},
    m_client_wait_queue{},
    m_server_lock{},
    m_client_lock{}
{
}

Connection::~Connection()
{
}

Result<void> Connection::open(FileDescription&)
{
	return ResultError(ERROR_SUCCESS);
}

Result<void> Connection::read(FileDescription& desc, void* buff, size_t offset, size_t size)
{
	if (desc.m_mode == OM_SERVER) {
		ScopedLock local_lock(m_server_lock);

		while (m_server_buffer.size() < size) {
			local_lock.release();
			Thread::current->wait_on(m_server_wait_queue);
			local_lock.acquire();
		}

		char* _buf = static_cast<char*>(buff);
		for (size_t i = 0; i < size; i++) {
			_buf[i] = m_server_buffer.dequeue();
		}

		m_server_wait_queue.wake_up_all();
	} else if (desc.m_mode == OM_CLIENT) {
		ScopedLock local_lock(m_client_lock);

		while (m_client_buffer.size() < size) {
			local_lock.release();
			Thread::current->wait_on(m_client_wait_queue);
			local_lock.acquire();
		}

		char* _buf = static_cast<char*>(buff);
		for (size_t i = 0; i < size; i++) {
			_buf[i] = m_client_buffer.dequeue();
		}

		m_client_wait_queue.wake_up_all();
	} else {
		ASSERT_NOT_REACHABLE();
	}
	return ResultError(ERROR_SUCCESS);
}

Result<void> Connection::write(FileDescription& desc, const void* buff, size_t offset, size_t size)
{
	if (desc.m_mode == OM_SERVER) {
		ScopedLock local_lock(m_client_lock);

		if (m_client_buffer.available_size() < size) {
			local_lock.release();
			Thread::current->wait_on(m_client_wait_queue);
			local_lock.acquire();
		}

		const char* _buf = static_cast<const char*>(buff);
		for (size_t i = 0; i < size; i++) {
			m_client_buffer.queue(_buf[i]);
		}

		m_client_wait_queue.wake_up();
	} else if (desc.m_mode == OM_CLIENT) {
		ScopedLock local_lock(m_server_lock);

		if (m_server_buffer.available_size() < size) {
			local_lock.release();
			Thread::current->wait_on(m_server_wait_queue);
			local_lock.acquire();
		}

		const char* _buf = static_cast<const char*>(buff);
		for (size_t i = 0; i < size; i++) {
			m_server_buffer.queue(_buf[i]);
		}

		m_server_wait_queue.wake_up();
	} else {
		ASSERT_NOT_REACHABLE();
	}
	return ResultError(ERROR_SUCCESS);
}

Result<bool> Connection::can_read(FileDescription&)
{
	return ResultError(ERROR_INVALID_OPERATION);
}

Result<bool> Connection::can_write(FileDescription&)
{
	return ResultError(ERROR_INVALID_OPERATION);
}

Result<void> Connection::close(FileDescription&)
{
	return ResultError(ERROR_SUCCESS);
}
