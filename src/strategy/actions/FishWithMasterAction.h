#ifndef _PLAYERBOT_FISHWITHMASTERACTION_H
#define _PLAYERBOT_FISHWITHMASTERACTION_H

#include "MovementActions.h"

class FishWithMasterAction : public MovementAction
{
public:
    FishWithMasterAction(PlayerbotAI* botAI, std::string const name = "fish with master") : MovementAction(botAI, name)
    {
    }
    bool Execute(Event event) override;
    bool isUseful() override;
};

#endif
