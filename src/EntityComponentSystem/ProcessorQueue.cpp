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
		for (Processor *processor : m_managedProcessors)
		{
			delete processor;
		}
	}

void ProcessorQueue::processAll(float deltaT)
{
	for (int32_t priority : m_priorities)
	{
		for (Processor *processor : m_processors[priority])
		{
			processor->doQuery(Processor::QF_CORE | Processor::QF_CREATE);
		}
	}

	for (int32_t priority : m_priorities)
	{
		for (Processor *processor : m_processors[priority])
		{
			processor->doCreate();
		}
	}

	for (int32_t priority : m_priorities)
	{
		for (Processor *processor : m_processors[priority])
		{
			processor->doProcess(deltaT);
		}
	}

	for (int32_t priority : m_priorities)
	{
		for (Processor *processor : m_processors[priority])
		{
			processor->doQuery(Processor::QF_DESTROY);
		}
	}

	for (int32_t priority : m_priorities)
	{
		for (Processor *processor : m_processors[priority])
		{
			processor->doDestroy();
		}
	}

}

void ProcessorQueue::renderAll(float deltaT)
{
	for (int32_t priority : m_priorities)
	{
		for (Processor *processor : m_processors[priority])
		{
			processor->doRender(deltaT);
		}
	}
}

void ProcessorQueue::registerProcessor(Processor *processor, int32_t priority, bool managed)
{
	auto it = m_processors.find(priority);
	if (it == m_processors.end())
	{
		it = m_processors.insert(std::pair<int32_t, std::vector<Processor*>>(priority, std::vector<Processor*>())).first;
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
