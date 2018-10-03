#pragma once

#include <types/numbers.h>

#include <worldgraph/Entity.h>

namespace perfectpixel {
	namespace behaviour {

		class BehaviourManager;

class Behaviour
{
public:
	friend class BehaviourManager;

	enum LifeCycle {
		LC_NEW,
		LC_ACTIVE,
		LC_DEAD
	};

	Behaviour(world::Entity entity)
		: m_entity(entity)
		, m_state(LC_NEW)
	{}

	virtual ~Behaviour()
	{}

public:
	virtual void onCreate() {}
	virtual void onUpdate(types::PpFloat deltaT) {}
	virtual void onDestroy() {};

	inline world::Entity getEntity()
	{
		return m_entity;
	}

private:
	world::Entity m_entity;
	LifeCycle m_state;

private:
	bool m_createFinished;
	bool m_destroyRequested;
};





} }

