#pragma once

#include <graphics/UITextComponent.h>
#include <graphics/DrawQueue.h>
#include <graphics/CBFGFont.h>
#include <graphics/GraphicsManager.h>

#include <EntityComponentSystem/Processor.h>
#include <EntityComponentSystem/QueryHelper.h>
#include <EntityComponentSystem/TransformComponent.h>

// FIXME Very temporary

namespace perfectpixel { namespace graphics {

	class UIProcessor : public ecs::Processor
	{
		typedef ecs::QueryHelper< ecs::With< UITextComponent, ecs::TransformComponent > > UIQuery;
	public:
		UIProcessor() : Processor(UIQuery::build()) {}

		virtual void onRender(const std::vector<ecs::Entity> &entities, float deltaT);

		static IFont *m_font;
		static GraphicsManager *m_gm; 
	};





} }
