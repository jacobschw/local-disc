#ifndef LOCAL_DISC_NAME_API_H
#define LOCAL_DISC_NAME_API_H

// Define LOCAL_DISC_LIB_API for exporting/importing symbols
#ifdef _WIN32
    #ifdef LOCAL_DISC_LIB_EXPORTS // This is defined by CMake when building the DLL
        #define LOCAL_DISC_LIB_API __declspec(dllexport)
    #else
        #define LOCAL_DISC_LIB_API __declspec(dllimport) // For consumers (less relevant for FFI loading)
    #endif
#else // Non-Windows (GCC/Clang)
    #define LOCAL_DISC_LIB_API __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

// --- Example API Functions ---

LOCAL_DISC_LIB_API int initialize_engine(const char* db_connection_string);

LOCAL_DISC_LIB_API const char* get_engine_version(); // C++ allocates, returns pointer to static string or managed buffer

// For results that need to be freed by the caller
LOCAL_DISC_LIB_API char* execute_query(const char* custom_query); // C++ allocates, Rust/Go calls free_query_result
LOCAL_DISC_LIB_API void free_query_result(char* result);

// For long-running operations like import, you might consider callbacks
typedef void (*ImportProgressCallback)(int percentage, const char* message);
LOCAL_DISC_LIB_API int start_import(const char* platform_name, const char* dump_file_path, ImportProgressCallback callback);


#ifdef __cplusplus
} // extern "C"
#endif

#endif // LOCAL_DISC_NAME_API_H