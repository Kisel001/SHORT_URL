//
// id_storage.h
//
//      Copyright (C) Kisel Studios. All rights reserved.
//
// Data structure for storing URL information.
//

#pragma once

#ifndef __id_storage_h_
#define __id_storage_h_

#include "db_storage.h"
#include "generator.h"

#include <optional>
#include <unordered_map>
#include <utility>

namespace ks
{
    class UrlStorage
    {
    private:
        const std::string db_filename = "url_data.db";
        DbStorage storage{db_filename};
        UrlGenerator generator;

    public:
        UrlStorage() = default;

        ~UrlStorage() = default;

        std::string Add(const std::string &url)
        {
            std::string id = generator.GenerateIdFiltred(url, storage);
            storage.Set(id, url);
            return id;
        }

        std::optional<std::string> Get(const std::string &id)
        {
            return storage.Get(id);
        }
    };
} // namespace ks

#endif // !__url_storage_h_
