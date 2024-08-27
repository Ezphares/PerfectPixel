#pragma once

#include "renderer/IFont.h"

#include "bedrock/PpException.h"
#include "bedrock/Singleton.h"
#include "bedrock/vectors.h"

#include <vector>

namespace perfectpixel { namespace renderer {

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
    bedrock::Vector3 m_position;
    float m_fontSize;
    std::string m_text;
    IFont *m_font;
};

class DrawQueueElementFactory
    : public bedrock::Singleton<DrawQueueElementFactory>
{
public:
    DrawQueueElement *allocate(DrawQueueElementType type)
    {
        DrawQueueElement *result;

        switch (type)
        {
        case DQET_SPRITE:
            // TODO: Not yet implemented
            throw bedrock::PpException(" Not yet implemented");
            break;
        case DQET_UI_TEXT:
            result = new UITextDrawQueueElement();
            break;
        default:
            throw bedrock::PpException("No allocator for selected DQET");
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
            throw bedrock::PpException("No allocator for selected DQET");
        }
    }
};

typedef std::vector<DrawQueueElement *> DrawQueue;

}} // namespace perfectpixel::renderer
