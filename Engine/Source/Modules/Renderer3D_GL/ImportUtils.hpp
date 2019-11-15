#pragma once

#include "Modules/Renderer3D_GL/GLResources/GLMesh.hpp"
#include "Modules/standard.hpp"

namespace vh {

namespace Utils {

/**
 * Imports *.obj file
 */
bool ImportWavefront(vh::GLMesh* mesh, const char* filename);

} // namespace Utils

} // namespace vh
