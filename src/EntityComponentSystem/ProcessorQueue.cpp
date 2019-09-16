#include <EntityComponentSystem/ProcessorQueue.h>

namespace perfectpixel { namespace ecs {

	ProcessorQueue::ProcessorQueue()
		: m_processors()
		, m_priorities()
		, m_managedProcessors()
	{
	}

	ProcessorQueue::~ProcessorQueue()
	{
		for (System *processor : m_managedProcessors)
		{
			delete processor;
		}
	}

void ProcessorQueue::processAll(float deltaT)
{
	for (int32_t priority : m_priorities)
	{
		for (System *processor : m_processors[priority])
		{
			processor->doQuery(System::QF_CORE | System::QF_CREATE);
		}
	}

	for (int32_t priority : m_priorities)
	{
		for (System *processor : m_processors[priority])
		{
			processor->doCreate();
		}
	}

	for (int32_t priority : m_priorities)
	{
		for (System *processor : m_processors[priority])
		{
			processor->doProcess(deltaT);
		}
	}

	for (int32_t priority : m_priorities)
	{
		for (System *processor : m_processors[priority])
		{
			processor->doQuery(System::QF_DESTROY);
		}
	}

	for (auto it = m_priorities.rbegin(); it != m_priorities.rend(); ++it)
	{
		for (auto jt = m_processors[*it].rbegin(); jt != m_processors[*it].rend(); ++jt)
		{
			(*jt)->doDestroy();
		}
	}

}

void ProcessorQueue::renderAll(float deltaT)
{
	for (int32_t priority : m_priorities)
	{
		for (System *processor : m_processors[priority])
		{
			processor->doRender(deltaT);
		}
	}
}

void ProcessorQueue::registerProcessor(System *processor, int32_t priority, bool managed)
{
	auto it = m_processors.find(priority);
	if (it == m_processors.end())
	{
		it = m_processors.insert(std::pair<int32_t, std::vector<System*>>(priority, std::vector<System*>())).first;
		m_priorities.push_back(priority);
		std::sort(m_priorities.begin(), m_priorities.end());
	}

	it->second.push_back(processor);

	if (managed)
	{
		m_managedProcessors.push_back(processor);
	}
}

}
}
