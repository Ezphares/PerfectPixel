
#include "core/UI/UISystem.h"

#include "core/UI/UIUtil.h"

namespace perfectpixel::core {

void UISystem::onRender(std::span<ecs::Entity> entities, float deltaT)
{
    (void)deltaT;

    if (!m_font)
        m_font = m_gm->getDefaultFont();

    for (auto entity : entities)
    {
        if (UITextComponent::Has(entity))
        {
            renderer::UITextDrawQueueElement *element
                = static_cast<renderer::UITextDrawQueueElement *>(
                    renderer::DrawQueueElementFactory::getInstance()->allocate(
                        renderer::DQET_UI_TEXT));

            element->m_font     = m_font;
            element->m_position = ecs::TransformComponent::Position(entity);
            element->m_text     = UITextComponent::Text(entity);
            element->m_fontSize = 20.0f;

            float width = element->m_font->getWidth(
                element->m_fontSize, element->m_text);
            int32_t alignment = UITextComponent::Alignment(entity);
            if ((alignment & UIUtil::UITA_CENTER) > 0)
            {
                element->m_position.x -= width * 0.5f;
            }
            else if ((alignment & UIUtil::UITA_RIGHT) > 0)
            {
                element->m_position.x -= width;
            }

            m_gm->queueDrawSingle(element);
        }
    }
}

renderer::IFont *UISystem::m_font           = nullptr;
renderer::RendererInterface *UISystem::m_gm = nullptr;

} // namespace perfectpixel::core
