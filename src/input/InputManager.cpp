#include <input/InputManager.h>

#include <types/PpException.h>

#include <boost/bind.hpp>

namespace perfectpixel {
	namespace input {

		InputManager::InputManager()
			: m_buttons()
			, m_axes()
			, m_buttonBindings()
		{
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
			return boost::bind(&InputManager::handleInput, this, _1, _2);
		}

		void InputManager::handleInput(types::KeyCode keyCode, types::KeyEvent keyEvent)
		{
			auto binding = m_buttonBindings.left.find(keyCode);
			if (binding != m_buttonBindings.left.end())
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
			types::PpInt id = m_buttons.size() + 1;

			m_buttons.insert(boost::bimap<types::PpInt, std::string>::value_type(id, name));

			m_buttonState[id] = false;
			m_buttonStatePrev[id] = false;

			return id;
		}

		perfectpixel::types::PpInt InputManager::registerAxis(const std::string &name)
		{
			// FIXME check already existing
			types::PpInt id = m_axes.size() + 1;

			m_axes.insert(boost::bimap<types::PpInt, std::string>::value_type(id, name));

			m_axisState[id] = 0.0f;
			m_axisStatePrev[id] = 0.0f;

			return id;
		}

		void InputManager::bindButton(const std::string &name, types::KeyCode keyCode)
		{
			auto it = m_buttons.right.find(name);
			if (it == m_buttons.right.end())
			{
				throw types::PpException("Tried to map unregistered button");
			}

			types::PpInt buttonId = it->second;

			if (m_buttonBindings.left.find(keyCode) != m_buttonBindings.left.end())
			{
				// FIXME unmap event
				m_buttonBindings.left.erase(keyCode);
			}

			auto existingMapping = m_buttonBindings.right.find(buttonId);
			if (existingMapping != m_buttonBindings.right.end())
			{
				// FIXME existing mapping unbind
				m_buttonBindings.right.erase(buttonId);
			}

			m_buttonBindings.insert(boost::bimap<types::KeyCode, types::PpInt>::value_type(keyCode, buttonId));
		}

		void InputManager::bindAxisToButtons(const std::string &axisName, const std::string &negativeButton, const std::string &positiveButton)
		{
			auto axis = m_axes.right.find(axisName);
			if (axis == m_axes.right.end())
			{
				throw types::PpException("Axis [" + axisName + "] not registered");
			}

			auto negative = m_buttons.right.find(negativeButton);
			if (negative == m_buttons.right.end())
			{
				throw types::PpException("Button [" + negativeButton + "] not registered");
			}

			auto positive = m_buttons.right.find(positiveButton);
			if (positive == m_buttons.right.end())
			{
				throw types::PpException("Button [" + positiveButton + "] not registered");
			}

			std::pair<types::PpInt, types::PpInt> axisButtons{ negative->second, positive->second };
			m_buttonAxisBindings[axisButtons] = axis->second;
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
			auto it = m_buttons.right.find(name);
			if (it == m_buttons.right.end())
			{
				throw types::PpException("Name [" + name + "] did not match a known button");
			}

			return m_buttonState[it->second];
		}

		bool InputManager::isButtonDown(types::PpInt id)
		{
			auto it = m_buttons.left.find(id);
			if (it == m_buttons.left.end())
			{
				throw types::PpException("Id did not match a known button");
			}

			return m_buttonState[id];
		}

		bool InputManager::wasButtonDownPrevious(const std::string &name)
		{
			auto it = m_buttons.right.find(name);
			if (it == m_buttons.right.end())
			{
				throw types::PpException("Name [" + name + "] did not match a known button");
			}

			return m_buttonStatePrev[it->second];
		}

		bool InputManager::wasButtonDownPrevious(types::PpInt id)
		{
			auto it = m_buttons.left.find(id);
			if (it == m_buttons.left.end())
			{
				throw types::PpException("Id did not match a known button");
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

	}
}
