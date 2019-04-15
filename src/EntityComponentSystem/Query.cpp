#include <EntityComponentSystem/Query.h>

#include <EntityComponentSystem/ComponentRegistry.h>

namespace perfectpixel
{
	namespace ecs {

		Query::Query()
			: m_with()
			, m_without()
			, m_lastResult()
		{}

		Query & Query::with(ComponentTypeId id)
		{
			m_with.push_back(id);
			return *this;
		}

		Query & Query::without(ComponentTypeId id)
		{
			m_without.push_back(id);
			return *this;
		}

		void Query::applyMask()
		{
			ComponentRegistry *registry = ComponentRegistry::Instance();

			for (auto id : m_with)
			{
				m_lastResult &= registry->getStorage(id)->getMask(id, m_lastResult);
			}

			for (auto id : m_without)
			{
				m_lastResult &= ~registry->getStorage(id)->getMask(id, m_lastResult);
			}
		}

		void Query::executeMaskOnly()
		{
			ComponentRegistry *registry = ComponentRegistry::Instance();

			executeMaskOnly(registry->getEntityManager()->all());
		}

		void Query::executeMaskOnly(const types::BitSet &start)
		{
			m_lastResult = start;
			applyMask();
		}

		std::vector<Entity> Query::execute(EntityManager::EntityFunc callback)
		{
			executeMaskOnly();
			return finalize(callback);
		}

		std::vector<Entity> Query::execute(const types::BitSet &start, EntityManager::EntityFunc callback)
		{
			executeMaskOnly(start);
			return finalize(callback);		
		}

		std::vector<Entity> Query::finalize(EntityManager::EntityFunc callback /*= 0*/)
		{
			ComponentRegistry *registry = ComponentRegistry::Instance();

			std::vector<Entity> result;
			registry->getEntityManager()->expandMask(m_lastResult, &result, callback);
			return result;
		}

		const perfectpixel::types::BitSet & Query::getLastResult() const
		{
			return m_lastResult;
		}

	}
}
