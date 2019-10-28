#pragma once

#include <array>
#include <vector>

namespace perfectpixel { namespace graphics { namespace _internal {

class UIShadow
{
public:
    UIShadow();

    enum class UIShadowState
    {
        UISS_INACTIVE = 0,

        // Generic states
        UISS_HOVER,
        UISS_HELD,

        UISS_REMEMBERED,

        UISS_CLICKED,
    };

    struct UIShadowStateEntry
    {
        int32_t m_id;
        UIShadowState m_state;
    };

    typedef std::vector<UIShadowStateEntry> UIShadowStateBuffer;

    std::array<UIShadowStateBuffer, 2> m_stateDoubleBuffer;
    uint8_t m_activeBuffer;
    // For code readability...
    uint8_t m_inactiveBuffer;

    void flip();
    UIShadowState getPreviousState(int32_t id) const;
    void setState(int32_t id, UIShadowState state);
};

}}} // namespace perfectpixel::graphics::_internal
