/* A precompiled header for the game */

#pragma once

/* Include the engine precompiled header */
#include "Engine.h"

/* Other engine includes */
#include "Engine/Application/IApp.h"
#include "Engine/Application/Window.h"

#include "Engine/Render/Render.h"

#include "Engine/Application/Input/InputManager.h"
#include "Engine/Application/Input/KeyCodes.h"

/* Project Specific includes */
#include "Scene/Scene.h"
#include "Scene/GameObject.h"
#include "Scene/Components/Components.h"
#include "Scene/Script.h"

#include "GameSettings.h"

namespace GameObject
{
	inline Settings g_Settings;
}//namespace EntityGame

using namespace Engine;