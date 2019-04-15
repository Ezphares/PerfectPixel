#include <EntityComponentSystem/Processor.h>
#include <EntityComponentSystem/EntityManager.h>
#include <EntityComponentSystem/LifecycleComponents.h>

#include <functional>

namespace perfectpixel {
	namespace ecs {

		Processor::Processor(Query query)
			: m_query(query)
		{
		}

		Processor::~Processor()
		{
		}

		void Processor::doQuery()
		{
			m_query.executeMaskOnly();
			m_queryState = m_query.getLastResult();
		}

		void Processor::doCreate()
		{
			onCreate(Query().with<CreatedLifecycleComponent>().execute(m_queryState));
		}

		void Processor::doProcess()
		{
			onPreProcess();
			onProcess(m_query.finalize());
		}

		void Processor::doDestroy()
		{
			onCreate(Query().with<DestroyedLifecycleComponent>().execute(m_queryState));
		}

		void Processor::onPreProcess()
		{
		}

		void Processor::onCreate(const std::vector<Entity> &entities)
		{
		}


		void Processor::onDestroy(const std::vector<Entity> &entities)
		{

		}

		typedef QueryHelper<With<>, Without<>> CollisionQuery;

		class CollisionProcessor : public Processor
		{
			CollisionProcessor()
				: Processor(CollisionQuery::build())
			{
			}

			virtual void onProcess(const std::vector<Entity> &entities)
			{
				for (Entity e : entities)
				{
					auto helper{ CollisionQuery::pack(e) };
				}
			}
		};

	}
}