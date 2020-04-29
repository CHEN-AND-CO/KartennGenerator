#include "ConfigLoader.hpp"

void ConfigLoader::load(const std::string &_path) {
  std::ifstream file{_path};

  if (!file) {
    throw "Error : Cannot open " + _path + " ...";
  }

  nlohmann::json json;
  file >> json;

  dataSourcesPath = json["DATASOURCES_PATH"].get<std::string>();
  psqlDb = json["PSQL_DB"].get<std::string>();
  psqlUser = json["PSQL_USER"].get<std::string>();
  psqlPass = json["PSQL_PASS"].get<std::string>();
}