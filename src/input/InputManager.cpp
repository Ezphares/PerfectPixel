#include <input/InputManager.h>

#include <types/PpException.h>

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

		perfectpixel::types::KeyCallback InputManager::getKeyCallback()
		{
			return std::bind(&InputManager::handleInput, this, std::placeholders::_1, std::placeholders::_2);
		}

		void InputManager::handleInput(types::KeyCode keyCode, types::KeyEvent keyEvent)
		{
			auto binding = m_buttonBindings.find(keyCode);
			if (binding != m_buttonBindings.end())
			{
				types::PpInt id = binding->second;

				bool
					target = keyEvent == types::KeyEvent::PP_KEYDOWN,
					current = m_buttonState[id];

				if (target != current)
				{
					// FIXME generate event
				}

				m_buttonState[id] = target;

				// Button axis updates
				for (auto &it : m_buttonAxisBindings)
				{
					const std::pair<types::PpInt, types::PpInt> &axisButtons = it.first;
					if (id == axisButtons.first || id == axisButtons.second)
					{
						m_axisState[it.second] =
							static_cast<types::PpFloat>(isButtonDown(axisButtons.second)) -
							static_cast<types::PpFloat>(isButtonDown(axisButtons.first));
					}
				}
			}
		}

		perfectpixel::types::PpInt InputManager::registerButton(const std::string &name)
		{
			// FIXME check already existing
			types::PpInt id = m_buttons.size();

			m_buttons.push_back(name);

			m_buttonState[id] = false;
			m_buttonStatePrev[id] = false;

			return id;
		}

		perfectpixel::types::PpInt InputManager::registerAxis(const std::string &name)
		{
			// FIXME check already existing
			types::PpInt id = m_axes.size();

			m_axes.push_back(name);

			m_axisState[id] = 0.0f;
			m_axisStatePrev[id] = 0.0f;

			return id;
		}

		perfectpixel::types::PpInt InputManager::lookupButton(const std::string &name) const
		{
			for (types::PpInt i = 0; i < static_cast<types::PpInt>(m_buttons.size()); ++i)
			{
				if (m_buttons[i] == name)
				{
					return i;
				}
			}
			throw types::PpException("Unrecognized button");
		}

		perfectpixel::types::PpInt InputManager::lookupAxis(const std::string &name) const
		{
			for (types::PpInt i = 0; i < static_cast<types::PpInt>(m_axes.size()); ++i)
			{
				if (m_axes[i] == name)
				{
					return i;
				}
			}

			throw types::PpException("Unrecognized axis");
		}

		void InputManager::bindButton(const std::string &name, types::KeyCode keyCode)
		{
			types::PpInt buttonId = lookupButton(name);

			for (types::PpInt i = 0; i < static_cast<types::PpInt>(m_buttons.size()); ++i)
			{
				if (m_buttons[i] == name)
				{
					buttonId = i;
					break;
				}
			}

			if (buttonId == 0)
			{
				throw types::PpException("Unrecognized button");
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
			types::PpInt axisId = lookupAxis(axisName);
			types::PpInt negativeId = lookupButton(negativeButton);
			types::PpInt positiveId = lookupButton(positiveButton);

			std::pair<types::PpInt, types::PpInt> axisButtons{ negativeId, positiveId };
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

		bool InputManager::isButtonDown(types::PpInt id)
		{
			if (id >= static_cast<types::PpInt>(m_buttons.size()))
			{
				throw types::PpException("Unregistered button ID");
			}

			return m_buttonState[id];
		}

		bool InputManager::wasButtonDownPrevious(const std::string &name)
		{
			return m_buttonStatePrev[lookupButton(name)];
		}

		bool InputManager::wasButtonDownPrevious(types::PpInt id)
		{
			if (id >= static_cast<types::PpInt>(m_buttons.size()))
			{
				throw types::PpException("Unregistered button ID");
			}

			return m_buttonStatePrev[id];
		}

		bool InputManager::wasButtonPressed(const std::string &name)
		{
			return (isButtonDown(name) && !wasButtonDownPrevious(name));
		}

		bool InputManager::wasButtonPressed(types::PpInt id)
		{
			return (isButtonDown(id) && !wasButtonDownPrevious(id));
		}

		bool InputManager::wasButtonReleased(const std::string &name)
		{
			return (wasButtonDownPrevious(name) && !isButtonDown(name));
		}

		bool InputManager::wasButtonReleased(types::PpInt id)
		{
			return (wasButtonDownPrevious(id) && !isButtonDown(id));
		}

		perfectpixel::types::PpFloat InputManager::getAxisState(const std::string &name)
		{
			return m_axisState[lookupAxis(name)];
		}

		perfectpixel::types::PpFloat InputManager::getAxisState(types::PpInt id)
		{
			return m_axisState[id];
		}

	}
}
