#pragma once

#include <Bedrock/Hash.h>
#include <Bedrock/defines.h>

#include <string>

namespace perfectpixel { namespace serialization {

class ISerializer
{
public:
    virtual ~ISerializer()
    {}

public:
    virtual void loadBuffer(const char *buffer, size_t bufferSize) = 0;
    virtual void loadBuffer(const std::string &buffer)             = 0;

    virtual void writeFloat(float val)             = 0;
    virtual void writeInt32(int32_t val)           = 0;
    virtual void writeUInt32(uint32_t val)         = 0;
    virtual void writeMappedUInt32(uint32_t val)   = 0;
    virtual void writeText(const std::string &val) = 0;
    virtual void writeIdentifier(int32_t val)      = 0;
    virtual void writeMapKey(int32_t val)          = 0;
#if PP_FULL_REFLECTION_ENABLED
    virtual void writeIdentifier(const std::string &val) = 0;
    virtual void writeMapKey(const std::string &val)     = 0;
#endif // PP_FULL_REFLECTION_ENABLED
    virtual void writeBinary(const void *p, uint32_t size) = 0;
    virtual void writeArrayStart(bool simple = false)      = 0;
    virtual void writeArrayEnd()                           = 0;
    virtual void writeMapStart()                           = 0;
    virtual void writeMapEnd()                             = 0;
    virtual void writeNull()                               = 0;

    virtual void readFloat(float *val)           = 0;
    virtual void readInt32(int32_t *val)         = 0;
    virtual void readUInt32(uint32_t *val)       = 0;
    virtual void readMappedUInt32(uint32_t *val) = 0;
    virtual void readText(std::string *val)      = 0;
    virtual void readIdentifier(int32_t *val)    = 0;
#if PP_FULL_REFLECTION_ENABLED
    virtual void readIdentifier(std::string *val) = 0;
    virtual std::string reverse(int32_t id)       = 0;
#endif // PP_FULL_REFLECTION_ENABLED
    virtual uint32_t readBinary(void *p, uint32_t maxSize)            = 0;
    virtual void readMapBegin()                                       = 0;
    virtual bool readMapKey(int32_t *val)                             = 0;
    virtual uint32_t readArrayStart()                                 = 0;
    virtual void readArrayEnd()                                       = 0;
    virtual bool isValueNull()                                        = 0;
    virtual void mapUInt32(uint32_t memory, uint32_t serialized = -1) = 0;
};

// TODO: Move these
ISerializer &operator<<(ISerializer &ostream, const std::string &str);
ISerializer &operator>>(ISerializer &istream, std::string &str);

ISerializer &operator<<(ISerializer &ostream, const float &num);
ISerializer &operator>>(ISerializer &istream, float &num);

ISerializer &operator<<(ISerializer &ostream, const uint32_t &uint);
ISerializer &operator>>(ISerializer &istream, uint32_t &uint);

ISerializer &operator<<(ISerializer &ostream, const int32_t &num);
ISerializer &operator>>(ISerializer &istream, int32_t &num);

ISerializer &operator<<(ISerializer &ostream, const bedrock::ID &id);
ISerializer &operator>>(ISerializer &istream, bedrock::ID &id);

}} // namespace perfectpixel::serialization
