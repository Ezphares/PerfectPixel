#pragma once

#include <Bedrock/InputCallback.h>
#include <Bedrock/numbers.h>

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

			bedrock::KeyCallback getKeyCallback();
			void handleInput(bedrock::KeyCode keyCode, bedrock::KeyEvent keyEvent);

			bedrock::PpInt registerButton(const std::string &name);
			bedrock::PpInt registerAxis(const std::string &name);
			bedrock::PpInt lookupButton(const std::string &name) const;
			bedrock::PpInt lookupAxis(const std::string &name) const;

			void bindButton(const std::string &name, bedrock::KeyCode keyCode);

			void bindAxisToButtons(
				const std::string &axisName,
				const std::string &negativeButton,
				const std::string &positiveButton);

			void clearState();

			bool isButtonDown(const std::string &name);
			bool isButtonDown(bedrock::PpInt id);
			bool wasButtonDownPrevious(const std::string &name);
			bool wasButtonDownPrevious(bedrock::PpInt id);
			bool wasButtonPressed(const std::string &name);
			bool wasButtonPressed(bedrock::PpInt id);
			bool wasButtonReleased(const std::string &name);
			bool wasButtonReleased(bedrock::PpInt id);

			bedrock::PpFloat getAxisState(const std::string &name);
			bedrock::PpFloat getAxisState(bedrock::PpInt id);

		private:
			std::vector<std::string> m_buttons;
			std::vector<std::string> m_axes;

			std::map<bedrock::KeyCode, bedrock::PpInt> m_buttonBindings;

			std::map<bedrock::PpInt, bool> m_buttonState;
			std::map<bedrock::PpInt, bool> m_buttonStatePrev;
			
			std::map<bedrock::PpInt, bedrock::PpFloat> m_axisState;
			std::map<bedrock::PpInt, bedrock::PpFloat> m_axisStatePrev;

			std::map<std::pair<bedrock::PpInt, bedrock::PpInt>, bedrock::PpInt> m_buttonAxisBindings;
		};
		
	}
}
