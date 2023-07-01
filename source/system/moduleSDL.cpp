#include "include/moduleSDL.h"
#include <common/include/common.h>

namespace Vision
{
namespace System
{

//********************************
//     STATIC
//********************************

EventManager EventManager::mInstance; 
System System::mInstance;

//********************************
//     (Singleton) Class System
//********************************

System::System() {}

//----------------------------------------------------------------
const bool System::Start()
{
    // Initialize SDL
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0)
    {
        LOG_STDERR("[SDL ERROR] - Cannot initialize SDL:\n- " << SDL_GetError());
        return false;
    }
    return true;
}

//----------------------------------------------------------------
void System::Quit()
{
    SDL_Quit();
}

//********************************
//     (Singleton) Class EventManager
//********************************

//----------------------------------------------------------------
EventManager::EventManager() : 
    mEvent()
{}

//----------------------------------------------------------------
Event& EventManager::PollEvent()
{
    SDL_PollEvent(&mInstance.mEvent.GetSDLEvent());
    mInstance.mEvent.UpdateAction();
    return mInstance.mEvent;
}

//********************************
//     Class Event
//********************************

//----------------------------------------------------------------
void Event::UpdateAction()
{
    mAction = FetchAction();
}

//********************************
//     Class Window
//********************************

//----------------------------------------------------------------
Window::Window(const int width, const int height, const char* title /* = "New window" */) :
    mWindow(nullptr), 
    mSurface(nullptr)
{
    SetGLAttributes();

    mWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (mWindow != nullptr)
    {
        mSurface = SDL_GetWindowSurface(mWindow);
        SDL_GLContext glContext = SDL_GL_CreateContext(mWindow);
        if (glContext != nullptr)
        {
            gladLoadGLLoader(SDL_GL_GetProcAddress);
            LOG_STDOUT("Vendor:" << glGetString(GL_VENDOR));
            LOG_STDOUT("Renderer:" << glGetString(GL_RENDERER));
            LOG_STDOUT("Version:" << glGetString(GL_VERSION));
            LOG_STDOUT("Shading Language:" << glGetString(GL_SHADING_LANGUAGE_VERSION));

            SDL_GL_MakeCurrent(mWindow, glContext);

            glEnable(GL_DEPTH_TEST);
            glViewport(0, 0, 800, 600);
        }
        else
        {
            LOG_STDERR("Could not create GLContext\n- " << SDL_GetError());
        }
        
    }
    else
    {
        LOG_STDERR("Window could not be created!\n- " << SDL_GetError());
    }
}

//----------------------------------------------------------------
Window::~Window()
{
    SDL_DestroyWindow(mWindow);
    mWindow = nullptr;
}

//----------------------------------------------------------------
SDL_Window* Window::operator&()
{
    return mWindow;
}

//----------------------------------------------------------------
void Window::Swap() const
{
    SDL_GL_SwapWindow(mWindow);
}

//----------------------------------------------------------------
void Window::SetGLAttributes()
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
}

} // namespace System
} // namespace Vision