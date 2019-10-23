#pragma once

namespace perfectpixel { namespace ecs {

	class ISystem
	{
        public:
            virtual ~ISystem() = default;

			virtual void earlyAudit() = 0;
            virtual void lateAudit() = 0;

			virtual void init() = 0;
            virtual void update(float deltaT) = 0;
            virtual void clean() = 0;

			virtual void render(float deltaT) = 0;
	};


}}

