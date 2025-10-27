#ifndef _I_KEY_INPUT_H_
#define _I_KEY_INPUT_H_

#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_keycode.h>

class IKeyInput
{
public:
    virtual ~IKeyInput() = default;
    virtual void OnKeyDown(SDL_Scancode scancode, SDL_Keymod mod) = 0;
    virtual void OnKeyUp(SDL_Scancode scancode, SDL_Keymod mod) = 0;
};

#endif // _I_KEY_INPUT_H_
