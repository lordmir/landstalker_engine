#ifndef _I_TICKABLE_H_
#define _I_TICKABLE_H_

class ITickable
{
public:
    virtual ~ITickable() = default;
    virtual void OnTick(unsigned int total_ticks) = 0;
};

#endif // _I_TICKABLE_H_
