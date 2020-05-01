#include "ConfigLoader.hpp"
#include "KartennGenerator.hpp"
#include <string>

#define isCharArrayNumeric(x)                                                  \
  std::all_of(std::string(x).begin(), std::string(x).end(), ::isdigit)

#define DEFAULT_CONFIG_PATH "config.json"

#define DEFAULT_WIDTH 7020  // A1 300pp
#define DEFAULT_HEIGHT 9930 // A1 300pp

int main(int argc, char **argv) {
  ConfigLoader conf;

  try {
    conf.load(DEFAULT_CONFIG_PATH);
  } catch (std::string error) {
    std::cerr << error << std::endl;

    return -2;
  }

  int width = DEFAULT_WIDTH, height = DEFAULT_HEIGHT;
  std::string mapPath, outputPath, town;

  if (argc < 4) {
    std::cerr << "Error: A township name or postcode must be provided\n";
    std::cerr << "Usage: rendermap <name|postcode> <model.xml> <output.png> "
                 "[<size_x> <size_y>]\n";
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
  kg.setCred(conf.getPsqlDb(), conf.getPsqlUser(), conf.getPsqlPass());
  kg.setDataSourcePath(conf.getDataSourcePath());
  kg.setModel(mapPath);
  kg.createDbSettings();
  kg.setSize(width, height);
  kg.render(town, outputPath);

  return 0;
}