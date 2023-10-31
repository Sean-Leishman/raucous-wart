#include <fstream>
#include <iostream>
#include <json.hpp>

class JSONParser
{
  private:
  nlohmann::json jsonData;

  public:
  JSONParser(const std::string& filename)
  {
    std::ifstream file(filename);
    if (file.is_open())
    {
      file >> jsonData;
    }
  }
};
