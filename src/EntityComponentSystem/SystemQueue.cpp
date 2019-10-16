#include <EntityComponentSystem/SystemQueue.h>

namespace perfectpixel { namespace ecs {

const int16_t SystemQueue::USER_PROCESSOR_DEFAULT_PRIORITY = 512u;

SystemQueue::SystemQueue()
	: m_systems()
	, m_managedProcessors()
{
}

SystemQueue::~SystemQueue()
{
	for (System *processor : m_managedProcessors)
	{
		delete processor;
	}
}

void SystemQueue::processAll(float deltaT)
{
	for (System *system : m_systems)
	{
            system->doQuery(System::QF_CORE | System::QF_CREATE);
	}

	for (System *system : m_systems)
    {
            system->doCreate();
	}

	for (System *system : m_systems)
    {
        system->doProcess(deltaT);
	}

	for (System *system : m_systems)
    {
        system->doQuery(System::QF_DESTROY);
	}

	for (auto it = m_systems.rbegin(); it != m_systems.rend(); ++it)
	{
		(*it)->doDestroy();
	}
}

void SystemQueue::renderAll(float deltaT)
{
    for (System *system : m_systems)
    {
		system->doRender(deltaT);
	}
}

void SystemQueue::registerSystem(System *system, int16_t priority, bool managed)
{
    m_systems.insert(priority, system);

	if (managed)
	{
		m_managedProcessors.push_back(system);
	}
}

}
}
