/* A precompiled header for the game */

#pragma once

/* std includes */
#include <typeindex>

/* Include the engine precompiled header */
#include "Engine.h"

/* Other engine includes */
#include "Engine/Application/IApp.h"
#include "Engine/Application/Window.h"

#include "Engine/Render/Render.h"

#include "Engine/Application/Input/InputManager.h"

/* Project Includes */
#include "ECS/Entity.h"
#include "ECS/Components/Components.h"
#include "ECS/Registry.h"

#include "Scene/Scene.h"

#include "GameSettings.h"

namespace EntityGame
{
	inline Settings g_Settings;
}//namespace EntityGame

using namespace Engine;