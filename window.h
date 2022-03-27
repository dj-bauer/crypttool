/* Copyright (C) 2022 Daniel Bauer- All Rights Reserved
 *
 * This file is part of DJ-Crypt.
 * DJ-Crypt is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 * DJ-Crypt is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with Foobar.
 * If not, see <https://www.gnu.org/licenses/>.
 */
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
