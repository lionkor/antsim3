#ifndef GUILAYER_H
#define GUILAYER_H

#include "Core/Object.h"

class GuiLayer : public Object {
    OBJNAME(GuiLayer)
private:
    class Application& m_app;

public:
    GuiLayer(Application& app);
};

#endif // GUILAYER_H
