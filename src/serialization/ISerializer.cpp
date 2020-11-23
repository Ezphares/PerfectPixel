#include <serialization/ISerializer.h>

namespace perfectpixel { namespace serialization {

ISerializer &operator<<(ISerializer &ostream, const std::string &str)
{
    ostream.writeText(str);
    return ostream;
}

ISerializer &operator<<(ISerializer &ostream, const uint32_t &uint)
{
    ostream.writeUInt32(uint);
    return ostream;
}

ISerializer &operator<<(ISerializer &ostream, const float &num)
{
    ostream.writeFloat(num);
    return ostream;
}

ISerializer &operator<<(ISerializer &ostream, const int32_t &num)
{
    ostream.writeInt32(num);
    return ostream;
}

perfectpixel::serialization::ISerializer &
operator<<(ISerializer &ostream, const bedrock::ID &id)
{
    ostream.writeIdentifier(id.m_hash);
    return ostream;
}

ISerializer &operator>>(ISerializer &istream, std::string &str)
{
    istream.readText(&str);
    return istream;
}

ISerializer &operator>>(ISerializer &istream, uint32_t &uint)
{
    istream.readUInt32(&uint);
    return istream;
}

ISerializer &operator>>(ISerializer &istream, float &num)
{
    istream.readFloat(&num);
    return istream;
}

ISerializer &operator>>(ISerializer &istream, int32_t &num)
{
    istream.readInt32(&num);
    return istream;
}

perfectpixel::serialization::ISerializer &
operator>>(ISerializer &istream, bedrock::ID &id)
{
    istream.readIdentifier(&id.m_hash);
#if PP_FULL_REFLECTION_ENABLED
    id.m_reverse = istream.reverse(id.m_hash);
#endif
    return istream;
}

}} // namespace perfectpixel::serialization
