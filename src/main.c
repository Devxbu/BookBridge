#include "../include/database/mongodb_client.h"
#include "../include/database/book_operations.h"

int main(){
    mongoc_collection_t* collection = connect_to_mongodb();
    add_book(collection, "test", "test", "test", 2020, 100, "test", "test", "test", "test");
    list_books(collection);
    print_book(collection, "68422bcef7f7694f490633d1");
    delete_book(collection, "68422bf1c993bd6204030271");
    update_book(collection, "68422bcef7f7694f490633d1", "test", "test", "test", 2020, 100, "test", "test", "test", "test");
    return 0;
}

