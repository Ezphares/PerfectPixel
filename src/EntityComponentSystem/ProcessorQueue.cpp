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

void ProcessorQueue::processAll(types::PpFloat deltaT)
{
	for (types::PpInt priority : m_priorities)
	{
		for (Processor *processor : m_processors[priority])
		{
			processor->doQuery(Processor::QF_CORE | Processor::QF_CREATE);
		}
	}

	for (types::PpInt priority : m_priorities)
	{
		for (Processor *processor : m_processors[priority])
		{
			processor->doCreate();
		}
	}

	for (types::PpInt priority : m_priorities)
	{
		for (Processor *processor : m_processors[priority])
		{
			processor->doProcess(deltaT);
		}
	}

	for (types::PpInt priority : m_priorities)
	{
		for (Processor *processor : m_processors[priority])
		{
			processor->doQuery(Processor::QF_DESTROY);
		}
	}

	for (types::PpInt priority : m_priorities)
	{
		for (Processor *processor : m_processors[priority])
		{
			processor->doDestroy();
		}
	}

}

void ProcessorQueue::renderAll(types::PpFloat deltaT)
{
	for (types::PpInt priority : m_priorities)
	{
		for (Processor *processor : m_processors[priority])
		{
			processor->doRender(deltaT);
		}
	}
}

void ProcessorQueue::registerProcessor(Processor *processor, types::PpInt priority, bool managed)
{
	auto it = m_processors.find(priority);
	if (it == m_processors.end())
	{
		it = m_processors.insert(std::pair<types::PpInt, std::vector<Processor*>>(priority, std::vector<Processor*>())).first;
		m_priorities.push_back(priority);
	}

	it->second.push_back(processor);

	if (managed)
	{
		m_managedProcessors.push_back(processor);
	}
}

}
}
