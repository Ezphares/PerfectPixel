#pragma once

#include <types/numbers.h>

#include <EntityComponentSystem/Entity.h>

#include <managerinterface/ManagerInterface.h>

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

	Behaviour(ecs::Entity entity)
		: m_entity(entity)
		, m_state(LC_NEW)
		, m_managerInterface(nullptr)
		, m_createFinished(false)
		, m_destroyRequested(false)
	{}

	virtual ~Behaviour()
	{}

public:
	virtual void onCreate() {}
	virtual void onUpdate(types::PpFloat deltaT) {}
	virtual void onDestroy() {};

	inline ecs::Entity getEntity()
	{
		return m_entity;
	}

	inline api::ManagerInterface *pp()
	{
		return m_managerInterface;
	}

private:
	ecs::Entity m_entity;
	LifeCycle m_state;
	api::ManagerInterface *m_managerInterface;

private:
	bool m_createFinished;
	bool m_destroyRequested;
};





} }

