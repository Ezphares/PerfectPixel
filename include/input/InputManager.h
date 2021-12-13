#pragma once

#include <Bedrock/InputCallback.h>
#include <Bedrock/numbers.h>

#include <map>
#include <set>
#include <string>
#include <vector>

namespace perfectpixel { namespace input {

class InputManager
{
public:
    InputManager();
    ~InputManager();

public:
    void update();

    bedrock::KeyCallback getKeyCallback();
    static void handleInputWrapper(
        void *instance, bedrock::KeyCode keyCode, bedrock::KeyEvent keyEvent);
    void handleInput(bedrock::KeyCode keyCode, bedrock::KeyEvent keyEvent);

    int32_t registerButton(const std::string &name);
    int32_t registerAxis(const std::string &name);
    int32_t lookupButton(const std::string &name) const;
    int32_t lookupAxis(const std::string &name) const;

    void bindButton(const std::string &name, bedrock::KeyCode keyCode);

    void bindAxisToButtons(
        const std::string &axisName,
        const std::string &negativeButton,
        const std::string &positiveButton);

    void clearState();

    bool isButtonDown(const std::string &name);
    bool isButtonDown(int32_t id);
    bool wasButtonDownPrevious(const std::string &name);
    bool wasButtonDownPrevious(int32_t id);
    bool wasButtonPressed(const std::string &name);
    bool wasButtonPressed(int32_t id);
    bool wasButtonReleased(const std::string &name);
    bool wasButtonReleased(int32_t id);

    float getAxisState(const std::string &name);
    float getAxisState(int32_t id);

private:
    std::vector<std::string> m_buttons;
    std::vector<std::string> m_axes;

    std::map<bedrock::KeyCode, int32_t> m_buttonBindings;

    std::map<int32_t, bool> m_buttonState;
    std::map<int32_t, bool> m_buttonStatePrev;

    std::map<int32_t, float> m_axisState;
    std::map<int32_t, float> m_axisStatePrev;

    std::map<std::pair<int32_t, int32_t>, int32_t> m_buttonAxisBindings;
};

}} // namespace perfectpixel::input
