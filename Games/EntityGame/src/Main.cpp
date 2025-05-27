#include "Game.h"

#include "EntityGameMain.h"

#ifdef E_DEBUG
int main()
{
	std::shared_ptr<EntityGame::EntityGameMain> game = std::make_shared<EntityGame::EntityGameMain>();
	Engine::Application::GetInstance().RunApplication(game);
	return 0;
}
#elif E_RELEASE
#include <Windows.h>
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	std::shared_ptr<EntityGame::EntityGameMain> game = std::make_shared<EntityGame::EntityGameMain>();
	Engine::Application::GetInstance().RunApplication(game);
	return 0;
}
#endif
