#pragma once

#include "ecs/QueryHelper.h"
#include "ecs/ISystem.h"

#include "bedrock/numbers.h"

namespace perfectpixel { namespace ecs {

class QuerySystem : public ISystem
{
public:
    typedef EntityManager::EntityList::iterator RangeLimit;
    typedef void (*SystemFunction)(
        const RangeLimit &begin, const RangeLimit &end, float deltaT);
    typedef void (*UntimedSystemFunction)(
        const RangeLimit &begin, const RangeLimit &end);

    enum QueryFlags
    {
        QF_CORE    = 0x0001,
        QF_CREATE  = 0x0002,
        QF_DESTROY = 0x0004,
        QF_RENDER  = 0x0008,

        QF_ALL = QF_CORE | QF_CREATE | QF_DESTROY | QF_RENDER,
    };

    QuerySystem(Query query);
    virtual ~QuerySystem() = default;

public:
    virtual void earlyAudit() override;
	virtual void lateAudit() override;
	virtual void init() override;
	virtual void update(float deltaT) override;
	virtual void clean() override;
    virtual void render(float deltaT) override;

public:
    void doQuery(int flags = QF_CORE);
    void doCreate();
    void doProcess(float deltaT);
    void doRender(float deltaT);
    void doDestroy();

protected:
    Query m_query;
    Query m_queryCreate;
    Query m_queryDestroy;
    Query m_queryRender;

    UntimedSystemFunction m_onCreate;
    UntimedSystemFunction m_onDestroy;
    SystemFunction m_onUpdate;
    SystemFunction m_onRender;

    bedrock::BitSet m_queryState;
};

}} // namespace perfectpixel::ecs
