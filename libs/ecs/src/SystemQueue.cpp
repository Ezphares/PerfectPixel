#include "ecs/SystemQueue.h"

namespace perfectpixel { namespace ecs {

const int16_t SystemQueue::USER_PROCESSOR_DEFAULT_PRIORITY = 512u;

SystemQueue::SystemQueue()
	: m_systems()
	, m_managedProcessors()
{
}

SystemQueue::~SystemQueue()
{
	for (ISystem *processor : m_managedProcessors)
	{
		delete processor;
	}
}

void SystemQueue::processAll(float deltaT)
{
	for (ISystem *system : m_systems)
	{
        system->earlyAudit();
	}

	for (ISystem *system : m_systems)
    {
        system->init();
	}

	for (ISystem *system : m_systems)
    {
        system->update(deltaT);
	}

	for (ISystem *system : m_systems)
    {
        system->lateAudit();
	}

	for (auto it = m_systems.rbegin(); it != m_systems.rend(); ++it)
	{
		(*it)->clean();
	}
}

void SystemQueue::renderAll(float deltaT)
{
    for (ISystem *system : m_systems)
    {
		system->render(deltaT);
	}
}

void SystemQueue::registerSystem(ISystem *system, int16_t priority, bool managed)
{
    m_systems.insert(priority, system);

	if (managed)
	{
		m_managedProcessors.push_back(system);
	}
}

}
}
