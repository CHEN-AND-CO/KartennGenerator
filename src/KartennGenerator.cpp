#include "KartennGenerator.hpp"

std::string KartennGenerator::getBboxExtent(std::string _townName)
{
  std::string bbox;

  try
  {
    pqxx::connection db{"user=" + user + " password=" + password +
                        " dbname=" + database};
    pqxx::work query{db};

    pqxx::result res =
        query.exec(getTownSQLReq(MARGINS_M, TOWNSHIP_ADMINLVL, _townName));

    std::cout << "Returned " << res.columns() << " columns and " << res.size()
              << " rows" << std::endl;

    if (res.size() > 0 && !res[0][0].is_null())
      bbox = res[0][0].as<std::string>();
    else
      throw std::runtime_error("Couldn't find this township in the database.");
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << '\n';
    std::cerr << ">> INFO:\n\tdb_user:\t" << user << "\n\tdb_passwd:\t"
              << password << "\n\tdb_name:\t" << database << "\n\ttownship:\t"
              << _townName << "\n";
    exit(-2);
  }

  return bbox;
}

std::vector<double> KartennGenerator::bboxToMapExtent(std::string _bbox)
{
  std::vector<double> mapExtent;
  _bbox = _bbox.substr(4, _bbox.length() - 5);
  std::cout << "Bounding box: " << _bbox << "\n";

  for (auto &coord : split(_bbox, " ,"))
  {
    mapExtent.push_back(std::stod(coord));
  }

  return mapExtent;
}

std::string KartennGenerator::getTownSQLReq(int _margin, std::string _adminLvl,
                                            std::string _townName)
{
  std::stringstream query;
  query << "SELECT ST_Expand(ST_Extent(way), " << _margin
        << ") as bbox FROM planet_osm_polygon WHERE admin_level = '"
        << _adminLvl << "' AND boundary = 'administrative' AND (name LIKE '%"
        << _townName << "%' OR (tags->\"ref:INSEE\") LIKE '" << _townName << "')";

  return query.str();
}

void KartennGenerator::createDbSettings()
{
  std::string path{
      model.substr(
          0, model.length() -
                 std::filesystem::path(model).filename().string().length()) +
      "db_settings"};
  std::ofstream out(path);

  auto lp = "<Parameter name =\"", mp = "\">", rp = "</Parameter>";

  out << lp << "type" << mp << "postgis" << rp << std::endl;
  out << lp << "host" << mp << "localhost" << rp << std::endl;
  out << lp << "user" << mp << user << rp << std::endl;
  out << lp << "dbname" << mp << database << rp << std::endl;
  out << lp << "password" << mp << password << rp << std::endl;
  out.flush();
}

void KartennGenerator::render(std::string _townName, std::string _output)
{
  auto mapExtent = bboxToMapExtent(getBboxExtent(_townName));

  std::cout << "Rendering map...\n";

  // Register Mapnik's fonts and plugins
  mapnik::datasource_cache::instance().register_datasources(dataSourcePath);
  mapnik::freetype_engine::register_font(
      "/usr/share/fonts/truetype/dejavu/DejaVuSansCondensed.ttf");

  // Set the map size and projection
  mapnik::Map m(width, height);
  mapnik::load_map(m, model);

  m.set_srs(MAPNIK_SRS);

  m.zoom_to_box(mapnik::box2d<double>(mapExtent[0], mapExtent[1], mapExtent[2],
                                      mapExtent[3]));

  std::cout << "Layers :\n";
  for (auto &&layer : m.layers())
  {
    std::cout << "\t" << layer.name() << "\n";
  }
  std::cout << "Scale: 1:" << m.scale() << "\n";

  // Rendering
  mapnik::image_rgba8 im(width, height);
  mapnik::agg_renderer<mapnik::image_rgba8> ren(m, im);
  ren.apply();
  mapnik::save_to_file(im, _output);

  std::cout << "Done !\n";
}

void KartennGenerator::setCred(std::string _d, std::string _u, std::string _p)
{
  database = _d;
  user = _u;
  password = _p;
}
void KartennGenerator::setSize(int _w, int _h)
{
  width = _w;
  height = _h;
}