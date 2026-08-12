//
// db_storage.h
//
//      Copyright (C) Kisel Studios. All rights reserved.
//
// Data structure for storing URL information.
//

#pragma once

#ifndef __db_storage_h_
#define __db_storage_h_

#include <SQLiteCpp/SQLiteCpp.h>

#include <optional>
#include <string>

namespace ks
{
    class DbStorage
    {
    private:
        SQLite::Database db;

    public:
        explicit DbStorage(const std::string &filename) : db(filename, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE)
        {
            db.exec(R"(
                CREATE TABLE IF NOT EXISTS data (
                    key   TEXT PRIMARY KEY,
                    value TEXT NOT NULL UNIQUE
                )
            )");
        }

        void Set(const std::string &key, const std::string &value)
        {
            SQLite::Statement query(db, R"(
            INSERT INTO data (key, value)
            VALUES (?, ?)
            ON CONFLICT(key) DO UPDATE SET value = excluded.value
        )");

            query.bind(1, key);
            query.bind(2, value);
            query.exec();
        }

        void Remove(const std::string &key)
        {
            SQLite::Statement query(db, "DELETE FROM data WHERE key = ?");

            query.bind(1, key);
            query.exec();
        }

        std::optional<std::string> FindValueByKey(const std::string &key) const
        {
            SQLite::Statement query(db, "SELECT value FROM data WHERE key = ?");

            query.bind(1, key);

            if (!query.executeStep())
                return std::nullopt;

            return query.getColumn(0).getString();
        }

        std::optional<std::string> FindKeyByValue(const std::string &value) const
        {
            SQLite::Statement query(db, "SELECT key FROM data WHERE value = ?");

            query.bind(1, value);

            if (!query.executeStep())
                return std::nullopt;

            return query.getColumn(0).getString();
        }
    };
} // namespace ks

#endif // !__db_storage_h_