#pragma once

#include "Modules/Renderer/Renderable/Mesh.hpp"
#include "Modules/standard.hpp"

namespace vh {

namespace Utils {

/**
 * Imports *.obj file
 */
bool ImportWavefront(vh::Mesh* mesh, const char* filename);

} // namespace Utils

} // namespace vh
