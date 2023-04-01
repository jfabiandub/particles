// Bryn Mawr College, alinen, 2020
//

#include <cmath>
#include <string>
#include <vector>
#include "agl/window.h"

using namespace std;
using namespace glm;
using namespace agl;

class Viewer : public Window {
public:
  Viewer() : Window() {
  }

  void setup() {
    setWindowSize(1000, 1000);
    renderer.loadShader("simple-texture",
      "../shaders/simple-texture.vs",
      "../shaders/simple-texture.fs");

    img.load("../textures/tree.png", true);
    renderer.loadTexture("tree", img, 0);
    // TODO: Use the width and the height of the image to scale the billboard

    renderer.loadTexture("grass", "../textures/grass.png", 0);
    renderer.blendMode(agl::BLEND);
  }


     void mouseMotion(int x, int y, int dx, int dy) {
      
      if (checker) { 
        // Convert pixel movement to rotation angles
         float sensitivity = 0.2f;
         float azimuthDelta = dx * sensitivity;
         float elevationDelta = dy * sensitivity;

        // Update camera position
          azimuth += azimuthDelta;
          elevation += elevationDelta;

        // Keep elevation within [-90, 90] degrees
         elevation = glm::clamp(elevation, -90, 90);

        // Convert spherical coordinates to cartesian coordinates
         float x = radius * glm::sin(glm::radians(static_cast<float>(azimuth))) * glm::cos(glm::radians(static_cast<float>(elevation)));
         float y = radius * glm::sin(glm::radians(static_cast<float>(elevation)));
         float z = radius * glm::cos(glm::radians(static_cast<float>(azimuth))) * glm::cos(glm::radians(static_cast<float>(elevation)));
         eyePos = vec3(x, y, z);
      }
   }


   void mouseDown(int button, int mods) {
      checker = true;
   }

   void mouseUp(int button, int mods) {
      checker = false;
   }

  void scroll(float dx, float dy) {
  }

  void draw() {
    renderer.beginShader("simple-texture");

    float aspect = ((float)width()) / height();
    renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);
    renderer.lookAt(eyePos, lookPos, up);

    // draw plane
    renderer.texture("Image", "grass");
    renderer.push();
    renderer.translate(vec3(0.0, -0.5, 0));
    renderer.scale(vec3(2.0f));
    renderer.plane();
    renderer.pop();

    // draw tree
    renderer.texture("Image", "tree");
    renderer.push();
    //renderer.scale();
    vec3 stable = normalize(eyePos - lookPos);
    renderer.rotate(atan2(stable.x, stable.z),vec3(0.0, 1.0, 0.0));
    renderer.translate(vec3(-0.5, -0.5, 0));
    renderer.scale(vec3(1.0, (float)img.height()/(float)img.width(), 1.0));
    renderer.quad(); // vertices span from (0,0,0) to (1,1,0)
    renderer.pop();

    renderer.endShader();
  }

protected:

  vec3 eyePos = vec3(0, 0, 2);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);

   float azimuth = 0;
   int elevation = 0;
   float radius = 10;
    Image img;
   bool checker = false;
};

int main(int argc, char** argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}
