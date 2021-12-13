#include <Resources/PNGImage.h>

#include <Bedrock/File.h>

#include <cstring>

namespace perfectpixel { namespace resources {

void PNGImage::PNGImageLoaderFunction(
    char *data,
    size_t dataSize,
    void **target,
    const bedrock::Opaque &userData,
    void * /* not used */)
{
    const PNGImageUserData *udata = userData.get<PNGImageUserData>();
    *target                       = new PNGImage();
    PNGImage::FromSimpleBuffer(
        reinterpret_cast<PNGImage *>(*target),
        data,
        dataSize,
        udata ? udata->m_bgr : false,
        udata ? udata->m_vflip : true);
}

bedrock::Point2 PNGImage::getSize() const
{
    return bedrock::Point2(
        static_cast<int32_t>(m_w), static_cast<int32_t>(m_h));
}

uint32_t PNGImage::getChannels() const
{
    return static_cast<uint32_t>(m_channels);
}

unsigned char *PNGImage::getBuffer() const
{
    return reinterpret_cast<unsigned char *>(m_buffer.m_idata);
}

size_t PNGImage::getBufferSize() const
{
    return m_buffer.m_size;
}

uint32_t PNGImage::getTextureHint() const
{
    return m_textureHint;
}

void PNGImage::setTextureHint(uint32_t hint)
{
    m_textureHint = hint;
}

PNGImage::PNGImage()
    : m_bitDepth(0)
    , m_colorType(0)
    , m_w(0)
    , m_h(0)
    , m_channels(0)
    , m_buffer()
    , m_textureHint(~0u)
{}

PNGImage::~PNGImage()
{}

PNGImage PNGImage::fromFile(
    const std::string &filename,
    bool switchRgb /*= false*/,
    bool vflip /*= true*/)
{
    PNGImage result;

    bedrock::File f = bedrock::File(filename);

    FromSimpleBuffer(&result, f.getBuffer(), f.getSize(), switchRgb, vflip);

    return result;
}

void PNGImage::FromSimpleBuffer(
    PNGImage *out_image, char *buffer, size_t bufferSize, bool bgr, bool vflip)
{
    const size_t PNG_HEADER_SIZE = 8;
    if (bufferSize < PNG_HEADER_SIZE)
    {
        throw bedrock::PpException(
            "Input buffer too small to contain PNG header");
    }

    // PNG header check
    png_byte header[8];
    memcpy(header, buffer, PNG_HEADER_SIZE);
    if (png_sig_cmp(header, 0, PNG_HEADER_SIZE))
    {
        throw bedrock::PpException("Could not parse PNG header");
    }

    SimpleBuffer pngReadBuffer;
    pngReadBuffer.m_buffer = buffer;
    pngReadBuffer.m_size   = bufferSize;
    pngReadBuffer.m_offset = 8;

    PngReadStruct readStruct = PngReadStruct();

    if (!readStruct.m_ok)
    {
        throw bedrock::PpException("Could not create PNG metadata structures");
    }

    // TODO: USe error callbacks instead
#if _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4611)
#endif
    if (setjmp(png_jmpbuf(readStruct.m_png)))
    {
        throw bedrock::PpException("Could not set jump");
    }
#if _MSC_VER
#pragma warning(pop)
#endif

    png_set_read_fn(readStruct.m_png, &pngReadBuffer, &ReadFromSimpleBuffer);
    png_set_sig_bytes(readStruct.m_png, 8);
    png_read_info(readStruct.m_png, readStruct.m_pngInfo);

    png_get_IHDR(
        readStruct.m_png,
        readStruct.m_pngInfo,
        &out_image->m_w,
        &out_image->m_h,
        &out_image->m_bitDepth,
        &out_image->m_colorType,
        NULL,
        NULL,
        NULL);

    out_image->m_channels
        = png_get_channels(readStruct.m_png, readStruct.m_pngInfo);

    if (bgr)
    {
        png_set_bgr(readStruct.m_png);
    }
    png_read_update_info(readStruct.m_png, readStruct.m_pngInfo);

    size_t rowbytes = png_get_rowbytes(readStruct.m_png, readStruct.m_pngInfo);

    out_image->m_buffer    = PngBuffer(rowbytes * out_image->m_h);
    png_bytepp rowpointers = new png_bytep[out_image->m_h];
    for (unsigned int i = 0; i < out_image->m_h; ++i)
    {
        unsigned int index = vflip ? out_image->m_h - 1 - i : i;
        rowpointers[index] = out_image->m_buffer.m_idata + i * rowbytes;
    }
    png_read_image(readStruct.m_png, rowpointers);

    delete[] rowpointers;
}

void PNGImage::ReadFromSimpleBuffer(
    png_structp png_ptr, png_bytep out_bytes, png_size_t count)
{
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (io_ptr == nullptr)
    {
        throw bedrock::PpException(
            "NULL passed to ReadFromSimpleBuffer from libpng");
    }

    SimpleBuffer *buffer = reinterpret_cast<SimpleBuffer *>(io_ptr);

    if (buffer->m_offset + count > buffer->m_size)
    {
        throw bedrock::PpException("libpng read past end of buffer");
    }

    memcpy(out_bytes, buffer->m_buffer + buffer->m_offset, count);
    buffer->m_offset += count;
}

PNGImage::PngReadStruct::PngReadStruct()
{
    m_ok = false;

    m_png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (m_png == NULL)
        return;

    m_pngInfo = png_create_info_struct(m_png);
    if (m_pngInfo == NULL)
        return;

    m_pngEnd = png_create_info_struct(m_png);
    if (m_pngEnd == NULL)
        return;

    m_ok = true;
}

PNGImage::PngReadStruct::~PngReadStruct()
{
    png_destroy_read_struct(&m_png, &m_pngInfo, &m_pngEnd);
}

PNGImage::PngBuffer::PngBuffer()
    : m_idata(NULL)
    , m_size(0)
{}

PNGImage::PngBuffer::PngBuffer(std::size_t size)
    : m_idata(static_cast<png_bytep>(malloc(sizeof(png_bytep) * size)))
    , m_size(size)
{}

PNGImage::PngBuffer::PngBuffer(const PngBuffer &copy)
    : m_idata(NULL)
    , m_size(copy.m_size)
{
    if (copy.m_idata != NULL)
    {
        m_idata
            = static_cast<png_bytep>(malloc(sizeof(png_bytep) * copy.m_size));
        if (m_idata)
        {
            memcpy(m_idata, copy.m_idata, sizeof(png_bytep) * copy.m_size);
        }
    }
}

PNGImage::PngBuffer::~PngBuffer()
{
    if (m_idata)
    {
        free(m_idata);
    }
}

PNGImage::PngBuffer &PNGImage::PngBuffer::operator=(const PngBuffer &copy)
{
    if (this != &copy)
    {
        delete m_idata;
        m_size  = copy.m_size;
        m_idata = NULL;

        if (copy.m_idata != NULL)
        {
            m_idata = static_cast<png_bytep>(
                malloc(sizeof(png_bytep) * copy.m_size));
            if (m_idata)
            {
                memcpy(m_idata, copy.m_idata, sizeof(png_bytep) * copy.m_size);
            }
        }
    }

    return *this;
}

}} // namespace perfectpixel::resources
