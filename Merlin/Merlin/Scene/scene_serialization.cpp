#include "Merlin/Scene/scene_serialization.hpp"

#include <fstream>

#include "yaml-cpp/yaml.h"

namespace Merlin
{

    void SceneSerializer::Serialize(const std::string& filepath,
                                    std::shared_ptr<GameScene> scene)
    {
        YAML::Emitter out;

        out << YAML::BeginMap;
        {
            out << YAML::Key << "Entities";
            out << YAML::Value;
            out << YAML::BeginSeq;
            {
                scene->VisitEntities(
                    [&](Entity entity)
                    {
                        auto& info_component =
                            entity.GetComponent<EntityInfoComponent>();
                        out << YAML::BeginMap;
                        out << YAML::Key << "Name";
                        out << YAML::Value << info_component.name;
                        out << YAML::Key << "UUID";
                        out << YAML::Value << info_component.uuid.ToString();
                        out << YAML::EndMap;
                    });
            }
            out << YAML::EndSeq;
        }
        out << YAML::EndMap;

        std::ofstream fout(filepath);
        fout << out.c_str();
    }

    void SceneSerializer::Deserialize(const std::string& filespath) {}
}  // namespace Merlin
