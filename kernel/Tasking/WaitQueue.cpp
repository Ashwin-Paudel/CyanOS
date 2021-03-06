#include "WaitQueue.h"
#include "ScopedLock.h"
#include "Thread.h"

WaitQueue::WaitQueue() : m_lock{}, m_threads{} {}

WaitQueue::~WaitQueue()
{
	wake_up_all(); // FIXME: is it okay to do so ?
}

void WaitQueue::terminate_blocked_thread(Thread& thread)
{
	ScopedLock local_lock(m_lock);

	m_threads.remove(thread);
}

void WaitQueue::wake_up(size_t num)
{
	ScopedLock local_lock(m_lock);

	while ((num--) && (!m_threads.is_empty())) {
		wake_up_one();
	}
}

void WaitQueue::wake_up_all()
{
	ScopedLock local_lock(m_lock);

	while (!m_threads.is_empty()) {
		wake_up_one();
	}
}

void WaitQueue::wake_up_one()
{
	if (!m_threads.size()) {
		return;
	}
	auto& thread_to_wake = m_threads.pop_front();
	thread_to_wake.wake_up_from_queue();
}