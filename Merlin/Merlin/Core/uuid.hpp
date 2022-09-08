#ifndef UUID_HPP
#define UUID_HPP
#include <stdint.h>

#include <memory>
#include <string>

namespace Merlin
{
    class UUID
    {
    private:
        class UUIDImpl;
        std::unique_ptr<UUIDImpl> m_uuid_impl;

    public:
        UUID();
        UUID(std::string string);
        ~UUID();
        UUID(const UUID& other);
        UUID& operator=(const UUID& other);
        std::string ToString() const;
        bool operator==(const UUID& other) const;
    };
}  // namespace Merlin
#endif