#include "Merlin/Scene/scene_serialization.hpp"

#include <fstream>

#include "Merlin/Core/logger.hpp"
#include "yaml-cpp/yaml.h"

namespace YAML
{
    template <>
    struct convert<glm::vec2>
    {
        static Node encode(const glm::vec2& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::vec2& rhs)
        {
            if (!node.IsSequence() || node.size() != 2) return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            return true;
        }
    };

    template <>
    struct convert<glm::vec3>
    {
        static Node encode(const glm::vec3& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::vec3& rhs)
        {
            if (!node.IsSequence() || node.size() != 3) return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };

    template <>
    struct convert<glm::vec4>
    {
        static Node encode(const glm::vec4& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::vec4& rhs)
        {
            if (!node.IsSequence() || node.size() != 4) return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };

    template <>
    struct convert<glm::quat>
    {
        static Node encode(const glm::quat& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::quat& rhs)
        {
            if (!node.IsSequence() || node.size() != 4) return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::quat& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
        return out;
    }

}  // namespace YAML

namespace Merlin
{
    void SerializeEntity(YAML::Emitter& out, Entity entity)
    {
        out << YAML::BeginMap;
        {
            auto& info_component = entity.GetComponent<EntityInfoComponent>();
            out << YAML::Key << "EntityInfo";
            out << YAML::Value;
            out << YAML::BeginMap;
            {
                out << YAML::Key << "Name";
                out << YAML::Value << info_component.name;

                out << YAML::Key << "UUID";
                out << YAML::Value << info_component.uuid.ToString();
            }
            out << YAML::EndMap;

            auto& tree_component = entity.GetComponent<EntityTreeComponent>();
            out << YAML::Key << "EntityTree";
            out << YAML::Value;
            out << YAML::BeginMap;
            {
                out << YAML::Key << "Parent";
                out << YAML::Value;
                if (tree_component.parent.has_value())
                    out << tree_component.parent->GetUUID().ToString();
                else
                    out << "None";
                out << YAML::Key << "Children";
                out << YAML::Value;
                out << YAML::BeginSeq;
                for (const auto& child : tree_component.children)
                {
                    out << child.GetUUID().ToString();
                }
                out << YAML::EndSeq;
            }
            out << YAML::EndMap;

            auto& transform_component =
                entity.GetComponent<TransformComponent>();
            out << YAML::Key << "Transform";
            out << YAML::Value;
            out << YAML::BeginMap;
            {
                out << YAML::Key << "Position";
                out << YAML::Value
                    << transform_component.transform.GetPosition();

                out << YAML::Key << "Rotation";
                out << YAML::Value
                    << transform_component.transform.GetOrientation();

                out << YAML::Key << "Scale";
                out << YAML::Value << transform_component.transform.GetScale();
            }
            out << YAML::EndMap;

            if (entity.HasComponent<PointLightComponent>())
            {
                auto& point_light_component =
                    entity.GetComponent<PointLightComponent>();
                out << YAML::Key << "PointLight";
                out << YAML::Value;
                out << YAML::BeginMap;
                {
                    out << YAML::Key << "Color";
                    out << YAML::Value << point_light_component.m_data.color;

                    out << YAML::Key << "RadiantFlux";
                    out << YAML::Value
                        << point_light_component.m_data.radiant_flux;

                    out << YAML::Key << "Range";
                    out << YAML::Value << point_light_component.m_data.range;
                }
                out << YAML::EndMap;
            }

            if (entity.HasComponent<DirectionalLightComponent>())
            {
                auto& directional_light_component =
                    entity.GetComponent<DirectionalLightComponent>();
                out << YAML::Key << "DirectionalLight";
                out << YAML::Value;
                out << YAML::BeginMap;
                {
                    out << YAML::Key << "Color";
                    out << YAML::Value
                        << directional_light_component.m_data.color;

                    out << YAML::Key << "Irradiance";
                    out << YAML::Value
                        << directional_light_component.m_data.irradiance;
                }
                out << YAML::EndMap;
            }

            if (entity.HasComponent<SpotLightComponent>())
            {
                auto& spot_light_component =
                    entity.GetComponent<SpotLightComponent>();
                out << YAML::Key << "SpotLight";
                out << YAML::Value;
                out << YAML::BeginMap;
                {
                    out << YAML::Key << "Color";
                    out << YAML::Value << spot_light_component.m_data.color;

                    out << YAML::Key << "RadiantIntensity";
                    out << YAML::Value
                        << spot_light_component.m_data.radiant_intensity;

                    out << YAML::Key << "FalloffRatio";
                    out << YAML::Value
                        << spot_light_component.m_data.falloff_ratio;

                    out << YAML::Key << "Range";
                    out << YAML::Value << spot_light_component.m_data.range;
                }
                out << YAML::EndMap;
            }

            if (entity.HasComponent<MeshRenderComponent>())
            {
                auto& mesh_render_component =
                    entity.GetComponent<MeshRenderComponent>();
                out << YAML::Key << "MeshRender";
                out << YAML::Value;
                out << YAML::BeginMap;
                {
                }
                out << YAML::EndMap;
            }

            if (entity.HasComponent<CameraComponent>())
            {
                auto& camera_component = entity.GetComponent<CameraComponent>();
                out << YAML::Key << "Camera";
                out << YAML::Value;
                out << YAML::BeginMap;
                {
                }
                out << YAML::EndMap;
            }
        }
        out << YAML::EndMap;
    }

    void DeserializeEntity(YAML::Node& entity,
                           Entity deserialized_entity,
                           std::shared_ptr<GameScene> scene)
    {
        auto& deserialized_info_component =
            deserialized_entity.GetComponent<EntityInfoComponent>();

        auto info_component = entity["EntityInfo"];
        if (info_component)
        {
            deserialized_info_component.name =
                info_component["Name"].as<std::string>();
        }

        auto deserialized_tree_component =
            deserialized_entity.GetComponent<EntityTreeComponent>();
        auto tree_component = entity["EntityTree"];
        if (tree_component)
        {
            auto children = tree_component["Children"];
            for (auto& child : children)
            {
                UUID child_uuid(child.as<std::string>());
                auto child_entity = scene->GetEntity(child_uuid);
                deserialized_entity.AddChild(child_entity.value());
            }
        }

        auto& deserialized_transform_component =
            deserialized_entity.GetComponent<TransformComponent>();
        auto transform_component = entity["Transform"];
        if (transform_component)
        {
            deserialized_transform_component.transform.SetPosition(
                transform_component["Position"].as<glm::vec3>());

            deserialized_transform_component.transform.SetOrientation(
                transform_component["Rotation"].as<glm::quat>());

            deserialized_transform_component.transform.SetScale(
                transform_component["Scale"].as<glm::vec3>());
        }

        auto point_light_component = entity["PointLight"];
        if (point_light_component)
        {
            auto& deserialized_point_light_component =
                deserialized_entity.AddComponent<PointLightComponent>();

            deserialized_point_light_component.m_data.color =
                point_light_component["Color"].as<glm::vec3>();

            deserialized_point_light_component.m_data.radiant_flux =
                point_light_component["RadiantFlux"].as<float>();

            deserialized_point_light_component.m_data.range =
                point_light_component["Range"].as<float>();
        }

        auto directional_light_component = entity["DirectionalLight"];
        if (directional_light_component)
        {
            auto& deserialized_directional_light_component =
                deserialized_entity.AddComponent<DirectionalLightComponent>();

            deserialized_directional_light_component.m_data.color =
                directional_light_component["Color"].as<glm::vec3>();

            deserialized_directional_light_component.m_data.irradiance =
                directional_light_component["Irradiance"].as<float>();
        }

        auto spot_light_component = entity["SpotLight"];
        if (spot_light_component)
        {
            auto& deserialized_spot_light_component =
                deserialized_entity.AddComponent<SpotLightComponent>();

            deserialized_spot_light_component.m_data.color =
                spot_light_component["Color"].as<glm::vec3>();

            deserialized_spot_light_component.m_data.falloff_ratio =
                spot_light_component["FalloffRatio"].as<float>();

            deserialized_spot_light_component.m_data.radiant_intensity =
                spot_light_component["RadiantIntensity"].as<float>();

            deserialized_spot_light_component.m_data.range =
                spot_light_component["Range"].as<float>();
        }

        auto mesh_render_component = entity["MeshRender"];
        if (mesh_render_component)
        {
            deserialized_entity.AddComponent<MeshRenderComponent>();
        }

        auto camera_component = entity["Camera"];
        if (camera_component)
        {
            deserialized_entity.AddComponent<CameraComponent>();
        }
    }

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
                scene->VisitEntities([&](Entity entity)
                                     { SerializeEntity(out, entity); });
            }
            out << YAML::EndSeq;
        }
        out << YAML::EndMap;

        std::ofstream fout(filepath);
        fout << out.c_str();
    }

    void SceneSerializer::Deserialize(const std::string& filepath,
                                      std::shared_ptr<GameScene> scene)
    {
        YAML::Node data;
        try
        {
            data = YAML::LoadFile(filepath);
        }
        catch (std::exception e)
        {
            ME_LOG_ERROR("Unable to load file at " + filepath);
            return;
        }

        auto entities = data["Entities"];
        if (entities)
        {
            // Create entities, registering uuids with scene
            for (auto entity : entities)
            {
                scene->CreateEntity(
                    UUID(entity["EntityInfo"]["UUID"].as<std::string>()));
            }
            // Deserialize entity, as all uuid references can be found
            for (auto entity : entities)
            {
                auto deserialized_entity = scene->GetEntity(
                    UUID(entity["EntityInfo"]["UUID"].as<std::string>()));
                DeserializeEntity(entity, deserialized_entity.value(), scene);
            }
        }
    }
}  // namespace Merlin
