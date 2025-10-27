#ifndef _IDRAWABLE_H_
#define _IDRAWABLE_H_

#include <SDL3/SDL.h>

class IDrawable
{
public:
    virtual ~IDrawable() = default;
    virtual void Draw(SDL_Renderer* renderer) = 0;
};

#endif // _IDRAWABLE_H_