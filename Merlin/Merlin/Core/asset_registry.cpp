#include "Merlin/Core/asset_registry.hpp"

namespace Merlin
{
    void AssetRegistry::RegisterAsset(Asset asset)
    {
        m_asset_map.insert(std::pair<UUID, Asset>(asset.GetUUID(), asset));
    }

    bool AssetRegistry::HasAsset(UUID uuid)
    {
        return m_asset_map.find(uuid) != m_asset_map.end();
    }

    Asset AssetRegistry::GetAsset(UUID uuid)
    {
        auto iterator = m_asset_map.find(uuid);
        return iterator->second;
    }

    void AssetRegistry::RemoveAsset(UUID uuid) { m_asset_map.erase(uuid); }

    void AssetRegistry::Clear() { m_asset_map.clear(); }

}  // namespace Merlin