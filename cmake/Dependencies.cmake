set(Dependencies_LINKER_LIBS "")

find_package(PQXX REQUIRED)
include_directories(SYSTEM ${PQXX_INCLUDE_DIRECTORIES})
list(APPEND Dependencies_LINKER_LIBS ${PQXX_LIBRARIES})


find_package(nlohmann_json 3.2.0 REQUIRED)
IF (${nlohmann_json_FOUND})
    MESSAGE(STATUS "nlohmann_json found !")
ELSE (${nlohmann_json_FOUND})
    set(JSON_BuildTests OFF CACHE INTERNAL "")
    execute_process(
        COMMAND bash "-c" "wget https://github.com/nlohmann/json.git"
    )
    add_subdirectory(json)
ENDIF (${nlohmann_json_FOUND})

list(APPEND Dependencies_LINKER_LIBS nlohmann_json::nlohmann_json)

find_package(Mapnik REQUIRED)
include_directories(${MAPNIK_INCLUDE_DIRS})
set(CMAKE_MODULE_LINKER_FLAGS ${MAPNIK_LDFLAGS})
set(CMAKE_SHARED_LINKER_FLAGS ${MAPNIK_LDFLAGS})
list(APPEND Dependencies_LINKER_LIBS ${MAPNIK_LIBRARIES})

find_package(ICU REQUIRED)
include_directories(${ICU_INCLUDE_DIRS})
list(APPEND Dependencies_LINKER_LIBS ${ICU_LIBRARIES})