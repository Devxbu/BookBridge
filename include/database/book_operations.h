#ifndef BOOK_OPERATIONS_H
#define BOOK_OPERATIONS_H

#include <mongoc/mongoc.h>

int add_book(mongoc_collection_t *collection, char* title, char* author, char* isbn, int publication_year, int pages, char* description, char* publisher, char* language, char* cover_url);
void list_books(mongoc_collection_t *collection);
void print_book(mongoc_collection_t *collection, const char *id_str);
int update_book(mongoc_collection_t *collection, const char *id_str, char* title, char* author, char* isbn, int publication_year, int pages, char* description, char* publisher, char* language, char* cover_url);
int delete_book(mongoc_collection_t *collection, const char *id_str);

#endif