#pragma once
#include <windows.h>
#include "ggl.h"
#include "Camera.hpp"

void Init(OpenGL::Camera* InCam);
void SetViewportSize(float width, float height);
void Draw();
