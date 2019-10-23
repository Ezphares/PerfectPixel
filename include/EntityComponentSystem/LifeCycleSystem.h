#pragma once

#include <EntityComponentSystem/ISystem.h>

namespace perfectpixel { namespace ecs {

	class LifeCycleSystem : public ISystem
	{
        public:
            virtual void earlyAudit() override;
            virtual void lateAudit() override;
            virtual void init() override;
            virtual void update(float deltaT) override;
            virtual void clean() override;
            virtual void render(float deltaT) override;
        };



} }
