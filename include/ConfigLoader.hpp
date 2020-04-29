#ifndef CONFIG_LOADER_HPP
#define CONFIG_LOADER_HPP

#include <nlohmann/json.hpp>
#include <string>
#include <fstream>

class ConfigLoader {
public:
  ConfigLoader() {}
  ConfigLoader(const std::string &_path) { load(_path); }

  void load(const std::string &_path);

  std::string getDataSourcePath() const & { return dataSourcesPath; }
  std::string getPsqlDb() const & { return psqlDb; }
  std::string getPsqlUser() const & { return psqlUser; }
  std::string getPsqlPass() const & { return psqlPass; }

private:
  std::string dataSourcesPath;
  std::string psqlDb, psqlUser, psqlPass;
};

#endif /* CONFIG_LOADER_HPP */