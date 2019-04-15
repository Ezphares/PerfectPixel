#include "managerinterface/Query.h"

#include "EntityComponentSystem/ComponentRegistry.h"

namespace perfectpixel
{
	namespace api {

		Query::Query()
			: m_with()
			, m_without()
			, m_lastResult()
		{}

		Query & Query::with(world::ComponentTypeId id)
		{
			m_with.push_back(id);
			return *this;
		}

		Query & Query::without(world::ComponentTypeId id)
		{
			m_without.push_back(id);
			return *this;
		}

		void Query::applyMask()
		{
			world::ComponentRegistry *registry = world::ComponentRegistry::Instance();

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
			world::ComponentRegistry *registry = world::ComponentRegistry::Instance();

			executeMaskOnly(registry->getEntityManager()->all());
		}

		void Query::executeMaskOnly(const types::BitSet &start)
		{
			m_lastResult = start;
			applyMask();
		}

		std::vector<perfectpixel::world::Entity> Query::execute(world::EntityManager::EntityFunc callback)
		{
			executeMaskOnly();
			return finalize(callback);
		}

		std::vector<perfectpixel::world::Entity> Query::execute(const types::BitSet &start, world::EntityManager::EntityFunc callback)
		{
			executeMaskOnly(start);
			return finalize(callback);		
		}

		std::vector<perfectpixel::world::Entity> Query::finalize(world::EntityManager::EntityFunc callback /*= 0*/)
		{
			world::ComponentRegistry *registry = world::ComponentRegistry::Instance();

			std::vector<world::Entity> result;
			registry->getEntityManager()->expandMask(m_lastResult, &result, callback);
			return result;
		}

		const perfectpixel::types::BitSet & Query::getLastResult() const
		{
			return m_lastResult;
		}

	}
}
