set(Dependencies_LINKER_LIBS "")

find_package(PQXX REQUIRED)
include_directories(SYSTEM ${PQXX_INCLUDE_DIRECTORIES})
list(APPEND Dependencies_LINKER_LIBS ${PQXX_LIBRARIES})

find_package(nlohmann_json REQUIRED)
list(APPEND Dependencies_LINKER_LIBS nlohmann_json::nlohmann_json)

find_package(Mapnik REQUIRED)
include_directories(${MAPNIK_INCLUDE_DIRS})
set(CMAKE_MODULE_LINKER_FLAGS ${MAPNIK_LDFLAGS})
set(CMAKE_SHARED_LINKER_FLAGS ${MAPNIK_LDFLAGS})
list(APPEND Dependencies_LINKER_LIBS ${MAPNIK_LIBRARIES})

find_package(ICU REQUIRED)
include_directories(${ICU_INCLUDE_DIRS})
list(APPEND Dependencies_LINKER_LIBS ${ICU_LIBRARIES})