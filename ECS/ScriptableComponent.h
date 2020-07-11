#ifndef SCRIPTABLECOMPONENT_H
#define SCRIPTABLECOMPONENT_H

#include "Component.h"
#include "Entity.h"
#include "Core/Object.h"

class ScriptableComponent : public Component
{
    OBJNAME(ScriptableComponent)
private:

public:
    ScriptableComponent(Entity&);
};

#endif // SCRIPTABLECOMPONENT_H
