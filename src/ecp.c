#include <curl/curl.h>

#include "ecp.h"

static CURL* s_curl = NULL;
static char ip[64] = {0};


const char* ecp_get_ip(void)
{
    return NULL;
}

void ecp_init(const char* ip)
{
    curl_global_init( CURL_GLOBAL_ALL );
    s_curl = curl_easy_init();
}

// Wrapper for sending keyboard letters with Lit_x or changing channels or volume or typical keypresses like home or left or right
void ecp_keypress(const char *keypress)
{
    char url[64];
    sprintf(url, "http://192.168.1.5:8060/keypress/%s", keypress);
    
    curl_easy_setopt(s_curl, CURLOPT_URL, url);
    curl_easy_setopt(s_curl, CURLOPT_POST, 1L);
    curl_easy_setopt(s_curl, CURLOPT_POSTFIELDS, "");
    
    CURLcode res = curl_easy_perform(s_curl);
    if (res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }
}
