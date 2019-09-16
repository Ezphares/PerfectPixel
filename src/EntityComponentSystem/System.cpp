#include <EntityComponentSystem/EntityManager.h>
#include <EntityComponentSystem/LifecycleComponents.h>
#include <EntityComponentSystem/System.h>
#include <EntityComponentSystem/TransformComponent.h>

#include <functional>

namespace perfectpixel { namespace ecs {

namespace {
typedef QueryHelper<With<>, Without<CreationDoneLifecycleComponent>>
    QueryHelperCreate;
typedef QueryHelper<With<DestroyedLifecycleComponent>> QueryHelperDestroy;
typedef QueryHelper<With<CreationDoneLifecycleComponent>> QueryHelperRender;
} // namespace

System::System(Query query)
    : m_query(query)
    , m_queryCreate(QueryHelperCreate::build())
    , m_queryDestroy(QueryHelperDestroy::build())
    , m_queryRender(QueryHelperRender::build())
	, m_onCreate(nullptr)
	, m_onDestroy(nullptr)
	, m_onUpdate(nullptr)
	, m_onRender(nullptr)
{}

System::~System() {}

void System::doQuery(int flags)
{
    if ((flags & QF_CORE) > 0)
    {
        m_query.executeMaskOnly();
        m_queryState = m_query.getLastResult();
    }

	// FIXME Comment these back in when creation and destruction is handled by their own system
    if (/*m_onCreate &&*/ (flags & QF_CREATE) > 0)
    {
        m_queryCreate.executeMaskOnly(m_queryState);
    }

    if (/*m_onDestroy &&*/ (flags & QF_DESTROY) > 0)
    {
        m_queryDestroy.executeMaskOnly(m_queryState);
    }
}

void System::doCreate()
{
    std::vector<Entity> entities = m_queryCreate.finalize();
    if (m_onCreate)
    {
        m_onCreate(entities.begin(), entities.end());
    }

	// TODO: Move this to a specific system
    for (Entity entity : entities)
    {
        CreationDoneLifecycleComponent::Register(entity);
    }
}

void System::doProcess(float deltaT)
{
    if (m_onUpdate)
    {
        EntityManager::EntityList result = m_query.finalize();
        m_onUpdate(result.begin(), result.end(), deltaT);
    }
}

void System::doRender(float deltaT)
{
	if (m_onRender)
	{
		EntityManager::EntityList result = m_queryRender.execute(m_queryState);
        m_onRender(result.begin(), result.end(), deltaT);
	}
}

void System::doDestroy()
{
	if (m_onDestroy)
	{
		EntityManager::EntityList result = m_queryDestroy.finalize();
        m_onDestroy(result.begin(), result.end());
	}
}
}} // namespace perfectpixel::ecs
