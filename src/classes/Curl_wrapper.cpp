

#include "Curl_wrapper.h"

Curl_wrapper::Curl_wrapper()
{
    curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
}

size_t Curl_wrapper::write_data(void *ptr, size_t size, size_t nmemb, std::string* buffer)
{
    size_t real_size = size * nmemb;
    buffer->append((char*)ptr, real_size);
    return real_size;
}

inline void Curl_wrapper::set_addr(std::string addr)
{
    curl_easy_setopt(curl, CURLOPT_URL, addr.c_str());
}

int Curl_wrapper::get(std::string url)
{
    set_addr(url);
    buffer.clear();
    return curl_easy_perform(curl);
}

std::string* Curl_wrapper::response()
{
    return &buffer;
}
