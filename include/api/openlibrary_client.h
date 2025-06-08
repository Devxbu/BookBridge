#ifndef OPENLIBRARY_CLIENT_H
#define OPENLIBRARY_CLIENT_H

#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../database/mongodb_client.h"
#include "../database/book_operations.h"

struct Memory {
    char *response;
    size_t size;
};

size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata);

cJSON* get_book_info(char* title);

void get_infos_and_save_db(cJSON* book);

#endif
