## Project: LocalDisc

**One-Liner:** A C++ powered data ingestion and custom query engine for local MusicBrainz and Discogs data dumps, exposed via a REST API.

### Project Goal

The primary goal of this project is to create a robust and efficient local system for music enthusiasts and developers to explore and analyze rich metadata from two major music information platforms: **MusicBrainz** and **Discogs**.

This involves two main components:

1.  **Data Ingestion & Storage:**
    *   Develop C++ tools to parse and import the publicly available data dumps from MusicBrainz (JSON format) and Discogs (XML format).
    *   Store this processed and structured data into a locally managed PostgreSQL database.
    *   The schema should be designed to effectively represent and potentially link information from both sources.

2.  **Custom Querying & API Access:**
    *   Design and implement a custom, high-level query language (QL) specifically tailored for exploring the music metadata stored in the database. This QL aims to be more intuitive for music-related queries than raw SQL.
    *   Develop a C++ query engine that parses this custom QL, translates it into efficient SQL queries, and executes them against the PostgreSQL database.
    *   Expose both the data ingestion functionality and the custom query engine via a RESTful API. This server component is planned to be implemented in Rust or Go for performance and modern API development practices.

**Key Features & Motivations:**

*   **Local Data Ownership:** Allows users to have a complete, local copy of these vast datasets for offline access, complex analysis, and custom tooling.
*   **Unified Access:** Provide a single point of access to data that might otherwise require interacting with two different APIs or data formats.
*   **Simplified Querying:** Offer a domain-specific query language that abstracts away SQL complexities for common music metadata lookups and explorations.
*   **Performance:** Leverage C++ for core data processing and querying for efficiency, especially with large datasets.
*   **Cross-Platform:** The core C++ library and the REST server are intended to be cross-platform (Windows, macOS, Linux).

### Current Project Status

**Last Updated:** \[2025-05-31]

**Overall Progress:** Initial Setup & C++ Core Foundation Established.

**Key Milestones Achieved:**

1.  **Project Scaffolding (C++ Core):**
    *   A cross-platform CMake build system for the core C++ shared library (`local_disc_core_lib` or `your_core_lib`) has been successfully configured.
    *   The project structure for the C++ library, including public API headers (`api.h`) and source file organization, is in place.
    *   Basic C-style API functions have been defined in `api.h` and stubbed out in `api.cpp` for initializing the engine, executing a placeholder query, and managing results.
    *   The C++ shared library successfully compiles on Windows (using MSVC via VSCode and CMake Tools). Linux and macOS compatibility is planned but not yet fully tested in a CI environment.
    *   The build output directory (`build/`) is correctly excluded from source control via `.gitignore`.

**Next Immediate Steps (Short-Term Focus):**

1.  **C++ Core - Database Interaction:**
    *   Integrate the `libpqxx` C++ library for PostgreSQL interaction.
    *   Implement the `initialize_engine` function in `api.cpp` to establish a connection to a PostgreSQL database.
    *   Define an initial version of the PostgreSQL database schema (`db_schema/postgres_schema.sql`) to store basic entities from Discogs (e.g., releases, artists).

2.  **C++ Core - Discogs Importer (Initial Pass):**
    *   Integrate an XML parsing library (e.g., `PugiXML`).
    *   Begin implementation of the Discogs data dump parser, focusing on extracting and mapping core entities (e.g., releases, artists, labels) to the defined database schema.
    *   Implement the `start_import` C API function to trigger the Discogs import process.

3.  **Testing Framework:**
    *   Set up a C++ unit testing framework (e.g., GoogleTest) and write initial tests for the API and database connection logic.

**Planned Future Work (Longer-Term):**

*   **C++ Core - Discogs Importer:** Develop the parser and importer for MusicBrainz JSON data dumps.
*   **C++ Core - Custom Query Language (QL):**
    *   Formalize the grammar and syntax of the custom QL.
    *   Implement the QL parser (e.g., using ANTLR or a custom parser).
    *   Develop the QL execution engine to translate QL queries into SQL.
*   **REST API Server (Rust/Go):**
    *   Choose between Rust and Go for the server implementation.
    *   Set up FFI (Foreign Function Interface) bindings to call the C++ shared library from the chosen server language.
    *   Develop REST API endpoints for triggering imports and executing queries.
*   **Advanced Database Schema:** Refine the database schema to handle complex relationships, linking between Discogs and MusicBrainz entities, and optimizing for query performance.
*   **Documentation:** Comprehensive documentation for the custom QL, REST API, and setup instructions.
*   **Cross-Platform Testing & CI/CD:** Implement a continuous integration pipeline to ensure builds and tests pass on Windows, macOS, and Linux.

**Open Questions / Design Considerations (for AI or human contributors):**

*   What are the most critical entities and relationships from Discogs and MusicBrainz to prioritize for the initial database schema and importers?
*   What specific query patterns or use cases should the custom query language be optimized for?
*   What are best practices for managing large, potentially long-running import operations asynchronously via the REST API?
*   Considering the FFI between C++ and Rust/Go, what are the most robust strategies for error handling and memory management for complex data structures returned by the query engine?

---

**How to use this resume:**

*   When asking an AI for help, you can preface your question with "Considering the project LocalDisc with the following status: \[paste the status section here], how would I..." This gives the AI crucial context.