#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "IKeyInput.h"
#include <SDL3/SDL_events.h>
#include <memory>
#include <set>
#include <unordered_map>

class Keyboard
{
public:
    ~Keyboard() = default;
    Keyboard(const Keyboard&) = delete;
    Keyboard& operator=(const Keyboard&) = delete;
    static Keyboard& GetInstance();

    void RegisterKeyInputHandler(std::shared_ptr<IKeyInput> handler);
    void UnregisterKeyInputHandler(std::shared_ptr<IKeyInput> handler);
    void OnKeyEvent(const SDL_KeyboardEvent& event);
    bool IsKeyPressed(SDL_Scancode keycode);
private:
    Keyboard() = default;
    std::set<std::shared_ptr<IKeyInput>> handlers;
    std::unordered_map<SDL_Scancode, bool> key_states;
};

#endif // _KEYBOARD_H_