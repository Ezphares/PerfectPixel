#pragma once

#include <EntityComponentSystem/QueryHelper.h>

namespace perfectpixel {
	namespace ecs {

		class Processor
		{
		public:
			Processor(Query query);
			virtual ~Processor();

		public:
			void doQuery();
			void doCreate();
			void doProcess();
			void doDestroy();

			virtual void onPreProcess();
			virtual void onCreate(const std::vector<Entity> &entities);
			virtual void onProcess(const std::vector<Entity> &entities) = 0;
			virtual void onDestroy(const std::vector<Entity> &entities);

		protected:
			Query m_query;

			types::BitSet m_queryState;
		};



	}
}