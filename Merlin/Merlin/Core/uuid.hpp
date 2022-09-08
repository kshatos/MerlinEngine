#ifndef UUID_HPP
#define UUID_HPP
#include <stdint.h>
#include <uuid.h>
namespace Merlin
{
    class UUID
    {
        static uuids::uuid_system_generator s_id_generator;

        uuids::uuid m_internal_id;
    public:
        UUID();
        UUID(std::string string);
        std::string ToString();
    };
}  // namespace Merlin
#endif