
#include <graphics/UIProcessor.h>

#include <graphics/UIUtil.h>

namespace perfectpixel { namespace graphics {
	
	void UIProcessor::onRender(const std::vector<ecs::Entity> &entities, types::PpFloat deltaT)
	{
		if (!m_font) m_font = m_gm->getDefaultFont();

		for (auto entity : entities)
		{
			if (UITextComponent::Has(entity))
			{
				UITextDrawQueueElement *element =
					static_cast<UITextDrawQueueElement *>(DrawQueueElementFactory::getInstance()->allocate(DQET_UI_TEXT));

				element->m_font = m_font;
				element->m_position = ecs::TransformComponent::Position(entity);
				element->m_text = UITextComponent::Text(entity);
				element->m_fontSize = 20.0f;

				types::PpFloat width = element->m_font->getWidth(element->m_fontSize, element->m_text);
				types::PpInt alignment = UITextComponent::Alignment(entity);
				if ((alignment & UIUtil::UITA_CENTER) > 0)
				{
					element->m_position.x() -= width * 0.5f;
				}
				else if ((alignment & UIUtil::UITA_RIGHT) > 0)
				{
					element->m_position.x() -= width;
				}

				m_gm->queueDrawSingle(element);
			}
		}
	}


	IFont *UIProcessor::m_font = nullptr;
	GraphicsManager *UIProcessor::m_gm = nullptr;




} }
