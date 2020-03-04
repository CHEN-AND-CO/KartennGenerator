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
#define DEFAULT_MAP_PATH "data/finistere/finistere.xml"
#define DEFAULT_OUTPUT_PATH "finistere.png"
#define DEFAULT_WIDTH 7020   // A1 300pp
#define DEFAULT_HEIGHT 9930  // A1 300pp

int main(int argc, char **argv) {
  int width = DEFAULT_WIDTH, height = DEFAULT_HEIGHT;
  std::string dataSourcesPath = DEFAULT_DATA_SOURCES_PATH,
              mapPath = DEFAULT_MAP_PATH, outputPath = DEFAULT_OUTPUT_PATH;

  switch (argc) {
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

  mapnik::parameters p;
    p["type"] = "postgis";
  p["host"] = "localhost";
  p["port"] = "5432";
  p["dbname"] = "etdm1";
  p["user"] = "etdm1";
  p["password"] = "chenco2020";

  std::cout << "[mapnik] Setting datasource cache from " << dataSourcesPath
            << " ..." << std::endl;
  mapnik::datasource_cache::instance().create(p);

  std::cout << "[mapnik] Loading Map (" << width << "x" << height << ") from "
            << mapPath << " ..." << std::endl;
  mapnik::Map map{width, height};
  mapnik::load_map(map, mapPath);
  map.set_srs(
      "+proj=merc +a=6378137 +b=6378137 +lat_ts=0.0 +lon_0=0.0 +x_0=0.0 +y_0=0 "
      "+k=1.0 +units=m +nadgrids=@null +wktext +no_defs");
  map.zoom_to_box(
      mapnik::box2d<double>(-471920.2, 6091823.2, -442437.7, 6109042.6));
  std::cout << "Scale: 1:" << map.scale() <<  std::endl;

  std::cout << "[mapnik] Rendering image (" << width << "x" << height << ") ..."
            << std::endl;
  mapnik::image_rgba8 image{width, height};
  mapnik::agg_renderer<mapnik::image_rgba8> renderer{map, image};
  renderer.apply();

  std::cout << "[mapnik] Saving file to " << outputPath << " ..." << std::endl;
  mapnik::save_to_file(image, outputPath);

  std::cout << "Done !" << std::endl;

  return 0;
}