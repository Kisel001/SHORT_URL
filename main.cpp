//
// main.cpp
//
//      Copyright (C) Kisel Studios. All rights reserved.
//
// Main server file.
//

#include <httplib.h>

#include "id_storage.h"

#include <iostream>
#include <optional>
#include <string>

namespace ks
{
    // Setting of url of site
    const std::string site_url = "http://localhost:8080/";
} // namespace ks

int main()
{
    httplib::Server server;
    ks::UrlStorage url_storage;

    server.Get(
        "/",
        [&url_storage](const httplib::Request &req, httplib::Response &res)
        {
            res.set_content(
                R"(
<html>
<body>
    <form method="POST" action="/submit">
        <input type="url" name="url"
               placeholder="https://example.com"
               required>
        <button type="submit">Create short URL</button>
    </form>
</body>
</html>
        )",
                "text/html; charset=UTF-8"
            );
        }
    );

    server.Post(
        "/submit",
        [&url_storage](const httplib::Request &req, httplib::Response &res)
        {
            if (!req.has_param("url"))
            {
                res.status = 400;
                res.set_content("URL does not specified", "text/plain");
                return;
            }

            std::string url = req.get_param_value("url");

#ifndef NDEBUG
            std::cout << "URL: " << url << std::endl;
#endif

            std::string short_id = url_storage.Add(url);
            std::string short_url = ks::site_url + short_id;

            res.set_content(
                "<h1>Create short URL:</h1>"
                "<p>" +
                    short_url +
                    "</p>"
                    "<p><a href=\"" +
                    ks::site_url + "\">Back</a></p>",
                "text/html; charset=UTF-8"
            );
        }
    );

    server.Get(
        R"(/([A-Za-z0-9_-]+))",
        [&url_storage](const httplib::Request &req, httplib::Response &res)
        {
            const std::string short_id = req.matches[1];

            std::optional<std::string> url = url_storage.Get(short_id);

            if (!url)
            {
                res.status = 404;
                res.set_content("<h1>404</h1><p>Short URL not found</p>", "text/html; charset=UTF-8");
                return;
            }

            res.set_redirect(*url);
        }
    );

    server.listen("0.0.0.0", 8080);
}
