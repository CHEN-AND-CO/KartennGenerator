#include "KartennGenerator.hpp"
#include <string>

#define isCharArrayNumeric(x)                                                  \
  std::all_of(std::string(x).begin(), std::string(x).end(), ::isdigit)

#define DEFAULT_DATA_SOURCES_PATH "/usr/lib/mapnik/input"
#define DEFAULT_MAP_PATH "data/finistere/finistere.xml"
#define DEFAULT_OUTPUT_PATH "finistere.png"
#define DEFAULT_WIDTH 7020  // A1 300pp
#define DEFAULT_HEIGHT 9930 // A1 300pp
#define DEFAULT_TOWN "Landerneau"

#define PSQL_HOST "217.182.204.82"
#define PSQL_DB "ohmybzh"
#define PSQL_USER "admin"
#define PSQL_PWD "G6hY75t"

int main(int argc, char **argv) {
  int width = DEFAULT_WIDTH, height = DEFAULT_HEIGHT;
  std::string dataSourcesPath = DEFAULT_DATA_SOURCES_PATH,
              mapPath = DEFAULT_MAP_PATH, outputPath = DEFAULT_OUTPUT_PATH,
              town = DEFAULT_TOWN;

   if ( argc < 4) {
        std::cerr << "Error: A township name or postcode must be provided\n";
        std::cerr << "Usage: rendermap <name|postcode> <model.xml> <output.png> [<size_x> <size_y>]\n";
        exit(-1);
    }

  switch (argc) {
  case 6:
    if (isCharArrayNumeric(argv[5]))
      height = std::atoi(argv[5]);
  case 5:
    if (isCharArrayNumeric(argv[4]))
      width = std::atoi(argv[4]);
  case 4:
    outputPath = argv[3];
  case 3:
    mapPath = argv[2];
  case 2:
    town = argv[1];
    break;
  default:
    break;
  }

  KartennGenerator kg;
  kg.setCred(PSQL_HOST, PSQL_DB, PSQL_USER, PSQL_PWD);
  kg.setModel(mapPath);
  kg.setSize(width, height);
  kg.render(town, outputPath);

  return 0;
}