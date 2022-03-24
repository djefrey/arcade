#pragma once

#include "arcade-interface/IGameModule.hpp"

class PacmanGameModule : public virtual IGameModule {
    ICore *coreHandle;

public:
    void init(ICore *coreHandle) override;
    void update() override;
    void draw() override;
};
