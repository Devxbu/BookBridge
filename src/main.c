#include "../include/database/mongodb_client.h"
#include "../include/database/book_operations.h"
#include "../include/api/openlibrary_client.h"
int main(){
    char *title = "The Hobbit";
    cJSON* book = get_book_info(title);
    get_infos_and_save_db(book);
    return 0;
}

