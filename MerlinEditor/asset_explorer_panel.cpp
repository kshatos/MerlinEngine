#include "asset_explorer_panel.hpp"

#include <imgui.h>

#include <Merlin/Core/asset_registry.hpp>
#include <Merlin/Core/file_util.hpp>
#include <Merlin/Core/logger.hpp>
#include <fstream>
#include <iostream>

namespace MerlinEditor
{
    const std::set<std::string> texture_extensions{".png", ".jpg"};
    const std::set<std::string> mesh_extensions{".stl", ".fbx"};
    const std::set<std::string> shader_extensions{".stl", ".fbx"};

    Merlin::AssetType ExtensionAssetType(std::string extension)
    {
        if (texture_extensions.find(extension) != texture_extensions.end())
            return Merlin::AssetType::Texture;
        if (mesh_extensions.find(extension) != mesh_extensions.end())
            return Merlin::AssetType::Mesh;
        if (shader_extensions.find(extension) != shader_extensions.end())
            return Merlin::AssetType::Shader;
        if (extension == ".scene") return Merlin::AssetType::Scene;
        return Merlin::AssetType::None;
    }

    void AssetExplorerPanel::DrawPanel()
    {
        ScanForAssets(m_project_path);
        ImGui::Begin(m_name.c_str());

        auto size = ImGui::GetContentRegionAvail();

        auto size1 = ImVec2(0.25f * size.x, size.y);
        auto size2 = ImVec2(size.x - size1.x, size.y);

        auto window_flags = ImGuiWindowFlags_AlwaysVerticalScrollbar;

        ImGui::BeginChild("Directories", size1, true, window_flags);
        DrawDirectoryTreeNode(m_project_path);
        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::BeginChild("Assets", size2, true, window_flags);
        DrawDirectoryView();
        ImGui::EndChild();

        ImGui::End();
    }

    void AssetExplorerPanel::DrawDirectoryTreeNode(std::filesystem::path path)
    {
        bool has_subdirectory = false;
        std::filesystem::directory_iterator directory_iterator(path);
        for (const auto& entry : directory_iterator)
        {
            if (entry.is_directory())
            {
                has_subdirectory = true;
                break;
            }
        }

        ImGuiTreeNodeFlags tree_flags =
            ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow;
        if (!has_subdirectory) tree_flags |= ImGuiTreeNodeFlags_Leaf;

        std::string path_string = path.stem().string();
        bool is_open = ImGui::TreeNodeEx(path_string.c_str(), tree_flags);
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
        {
            m_current_path = path;
        }

        if (is_open)
        {
            std::filesystem::directory_iterator directory_iterator(path);
            for (const auto& entry : directory_iterator)
            {
                if (entry.is_directory())
                {
                    DrawDirectoryTreeNode(entry.path());
                }
            }
            ImGui::TreePop();
        }
    }

    void AssetExplorerPanel::DrawDirectoryView()
    {
        bool back_clicked = ImGui::Button("BACK");
        ImGui::SameLine();

        auto relative_path = std::filesystem::relative(
            m_current_path, m_project_path.parent_path());
        auto relative_path_string = relative_path.string();
        ImGui::Text("%s", relative_path_string.c_str());

        ImGui::Separator();

        float cellSize = 200.0f;
        float panelWidth = ImGui::GetContentRegionAvail().x;
        int columnCount = (int)(panelWidth / cellSize);
        if (columnCount < 1) columnCount = 1;

        ImGui::Columns(columnCount, 0, false);

        std::filesystem::directory_iterator directory_iterator(m_current_path);
        for (const auto& entry : directory_iterator)
        {
            DrawDirectoryEntry(entry);
        }
        if (back_clicked && m_current_path != m_project_path)
        {
            m_current_path = m_current_path.parent_path();
        }
    }

    void AssetExplorerPanel::DrawDirectoryEntry(
        std::filesystem::directory_entry entry)
    {
        const auto& path = entry.path();
        bool is_directory = entry.is_directory();
        bool is_registered = m_registered_asset_paths.find(path) !=
                             m_registered_asset_paths.end();
        if (!is_registered && !is_directory) return;

        std::string filename_string = path.filename().string();
        if (ImGui::Button(filename_string.c_str()))
        {
            if (entry.is_directory())
            {
                m_current_path = entry.path();
            }
        }

        ImGui::NextColumn();
    }

    void AssetExplorerPanel::ScanForAssets(std::filesystem::path directory_path)
    {
        // Check for new assets
        std::filesystem::directory_iterator directory_iterator(directory_path);
        for (const auto& entry : directory_iterator)
        {
            auto entry_path = entry.path();
            if (entry.is_directory())
            {
                ScanForAssets(entry_path);
            }
            else
            {
                auto extension = entry_path.extension().string();
                auto asset_type = ExtensionAssetType(extension);
                if (asset_type == Merlin::AssetType::None) continue;
                bool is_registered =
                    m_registered_asset_paths.find(entry_path) !=
                    m_registered_asset_paths.end();
                if (!is_registered)
                {
                    RegisterAsset(entry_path);
                }
            }
        }
    }

    void AssetExplorerPanel::RegisterAsset(std::filesystem::path asset_path)
    {
        auto extension = asset_path.extension().string();
        auto asset_type = ExtensionAssetType(extension);

        auto meta_path = asset_path;
        meta_path.replace_extension(".asset");

        if (!std::filesystem::exists(meta_path))
        {
            Merlin::UUID asset_uuid;
            std::ofstream outfile;
            outfile.open(meta_path, std::ios::out | std::ios::trunc);
            outfile << asset_uuid.ToString();
            outfile.close();
        }

        std::ifstream inputfile;
        inputfile.open(meta_path);
        std::string uuid_string;
        inputfile >> uuid_string;
        Merlin::UUID asset_uuid(uuid_string);
        inputfile.close();

        std::shared_ptr<void> asset_pointer = nullptr;
        switch (asset_type)
        {
            case Merlin::AssetType::Mesh:
            {
                auto mesh_data = Merlin::LoadMesh(asset_path);
                auto mesh_buffer = Merlin::Renderer::CreateMeshBuffer(
                    mesh_data->GetVertexDataPointer(),
                    mesh_data->GetVertexCount() * sizeof(Merlin::Vertex_XNTBUV),
                    mesh_data->GetIndexDataPointer(),
                    mesh_data->GetTriangleCount() * 3,
                    Merlin::Vertex_XNTBUV::GetLayout());
                asset_pointer = std::static_pointer_cast<void>(mesh_buffer);
                break;
            }
            case Merlin::AssetType::Texture:
            {
                auto data = Merlin::LoadTexture(asset_path);
                auto texture = Merlin::Renderer::CreateTexture2D(data);
                asset_pointer = std::static_pointer_cast<void>(texture);
                break;
            }
            case Merlin::AssetType::Shader:
                break;
            case Merlin::AssetType::Scene:
                break;
            default:
                break;
        }
        auto& registry = m_application->GetAssetRegistry();

        Merlin::Asset asset(asset_uuid, asset_type, asset_pointer);
        registry.RegisterAsset(asset);

        m_registered_asset_paths.insert(asset_path);
    }

}  // namespace MerlinEditor