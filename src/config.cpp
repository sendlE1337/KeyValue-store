#include "../include/cache/config.h"
#include <fstream>
#include <iostream>

namespace
{
  constexpr std::array<std::string_view, 3> SUPPORTED_POLICIES{"LRU", "LFU", "FIFO"};
  constexpr int MAX_CACHE_SIZE = 10000;
}

Config::Config() = default;
Config::Config(const std::string &filename) : file_path_(filename)
{
  std::ifstream file(file_path_);
  if (!file.is_open())
  {
    throw std::runtime_error("Cannot open config file: " + file_path_);
  }
  file >> config_data_;

  if (!config_data_.contains("cache_limit") || !config_data_.contains("evictionPolicy"))
  {
    reset_to_default();
  }
}

bool Config::is_valid() const
{
  return validate_cache_limit(config_data_["cache_limit"]) &&
         validate_policy(config_data_["evictionPolicy"]);
}

void Config::reset_to_default()
{
  config_data_["cache_limit"] = 1000;
  config_data_["evictionPolicy"] = "LRU";
  save();
}

void Config::save() const
{
  std::ofstream file(file_path_);
  if (!file.is_open())
  {
    throw std::runtime_error("Failed to save config to: " + file_path_);
  }
  file << config_data_.dump(4);
}

// Приватные методы валидации
bool Config::validate_policy(const std::string &policy) const
{
  std::string upper_policy;
  std::transform(policy.begin(), policy.end(),
                 std::back_inserter(upper_policy), ::toupper);

  return std::find(SUPPORTED_POLICIES.begin(),
                   SUPPORTED_POLICIES.end(),
                   upper_policy) != SUPPORTED_POLICIES.end();
}

bool Config::validate_cache_limit(int limit) const
{
  return limit > 0 && limit <= MAX_CACHE_SIZE;
}

size_t Config::getCacheLimit() const
{
  return config_data_["cache_limit"];
}
std::string Config::getEvictionPolicy() const
{
  return config_data_["evictionPolicy"];
}
