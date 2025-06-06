#ifndef MONGODB_CLIENT_H
#define MONGODB_CLIENT_H

#include <mongoc/mongoc.h>

mongoc_collection_t* connect_to_mongodb();

#endif