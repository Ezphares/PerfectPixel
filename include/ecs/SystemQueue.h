#pragma once

#include "ecs/ComponentStorageConcept.h"
#include "ecs/ISystem.h"

#include "bedrock/PriorityVector.h"
#include "bedrock/numbers.h"

#include <map>
#include <vector>

namespace perfectpixel { namespace ecs {

class SystemQueue
{
public:
    static const int16_t USER_PROCESSOR_DEFAULT_PRIORITY;

public:
    SystemQueue();
    virtual ~SystemQueue();

    void processAll(float deltaT);
    void renderAll(float deltaT);

    void registerSystem(ISystem *system, int16_t priority, bool managed);

private:
    bedrock::PriorityVector<ISystem *> m_systems;
    std::vector<ISystem *> m_managedProcessors;
};

}} // namespace perfectpixel::ecs
