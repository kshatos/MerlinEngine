#include "Merlin/Core/uuid.hpp"

namespace Merlin
{
    UUID::UUID() { m_internal_id = s_id_generator(); }

    UUID::UUID(std::string string)
    {
        auto id = uuids::uuid::from_string(string);
        if (!id.has_value())
        {
            throw std::runtime_error("UUID string invalid.");
        }
        m_internal_id = id.value();
    }

    std::string UUID::ToString() { return uuids::to_string(m_internal_id); }

}  // namespace Merlin