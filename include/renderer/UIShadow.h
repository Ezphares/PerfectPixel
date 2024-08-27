#pragma once

#include <array>
#include <cstdint>
#include <vector>

namespace perfectpixel { namespace renderer { namespace _internal {

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
        std::int32_t m_id;
        UIShadowState m_state;
    };

    typedef std::vector<UIShadowStateEntry> UIShadowStateBuffer;

    std::array<UIShadowStateBuffer, 2> m_stateDoubleBuffer;
    std::uint8_t m_activeBuffer;
    // For code readability...
    std::uint8_t m_inactiveBuffer;

    void flip();
    UIShadowState getPreviousState(std::int32_t id) const;
    void setState(std::int32_t id, UIShadowState state);
};

}}} // namespace perfectpixel::renderer::_internal
