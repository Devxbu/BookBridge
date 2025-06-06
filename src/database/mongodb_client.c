#include <stdio.h>
#include <stdlib.h>
#include <mongoc/mongoc.h>
#include "../../include/database/mongodb_client.h"

mongoc_collection_t* connect_to_mongodb() {
    mongoc_init();

    const char *uri_string = "mongodb://localhost:27017";
    mongoc_client_t *client = mongoc_client_new(uri_string);
    mongoc_collection_t *collection = mongoc_client_get_collection(client, "book_bridge", "books");

    return collection;
}
