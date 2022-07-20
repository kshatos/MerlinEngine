#include "Merlin/Core/file_util.hpp"
#include "Merlin/Render/texture2d_data.hpp"
#include "Merlin/Core/logger.hpp"
#include "stb/stb_image.h"
#include <stdexcept>
#include <algorithm>


namespace Merlin
{
    Texture2DData load_texture(std::string filepath)
    {
        int width, height, channel_count;
        // TODO: Add options for fewer channels and flip
        stbi_set_flip_vertically_on_load(1);
        unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channel_count, STBI_rgb_alpha);
        if (data==nullptr)
        {
            ME_LOG_ERROR("Failed to load texture at: " + filepath);
            throw std::runtime_error("Failed to load texture at: " + filepath);
        }
        Texture2DData texture_data(width, height, 4, data);
        stbi_image_free(data);

        return texture_data;
    }
}