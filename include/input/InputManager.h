#pragma once

#include <types/InputCallback.h>
#include <types/numbers.h>

#include <vector>
#include <set>
#include <map>
#include <string>

namespace perfectpixel {
	namespace input {

		class InputManager
		{
		public:
			InputManager();
			~InputManager();

		public:
			void update();

			types::KeyCallback getKeyCallback();
			void handleInput(types::KeyCode keyCode, types::KeyEvent keyEvent);

			types::PpInt registerButton(const std::string &name);
			types::PpInt registerAxis(const std::string &name);
			types::PpInt lookupButton(const std::string &name) const;
			types::PpInt lookupAxis(const std::string &name) const;

			void bindButton(const std::string &name, types::KeyCode keyCode);

			void bindAxisToButtons(
				const std::string &axisName,
				const std::string &negativeButton,
				const std::string &positiveButton);

			void clearState();

			bool isButtonDown(const std::string &name);
			bool isButtonDown(types::PpInt id);
			bool wasButtonDownPrevious(const std::string &name);
			bool wasButtonDownPrevious(types::PpInt id);
			bool wasButtonPressed(const std::string &name);
			bool wasButtonPressed(types::PpInt id);
			bool wasButtonReleased(const std::string &name);
			bool wasButtonReleased(types::PpInt id);

			types::PpFloat getAxisState(const std::string &name);

		private:
			std::vector<std::string> m_buttons;
			std::vector<std::string> m_axes;

			std::map<types::KeyCode, types::PpInt> m_buttonBindings;

			std::map<types::PpInt, bool> m_buttonState;
			std::map<types::PpInt, bool> m_buttonStatePrev;
			
			std::map<types::PpInt, types::PpFloat> m_axisState;
			std::map<types::PpInt, types::PpFloat> m_axisStatePrev;

			std::map<std::pair<types::PpInt, types::PpInt>, types::PpInt> m_buttonAxisBindings;
		};
		
	}
}
