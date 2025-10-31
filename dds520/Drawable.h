#ifndef _DRAWABLE_H_
#define _DRAWABLE_H_

#include "IDrawable.h"

class Drawable : public IDrawable
{
public:
    virtual ~Drawable() = default;
    virtual void OnTick(unsigned int total_ticks) override {}
    virtual void OnUpdate(double delta) override {}
    virtual void Draw(SDL_Renderer* renderer) override {}
};

#endif // _DRAWABLE_H_
