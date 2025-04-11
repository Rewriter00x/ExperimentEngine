#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <filesystem>
#include <typeindex>

#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <sstream>
#include <fstream>
#undef _CRT_SECURE_NO_WARNINGS

#include "ExperimentForward.h"
#include "ExperimentGlobal.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/rotate_vector.hpp"
#undef GLM_ENABLE_EXPERIMENTAL

#include "Engine/System/Miscellaneous/IntegerTypes.h"
#include "Engine/System/Miscellaneous/PtrTypes.h"

#include "Engine/System/Random.h"
#include "Engine/System/UUID.h"

#include "Engine/System/AssetManager/AssetManager.h"
#include "Engine/System/Log/Log.h"

#include "Engine/System/Macro/CoreMacro.h"
#include "Engine/System/Macro/Assert.h"

#include "Engine/System/KeyCodes.h"
#include "Engine/System/MouseCodes.h"

#include "Engine/System/Input.h"

#include "Engine/Events/Event.h"
#include "Engine/Events/EventDispatcher.h"

#include "Engine/Events/KeyEvents.h"
#include "Engine/Events/MouseEvents.h"
#include "Engine/Events/WindowEvents.h"

#include "Engine/System/Module.h"
#include "Engine/System/ModuleManager.h"
#include "Engine/System/Application.h"
#include "Engine/System/Window.h"

#include "Engine/ECS/World.h"
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/Components/ComponentUtils.h"
