#include "../../include/api/openlibrary_client.h"

size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    size_t totalSize = size * nmemb;
    struct Memory *mem = (struct Memory *)userdata;

    char *temp = realloc(mem->response, mem->size + totalSize + 1);
    if (temp == NULL)
        return 0;

    mem->response = temp;
    memcpy(&(mem->response[mem->size]), ptr, totalSize);
    mem->size += totalSize;
    mem->response[mem->size] = '\0';

    return totalSize;
}

void free_memory(struct Memory *mem)
{
    if (mem->response)
    {
        free(mem->response);
        mem->response = NULL;
        mem->size = 0;
    }
}

cJSON *get_book_info(char *title)
{
    CURL *curl = curl_easy_init();
    struct Memory chunk = {NULL, 0};
    cJSON *result = NULL;

    if (!curl)
    {
        fprintf(stderr, "Curl init failed\n");
        goto cleanup;
    }

    // Kitap başlığı URL için encode edilir
    char *escaped_title = curl_easy_escape(curl, title, 0);
    if (!escaped_title)
    {
        fprintf(stderr, "Title escaping failed\n");
        goto cleanup;
    }

    char url[512];
    snprintf(url, sizeof(url), "https://openlibrary.org/api/books?bibkeys=Title:%s&format=json&jscmd=data", escaped_title);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        goto cleanup;
    }

    // JSON parse
    cJSON *json = cJSON_Parse(chunk.response);
    if (!json)
    {
        fprintf(stderr, "Error parsing JSON\n");
        goto cleanup;
    }

    char book_key[256];
    snprintf(book_key, sizeof(book_key), "Title:%s", title);

    cJSON *book_obj = cJSON_GetObjectItem(json, book_key);
    if (!book_obj)
    {
        fprintf(stderr, "Book not found in JSON\n");
        cJSON_Delete(json);
        goto cleanup;
    }

    result = cJSON_Duplicate(book_obj, 1);
    cJSON_Delete(json);

cleanup:
    if (curl)
        curl_easy_cleanup(curl);
    if (escaped_title)
        curl_free(escaped_title);
    free_memory(&chunk);
    return result;
}

void get_infos_and_save_db(cJSON *book)
{
    if (!book)
    {
        fprintf(stderr, "Book info could not be retrieved\n");
        return;
    }

    cJSON *title = cJSON_GetObjectItem(book, "title");
    cJSON *number_of_pages = cJSON_GetObjectItem(book, "number_of_pages");
    cJSON *authors = cJSON_GetObjectItem(book, "authors");
    cJSON *isbn = cJSON_GetObjectItem(book, "identifiers");
    cJSON *publish_date = cJSON_GetObjectItem(book, "publish_date");
    cJSON *publishers = cJSON_GetObjectItem(book, "publishers");
    cJSON *cover = cJSON_GetObjectItem(book, "cover");

    if (title && title->valuestring ||
        number_of_pages && cJSON_IsNumber(number_of_pages) ||
        authors && cJSON_IsArray(authors) ||
        isbn && cJSON_GetObjectItem(isbn, "isbn_10") ||
        publish_date && publish_date->valuestring ||
        publishers && cJSON_IsArray(publishers) ||
        cover)
    {

        // Title
        const char *title = cJSON_GetObjectItem(book, "title") ? cJSON_GetObjectItem(book, "title")->valuestring : "Unknown";

        // Author
        const char *author = "Unknown";
        cJSON *authors = cJSON_GetObjectItem(book, "authors");
        if (authors && cJSON_IsArray(authors))
        {
            cJSON *first_author = cJSON_GetArrayItem(authors, 0);
            if (first_author && cJSON_GetObjectItem(first_author, "name"))
            {
                author = cJSON_GetObjectItem(first_author, "name")->valuestring;
            }
        }

        // ISBN
        const char *isbn = "Unknown";
        cJSON *identifiers = cJSON_GetObjectItem(book, "identifiers");
        if (identifiers && cJSON_GetObjectItem(identifiers, "isbn_10"))
        {
            cJSON *isbn_array = cJSON_GetObjectItem(identifiers, "isbn_10");
            if (cJSON_IsArray(isbn_array) && cJSON_GetArrayItem(isbn_array, 0))
            {
                isbn = cJSON_GetArrayItem(isbn_array, 0)->valuestring;
            }
        }

        // Year
        int year = 0;
        const char *date_str = cJSON_GetObjectItem(book, "publish_date") ? cJSON_GetObjectItem(book, "publish_date")->valuestring : NULL;
        if (date_str)
        {
            year = atoi(date_str); // Sadece yıl için daha iyileştirme yapılabilir
        }

        // Pages
        int pages = 0;
        if (cJSON_GetObjectItem(book, "number_of_pages"))
        {
            pages = cJSON_GetObjectItem(book, "number_of_pages")->valueint;
        }

        // Publisher
        const char *publisher = "Unknown";
        cJSON *publishers = cJSON_GetObjectItem(book, "publishers");
        if (publishers && cJSON_IsArray(publishers) && cJSON_GetArrayItem(publishers, 0))
        {
            publisher = cJSON_GetArrayItem(publishers, 0)->valuestring;
        }

        // Cover
        const char *cover_url = "Unknown";
        cJSON *cover = cJSON_GetObjectItem(book, "cover");
        if (cover && cJSON_GetObjectItem(cover, "medium"))
        {
            cover_url = cJSON_GetObjectItem(cover, "medium")->valuestring;
        }

        // Veritabanına ekle
        add_book(connect_to_mongodb(), (char *)title, (char *)author, (char *)isbn, year, pages, (char *)publisher, (char *)cover_url);

        printf("Book added to database\n");
    }
    else
    {
        printf("Incomplete book data, skipping database insert.\n");
    }

    cJSON_Delete(book);
}
