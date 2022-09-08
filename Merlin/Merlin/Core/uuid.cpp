#include "Merlin/Core/uuid.hpp"

#include <uuid.h>

namespace Merlin
{

    class UUID::UUIDImpl
    {
    public:
        static uuids::uuid_system_generator s_id_generator;

        uuids::uuid m_internal_id;

        UUIDImpl() { m_internal_id = s_id_generator(); }
        UUIDImpl(std::string string)
        {
            auto id = uuids::uuid::from_string(string);
            if (!id.has_value())
            {
                throw std::runtime_error("UUID string invalid.");
            }
            m_internal_id = id.value();
        }
        std::string UUIDImpl::ToString()
        {
            return uuids::to_string(m_internal_id);
        }
    };

    uuids::uuid_system_generator UUID::UUIDImpl::s_id_generator =
        uuids::uuid_system_generator();

    UUID::UUID() { m_uuid_impl = std::make_unique<UUIDImpl>(); }

    UUID ::~UUID() = default;

    UUID::UUID(const UUID& other)
    {
        m_uuid_impl = std::move(std::make_unique<UUIDImpl>(other.ToString()));
    }

    UUID& UUID::operator=(const UUID& other)
    {
        m_uuid_impl = std::move(std::make_unique<UUIDImpl>(other.ToString()));
        return *this;
    }

    UUID::UUID(std::string string)
    {
        m_uuid_impl = std::make_unique<UUIDImpl>(string);
    }

    std::string UUID::ToString() const { return m_uuid_impl->ToString(); }

    bool UUID::operator==(const UUID& other) const
    {
        return m_uuid_impl->m_internal_id == other.m_uuid_impl->m_internal_id;
    }
}  // namespace Merlin