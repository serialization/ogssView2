/*   ____  _____________                                                      *\
 *  / __ \/ ___/ __/ __/  Your OGSS/Cpp Binding                               * 
 * / /_/ / (_\_\ \_\ \    <<debug>>                                           * 
 * \____/\___/___/___/    by: <<some developer>>                              * 
\*                                                                            */
#ifndef OGSS_CPP_GENERATED_EMPTY_FILE
#define OGSS_CPP_GENERATED_EMPTY_FILE

#include <ogss/api/File.h>
#include <ogss/fieldTypes/ArrayType.h>
#include <ogss/fieldTypes/ListType.h>
#include <ogss/fieldTypes/SetType.h>
#include <ogss/fieldTypes/MapType.h>

#include "Pools.h"

namespace empty {
    namespace api {
        /**
         * An OGSS file that corresponds to your specification. Have fun!
         *
         * @author Timm Felden
         */
        struct File : public ::ogss::api::File {


            /**
             * Reads a binary OGSS file and turns it into an instance of this class.
             */
            static File *open(const std::string &path, uint8_t mode = ::ogss::api::ReadMode::read | ::ogss::api::WriteMode::write);

        private:

            //! note: consumes init
            explicit File(::ogss::internal::StateInitializer* init);
        };
}}

#endif