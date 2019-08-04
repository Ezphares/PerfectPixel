#include <EntityComponentSystem/Query.h>

namespace perfectpixel
{
	namespace ecs {

		Query::Query(QueryFunction queryFunction)
			: m_lastResult()
			, m_queryFunction(queryFunction)
		{}

		void Query::applyMask()
		{
			if (m_queryFunction != nullptr)
			{
				m_queryFunction(m_lastResult);

			}
		}

		void Query::executeMaskOnly()
		{
			executeMaskOnly(EntityManager::getInstance()->all());
		}

		void Query::executeMaskOnly(const bedrock::BitSet &start)
		{
			m_lastResult = start;
			applyMask();
		}

		std::vector<Entity> Query::execute(EntityManager::EntityFunc callback)
		{
			executeMaskOnly();
			return finalize(callback);
		}

		std::vector<Entity> Query::execute(const bedrock::BitSet &start, EntityManager::EntityFunc callback)
		{
			executeMaskOnly(start);
			return finalize(callback);		
		}

		std::vector<Entity> Query::finalize(EntityManager::EntityFunc callback /*= 0*/)
		{
			std::vector<Entity> result;
			EntityManager::getInstance()->expandMask(m_lastResult, &result, callback);
			return result;
		}

		const perfectpixel::bedrock::BitSet & Query::getLastResult() const
		{
			return m_lastResult;
		}

	}
}
