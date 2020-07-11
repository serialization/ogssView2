/*   ____  _____________                                                      *\
 *  / __ \/ ___/ __/ __/  Your OGSS/Cpp Binding                               * 
 * / /_/ / (_\_\ \_\ \    <<debug>>                                           * 
 * \____/\___/___/___/    by: <<some developer>>                              * 
\*                                                                            */
#ifndef OGSS_CPP_GENERATED_EMPTY_STRING_KEEPER
#define OGSS_CPP_GENERATED_EMPTY_STRING_KEEPER

#include <ogss/api/String.h>
#include <ogss/internal/AbstractStringKeeper.h>

namespace empty {
    namespace internal {
        struct StringKeeper : public ::ogss::internal::AbstractStringKeeper {
            StringKeeper() : AbstractStringKeeper(0) {}
        };
        extern StringKeeper SK;
    }
}

#endif