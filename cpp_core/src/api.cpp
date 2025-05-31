#include "local_disc/api.h" // Use quotes for local project headers
#include <string>
#include <vector>
#include <cstring> // For strcpy, strlen
#include <iostream> // For cout (temporary)

// --- Internal State (Example) ---
static std::string G_DB_CONNECTION_STRING;
static bool G_INITIALIZED = false;

// --- API Function Implementations ---

LOCAL_DISC_LIB_API int initialize_engine(const char* db_connection_string) {
    if (db_connection_string == nullptr) {
        return -1; // Error: null connection string
    }
    G_DB_CONNECTION_STRING = db_connection_string;
    G_INITIALIZED = true;
    std::cout << "[C++] Engine initialized with: " << G_DB_CONNECTION_STRING << std::endl;
    // TODO: Actually connect to PostgreSQL here using libpqxx
    return 0; // Success
}

LOCAL_DISC_LIB_API const char* get_engine_version() {
    // For const char* returned directly, it usually points to a static string
    // or a buffer managed by the C++ side that lives long enough.
    // Be careful with memory management if this string is dynamic.
    static const char* version = "0.1.0-alpha";
    return version;
}

LOCAL_DISC_LIB_API char* execute_query(const char* custom_query) {
    if (!G_INITIALIZED || custom_query == nullptr) {
        return nullptr;
    }
    std::cout << "[C++] Executing query: " << custom_query << std::endl;

    // --- Simulate a result ---
    std::string result_str = "Result for query: ";
    result_str += custom_query;
    result_str += " (from C++)";

    // Allocate memory that the caller (Rust/Go via FFI) will free using free_query_result
    // It's crucial that the allocation/deallocation mechanism is consistent if crossing module boundaries
    // on Windows (e.g. both use same C runtime). Using `new char[]` and `delete[] char` is common.
    // `malloc` and `free` are also options if you want to be C-runtime agnostic.
    char* c_result = new char[result_str.length() + 1];
    #ifdef _MSC_VER
        strcpy_s(c_result, result_str.length() + 1, result_str.c_str());
    #else
        strcpy(c_result, result_str.c_str());
    #endif

    return c_result;
}

LOCAL_DISC_LIB_API void free_query_result(char* result) {
    if (result != nullptr) {
        std::cout << "[C++] Freeing query result." << std::endl;
        delete[] result; // Must match the allocation in execute_query
    }
}

LOCAL_DISC_LIB_API int start_import(const char* platform_name, const char* dump_file_path, ImportProgressCallback callback) {
    if (!G_INITIALIZED || platform_name == nullptr || dump_file_path == nullptr) {
        return -1; // Error
    }
    std::cout << "[C++] Starting import for " << platform_name << " from " << dump_file_path << std::endl;

    // Simulate import progress
    for (int i = 0; i <= 100; i += 10) {
        // Simulate work
        #ifdef _WIN32
            // Sleep(50); // Windows
        #else
            // usleep(50000); // POSIX
        #endif

        if (callback) {
            std::string msg = "Processing step " + std::to_string(i/10);
            callback(i, msg.c_str());
        }
    }

    if (callback) {
        callback(100, "Import complete.");
    }
    std::cout << "[C++] Import finished." << std::endl;
    return 0; // Success
}