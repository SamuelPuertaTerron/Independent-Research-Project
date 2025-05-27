/* The main entry for this game */
#include "Game.h"
#include "GameObjectGame.h"

#ifdef E_DEBUG
int main()
{
	std::shared_ptr<GameObject::GameObjectGame> game = std::make_shared<GameObject::GameObjectGame>();
	Engine::Application::GetInstance().RunApplication(game);
	return 0;
}
#elif E_RELEASE
#include <Windows.h>
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	std::shared_ptr<GameObject::GameObjectGame> game = std::make_shared<GameObject::GameObjectGame>();
	Engine::Application::GetInstance().RunApplication(game);
	return 0;
}
#endif
