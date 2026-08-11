//
// generator.h
//
//      Copyright (C) Kisel Studios. All rights reserved.
//
// Generator of shorten URL header file.
//

#pragma once

#ifndef __generator_h_
#define __generator_h_

#include "db_storage.h"

#include <random>
#include <string>

namespace ks
{
    class UrlGenerator
    {
    private:
        std::random_device rd;
        std::mt19937 rnd{rd()};

    public:
        UrlGenerator() = default;

        ~UrlGenerator() = default;

        std::string GenerateId(const std::string &url)
        {
            static constexpr char chars[] = "0123456789"
                                            "abcdefghijklmnopqrstuvwxyz"
                                            "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

            std::string id;
            id.reserve(6);

            for (int i = 0; i < 6; ++i)
            {
                id += chars[rnd() % (sizeof(chars) - 1)];
            }

            return id;
        }

        std::string GenerateIdFiltred(const std::string &url, const DbStorage &urls)
        {
            std::string id = GenerateId(url);

            while (urls.Get(id))
            {
                id = GenerateId(url);
            }

            return id;
        }
    };
} // namespace ks

#endif // !__generator_h_
