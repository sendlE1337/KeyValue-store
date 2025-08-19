#ifndef CONFIG_CLASS_H
#define CONFIG_CLASS_H

#include "../external/json.hpp"
#include <string>
#include <array>
class Config
{
public:
  Config();
  explicit Config(const std::string &filename);

  bool is_valid() const;
  void reset_to_default();
  void save() const;

  std::string eviction_policy() const;
  size_t cache_limit() const;

  size_t getCacheLimit() const;
  std::string getEvictionPolicy() const;

private:
  nlohmann::json config_data_;
  std::string file_path_;

  bool validate_policy(const std::string &policy) const;
  bool validate_cache_limit(int limit) const;
};
#endif