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

	Behaviour(world::Entity entity)
		: m_entity(entity)
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

private:
	bool m_createFinished;
	bool m_destroyRequested;
};





} }

