#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <getopt.h>

#include "ReqManager.hpp"

inline void prettyPrint(const std::string &rawRes)
{
    if (nlohmann::json::accept(rawRes)) {
        auto jsonRes = nlohmann::json::parse(rawRes);
        std::cout << jsonRes.dump(4) << std::endl;
    } else {
        std::cout << rawRes << std::endl;
    }
}

void printHelp() {
    std::cout << "Usage: rakman [OPTIONS]\n"
              << "A lightweight CLI HTTP client.\n\n"
              << "Options:\n"
              << "  -h          Show this help message and exit\n"
              << "  -m METHOD   HTTP Method (GET, POST, DELETE). Default: GET\n"
              << "  -u URL      Target URL (Required)\n"
              << "  -d DATA     JSON body for POST requests\n";
}

int main(int argc, char *argv[])
{
    ReqManager  manager;
    
    std::string method = "GET";
    std::string url;
    std::string body;

    int opt;

    while ((opt = getopt(argc, argv, "hm:u:d:")) != -1) {
        switch (opt) {
            case 'h':
                printHelp();
                return 0;
            case 'm':
                method = optarg;
                break;
            case 'u':
                url = optarg;
                break;
            case 'd':
                body = optarg;
                break;
            case '?':
                return -1;
            default:
                return -1;
        }
    }

    if (url.empty()) {
        std::cerr << "Error: URL is required. Use -u <url>\n";
        return 1;
    }

    manager.cleanResponseBody();

    if (method == "GET") {
        if (!manager.sendGet(url)) {
            std::cout << manager.getResponseBody() << "\n";
            return 1;
        } else {
            prettyPrint(manager.getResponseBody());
            return 0;
        }
    } 
    else if (method == "POST") {
        if (body.empty()) {
            std::cerr << "Error: POST requests require a body. Use -d <body>\n";
            return 1;
        }
        if (!nlohmann::json::accept(body)) {
            std::cerr << "Error: Invalid JSON format.\n";
            return 1;
        }
        if (!manager.sendPost(url, body)) {
            std::cout << manager.getResponseBody() << "\n";
            return 1;
        } else {
            prettyPrint(manager.getResponseBody());
            return 0;
        }    
    }
    else if (method == "DELETE") {
        if (!manager.sendDelete(url)) {
            std::cout << manager.getResponseBody() << "\n";
        } else {
            prettyPrint(manager.getResponseBody());
            return 0;
        }
    } 
    else {
        std::cerr << "Error: Unsupported method '" << method << "'\n";
        return 1;
    }
}