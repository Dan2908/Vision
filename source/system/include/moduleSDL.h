#pragma once
#include <thirdparty.h>

namespace Vision
{
namespace System 
{

enum class eActions
{
	INVALID = -1,
	NONE,
	QUIT,
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Event 
{
	SDL_Event mEvent;
	eActions mAction;
	inline const eActions FetchAction()
	{
		if (mEvent.type == SDL_QUIT)
		{
			return eActions::QUIT;
		}
		if (mEvent.type == SDL_KEYDOWN)
		{
			switch (mEvent.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				return eActions::QUIT;
			case SDLK_UP:
			case SDLK_w:
				return eActions::UP;
			case SDLK_DOWN:
			case SDLK_s:
				return eActions::DOWN;
			case SDLK_LEFT:
			case SDLK_a:
				return eActions::LEFT;
			case SDLK_RIGHT:
			case SDLK_d:
				return eActions::RIGHT;
			default:
				return eActions::INVALID;
			}
		}
		return eActions::NONE;
	}

public:
	void UpdateAction();
	inline eActions GetAction() { return mAction; }
	inline SDL_Event& GetSDLEvent() { return mEvent; }
};

class Window
{
	SDL_Window* mWindow;
	SDL_Surface* mSurface;
	void SetGLAttributes();
public:
	Window(const int width, const int height, const char* title = "New window");
	~Window();
	SDL_Window* operator&();
	void Swap() const;
};

/**
 * @brief Singleton class to manage input events from the user.
 */
class EventManager
{
	static EventManager mInstance; // Unique instance of the EventManager
	Event mEvent; // Current event

	/**
	 * @brief Private constructor of EventManager.
	 * Constructs the EventManager object. Private to enforce singleton pattern.
	 */
	EventManager();

public:
	/**
	 * @brief Retrieves the current event by polling for events.
	 * @return Reference to the current event.
	 */
	static Event& PollEvent();
};

/**
 * @brief Singleton class representing the system.
 */
class System
{
	static System mInstance;
	System();
public:
	/**
	 * @brief Starts the system.
	 *
	 * This function starts the system and returns a boolean value indicating
	 * whether the system was successfully started or not.
	 *
	 * @return True if the system starts successfully, false otherwise.
	 */
	static const bool Start();
	/**
	 * @brief Quits the system.
	 *
	 * This function quits the system and performs any necessary cleanup or
	 * termination procedures.
	 */
	static void Quit();
};

} // namespace System
} // namespace Vision