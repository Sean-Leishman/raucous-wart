#include "renderer.hpp"

int main()
{
  std::string filename(
      "/home/seanleishman/University/cg/cw2/materials/binary_primitves.json");
  Renderer renderer;
  renderer.load_file(filename);
  renderer.render_frame();

  return 0;
}
