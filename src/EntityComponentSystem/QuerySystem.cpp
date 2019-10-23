#include <EntityComponentSystem/EntityManager.h>
#include <EntityComponentSystem/LifecycleComponents.h>
#include <EntityComponentSystem/QuerySystem.h>
#include <EntityComponentSystem/TransformComponent.h>
#include <EntityComponentSystem/FastUpdate.h>

#include <functional>

namespace perfectpixel { namespace ecs {

namespace {
typedef QueryHelper<With<>, Without<CreationDoneLifecycleComponent>>
    QueryHelperCreate;
typedef QueryHelper<With<DestroyedLifecycleComponent>> QueryHelperDestroy;
typedef QueryHelper<With<CreationDoneLifecycleComponent>> QueryHelperRender;
} // namespace

QuerySystem::QuerySystem(Query query)
    : m_query(query)
    , m_queryCreate(QueryHelperCreate::build())
    , m_queryDestroy(QueryHelperDestroy::build())
    , m_queryRender(QueryHelperRender::build())
	, m_onCreate(nullptr)
	, m_onDestroy(nullptr)
	, m_onUpdate(nullptr)
	, m_onRender(nullptr)
{}

void QuerySystem::earlyAudit()
{
    doQuery(QF_CORE | QF_CREATE);
}

void QuerySystem::lateAudit()
{
    doQuery(QF_DESTROY);
}

void QuerySystem::init()
{
    doCreate();
}

void QuerySystem::update(float deltaT)
{
    doProcess(deltaT);
}

void QuerySystem::clean()
{
    doDestroy();
}

void QuerySystem::render(float deltaT)
{
    doRender(deltaT);
}

void QuerySystem::doQuery(int flags)
{
    if ((flags & QF_CORE) > 0)
    {
        m_query.executeMaskOnly();
        m_queryState = m_query.getLastResult();
    }

    if (m_onCreate && (flags & QF_CREATE) > 0)
    {
        m_queryCreate.executeMaskOnly(m_queryState);
    }

    if (m_onDestroy && (flags & QF_DESTROY) > 0)
    {
        m_queryDestroy.executeMaskOnly(m_queryState);
    }
}

void QuerySystem::doCreate()
{
    if (m_onCreate)
    {
        std::vector<Entity> entities = m_queryCreate.finalize();
        m_onCreate(entities.begin(), entities.end());
    }
}

void QuerySystem::doProcess(float deltaT)
{
    if (m_onUpdate)
    {
        EntityManager::EntityList result = m_query.finalize();
        m_onUpdate(result.begin(), result.end(), deltaT);
    }
}

void QuerySystem::doRender(float deltaT)
{
	if (m_onRender)
	{
		EntityManager::EntityList result = m_queryRender.execute(m_queryState);
        m_onRender(result.begin(), result.end(), deltaT);
	}
}

void QuerySystem::doDestroy()
{
	if (m_onDestroy)
	{
		EntityManager::EntityList result = m_queryDestroy.finalize();
        m_onDestroy(result.begin(), result.end());
	}
}
}} // namespace perfectpixel::ecs
