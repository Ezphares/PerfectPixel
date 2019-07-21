#pragma once

#include <EntityComponentSystem/QueryHelper.h>

#include <Bedrock/numbers.h>

namespace perfectpixel { namespace ecs {

		class Processor
		{
		public:
			enum QueryFlags
			{
				QF_CORE =		0x0001,
				QF_CREATE =		0x0002,
				QF_DESTROY =	0x0004,
				QF_RENDER =		0x0008,

				QF_ALL =		QF_CORE | QF_CREATE | QF_DESTROY | QF_RENDER,
			};

			Processor(Query query);
			virtual ~Processor();

		public:
			void doQuery(int flags = QF_CORE);
			void doCreate();
			void doProcess(float deltaT);
			void doRender(float deltaT);
			void doDestroy();

			virtual void onCreate(const std::vector<Entity> &entities);
			virtual void onUpdate(const std::vector<Entity> &entities, float deltaT);
			virtual void onRender(const std::vector<Entity> &entities, float deltaT);
			virtual void onDestroy(const std::vector<Entity> &entities);

		protected:
			Query m_query;
			Query m_queryCreate;
			Query m_queryDestroy;
			Query m_queryRender;

			bedrock::BitSet m_queryState;
		}; 

} }
