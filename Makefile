# Makefile for Library Server Project

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
DEBUG_FLAGS = -DDEBUG -g -O0
RELEASE_FLAGS = -O2 -DNDEBUG

# Library paths for compilation (already included above)

# Linker flags
LDFLAGS = -L/opt/homebrew/lib -lcjson -lmongoc-1.0 -lbson-1.0 -lcurl -ljson-c

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
TEST_DIR = tests

# Library flags
MONGO_FLAGS = `pkg-config --cflags --libs libmongoc-1.0 libbson-1.0`
CURL_FLAGS = `pkg-config --cflags --libs libcurl`
JSON_FLAGS = `pkg-config --cflags --libs json-c`
CJSON_FLAGS = -I/opt/homebrew/include -L/opt/homebrew/lib -lcjson

# Include directories
INCLUDES = -I$(INCLUDE_DIR) -I/opt/homebrew/include -I/opt/homebrew/include/libmongoc-1.0 -I/opt/homebrew/include/libbson-1.0

# Source files
MAIN_SRC = $(SRC_DIR)/main.c
DB_SOURCES = $(SRC_DIR)/database/mongodb_client.c $(SRC_DIR)/database/book_operations.c
SERVER_SOURCES = $(SRC_DIR)/server/socket_server.c $(SRC_DIR)/server/client_handler.c
API_SOURCES = $(SRC_DIR)/api/openlibrary_client.c
PROTOCOL_SOURCES = $(SRC_DIR)/protocol/message_parser.c $(SRC_DIR)/protocol/response_builder.c
UTILS_SOURCES = $(SRC_DIR)/utils/json_parser.c $(SRC_DIR)/utils/logger.c $(SRC_DIR)/utils/string_utils.c

# Object files
MAIN_OBJ = $(BUILD_DIR)/main.o
DB_OBJECTS = $(BUILD_DIR)/mongodb_client.o $(BUILD_DIR)/book_operations.o
SERVER_OBJECTS = $(BUILD_DIR)/socket_server.o $(BUILD_DIR)/client_handler.o
API_OBJECTS = $(BUILD_DIR)/openlibrary_client.o
PROTOCOL_OBJECTS = $(BUILD_DIR)/message_parser.o $(BUILD_DIR)/response_builder.o
UTILS_OBJECTS = $(BUILD_DIR)/json_parser.o $(BUILD_DIR)/logger.o $(BUILD_DIR)/string_utils.o

ALL_OBJECTS = $(MAIN_OBJ) $(DB_OBJECTS) $(SERVER_OBJECTS) $(API_OBJECTS) $(PROTOCOL_OBJECTS) $(UTILS_OBJECTS)

# Object files
MAIN_OBJ = $(BUILD_DIR)/main.o
DB_OBJECTS = $(BUILD_DIR)/mongodb_client.o $(BUILD_DIR)/book_operations.o
SERVER_OBJECTS = $(BUILD_DIR)/socket_server.o $(BUILD_DIR)/client_handler.o
API_OBJECTS = $(BUILD_DIR)/openlibrary_client.o
PROTOCOL_OBJECTS = $(BUILD_DIR)/message_parser.o $(BUILD_DIR)/response_builder.o
UTILS_OBJECTS = $(BUILD_DIR)/json_parser.o $(BUILD_DIR)/logger.o $(BUILD_DIR)/string_utils.o

ALL_OBJECTS = $(MAIN_OBJ) $(DB_OBJECTS) $(SERVER_OBJECTS) $(API_OBJECTS) $(PROTOCOL_OBJECTS) $(UTILS_OBJECTS)

# Target executables
MAIN_TARGET = $(BUILD_DIR)/library-server
TEST_TARGET = $(BUILD_DIR)/test_mongodb

# Default target
all: $(MAIN_TARGET)

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean target (remove build files)
clean:
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/*~ $(BUILD_DIR)/.*~
	rm -f $(MAIN_TARGET) $(TEST_TARGET)

# Format target (requires clang-format)
format:
	find $(SRC_DIR) $(INCLUDE_DIR) -name "*.c" -o -name "*.h" | xargs clang-format -i

# Debug build
debug: CFLAGS += $(DEBUG_FLAGS)
debug: all

# Release build
release: CFLAGS += $(RELEASE_FLAGS)
release: all

# Install target (modify paths as needed)
install: $(MAIN_TARGET)
	install -d $(DESTDIR)/usr/local/bin
	install -m 755 $(MAIN_TARGET) $(DESTDIR)/usr/local/bin/

.PHONY: all clean format debug release install

# Main application
$(MAIN_TARGET): $(BUILD_DIR) $(ALL_OBJECTS)
	$(CC) $(ALL_OBJECTS) -o $@ $(MONGO_FLAGS) $(CURL_FLAGS) $(JSON_FLAGS) $(LDFLAGS)

# Individual object files
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/mongodb_client.o: $(SRC_DIR)/database/mongodb_client.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/book_operations.o: $(SRC_DIR)/database/book_operations.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/socket_server.o: $(SRC_DIR)/server/socket_server.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/client_handler.o: $(SRC_DIR)/server/client_handler.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/openlibrary_client.o: $(SRC_DIR)/api/openlibrary_client.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/message_parser.o: $(SRC_DIR)/protocol/message_parser.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/response_builder.o: $(SRC_DIR)/protocol/response_builder.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/json_parser.o: $(SRC_DIR)/utils/json_parser.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/logger.o: $(SRC_DIR)/utils/logger.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/string_utils.o: $(SRC_DIR)/utils/string_utils.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Test programs
test: $(TEST_TARGET)

$(TEST_TARGET): $(BUILD_DIR) $(DB_OBJECTS) $(UTILS_OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) $(TEST_DIR)/test_mongodb.c $(DB_OBJECTS) $(UTILS_OBJECTS) -o $@ $(MONGO_FLAGS)

# Quick build for current development (only database modules)
quick: $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $(SRC_DIR)/main.c $(DB_SOURCES) -o $(BUILD_DIR)/mongo_app $(MONGO_FLAGS)

# Debug build
debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(MAIN_TARGET)

# Release build
release: CFLAGS += $(RELEASE_FLAGS)
release: $(MAIN_TARGET)

# Run the application
run: $(MAIN_TARGET)
	./$(MAIN_TARGET)

# Run tests
run-test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Run with valgrind for memory leak detection
valgrind: $(MAIN_TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(MAIN_TARGET)

# Clean build files
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(SRC_DIR)/*.o $(SRC_DIR)/*/*.o

# Clean logs
clean-logs:
	rm -f logs/*.log

# Full clean
clean-all: clean clean-logs

# Install dependencies (Ubuntu/Debian)
install-deps:
	sudo apt-get update
	sudo apt-get install -y libmongoc-1.0-dev libbson-1.0-dev libcurl4-openssl-dev libjson-c-dev

# Help
help:
	@echo "Available targets:"
	@echo "  all          - Build main application"
	@echo "  quick        - Quick build (only database modules)"
	@echo "  test         - Build test programs"
	@echo "  clean        - Remove all build files"
	@echo "  debug        - Build with debug symbols"
	@echo "  release      - Build optimized release version"
	@echo "  install      - Install the application"
	@echo "  format       - Format C code using clang-format"
	@echo "  help         - Show this help message"
	@echo "  valgrind     - Run with valgrind"
	@echo "  clean-logs   - Clean log files"
	@echo "  clean-all    - Clean everything"
	@echo "  install-deps - Install dependencies"

# Phony targets
.PHONY: all test quick debug release run run-test valgrind clean clean-logs clean-all install-deps help