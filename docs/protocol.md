# Book Bridge Protocol Documentation

This document explains the communication protocol between the Book Bridge server and clients.

## General Information

- **Protocol**: TCP Socket üzerinden JSON messages
- **Port**: 8080 (default, configurable)
- **Character Encoding**: UTF-8
- **Message Format**: JSON

## Connection Establishment

Client connects to the server via TCP socket:

```c
// Example connection
int sock = socket(AF_INET, SOCK_STREAM, 0);
struct sockaddr_in server_addr;
server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(8080);
server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
```

## Message Structure

### Request Messages

All request messages have the following structure:

```json
{
  "action": "action_name",
  "data": {
    // action specific parameters
  }
}
```

### Response Messages

All response messages have the following structure:

```json
{
  "status": "success|error",
  "message": "Status açıklaması",
  "data": {
    // response specific data
  }
}
```

## Supported Commands

### 1. List Books

**Request:**
```json
{
  "action": "list_books"
}
```

**Response (Başarılı):**
```json
{
  "status": "success",
  "message": "Books retrieved successfully",
  "data": {
    "books": [
      {
        "id": "60f1b2b3c4d5e6f7g8h9i0j1",
        "title": "The Great Gatsby",
        "author": "F. Scott Fitzgerald",
        "isbn": "9780743273565",
        "publication_year": 1925,
        "pages": 180,
        "added_date": "2024-01-15T10:30:00Z"
      },
      {
        "id": "60f1b2b3c4d5e6f7g8h9i0j2",
        "title": "To Kill a Mockingbird",
        "author": "Harper Lee",
        "isbn": "9780061120084",
        "publication_year": 1960,
        "pages": 376,
        "added_date": "2024-01-16T14:20:00Z"
      }
    ],
    "total_count": 2
  }
}
```

**Response (Hata):**
```json
{
  "status": "error",
  "message": "Database connection failed",
  "data": {}
}
```

### 2. Get Book Details

**Request:**
```json
{
  "action": "get_book",
  "data": {
    "book_id": "60f1b2b3c4d5e6f7g8h9i0j1"
  }
}
```

**Response (Başarılı):**
```json
{
  "status": "success",
  "message": "Book retrieved successfully",
  "data": {
    "book": {
      "id": "60f1b2b3c4d5e6f7g8h9i0j1",
      "title": "The Great Gatsby",
      "author": "F. Scott Fitzgerald",
      "isbn": "9780743273565",
      "publication_year": 1925,
      "pages": 180,
      "description": "A classic American novel set in the Jazz Age...",
      "publisher": "Charles Scribner's Sons",
      "language": "English",
      "added_date": "2024-01-15T10:30:00Z",
      "updated_date": "2024-01-15T10:30:00Z"
    }
  }
}
```

**Response (Kitap Bulunamadı):**
```json
{
  "status": "error",
  "message": "Book not found",
  "data": {}
}
```

### 3. Delete Book

**Request:**
```json
{
  "action": "delete_book",
  "data": {
    "book_id": "60f1b2b3c4d5e6f7g8h9i0j1"
  }
}
```

**Response (Başarılı):**
```json
{
  "status": "success",
  "message": "Book deleted successfully",
  "data": {
    "deleted_book_id": "60f1b2b3c4d5e6f7g8h9i0j1"
  }
}
```

**Response (Kitap Bulunamadı):**
```json
{
  "status": "error",
  "message": "Book not found",
  "data": {}
}
```

### 4. Add Book

**Request:**
```json
{
  "action": "add_book",
  "data": {
    "title": "1984",
    "author": "George Orwell"
  }
}
```

**Response (Başarılı):**
```json
{
  "status": "success",
  "message": "Book added successfully",
  "data": {
    "book": {
      "id": "60f1b2b3c4d5e6f7g8h9i0j3",
      "title": "1984",
      "author": "George Orwell",
      "isbn": "9780451524935",
      "publication_year": 1949,
      "pages": 328,
      "description": "A dystopian social science fiction novel...",
      "publisher": "Secker & Warburg",
      "language": "English",
      "added_date": "2024-01-17T16:45:00Z"
    }
  }
}
```

**Response (Open Library'de Bulunamadı):**
```json
{
  "status": "error",
  "message": "Book not found in Open Library",
  "data": {}
}
```

**Response (Zaten Mevcut):**
```json
{
  "status": "error",
  "message": "Book already exists in database",
  "data": {
    "existing_book_id": "60f1b2b3c4d5e6f7g8h9i0j1"
  }
}
```

## Error Codes

| Error Code | Description |
|------------|-------------|
| 400 | Invalid JSON format |
| 401 | Invalid action |
| 404 | Kaynak bulunamadı |
| 409 | Kaynak zaten mevcut |
| 500 | Sunucu hatası |
| 503 | Veritabanı bağlantı hatası |
| 504 | Open Library API timeout |

## Connection Management

### Connection Closure

Client can close the connection:
```json
{
  "action": "disconnect"
}
```

Server response:
```json
{
  "status": "success",
  "message": "Connection closed successfully",
  "data": {}
}
```

### Heartbeat

Uzun süreli bağlantılar için heartbeat:
```json
{
  "action": "ping"
}
```

Server response:
```json
{
  "status": "success",
  "message": "pong",
  "data": {
    "server_time": "2024-01-17T16:45:00Z"
  }
}
```

## Limits

- **Maximum message size**: 4096 bytes
- **Maximum concurrent connections**: 50 (configurable)
- **Connection timeout**: 300 seconds
- **Rate limiting**: 60 requests per connection per minute

## Example Client Code

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        return -1;
    }
    
    // Kitapları listele
    char request[] = "{\"action\": \"list_books\"}";
    send(sock, request, strlen(request), 0);
    
    char response[4096];
    int bytes_received = recv(sock, response, sizeof(response), 0);
    response[bytes_received] = '\0';
    
    printf("Response: %s\n", response);
    
    close(sock);
    return 0;
}
```

## Security Notes

- SSL/TLS support is not implemented yet
- Authentication system is not implemented yet
- Security measures should be implemented in production

## Version History

- **v1.0**: Initial version - Basic CRUD operations
- **v1.1**: Planned - SSL/TLS support
- **v1.2**: Planned - Authentication system
