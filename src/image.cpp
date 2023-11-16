#include "image.hpp"

#include <fstream>
#include <vector>

PPMImage::PPMImage(int width, int height, int max_color_value)
    : width(width), height(height), max_color_value(max_color_value)
{
  data.resize(width, std::vector<PPMColor>(height));
}

void PPMImage::set_pixel(int x, int y, int r, int g, int b)
{
  PPMColor color = PPMColor{r, g, b};
  set_pixel(x,y , color);
}

void PPMImage::set_pixel(int x, int y, PPMColor color){
  if (x < 0 || x >= width || y < 0 || y >= height)
    return;
  data[y][x] = color;
}

PPMColor PPMImage::get_pixel(float u, float v) const {
  int x = u * width;
  int y = v * height;
  return data[y][x];
}

bool PPMImage::save_to_file(const std::string& filename)
{
  std::ofstream image_file(filename);
  if (!image_file.is_open())
  {
    std::cerr << "Failed to open file for writing." << std::endl;
    return false;
  }

  // Write header
  image_file << "P3\n";
  image_file << width << " " << height << "\n";
  image_file << max_color_value << "\n";

  // Write pixel data
  for (int y = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
      image_file << data[height - y - 1][width - x - 1] << "\n";
    }
  }

  image_file.close();
  return true;
}

bool PPMImage::read_from_file(const std::string& filename){
  std::ifstream file(filename, std::ios::binary);
  if (!file.is_open()){
    std::cerr << "failed to open file: " << filename << std::endl;
    return false;
  }

  std::string line;
  std::getline(file, line);
  bool p6 = false;
  if (line == "P6") {
    p6 = true;
  }

  // Skip comments
  while (file.peek() == '#') {
    std::getline(file, line);
  }
  // Read image dimensions
  file >> width >> height;

  // Read max color value (assuming it's 255)
  int maxVal;
  file >> maxVal;

  // Skip the remaining whitespace
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  // Read pixel data
  data.resize(height);
  std::vector<unsigned char> out(width * height * 3);
  if (p6)
  {
    file.read(reinterpret_cast<char*>(out.data()), out.size());
  }

  for (int y=0; y<height; ++y){
    data[y].resize(width);
    if (p6)
    {
      for (int x=0; x < width; ++x){
        int index = (y * width + x) * 3;
        PPMColor color{static_cast<float>(out[index]/maxVal), static_cast<float>(out[index+1]/maxVal), static_cast<float>(out[index+2]/maxVal)};

        data[y][x] = color;
      }
    }
    else {
      for (int x=0; x<width; ++x){
        float r, g, b;
        file >> r >> g >> b;
        PPMColor color{r/max_color_value,g/max_color_value,b/max_color_value};
        data[y][x] = color;
      }
    }
  }

  if (!file) {
    std::cerr << "Error occurred while reading the PPM file" << std::endl;
    return false;
  }

  file.close();
  save_to_file("/home/seanleishman/University/cg/cw2/materials/Martini2.ppm");
  return true;


}