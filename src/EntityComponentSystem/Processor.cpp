#include <EntityComponentSystem/Processor.h>
#include <EntityComponentSystem/EntityManager.h>
#include <EntityComponentSystem/LifecycleComponents.h>
#include <EntityComponentSystem/ComponentRegistry.h>
#include <EntityComponentSystem/TransformComponent.h>

#include <functional>

namespace perfectpixel {
	namespace ecs {

		Processor::Processor(Query query)
			: m_query(query)
			, m_queryCreate()
			, m_queryDestroy()
		{
			m_queryCreate.without<CreationDoneLifecycleComponent>();
			m_queryDestroy.with<DestroyedLifecycleComponent>();
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
				ComponentRegistry::Instance()->getStorage<CreationDoneLifecycleComponent>()->registerComponent(CreationDoneLifecycleComponent(entity));
			}
		}

		void Processor::doProcess(types::PpFloat deltaT)
		{
			onProcess(m_query.finalize(), deltaT);
		}

		void Processor::doRender(types::PpFloat deltaT)
		{
			// Do not render uninitialized components
			Query render;
			onRender(render.with<CreationDoneLifecycleComponent>().execute(m_queryState), deltaT);
		}

		void Processor::doDestroy()
		{
			std::vector<Entity> entities = m_queryDestroy.finalize();
			onDestroy(entities);
			for (Entity entity : entities)
			{
				ComponentRegistry::Instance()->getStorage<CreationDoneLifecycleComponent>()->removeComponent(entity);
				ComponentRegistry::Instance()->getStorage<DestroyedLifecycleComponent>()->removeComponent(entity);
				ComponentRegistry::Instance()->getEntityManager()->kill(entity);
			}
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

		typedef QueryHelper<With<TransformComponent>> IntegratorQuery;

		class IntegratorProcessor : public Processor
		{
			IntegratorProcessor()
				: Processor(IntegratorQuery::build())
			{
			}

			virtual void onProcess(const std::vector<Entity> &entities, types::PpFloat deltaT)
			{
				for (Entity e : entities)
				{
					auto helper{ IntegratorQuery::pack(e) };
					auto *trs = helper.get<TransformComponent>();
					trs->m_data.m_position += trs->m_data.m_velocity * deltaT;
				}
			}
		};

	}
}