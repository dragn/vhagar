#pragma once

#include "Modules/Renderer3D_GL/ResourceTypes/RMesh_GL.hpp"
#include "Modules/standard.hpp"

namespace vh {

namespace Utils {

/**
 * Imports *.obj file
 */
bool ImportWavefront(vh::RMesh_GL* mesh, const char* filename);

} // namespace Utils

} // namespace vh
