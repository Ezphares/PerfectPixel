#pragma once

#include <types/numbers.h>
#include <worldgraph/Entity.h>

namespace perfectpixel {
	namespace behaviour {

class Behaviour
{
public:
	Behaviour(world::Entity entity)
		: m_entity(entity)
	{}

	virtual ~Behaviour()
	{}

public:
	virtual void create() {}
	virtual void update(types::PpFloat deltaT) {}

private:
	world::Entity m_entity;

};






} }

