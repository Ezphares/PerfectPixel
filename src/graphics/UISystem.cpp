
#include <graphics/UISystem.h>

#include <graphics/UIUtil.h>

namespace perfectpixel { namespace graphics {

void UISystem::onRender(
    const RangeLimit &begin, const RangeLimit &end, float deltaT)
{
    (void)deltaT;

    if (!m_font)
        m_font = m_gm->getDefaultFont();

    for (auto it = begin; it != end; ++it)
    {
        ecs::Entity entity = *it;
        if (UITextComponent::Has(entity))
        {
            UITextDrawQueueElement *element
                = static_cast<UITextDrawQueueElement *>(
                    DrawQueueElementFactory::getInstance()->allocate(
                        DQET_UI_TEXT));

            element->m_font     = m_font;
            element->m_position = ecs::TransformComponent::Position(entity);
            element->m_text     = UITextComponent::Text(entity);
            element->m_fontSize = 20.0f;

            float width = element->m_font->getWidth(
                element->m_fontSize, element->m_text);
            int32_t alignment = UITextComponent::Alignment(entity);
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

IFont *UISystem::m_font         = nullptr;
GraphicsManager *UISystem::m_gm = nullptr;

}} // namespace perfectpixel::graphics
