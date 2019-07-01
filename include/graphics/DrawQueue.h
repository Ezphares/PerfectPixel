#pragma once

#include <graphics/IFont.h>

#include <types/vectors.h>
#include <types/Singleton.h>
#include <types/PpException.h>

#include <vector>

namespace perfectpixel { namespace graphics {

	enum DrawQueueElementType
	{
		DQET_SPRITE,

		DQET_UI_TEXT,
	};

	struct DrawQueueElement
	{
		DrawQueueElementType m_type;
	};

	struct UITextDrawQueueElement : public DrawQueueElement
	{
		types::Vector3 m_position;
		types::PpFloat m_fontSize;
		std::string m_text;
		IFont *m_font;
	};

	class DrawQueueElementFactory : public types::Singleton<DrawQueueElementFactory>
	{
	public:
		DrawQueueElement *allocate(DrawQueueElementType type)
		{
			DrawQueueElement *result;

			switch (type)
			{
			case DQET_SPRITE:
				break;
			case DQET_UI_TEXT:
				result = new UITextDrawQueueElement();
				break;
			default:
				throw types::PpException("No allocator for selected DQET");
			}

			result->m_type = type;
			return result;
		}

		void deallocate(DrawQueueElement *element)
		{
			switch (element->m_type)
			{
			case DQET_UI_TEXT:
				delete static_cast<UITextDrawQueueElement *>(element);
				break;

			default:
				throw types::PpException("No allocator for selected DQET");
			}
		}
	};

	typedef std::vector<DrawQueueElement *> DrawQueue;

} }
