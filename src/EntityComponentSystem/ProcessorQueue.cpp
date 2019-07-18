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

void ProcessorQueue::processAll(bedrock::PpFloat deltaT)
{
	for (bedrock::PpInt priority : m_priorities)
	{
		for (Processor *processor : m_processors[priority])
		{
			processor->doQuery(Processor::QF_CORE | Processor::QF_CREATE);
		}
	}

	for (bedrock::PpInt priority : m_priorities)
	{
		for (Processor *processor : m_processors[priority])
		{
			processor->doCreate();
		}
	}

	for (bedrock::PpInt priority : m_priorities)
	{
		for (Processor *processor : m_processors[priority])
		{
			processor->doProcess(deltaT);
		}
	}

	for (bedrock::PpInt priority : m_priorities)
	{
		for (Processor *processor : m_processors[priority])
		{
			processor->doQuery(Processor::QF_DESTROY);
		}
	}

	for (bedrock::PpInt priority : m_priorities)
	{
		for (Processor *processor : m_processors[priority])
		{
			processor->doDestroy();
		}
	}

}

void ProcessorQueue::renderAll(bedrock::PpFloat deltaT)
{
	for (bedrock::PpInt priority : m_priorities)
	{
		for (Processor *processor : m_processors[priority])
		{
			processor->doRender(deltaT);
		}
	}
}

void ProcessorQueue::registerProcessor(Processor *processor, bedrock::PpInt priority, bool managed)
{
	auto it = m_processors.find(priority);
	if (it == m_processors.end())
	{
		it = m_processors.insert(std::pair<bedrock::PpInt, std::vector<Processor*>>(priority, std::vector<Processor*>())).first;
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
