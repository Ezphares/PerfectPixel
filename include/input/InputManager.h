#pragma once

#include <types/InputCallback.h>
#include <types/numbers.h>

#include <boost/bimap.hpp>

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
			boost::bimap<types::PpInt, std::string> m_buttons;
			boost::bimap<types::PpInt, std::string> m_axes;

			boost::bimap<types::KeyCode, types::PpInt> m_buttonBindings;

			std::map<types::PpInt, bool> m_buttonState;
			std::map<types::PpInt, bool> m_buttonStatePrev;
			
			std::map<types::PpInt, types::PpFloat> m_axisState;
			std::map<types::PpInt, types::PpFloat> m_axisStatePrev;

			std::map<std::pair<types::PpInt, types::PpInt>, types::PpInt> m_buttonAxisBindings;
		};
		
	}
}
