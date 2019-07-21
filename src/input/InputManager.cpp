#include <input/InputManager.h>

#include <Bedrock/PpException.h>

#include <functional>

namespace perfectpixel {
	namespace input {

		InputManager::InputManager()
			: m_buttons()
			, m_axes()
			, m_buttonBindings()
		{
			registerButton("NOBUTTON");
			registerButton("NOAXIS");
		}


		InputManager::~InputManager()
		{
		}

		void InputManager::update()
		{
			m_buttonStatePrev = m_buttonState;
			m_axisStatePrev = m_axisState;
		}

		perfectpixel::bedrock::KeyCallback InputManager::getKeyCallback()
		{
			return std::bind(&InputManager::handleInput, this, std::placeholders::_1, std::placeholders::_2);
		}

		void InputManager::handleInput(bedrock::KeyCode keyCode, bedrock::KeyEvent keyEvent)
		{
			auto binding = m_buttonBindings.find(keyCode);
			if (binding != m_buttonBindings.end())
			{
				int32_t id = binding->second;

				bool
					target = keyEvent == bedrock::KeyEvent::PP_KEYDOWN,
					current = m_buttonState[id];

				if (target != current)
				{
					// FIXME generate event
				}

				m_buttonState[id] = target;

				// Button axis updates
				for (auto &it : m_buttonAxisBindings)
				{
					const std::pair<int32_t, int32_t> &axisButtons = it.first;
					if (id == axisButtons.first || id == axisButtons.second)
					{
						m_axisState[it.second] =
							static_cast<float>(isButtonDown(axisButtons.second)) -
							static_cast<float>(isButtonDown(axisButtons.first));
					}
				}
			}
		}

		int32_t InputManager::registerButton(const std::string &name)
		{
			// FIXME check already existing
			int32_t id = m_buttons.size();

			m_buttons.push_back(name);

			m_buttonState[id] = false;
			m_buttonStatePrev[id] = false;

			return id;
		}

		int32_t InputManager::registerAxis(const std::string &name)
		{
			// FIXME check already existing
			int32_t id = m_axes.size();

			m_axes.push_back(name);

			m_axisState[id] = 0.0f;
			m_axisStatePrev[id] = 0.0f;

			return id;
		}

		int32_t InputManager::lookupButton(const std::string &name) const
		{
			for (int32_t i = 0; i < static_cast<int32_t>(m_buttons.size()); ++i)
			{
				if (m_buttons[i] == name)
				{
					return i;
				}
			}
			throw bedrock::PpException("Unrecognized button");
		}

		int32_t InputManager::lookupAxis(const std::string &name) const
		{
			for (int32_t i = 0; i < static_cast<int32_t>(m_axes.size()); ++i)
			{
				if (m_axes[i] == name)
				{
					return i;
				}
			}

			throw bedrock::PpException("Unrecognized axis");
		}

		void InputManager::bindButton(const std::string &name, bedrock::KeyCode keyCode)
		{
			int32_t buttonId = lookupButton(name);

			for (int32_t i = 0; i < static_cast<int32_t>(m_buttons.size()); ++i)
			{
				if (m_buttons[i] == name)
				{
					buttonId = i;
					break;
				}
			}

			if (buttonId == 0)
			{
				throw bedrock::PpException("Unrecognized button");
			}


			for (auto it = m_buttonBindings.begin(); it != m_buttonBindings.end(); ++it)
			{
				if (it->first == keyCode || it->second == buttonId)
				{
					it = m_buttonBindings.erase(it);
				}
			}

			m_buttonBindings[keyCode] = buttonId;
		}

		void InputManager::bindAxisToButtons(const std::string &axisName, const std::string &negativeButton, const std::string &positiveButton)
		{
			int32_t axisId = lookupAxis(axisName);
			int32_t negativeId = lookupButton(negativeButton);
			int32_t positiveId = lookupButton(positiveButton);

			std::pair<int32_t, int32_t> axisButtons{ negativeId, positiveId };
			m_buttonAxisBindings[axisButtons] = axisId;
		}

		void InputManager::clearState()
		{
			for (auto it = m_buttonState.begin(); it != m_buttonState.end(); ++it)
			{
				it->second = false;
			}

			for (auto it = m_axisState.begin(); it != m_axisState.end(); ++it)
			{
				it->second = 0.0f;
			}

			update();
		}

		bool InputManager::isButtonDown(const std::string &name)
		{
			return m_buttonState[lookupButton(name)];
		}

		bool InputManager::isButtonDown(int32_t id)
		{
			if (id >= static_cast<int32_t>(m_buttons.size()))
			{
				throw bedrock::PpException("Unregistered button ID");
			}

			return m_buttonState[id];
		}

		bool InputManager::wasButtonDownPrevious(const std::string &name)
		{
			return m_buttonStatePrev[lookupButton(name)];
		}

		bool InputManager::wasButtonDownPrevious(int32_t id)
		{
			if (id >= static_cast<int32_t>(m_buttons.size()))
			{
				throw bedrock::PpException("Unregistered button ID");
			}

			return m_buttonStatePrev[id];
		}

		bool InputManager::wasButtonPressed(const std::string &name)
		{
			return (isButtonDown(name) && !wasButtonDownPrevious(name));
		}

		bool InputManager::wasButtonPressed(int32_t id)
		{
			return (isButtonDown(id) && !wasButtonDownPrevious(id));
		}

		bool InputManager::wasButtonReleased(const std::string &name)
		{
			return (wasButtonDownPrevious(name) && !isButtonDown(name));
		}

		bool InputManager::wasButtonReleased(int32_t id)
		{
			return (wasButtonDownPrevious(id) && !isButtonDown(id));
		}

		float InputManager::getAxisState(const std::string &name)
		{
			return m_axisState[lookupAxis(name)];
		}

		float InputManager::getAxisState(int32_t id)
		{
			return m_axisState[id];
		}

	}
}
