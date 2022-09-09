#ifndef SCENE_SERIALIZATION_HPP
#define SCENE_SERIALIZATION_HPP
#include <string>

#include "Merlin/Scene/game_scene.hpp"

namespace Merlin
{

    class SceneSerializer
    {
    public:
        static void Serialize(const std::string& filepath,
                              std::shared_ptr<GameScene> scene);
        static void Deserialize(const std::string& filepath,
                                std::shared_ptr<GameScene> scene);
        static std::string SerializeEntity(Merlin::Entity entity);
        static void DeserializeEntity(std::string serialized_entity,
                                 Entity deserialized_entity,
                                 std::shared_ptr<GameScene> scene);
    };

}  // namespace Merlin
#endif