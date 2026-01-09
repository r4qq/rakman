#include <nlohmann/json.hpp>
#include <iostream>
#include <string>
#include <limits> 

#include "ReqManager.hpp"

void printMenu() 
{
    std::cout << "\n=== RAKMAN CLI ===" << std::endl;
    std::cout << "1. Send GET Request" << std::endl;
    std::cout << "2. Send POST Request" << std::endl;
    std::cout << "3. Exit" << std::endl;
    std::cout << "Select option: ";
}

int main()
{
    ReqManager manager;
    int choice = 0;
    std::string url;
    std::string body;

    while (true) 
    {
        printMenu();
        
        if (!(std::cin >> choice))
        {
            std::cout << "Invalid input. Please enter a number." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        // CLEAR THE BUFFER (Crucial step!)
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 3) {
            std::cout << "Exiting..." << std::endl;
            break;
        }

        switch (choice) 
        {
            case 1: 
            { // GET
                std::cout << "Enter URL: ";
                std::getline(std::cin, url);

                if (url.empty()) 
                {
                    url = "example.com"; 
                    std::cout << "Using default: " << url << std::endl;
                }

                std::cout << "Sending GET..." << std::endl;
                if (manager.sendGet(url)) 
                {
                    try 
                    {
                        auto jsonRes = nlohmann::json::parse(manager.getResponseBody());
                        std::cout << jsonRes.dump(4) << std::endl; // dump(4) = pretty print
                    }
                    catch (...) 
                    {
                        std::cout << manager.getResponseBody() << std::endl;
                    }
                } 
                else 
                {
                    std::cerr << "Request Failed!" << std::endl;
                }
                break;
            }
            case 2: { // POST
                std::cout << "Enter URL: ";
                std::getline(std::cin, url);
                if (url.empty()) url = "https://jsonplaceholder.typicode.com/posts";
                std::cout << "enter json: ";
                std::getline(std::cin, body);
                if (!nlohmann::json::accept(body)) {
                    std::cerr << "json invalid" << std::endl;
                    break;
                }
                std::cout << "Sending POST..." << std::endl;
                manager.sendPost(url, body);   
                std::cout << "Response: " << manager.getResponseBody() << std::endl;
                break;
            }
            default:
                std::cout << "Unknown option." << std::endl;
                break;
        }
        manager.cleanResponseBody();
    }
    return 0;
}