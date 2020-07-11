/*   ____  _____________                                                      *\
 *  / __ \/ ___/ __/ __/  Your OGSS/Cpp Binding                               * 
 * / /_/ / (_\_\ \_\ \    <<debug>>                                           * 
 * \____/\___/___/___/    by: <<some developer>>                              * 
\*                                                                            */

#include <cstddef>

#include <ogss/fieldTypes/ArrayType.h>
#include <ogss/fieldTypes/ListType.h>
#include <ogss/fieldTypes/SetType.h>
#include <ogss/fieldTypes/MapType.h>

#include <ogss/internal/EnumPool.h>
#include <ogss/internal/PoolBuilder.h>
#include <ogss/internal/StateInitializer.h>

#include "enums.h"
#include "File.h"
#include "StringKeeper.h"

namespace empty {
    namespace internal {
        StringKeeper SK;

        struct PB final : public ::ogss::internal::PoolBuilder {
            PB() : ::ogss::internal::PoolBuilder(10) {}

            const ::ogss::internal::AbstractStringKeeper *getSK() const final {
                return &SK;
            }

            uint32_t kcc(int id) const final {
                return -1u;
            }

            ogss::fieldTypes::HullType *makeContainer(uint32_t kcc, ::ogss::TypeID tid,
                                                      ogss::fieldTypes::FieldType *kb1,
                                                      ogss::fieldTypes::FieldType *kb2) const final {
                ogss::fieldTypes::HullType * r;
                return nullptr;
                return r;
            }

            ogss::api::String name(int id) const final {
                return nullptr;
            }

            ogss::internal::AbstractPool* make(int id, ::ogss::TypeID index) const final {
                return nullptr;
            }

            ogss::api::String enumName(int id) const final {
                return nullptr;
            }

            ogss::internal::AbstractEnumPool *enumMake(
                    int id, ogss::TypeID index, const std::vector<ogss::api::String> &foundValues) const final {
                return nullptr;
            }
        };
    }
}

::empty::api::File *::empty::api::File::open(const std::string &path, uint8_t mode) {
    ::empty::internal::PB pb;
    return new ::empty::api::File(::ogss::internal::StateInitializer::make(path, pb, mode));
}

::empty::api::File::File(::ogss::internal::StateInitializer *init)
        : ::ogss::api::File(init) {
    delete init;
}
