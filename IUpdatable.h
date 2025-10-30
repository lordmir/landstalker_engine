#ifndef _IUPDATABLE_H_
#define _IUPDATABLE_H_

class IUpdatable
{
public:
    virtual ~IUpdatable() = default;
    virtual void OnUpdate(double delta) = 0;
};

#endif // _IUPDATABLE_H_
