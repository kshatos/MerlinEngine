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
        static UUID Nil();
        UUID();
        UUID(std::string string);
        ~UUID();
        UUID(const UUID& other);
        UUID& operator=(const UUID& other);
        std::string ToString() const;
        bool IsNil();
        bool operator==(const UUID& other) const;
    };
}  // namespace Merlin

template <>
struct std::hash<Merlin::UUID>
{
    std::size_t operator()(Merlin::UUID const& s) const noexcept
    {
        return std::hash<std::string>{}(s.ToString());
    }
};
#endif