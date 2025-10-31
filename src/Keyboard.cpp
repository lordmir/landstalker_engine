#include "Keyboard.h"

Keyboard& Keyboard::GetInstance()
{
    static Keyboard instance;
    return instance;
}

void Keyboard::RegisterKeyInputHandler(std::shared_ptr<IKeyInput> handler)
{
    handlers.insert(handler);
}

void Keyboard::UnregisterKeyInputHandler(std::shared_ptr<IKeyInput> handler)
{
    handlers.erase(handler);
}

void Keyboard::OnKeyEvent(const SDL_KeyboardEvent& event)
{
    if (event.type == SDL_EVENT_KEY_DOWN)
    {
        key_states[event.scancode] = true;
        for (const auto& handler : handlers) {
            handler->OnKeyDown(event.scancode, event.mod);
        }
    }
    else if (event.type == SDL_EVENT_KEY_UP)
    {
        key_states[event.scancode] = false;
        for (const auto& handler : handlers) {
            handler->OnKeyUp(event.scancode, event.mod);
        }
    }
}

bool Keyboard::IsKeyPressed(SDL_Scancode keycode) {
    auto it = key_states.find(keycode);
    return it != key_states.end() && it->second;
}
