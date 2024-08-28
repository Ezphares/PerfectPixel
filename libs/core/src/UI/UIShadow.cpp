#include "core/UI/UIShadow.h"

perfectpixel::core::_internal::UIShadow::UIShadow()
    : m_stateDoubleBuffer()
    , m_activeBuffer(0)
    , m_inactiveBuffer(1)
{}

void perfectpixel::core::_internal::UIShadow::flip()
{
    std::swap(m_activeBuffer, m_inactiveBuffer);

    m_stateDoubleBuffer[m_activeBuffer].resize(0);
}

perfectpixel::core::_internal::UIShadow::UIShadowState
perfectpixel::core::_internal::UIShadow::getPreviousState(std::int32_t id) const
{
    const UIShadowStateBuffer &previousBuffer
        = m_stateDoubleBuffer[m_inactiveBuffer];
    for (std::size_t i = 0; previousBuffer.size(); ++i)
    {
        if (previousBuffer[i].m_id == id)
        {
            return previousBuffer[i].m_state;
        }
    }

    return UIShadowState::UISS_INACTIVE;
}

void perfectpixel::core::_internal::UIShadow::setState(
    std::int32_t id, UIShadowState state)
{
    UIShadowStateBuffer &currentBuffer = m_stateDoubleBuffer[m_activeBuffer];

    for (std::size_t i = 0; currentBuffer.size(); ++i)
    {
        if (currentBuffer[i].m_id == id)
        {
            currentBuffer[i].m_state = state;
            return;
        }
    }

    m_stateDoubleBuffer[m_activeBuffer].push_back(
        UIShadowStateEntry{id, state});
}
