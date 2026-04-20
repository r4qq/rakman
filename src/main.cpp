#include <iostream>
#include <limits>
#include <nlohmann/json.hpp>
#include <string>

#include "ReqManager.hpp"

void printMenu()
{
    std::cout << "\n=== RAKMAN CLI ===" << std::endl;
    std::cout << "1. Send GET Request" << std::endl;
    std::cout << "2. Send POST Request" << std::endl;
    std::cout << "3. Send DELETE Request" << std::endl;
    std::cout << "4. Exit" << std::endl;
    std::cout << "Select option: ";
}

inline void prettyPrint(const std::string &rawRes)
{
    auto jsonRes = nlohmann::json::parse(rawRes);
    std::cout << jsonRes.dump(4) << std::endl;
}

int main()
{
    ReqManager  manager;
    int         choice = 0;
    std::string url;
    std::string body;

    while (true) {
        printMenu();
        if (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a number." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        // buff clean
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 4) {
            std::cout << "Exiting..." << std::endl;
            break;
        }

        switch (choice) {
        case 1: { // GET
            manager.cleanResponseBody();
            std::cout << "Enter URL: ";
            std::getline(std::cin, url);
            if (url.empty()) {
                std::cerr << "Empty Url";
                break;
            }
            if (!manager.sendGet(url)) {
                std::cout << manager.getResponseBody() << std::endl;
                break;
            }
            prettyPrint(manager.getResponseBody());
            break;
        }
        case 2: { // POST
            manager.cleanResponseBody();
            std::cout << "Enter URL: ";
            std::getline(std::cin, url);
            if (url.empty()) {
                std::cerr << "Empty Url";
                break;
            }
            std::cout << "Enter JSON: ";
            std::getline(std::cin, body);
            if (!nlohmann::json::accept(body)) {
                std::cerr << "JSON Invalid" << std::endl;
                break;
            }
            if (!manager.sendPost(url, body)) {
                std::cout << manager.getResponseBody() << std::endl;
                break;
            }
            prettyPrint(manager.getResponseBody());
            break;
        }
        case 3: {
            manager.cleanResponseBody();
            std::cout << "Enter Url: ";
            std::getline(std::cin, url);
            if (url.empty()) {
                std::cerr << "Empty Url";
                break;
            }
            if (!manager.sendDelete(url)) {
                std::cout << manager.getResponseBody() << std::endl;
                break;
            }
            prettyPrint(manager.getResponseBody());
            break;
        }
        default:
            manager.cleanResponseBody();
            std::cout << "Unknown option." << std::endl;
            break;
        }
    }
    return 0;
}