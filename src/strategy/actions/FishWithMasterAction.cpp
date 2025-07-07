#include "FishWithMasterAction.h"

#include <cmath>

#include "Playerbots.h"

bool FishWithMasterAction::isUseful()
{
    Player* master = botAI->GetMaster();
    return master && botAI->GetState() != BOT_STATE_COMBAT;
}

bool FishWithMasterAction::Execute(Event /*event*/)
{
    Player* master = botAI->GetMaster();
    if (!master)
        return false;

    if (sServerFacade->GetDistance2d(bot, master) > 10.0f)
        MoveNear(master, 1.5f);

    bot->StopMoving();
    bot->SetFacingTo(master);
    bot->SetFacingTo(master->GetOrientation());

    Item* pole = botAI->FindItemInInventory(
        [](ItemTemplate const* proto) -> bool
        { return proto->Class == ITEM_CLASS_WEAPON && proto->SubClass == ITEM_SUBCLASS_WEAPON_FISHING_POLE; });

    if (!pole)
    {
        botAI->TellMaster("I need a fishing pole.");
        return false;
    }

    Item* main = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
    if (!main || main->GetTemplate()->SubClass != ITEM_SUBCLASS_WEAPON_FISHING_POLE)
    {
        WorldPacket packet(CMSG_AUTOEQUIP_ITEM_SLOT, 2);
        packet << pole->GetGUID() << uint8(EQUIPMENT_SLOT_MAINHAND);
        bot->GetSession()->HandleAutoEquipItemSlotOpcode(packet);
    }

    float x = master->GetPositionX() + std::cos(master->GetOrientation()) * 2.0f;
    float y = master->GetPositionY() + std::sin(master->GetOrientation()) * 2.0f;
    float z = master->GetPositionZ();

    botAI->CastSpell(FISHING_SPELL_ID, x, y, z);
    botAI->TellMaster("Fishing started");

    botAI->ChangeEngine(BOT_STATE_FISHING);

    uint32 biteDelay = urand(10000, 30000);
    botAI->AddTimedEvent([this]() { botAI->TellMaster("Caught something"); }, biteDelay);

    return true;
}
