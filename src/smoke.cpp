// Bryn Mawr College, Jfabiandub 2023
//

/* FOR A SMOKE, PARTICLES THAT GROW IN SIZE AND INCREASE IN TRANSPARENCY
*/
#include <cmath>
#include <string>
#include <vector>
#include "agl/window.h"

using namespace std;
using namespace glm;
using namespace agl;

struct Particle {
  glm::vec3 pos;
  glm::vec3 vel;
  glm::vec4 color;
  float rot;
  float size;
  float life;
};

class Viewer : public Window {
public:
  Viewer() : Window() {
  }

  void setup() {
    setWindowSize(1000, 1000);
    renderer.setDepthTest(false);
    renderer.blendMode(agl::BLEND); //was blend
   
  }

  void createParticles(int size)
  {
    renderer.loadTexture("particle", "../textures/smoke7.png", 0);
    srand(time(0));
    for (int i = 0; i < size; i++)
    {
      Particle particle;
      particle.color = vec4(1, 1, 1, agl::random(0.2, 1.0)); // Random transparency
      particle.size = agl::random(0.01, 0.03); // Random size
      particle.rot = agl::randomUnitCube().x * 360.0;
      particle.pos = position; // Start at the center
      particle.vel = agl::randomUnitCube() * 0.1f; // Random velocity
      particle.life = 0.0;
      mParticles.push_back(particle);
    }
  }

  void updateSmoke()
  {
    for (int i = 0; i < mParticles.size(); i++)
    {
      float n = (float)rand() / RAND_MAX;
      Particle &particle = mParticles[i];
      particle.pos += particle.vel * dt(); // Update position based on velocity
      particle.size += (0.01 + 0.05 * n) * dt(); // Increase size randomly
      particle.color.a -= 0.2f * clamp(n, 0.01f, 1.5f) * dt(); // Decrease transparency
      particle.vel += agl::randomUnitCube() * 0.01f; // Spread out more in all directions
      particle.vel.y = abs(particle.vel.y);
      particle.vel.x = clamp(particle.vel.x, -0.02f, 0.02f);
      particle.vel.z = clamp(particle.vel.z, -0.04f, 0.04f);

      if (particle.color.a <= 0.0 || particle.pos.y > 1.0f)
      {
        // Reset particle if transparency reaches 0 or goes above screen
        particle.color = vec4(1, 1, 1, agl::random(0.2, 1.0)); // Random transparency
        particle.size = agl::random(0.01, 0.03); // Random size
        particle.rot = agl::randomUnitCube().x * 360.0;
        particle.pos = position; // Start at the center
        particle.vel = agl::randomUnitCube() * 0.1f; // Random velocity
        particle.life = 0.0;
      }
    }
  }


  void drawSmoke()
  {
    renderer.texture("image", "particle");
    for (int i = 0; i < mParticles.size(); i++)
    {
      Particle particle = mParticles[i];
      renderer.sprite(particle.pos, particle.color, particle.size, particle.rot);
    }
  }

  void mouseMotion(int x, int y, int dx, int dy) {
  }

  void mouseDown(int button, int mods) {
  }

  void mouseUp(int button, int mods) {
  }

  void scroll(float dx, float dy) {
    eyePos.z += dy;
  }

  void keyUp(int key, int mods) {
  }

  void draw() {

    renderer.beginShader("sprite");

    float aspect = ((float)width()) / height();
    renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);
    vec3 r = agl::randomUnitCube() * 0.25f;
    position = vec3(r.x, 0, 0);
    renderer.lookAt(eyePos, lookPos, up);
    //renderer.sprite(position, vec4(1.0f), 0.25f);
    if(mParticles.size() < 75){
      createParticles(1);
    }
    updateSmoke();
    drawSmoke();
    renderer.endShader();
  }

protected:

  vec3 eyePos = vec3(0, 0, 3);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);
  vec3 position = vec3(1, 0, 0);
  int frames = 0;

  std::vector<Particle> mParticles;
};

int main(int argc, char** argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}
