#ifndef SCENE_SERIALIZATION_HPP
#define SCENE_SERIALIZATION_HPP
#include <string>

#include "Merlin/Scene/game_scene.hpp"

namespace Merlin
{

    class SceneSerializer
    {
    public:
        void Serialize(const std::string& filepath,
                              std::shared_ptr<GameScene> scene);
        void Deserialize(const std::string& filespath);
    };

}  // namespace Merlin
#endif