#include <EntityComponentSystem/Processor.h>
#include <EntityComponentSystem/EntityManager.h>
#include <EntityComponentSystem/LifecycleComponents.h>
#include <EntityComponentSystem/TransformComponent.h>

#include <functional>

namespace perfectpixel { namespace ecs {

	namespace {
		typedef QueryHelper<With<>, Without<CreationDoneLifecycleComponent>> QueryHelperCreate;
		typedef QueryHelper<With<DestroyedLifecycleComponent>> QueryHelperDestroy;
		typedef QueryHelper<With<CreationDoneLifecycleComponent>> QueryHelperRender;
	}


	Processor::Processor(Query query)
		: m_query(query)
		, m_queryCreate(QueryHelperCreate::build())
		, m_queryDestroy(QueryHelperDestroy::build())
	{
	}

	Processor::~Processor()
	{
	}

	void Processor::doQuery(int flags)
	{
		if ((flags & QF_CORE) > 0)
		{
			m_query.executeMaskOnly();
			m_queryState = m_query.getLastResult();
		}

		if ((flags & QF_CREATE) > 0)
		{
			m_queryCreate.executeMaskOnly(m_queryState);
		}

		if ((flags & QF_DESTROY) > 0)
		{
			m_queryDestroy.executeMaskOnly(m_queryState);
		}
	}

	void Processor::doCreate()
	{
		std::vector<Entity> entities = m_queryCreate.finalize();
		onCreate(entities);
		for (Entity entity : entities)
		{
			CreationDoneLifecycleComponent::Register(entity);
		}
	}

	void Processor::doProcess(types::PpFloat deltaT)
	{
		onProcess(m_query.finalize(), deltaT);
	}

	void Processor::doRender(types::PpFloat deltaT)
	{
		// Do not render uninitialized components
		Query render = QueryHelperRender::build();
		onRender(render.execute(m_queryState), deltaT);
	}

	void Processor::doDestroy()
	{
		std::vector<Entity> entities = m_queryDestroy.finalize();
		onDestroy(entities);
	}

	void Processor::onCreate(const std::vector<Entity> &entities)
	{
	}


	void Processor::onProcess(const std::vector<Entity> &entities, types::PpFloat deltaT)
	{
	}

	void Processor::onRender(const std::vector<Entity> &entities, types::PpFloat deltaT)
	{
	}

	void Processor::onDestroy(const std::vector<Entity> &entities)
	{
	}
} }
