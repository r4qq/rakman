#include <cstddef>
#include <curl/curl.h>
#include <curl/easy.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

#include "ReqManager.hpp"

ReqManager::ReqManager()
{
    this->_handle = curl_easy_init();
}

ReqManager::~ReqManager() 
{
    curl_easy_cleanup(this->_handle);
}

bool ReqManager::sendGet(const std::string &url)
{
    if(!this->_handle) 
    { 
        return false;
    }

    curl_easy_reset(this->_handle);

    curl_easy_setopt(this->_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(this->_handle, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(this->_handle, CURLOPT_WRITEDATA, &_responseBody);

    auto response = curl_easy_perform(this->_handle);

    if (response != CURLE_OK) 
    {
        std::cerr << curl_easy_strerror(response) << std::endl;
        return false; 
    }

    return true;
}

bool ReqManager::sendPost(const std::string &url, const std::string &data)
{
    if(!this->_handle) 
    {
        return false; 
    }

    curl_easy_reset(this->_handle);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(this->_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(this->_handle, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(this->_handle, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(this->_handle, CURLOPT_WRITEDATA, &_responseBody);
    curl_easy_setopt(this->_handle, CURLOPT_HTTPHEADER, headers);

    auto response = curl_easy_perform(this->_handle);

    curl_slist_free_all(headers);
    
    if (response != CURLE_OK) 
    {
        std::cerr << curl_easy_strerror(response) << std::endl;
        return false; 
    }

    return true;
}

bool ReqManager::sendDelete(const std::string &url)
{
    if (!this->_handle) 
    {
        return false;
    }

    curl_easy_reset(this->_handle);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(this->_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(this->_handle, CURLOPT_CUSTOMREQUEST , "DELETE");
    curl_easy_setopt(this->_handle, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(this->_handle, CURLOPT_WRITEDATA, &_responseBody);
    curl_easy_setopt(this->_handle, CURLOPT_HTTPHEADER, headers);

    auto response = curl_easy_perform(this->_handle);

    curl_slist_free_all(headers);

    if (response != CURLE_OK) {
        std::cerr << curl_easy_strerror(response) << std::endl;
        return false; 
    }

    return true;
}

size_t ReqManager::WriteCallback(void* contents, size_t size, size_t nmeb, void* userp)
{
    size_t totalSize = size * nmeb;
    auto readBuffer = static_cast<std::string*>(userp);
    readBuffer->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

//utils
const std::string& ReqManager::getResponseBody() const { return _responseBody; };
void ReqManager::cleanResponseBody() { _responseBody.clear(); }
