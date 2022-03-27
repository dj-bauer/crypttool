#include <GLFW/glfw3.h> // Will drag system OpenGL headers

using namespace std;

namespace Window
{
extern GLFWwindow* window;

void glfw_error_callback(int error, const char* description);

int init();
void update();
void render();
void stop();
}
