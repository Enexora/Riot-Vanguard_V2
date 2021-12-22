#pragma once
#include <Includes.h>

std::uint8_t* load_image(unsigned short id, int width, int height, const std::string& type = "PNG")
  {
        // csgo->module is the module you get from creating a thread in DLLMain
        // static DWORD __stdcall thread( const LPVOID parameter )
        // csgo->module = HMODULE( parameter )
        auto resource = FindResource(csgo->module, MAKEINTRESOURCE(id), type.data());
        auto loaded_resource = LoadResource(csgo->module, resource);
        auto resource_ptr = LockResource(loaded_resource);
        auto resource_size = SizeofResource(csgo->module, resource);

        std::vector< std::uint8_t > image;

        auto w = std::uint32_t(width);
        auto h = std::uint32_t(height);

        if (const auto error = lodepng::decode(image, w, h, (unsigned char*)resource_ptr, resource_size))
            exit(0);

        const auto data = new std::uint8_t[image.size()];
        std::copy(image.begin(), image.end(), data);

        return data;
    }