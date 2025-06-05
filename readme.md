# Book Bridge

A socket-based library management system developed in C. Users can connect to the server via TCP socket and perform book operations.

## Features

- **Book Listing**: List all books in the database
- **Book Details**: View detailed information of a specific book
- **Deleting a Book**: Delete existing books from the database
- **Adding a Book**: Adding new books using the Open Library API

## Technologies

- **Programming Language**: C
- **Database**: MongoDB
- **Network Communication**: TCP Socket
- **External API**: Open Library API
- **JSON Processing**: JSON-C (or similar)
- **HTTP Client**: libcurl

## System Requirements

### Required Libraries
```bash
# Ubuntu/Debian
sudo apt-get install libmongoc-1.0-dev libbson-1.0-dev
sudo apt-get install libcurl4-openssl-dev
sudo apt-get install libjson-c-dev

# CentOS/RHEL
sudo yum install mongo-c-driver-devel
sudo yum install libcurl-devel
sudo yum install json-c-devel
```

### MongoDB Setup
```bash
# Install and run MongoDB on your system
sudo systemctl start mongod
sudo systemctl enable mongod
```

## Installation

1. Clone the project:
``bash
git clone <repository-url>
cd library-server
```

2. Compile the project:
```bash
make
```

3. Edit the configuration file:
``bash
cp config/server.conf.example config/server.conf
# edit server.conf file according to your needs
```

4. Start the server:
```bash
./build/library-server
```

## Configuration

You can configure the following settings in the `config/server.conf` file:

```ini
# Server settings
SERVER_PORT=8080
MAX_CLIENTS=50

# MongoDB settings
MONGODB_URI=mongodb://localhost:27017
DATABASE_NAME=library_db
COLLECTION_NAME=books

# Open Library API
OPENLIBRARY_BASE_URL=https://openlibrary.org/api

# Logging
LOG_LEVEL=INFO
LOG_FILE=logs/server.log
```

## Usage

### Server Start
```bash
./build/library-server
```

### Client Connection
Any TCP client can connect to the server:

```bash
# Telnet test
telnet localhost 8080

# Netcat test
nc localhost 8080
```

### Command Examples

```json
// List all books
{"action": "list_books"}

// Get book details
{"action": "get_book", "book_id": "60f1b2b3c4d5e6f7g8h9i0j1"}

// Delete a book
{"action": "delete_book", "book_id": "60f1b2b3c4d5e6f7g8h9i0j1"}

// Add a book
{"action": "add_book", "title": "The Great Gatsby", "author": "F. Scott Fitzgerald"}
```

## Project Structure

```
library-server/
├── src/           # Kaynak kodlar
├── include/       # Header dosyaları
├── tests/         # Test dosyaları
├── build/         # Derleme çıktıları
├── logs/          # Log files
├── config/        # Configuration files
└── docs/          # Documentation
```

## Development

### Debug Build
```bash
make debug
```

### Run Tests
```bash
make test
```

### Memory Leak Control
```bash
make valgrind
```

### Clean
```bash
make clean
```

## API Protocol

Detailed protocol information can be found in the [protocol.md](docs/protocol.md) file.

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push the branch (`git push origin feature/amazing-feature`)
5. Create a Pull Request

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contact

Project owner: [Your Name]
Email: [your.email@example.com]

## Known Issues

- [ ] Performance issues with many concurrent connections
- [ ] Open Library API rate limiting not implemented
- [ ] SSL/TLS support not implemented

## Future Features

- [ ] SSL/TLS support
- [ ] User authentication system
- [ ] Book categories
- [ ] Advanced search features
- [ ] Web interface