#pragma once

#include "ExperimentForward.h"

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <filesystem>

#include <array>
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include <fstream>
#undef _CRT_SECURE_NO_WARNINGS

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Engine/System/Miscellaneous/IntegerTypes.h"
#include "Engine/System/Miscellaneous/PtrTypes.h"

#include "Engine/System/Log/Log.h"

#include "Engine/System/Macro/CoreMacro.h"
#include "Engine/System/Macro/Assert.h"

#include "Engine/System/KeyCodes.h"
#include "Engine/System/MouseCodes.h"

#include "Engine/Events/Event.h"
#include "Engine/Events/EventDispatcher.h"

#include "Engine/Events/KeyEvents.h"
#include "Engine/Events/MouseEvents.h"
#include "Engine/Events/WindowEvents.h"

#include "Engine/System/Application.h"
#include "Engine/System/Window.h"
