#ifndef OHMYBZH_GENERATOR_HPP
#define OHMYBZH_GENERATOR_HPP

#include <mapnik/agg_renderer.hpp>
#include <mapnik/datasource_cache.hpp>
#include <mapnik/image.hpp>
#include <mapnik/image_util.hpp>
#include <mapnik/layer.hpp>
#include <mapnik/load_map.hpp>
#include <mapnik/map.hpp>
#include <mapnik/plugin.hpp>
#include <mapnik/proj_transform.hpp>
#include <mapnik/projection.hpp>
#include <pqxx/pqxx>

#include <exception>
#include <string>
#include <vector>
#include <filesystem>
#include <iostream>
#include <fstream>

#include "Tools.hpp"

#define WIDTH 8000
#define HEIGHT 5000
#define MARGINS_M 1000
#define TOWNSHIP_ADMINLVL "8"

#define MAPNIK_SRS                                                           \
  "+proj=merc +a=6378137 +b=6378137 +lat_ts=0.0 +lon_0=0.0 +x_0=0.0 +y_0=0 " \
  "+k=1.0 +units=m +nadgrids=@null +wktext +no_defs"

class KartennGenerator
{
public:
  KartennGenerator() {}

  std::string getBboxExtent(std::string _townName);
  std::vector<double> bboxToMapExtent(std::string _bbox);

  std::string getTownSQLReq(int _margin, std::string _adminLvl,
                            std::string _townName);

  void setDataSourcePath(std::string _d) { dataSourcePath = _d; }
  void setModel(std::string _m) { model = _m; }
  void setCred(std::string _d, std::string _u, std::string _p);
  void setSize(int _w, int _h);

  void createDbSettings();
  void render(std::string _townName, std::string _output);

private:
  std::string dataSourcePath;
  std::string model;
  std::string database, user, password;
  int width = WIDTH, height = HEIGHT;
};

#endif /* OHMYBZH_GENERATOR_HPP */