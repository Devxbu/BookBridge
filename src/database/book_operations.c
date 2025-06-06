#include "../../include/database/book_operations.h"

int add_book(mongoc_collection_t *collection, char* title, char* author, char* isbn, int publication_year, int pages, char* description, char* publisher, char* language, char* cover_url) {
    bson_t *doc = BCON_NEW(
        "title", BCON_UTF8(title),
        "author", BCON_UTF8(author),
        "isbn", BCON_UTF8(isbn),
        "publication_year", BCON_INT32(publication_year),
        "pages", BCON_INT32(pages),
        "description", BCON_UTF8(description),
        "publisher", BCON_UTF8(publisher),
        "language", BCON_UTF8(language),
        "cover_url", BCON_UTF8(cover_url)
    );

    if (!mongoc_collection_insert_one(collection, doc, NULL, NULL, NULL)) {
        fprintf(stderr, "Kitap eklenemedi.\n");
        return 0;
    } else {
        printf("Kitap başarıyla eklendi.\n");
        return 1;
    }
    bson_destroy(doc);
}

void list_books(mongoc_collection_t *collection) {
    bson_t *query = bson_new();
    mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);
    const bson_t *doc;

    while (mongoc_cursor_next(cursor, &doc)) {
        char *str = bson_as_canonical_extended_json(doc, NULL);
        printf("%s\n", str);
        bson_free(str);
    }

    bson_destroy(query);
    mongoc_cursor_destroy(cursor);
}

void print_book(mongoc_collection_t *collection, const char *id_str) {
    bson_oid_t oid;
    bson_oid_init_from_string(&oid, id_str);

    bson_t *query = bson_new();
    BSON_APPEND_OID(query, "_id", &oid);

    mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);
    const bson_t *doc;

    if (mongoc_cursor_next(cursor, &doc)) {
        char *str = bson_as_canonical_extended_json(doc, NULL);
        printf("%s\n", str);
        bson_free(str);
    } else {
        fprintf(stderr, "Kitap bulunamadı.\n");
    }

    bson_destroy(query);
    mongoc_cursor_destroy(cursor);
}

int update_book(mongoc_collection_t *collection, const char *id_str, char* title, char* author, char* isbn, int publication_year, int pages, char* description, char* publisher, char* language, char* cover_url) {
    bson_oid_t oid;
    bson_oid_init_from_string(&oid, id_str);

    bson_t *query = bson_new();
    BSON_APPEND_OID(query, "_id", &oid);

    bson_t *update = BCON_NEW(
        "$set", "{",
        "title", BCON_UTF8(title),
        "author", BCON_UTF8(author),
        "isbn", BCON_UTF8(isbn),
        "publication_year", BCON_INT32(publication_year),
        "pages", BCON_INT32(pages),
        "description", BCON_UTF8(description),
        "publisher", BCON_UTF8(publisher),
        "language", BCON_UTF8(language),
        "cover_url", BCON_UTF8(cover_url),
        "}"
    );

    if (!mongoc_collection_update_one(collection, query, update, NULL, NULL, NULL)) {
        fprintf(stderr, "Güncelleme başarısız.\n");
        return 0;
    } else {
        printf("Kitap güncellendi.\n");
        return 1;
    }

    bson_destroy(query);
    bson_destroy(update);
}

int delete_book(mongoc_collection_t *collection, const char *id_str) {
    bson_oid_t oid;
    bson_oid_init_from_string(&oid, id_str);

    bson_t *query = bson_new();
    BSON_APPEND_OID(query, "_id", &oid);

    if (!mongoc_collection_delete_one(collection, query, NULL, NULL, NULL)) {
        fprintf(stderr, "Silme işlemi başarısız.\n");
        return 0;
    } else {
        printf("Kitap silindi.\n");
        return 1;
    }

    bson_destroy(query);
}