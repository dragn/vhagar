#pragma once

#include "../Renderable/Mesh.hpp"

namespace vh {

namespace Utils {

/**
 * Imports *.obj file
 */
bool ImportWavefront(vh::Mesh* mesh, const char* filename);

} // namespace Utils

} // namespace vh
