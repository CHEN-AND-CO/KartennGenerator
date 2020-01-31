#include <string>

#include <mapnik/agg_renderer.hpp>
#include <mapnik/datasource_cache.hpp>
#include <mapnik/image.hpp>
#include <mapnik/image_util.hpp>
#include <mapnik/load_map.hpp>
#include <mapnik/map.hpp>
#include <mapnik/plugin.hpp>

#define isCharArrayNumeric(x) \
  std::all_of(std::string(x).begin(), std::string(x).end(), ::isdigit)

#define DEFAULT_DATA_SOURCES_PATH "/usr/lib/mapnik/3.0/input"
#define DEFAULT_MAP_PATH "data/world/world.xml"
#define DEFAULT_OUTPUT_PATH "our_world.png"
#define DEFAULT_WIDTH 7020   // A1 300pp
#define DEFAULT_HEIGHT 9930  // A1 300pp

int main(int argc, char **argv) {
  int width = DEFAULT_WIDTH, height = DEFAULT_HEIGHT;
  std::string dataSourcesPath = DEFAULT_DATA_SOURCES_PATH,
              mapPath = DEFAULT_MAP_PATH, outputPath = DEFAULT_OUTPUT_PATH;

  switch (argc) {
    case 6:
      dataSourcesPath = argv[5];
    case 5:
      if (isCharArrayNumeric(argv[4])) height = std::atoi(argv[4]);
    case 4:
      if (isCharArrayNumeric(argv[3])) width = std::atoi(argv[3]);
      break;
    case 3:
      mapPath = argv[2];
    case 2:
      outputPath = argv[1];
    default:
      break;
  }

  std::cout << "[mapnik] Setting datasource cache from " << dataSourcesPath
            << " ..." << std::endl;
  mapnik::datasource_cache::instance().register_datasources(dataSourcesPath);

  std::cout << "[mapnik] Loading Map (" << width << "x" << height << ") from "
            << mapPath << " ..." << std::endl;
  mapnik::Map map{width, height};
  mapnik::load_map(map, mapPath);
  map.zoom_all();

  std::cout << "[mapnik] Rendering image (" << width << "x" << height << ") ..."
            << std::endl;
  mapnik::image_rgba8 image{width, height};
  mapnik::agg_renderer<mapnik::image_rgba8> renderer{map, image};
  renderer.apply();

  std::cout << "[mapnik] Saving file to " << outputPath << " ..." << std::endl;
  mapnik::save_to_file(image, outputPath);

  return 0;
}