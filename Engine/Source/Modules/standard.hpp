#pragma once

//////////////////////////////////////////////////////////////////////////
// Inherit Core standard header
#include "Core/standard.hpp"

//////////////////////////////////////////////////////////////////////////
// Standard C++
#include <map>
#include <queue>
#include <sstream>
#include <iterator>
#include <fstream>
#include <limits>
#include <mutex>
#include <forward_list>
#include <regex>

//////////////////////////////////////////////////////////////////////////
// Platform-specific
#if CMAKE_PLATFORM_WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#elif CMAKE_PLATFORM_UNIX
#define GetCurrentDir getcwd
#endif

//////////////////////////////////////////////////////////////////////////
// VhCore
#include "Core/App/App.hpp"
#include "Core/App/Module.hpp"
#include "Core/Actor/Actor.hpp"
#include "Core/Actor/ActorBehavior.hpp"
#include "Core/Actor/ActorFactory.hpp"
#include "Core/Factory.hpp"
#include "Core/Modules/MWorld.hpp"

//////////////////////////////////////////////////////////////////////////
// VhUtils
#include "Utils/FileSystem.hpp"
#include "Utils/GLUtils.hpp"
#ifdef WITH_PHYSX
#include "Utils/PhysXUtils.hpp"
#endif
#include "Utils/Network/Socket.hpp"
#include "Utils/Network/TcpServer.hpp"
#include "Utils/Network/TcpLogSink.hpp"
