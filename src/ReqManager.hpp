#pragma once

#include <cstddef>
#include <curl/curl.h>
#include <string>

class ReqManager {
  private:
    CURL       *_handle;
    std::string _responseBody;

    static size_t WriteCallback(void *contents, size_t size, size_t nmeb,
                                void *userp);

  public:
    ReqManager();
    ~ReqManager();

    bool sendGet(const std::string &url);
    bool sendPost(const std::string &url, const std::string &data);
    bool sendDelete(const std::string &url);
    const std::string &getResponseBody() const;
    void               cleanResponseBody();
};