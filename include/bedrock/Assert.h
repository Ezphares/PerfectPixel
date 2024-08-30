#pragma once
#define PP_DBG 1

namespace perfectpixel::bedrock {
extern void (*assertHandler)(const char *);
}
#if PP_DBG

#include <iostream>
#include <sstream>

#define PP_ASSERT(expr, msg) PP_ASSERT_IMPL(expr, msg, __FILE__, __LINE__)

#define PP_ASSERT_IMPL(expr, msg, file, line)                                  \
    {                                                                          \
        if (!(expr))                                                           \
        {                                                                      \
            std::stringstream fullmsg;                                         \
            fullmsg << file << "(" << line << ")\n\n"                          \
                    << msg << "\n(" << #expr << ")";                           \
            if (::perfectpixel::bedrock::assertHandler)                        \
            {                                                                  \
                ::perfectpixel::bedrock::assertHandler(fullmsg.str().c_str()); \
            }                                                                  \
            else                                                               \
            {                                                                  \
                std::cerr << fullmsg.str() << std::endl;                       \
            }                                                                  \
        }                                                                      \
    }
#else
#define PP_ASSERT(expr, msg)
#endif