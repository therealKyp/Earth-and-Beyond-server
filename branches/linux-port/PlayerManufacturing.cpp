#include "PlayerClass.h"
#include "PlayerManufacturing.h"
#include "ItemBaseManager.h"

void Player::HandleManufactureTerminal(unsigned char *data)
{
    ManufactureData * Packet = (ManufactureData *) data;
    long Terminal = ntohl(Packet->Data);
	LogDebug("ManufactureTerminal: %d\n", Terminal);
    switch (Terminal)
    {
        case 0:
            switch (ManuIndex()->GetMode())
            {
                case MODE_MANUFACTURE: // Manufacturing (check for override)
                    break;
                case MODE_ANALIZE: // Analize (check for target and override)
                    break;
                case MODE_DISMANTLE: // Dismantle (check for target) override???
                    break;
                case MODE_REFINE: // Refining (nothing to check)
                    break;
                default:
                    LogMessage("ManufactureTerminal 0 - Unknown Previous: %d\n",ManuIndex()->GetMode());
                    break;
            }

            ManuIndex()->TerminalReset(m_NumFormulas);
            break;
        case 1:
            ManuIndex()->SetMode(MODE_MANUFACTURE);
            break;
        case 2:
            ManuIndex()->SetMode(MODE_ANALIZE);
            break;
        case 4:
            ManuIndex()->SetMode(MODE_REFINE);
            BuildRefineList();
            break;
        default:
            LogMessage("ManufactureTerminal - Unknown Terminal: %d\n",Terminal);
            break;
    }

    ManuIndex()->SetDifficulty(DIFFICULTY_AUTOMATIC);
    ResetManuItems();
    SendAuxManu();
}

void Player::HandleManufactureCategorySelection(unsigned char *data)
{
    ManufactureData * Packet = (ManufactureData *) data;
    long Category = ntohl(Packet->Data);
	LogDebug("ManufactureCategorySelection: %d\n", Category);
    ManuIndex()->SetCurrentItemCat(Category);
    SendAuxManu();
}

void Player::HandleManufactureSetItem(unsigned char *data)
{
    ManufactureData * Packet = (ManufactureData *) data;
    //long Item = ntohl(Packet->Data); //this was done with no bit reverse, may need to test
	long Item = Packet->Data;

	if (Item > 0xFFFF)
	{
		Item = ntohl(Packet->Data);  //lets hedge our bets till we know one way or another
	}

    LogDebug("ManufactureSetItem: %d\n", Item);
}

void Player::HandleRefineSetItem(unsigned char *data)
{
    ManufactureData * Packet = (ManufactureData *) data;
    //long Item = ntohl(Packet->Data); //this was done with no bit reverse, may need to test
	long Item = Packet->Data;

	if (Item > 0xFFFF)
	{
		Item = ntohl(Packet->Data);  //lets hedge our bets till we know one way or another
	}

    if (m_Manufacturing)
    {
        return;
    }

    SendItemBase(Item);

    m_CurManuItem = g_ItemBaseMgr->GetItem(Item);

    if (!m_CurManuItem)
    {
        SendVaMessage("Invalid Item!");
        return;
    }

    ResetManuItems();

    ManuIndex()->Target.Item[0].SetItemTemplateID(m_CurManuItem->ItemTemplateID());

    for (int i=0;i<6;i++)
    {
        ManuIndex()->Components.Item[i].SetItemTemplateID(m_CurManuItem->Component(i));
    }

    ManuIndex()->SetValidity(0);
    ManuIndex()->SetAdditionalIterations(0);
    ManuIndex()->SetBaseCost(200);
    ManuIndex()->SetNegotiatedCost(150);

    CheckItemRequirements();

    SendAuxManu();
}

void Player::HandleManufactureAction(unsigned char *data)
{
    ManufactureData * Packet = (ManufactureData *) data;
    long Action = ntohl(Packet->Data);
	LogDebug("ManufactureAction: %d\n", Action);

    switch (Action)
    {
        case ACTION_LEAVE_TERMINAL:
            break;

        case ACTION_REFINE:
            // Check for components again
            if (!HasComponents())
            {
                ManuIndex()->SetValidity(VALIDITY_MISSING_COMPONENTS);
                ManuIndex()->SetFailureMessage("Missing Component(s)");
                SendAuxManu();
                return;
            }

            // Remove components and credits and set a time to give item
            RemoveComponentsFromInventory();
            PlayerIndex()->SetCredits(PlayerIndex()->GetCredits() - ManuIndex()->GetNegotiatedCost());
			SaveCreditLevel();
            SendAuxPlayer();
            SendAuxShip();
            m_SectorMgr->AddTimedCall(this, B_MANUFACTURE_ACTION, 3000, 0, ACTION_REFINE);
            m_Manufacturing = true;
            break;

        case ACTION_REFINE_STACK:
            // Check for components again
            if (!HasComponents(ManuIndex()->GetAdditionalIterations() + 1))
            {
                ManuIndex()->SetValidity(VALIDITY_MISSING_COMPONENTS);
                ManuIndex()->SetFailureMessage("Missing Component(s)");
                SendAuxManu();
                return;
            }

			if (PlayerIndex()->GetCredits() < ManuIndex()->GetNegotiatedCost() * (ManuIndex()->GetAdditionalIterations() + 1))
			{
                ManuIndex()->SetValidity(VALIDITY_INSUFFICIENT_CREDITS);
                ManuIndex()->SetFailureMessage("Insufficient Credits");
                SendAuxManu();
                return;
			}

            // Remove components and credits and set a time to give item
            RemoveComponentsFromInventory(ManuIndex()->GetAdditionalIterations() + 1);    
            PlayerIndex()->SetCredits(PlayerIndex()->GetCredits() - ManuIndex()->GetNegotiatedCost() * (ManuIndex()->GetAdditionalIterations() + 1));
			SaveCreditLevel();
            SendAuxPlayer();
            SendAuxShip();
            m_SectorMgr->AddTimedCall(this, B_MANUFACTURE_ACTION, 3000, 0, ACTION_REFINE_STACK);
            m_Manufacturing = true;
            break;
        default:
            LogMessage("ManufactureAction - Unknown Action: %d\n",Action);
            break;
    }
}

void Player::HandleManufactureLevelFilter(unsigned char *data)
{
    ManufactureTechLevelFilter * Filter = (ManufactureTechLevelFilter *) data;
    char Enable   = Filter->Enable;
    long BitField = ntohl(Filter->BitField);

    LogDebug("ManufactureLevelFilter - Enable: %d BitField %d\n",Enable,BitField);
    if (Enable)
    {
        ManuIndex()->SetTechFilterBitField(ManuIndex()->GetTechFilterBitField() | BitField);
    }
    else
    {
        ManuIndex()->SetTechFilterBitField(ManuIndex()->GetTechFilterBitField() & ~BitField);
    }
    LogDebug("Current Filter: %d\n",ManuIndex()->GetTechFilterBitField());
    SendAuxManu();
}

void Player::BuildRefineList()
{
    unsigned int i,j;
    bool InList = false;
    ItemBase * ItemData = 0;
    ItemBase * RefineItemData = 0;
    m_NumFormulas = 0; // Should be at zero already!

    for (i=0;i<40;i++)
    {
        if (ShipIndex()->Inventory.CargoInv.Item[i].GetItemTemplateID() > 0)
        {
            ItemData = g_ItemBaseMgr->GetItem(ShipIndex()->Inventory.CargoInv.Item[i].GetItemTemplateID());

            if (ItemData && ItemData->ReginesInto())
            {
                if (RefineItemData = g_ItemBaseMgr->GetItem(ItemData->ReginesInto()))
                {
                
                    InList = false;

                    for (j=0;j<m_NumFormulas;j++)
                    {
                        if (ManuIndex()->KnownFormulas.Formula[j].GetItemID() == ItemData->ReginesInto())
                        {
                            InList = true;
                            break;
                        }
                    }

                    if (!InList)
                    {
                        ManuIndex()->KnownFormulas.Formula[m_NumFormulas].SetItemName(RefineItemData->Name());
                        ManuIndex()->KnownFormulas.Formula[m_NumFormulas].SetItemID(RefineItemData->ItemTemplateID());
                        ManuIndex()->KnownFormulas.Formula[m_NumFormulas].SetTechLevel(RefineItemData->TechLevel());
                        m_NumFormulas++;
                    }
                }
            }
        }
    }
}

void Player::ManufactureTimedReturn(long Action)
{
    // We just came back from a manufacturing action
    LogDebug("ManufactureTimedReturn - Action: %d\n",Action);
    char msg_buffer[64];
    m_Manufacturing = false;
    switch (Action)
    {
        case ACTION_REFINE:

            _snprintf(msg_buffer, 64, "Manufactured %s:", m_CurManuItem->Name());
            AwardTradeXP(msg_buffer,m_CurManuItem->TechLevel() * 50,true);
            CargoAddItem(ManuIndex()->Target.Item[0].GetItemTemplateID(), 1);
            ManuIndex()->SetValidity(VALIDITY_ATTEMPT_NORMAL_SUCCESS);
            SendAuxPlayer();
            SendAuxShip();
            SendAuxManu();
            break;

        case ACTION_REFINE_STACK:

            _snprintf(msg_buffer, 64, "Manufactured %s:", m_CurManuItem->Name());
            AwardTradeXP(msg_buffer,m_CurManuItem->TechLevel() * 50 * ManuIndex()->GetAdditionalIterations() + 1,true);
            CargoAddItem(ManuIndex()->Target.Item[0].GetItemTemplateID(), ManuIndex()->GetAdditionalIterations() + 1);
            ManuIndex()->SetValidity(VALIDITY_ATTEMPT_NORMAL_SUCCESS);
            SendAuxPlayer();
            SendAuxShip();
            SendAuxManu();
            break;

        default:
            LogMessage("ManufactureTimedReturn - Unknown Action: %d\n",Action);
            break;
    }
}

void Player::CheckItemRequirements()
{
    int i, j, ItemCount = 0;
    bool HaveComponents = true;

    for (i=0;i<6;i++)
    {
        if (ManuIndex()->Components.Item[i].GetItemTemplateID() > 0)
        {
            ItemCount = CargoItemCount(ManuIndex()->Components.Item[i].GetItemTemplateID());

            for (j=0;j<i;j++)
            {
                if (ManuIndex()->Components.Item[j].GetItemTemplateID() == ManuIndex()->Components.Item[i].GetItemTemplateID())
                {
                    ItemCount--;
                }
            }
            
            if (ItemCount > 0)
            {
                ManuIndex()->Components.Item[i].SetStackCount(1);            
            }
            else
            {
                HaveComponents = false;
                ManuIndex()->Components.Item[i].SetStackCount(0);            
            }
        }
        else
        {
            ManuIndex()->Components.Item[i].SetStackCount(0);
        }
    }

    ManuIndex()->Target.Item[0].SetStackCount(HaveComponents ? 1 : 0);

    if (!HaveComponents)
    {
        ManuIndex()->SetValidity(VALIDITY_MISSING_COMPONENTS);
        ManuIndex()->SetFailureMessage("Missing Component(s)");
    }
    else if (ManuIndex()->GetNegotiatedCost() > PlayerIndex()->GetCredits())
    {
        ManuIndex()->SetValidity(VALIDITY_INSUFFICIENT_CREDITS);
        ManuIndex()->SetFailureMessage("Insufficient Credits");
    }
    else if (CanCargoAddItem(ManuIndex()->Target.Item[0].GetItemTemplateID(), 1) != 1)
    {
        ManuIndex()->SetValidity(VALIDITY_INSUFFICIENT_CARGO);
        ManuIndex()->SetFailureMessage("Inventory Full");
    }
    else
    {
        ManuIndex()->SetValidity(VALIDITY_VALID);
        ManuIndex()->SetFailureMessage("");
    }

    if (ManuIndex()->GetMode() == MODE_REFINE)
    {
        SetRefineIterations();
    }
}

void Player::SetRefineIterations()
{
    // This is assuming that refining always takes the SAME item
    // Example 2 Iron Ore -> 1 Iron
    // Never 2 Iron Ore + 1 Catalyst

    long Iterations, Count = 0;

    for (int i=0;i<6;i++)
    {
        if (ManuIndex()->Components.Item[i].GetItemTemplateID() > 0)
        {
            Count++;
        }
    }
    
    if (Count == 0)
    {
        LogMessage("SetRefineIterations - Count = 0\n");
        return;
    }

    // Get the maximum iterations
    Iterations = (CargoItemCount(ManuIndex()->Components.Item[0].GetItemTemplateID()) / Count) - 1;
    // Now check for room

    if (Iterations > 0)
        Count = CargoAddItemCount(ManuIndex()->Target.Item[0].GetItemTemplateID()) / Iterations;

    if (Count < Iterations)
        Iterations = Count;

    // Now check for price
    if (PlayerIndex()->GetCredits() < (ManuIndex()->GetNegotiatedCost() * Iterations))
    {
        Iterations = long(PlayerIndex()->GetCredits() / ManuIndex()->GetNegotiatedCost());
    }

    ManuIndex()->SetAdditionalIterations(Iterations);
}

void Player::ResetManuItems()
{
    ManuIndex()->Override.Item[0].Empty();
    ManuIndex()->Target.Item[0].Empty();
    ManuIndex()->Components.Item[0].Empty();
    ManuIndex()->Components.Item[1].Empty();
    ManuIndex()->Components.Item[2].Empty();
    ManuIndex()->Components.Item[3].Empty();
    ManuIndex()->Components.Item[4].Empty();
    ManuIndex()->Components.Item[5].Empty();
}

void Player::RemoveComponentsFromInventory(long Multiplier)
{
    for (int i=0;i<6;i++)
    {
        if (ManuIndex()->Components.Item[i].GetItemTemplateID() > 0)
        {
            CargoRemoveItem(ManuIndex()->Components.Item[i].GetItemTemplateID(), Multiplier);
        }
    }
}

bool Player::HasComponents(long Multiplier)
{
    int i, j, ItemCount;
    for (i=0;i<6;i++)
    {
        if (ManuIndex()->Components.Item[i].GetItemTemplateID() > 0)
        {
            ItemCount = CargoItemCount(ManuIndex()->Components.Item[i].GetItemTemplateID());

            for (j=0;j<i;j++)
            {
                if (ManuIndex()->Components.Item[j].GetItemTemplateID() == ManuIndex()->Components.Item[i].GetItemTemplateID())
                {
                    ItemCount -= Multiplier;
                }
            }

            if (ItemCount <= 0)
            {
                return false;
            }
        }
    }

    return true;
}

