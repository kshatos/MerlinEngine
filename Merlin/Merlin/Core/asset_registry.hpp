#ifndef ASSET_REGISTRY_HPP
#define ASSET_REGISTRY_HPP
#include <memory>
#include <unordered_map>

#include "Merlin/Core/uuid.hpp"

namespace Merlin
{
    enum class AssetType
    {
        None,
        Mesh,
        Texture,
        Shader,
        Scene
    };

    class Asset
    {
        UUID m_uuid;
        AssetType m_type;
        std::shared_ptr<void> m_asset;

    public:
        Asset(UUID uuid, AssetType type, std::shared_ptr<void> asset)
            : m_uuid(uuid), m_type(type), m_asset(asset)
        {
        }

        template <typename T>
        std::shared_ptr<T> Get()
        {
            return std::dynamic_pointer_cast<T>(m_asset_pointer);
        }

        inline AssetType GetAssetType() { return m_type; }

        inline UUID GetUUID() { return m_uuid; }
    };

    class AssetRegistry
    {
        std::unordered_map<UUID, Asset> m_asset_map;

    public:
        void RegisterAsset(Asset asset);
        bool HasAsset(UUID uuid);
        Asset GetAsset(UUID uuid);
        void RemoveAsset(UUID uuid);
        void Clear();
    };

}  // namespace Merlin
#endif