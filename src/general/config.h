//
//  config.hpp
//  pypse
//
//  Created by Florent Jacquemard on 18/11/2022.
//

#ifndef config_hpp
#define config_hpp

#include <iostream>


/// Platform = execution architecture
#ifndef PLATFORM_DARWIN
#error ERROR qpseconfig.h : no tag PLATFORM_DARWIN
#endif

#ifndef PLATFORM_LINUX
#error ERROR pseconfig.h : no tag PLATFORM_LINUX
#endif

#ifndef PLATFORM_CYGWIN
#error ERROR pseconfig.h : no tag PLATFORM_CYGWIN
#endif

#ifndef PLATFORM_FREEBSD
#error ERROR pseconfig.h : no tag PLATFORM_FREEBSD
#endif

#ifndef PSE_PLATFORM
#error ERROR pseconfig.h : flag PSE_PLATFORM undefined
#endif

//extern const char* configuration_qparse_archi;

#if PSE_PLATFORM == PLATFORM_DARWIN    // we are on MAC OS
#elif PSE_PLATFORM == PLATFORM_LINUX   // we are on a Linux
#elif PSE_PLATFORM == PLATFORM_CYGWIN  // we are on a Windows
#elif PSE_PLATFORM == PLATFORM_FREEBSD // we are on a Free BSD
#else
#error ERROR config.h : invalid PSE_PLATFORM
#endif


/// patch for a bug in g++
/// see https://stackoverflow.com/questions/12975341/to-string-is-not-a-member-of-std-says-g-mingw
#if QP_PLATFORM == PLATFORM_LINUX
namespace patch
{
    template < typename T > std::string to_string(const T& n)
    {
        std::ostringstream os;
        os << n;
        return os.str();
    }
}
#endif

/// System information
extern long virtual_memory_size();
extern long resident_memory_size();



#endif /* config_hpp */
