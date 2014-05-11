// PlayerInventory.cpp
//
// Contains all methods to modify / access player inventory

#include "PlayerClass.h"
#include "ItemBaseManager.h"

void Player::SendItemBase(long ItemID)
{
    g_ItemBaseMgr->SendItem(this, &m_ItemList, ItemID);
}

// Returns the count of items with ID in cargo inventory
long Player::CargoItemCount(long ItemID)
{
    long ItemCount = 0;

    for (u32 i=0; i<ShipIndex()->Inventory.GetCargoSpace(); i++)
    {
        if (ShipIndex()->Inventory.CargoInv.Item[i].GetItemTemplateID() == ItemID)
        {
            ItemCount += ShipIndex()->Inventory.CargoInv.Item[i].GetStackCount();
        }
    }

    return ItemCount;
}

bool Player::CanCargoAddItem(_Item * myItem)
{
    return CanCargoAddItem(myItem->ItemTemplateID, myItem->StackCount, myItem->Quality);
}

// Returns whether or not the item with a given stack and quality can be added to inventory
bool Player::CanCargoAddItem(long ItemID, long Stack, float Quality)
{
    return (CargoAddItemCount(ItemID, Quality) >= Stack);
}

// Returns how many items with a given quality can be added to inventory
long Player::CargoAddItemCount(long ItemID, float Quality)
{
    long Count = 0;
    ItemBase * ItemData = g_ItemBaseMgr->GetItem(ItemID);

    if (!ItemData)
    {
        return 0;
    }

    for (u32 i=0; i<ShipIndex()->Inventory.GetCargoSpace(); i++)
    {
        if (ShipIndex()->Inventory.CargoInv.Item[i].GetItemTemplateID() == ItemID &&
            ShipIndex()->Inventory.CargoInv.Item[i].GetQuality() == Quality)
        {
            // We found an item stack of the same item and quality
            Count += (ItemData->MaxStack() - ShipIndex()->Inventory.CargoInv.Item[i].GetStackCount());
        }
        else if (ShipIndex()->Inventory.CargoInv.Item[i].GetItemTemplateID() == -1)
        {
            Count += ItemData->MaxStack();
        }
    }

    return Count;
}

int Player::CargoAddItem(long ItemID, long Stack, int TradeStack)
{
    _Item TempData;
    memset(&TempData, 0, sizeof(_Item));
    TempData.ItemTemplateID = ItemID;
    TempData.StackCount = Stack;
    TempData.Quality = 1.0f;
    TempData.Structure = 1.0f;
    TempData.TradeStack = TradeStack;

    return CargoAddItem(&TempData);
}


struct QualityArray 
{
	int ItemType;
	int ItemField;
	int Direction;		// 0 = Up 1 = Down
	float MaxQuality;
	float MinQuality;
};

bool Player::QualityCalculator(_Item * myItem)
{
	char InstanceInfo[64];
	char IInstanceInfo[64];
	int QArraySize = 7;
	QualityArray QArray[] = {{14, 5, 0, 1.55f, 0.01f },		// Beams
							 {15,21, 1, 1.55f, 0.01f },		// ML
							 {16,21, 1, 1.55f, 0.01f },		// Projectiles
							 { 2,26, 0, 1.55f, 0.01f },		// Shields
							 { 6,31, 0, 1.35f, 0.01f },		// Engines Thrust
							 { 6,34, 1, 1.55f, 0.01f },		// Engines Warp Drain
							 { 7,20, 0, 1.55f, 0.01f }		// Reactors
							};

	ItemBase * myItemBase = g_ItemBaseMgr->GetItem(myItem->ItemTemplateID);

	memset(InstanceInfo, 0, sizeof(InstanceInfo));

	if (!myItemBase) return false; // early return if item invalid

	for(int x=0;x<QArraySize;x++)
	{
		if (QArray[x].ItemType == myItemBase->ItemType())
		{
			int FieldID = QArray[x].ItemField;
			// Get Field Type
			int FieldType = myItemBase->FieldType(FieldID);
			float FieldData = 0;

			// Read in Data
			switch(FieldType)
			{
				// Float
				case 1:
					FieldData = myItemBase->Fields(FieldID)->fData;
					break;
				// Int
				case 2:
					FieldData = (float) myItemBase->Fields(FieldID)->iData;
					break;
			}

			// Calculate Real Quality Percent
			float RealPercent;
			float ChangeRate;
			float ItemQuality = myItem->Quality;
			float NewFieldValue = 0;

			// Calculate real percent from numbers
			if (ItemQuality < 1.0f)
			{
				ChangeRate = ((1.0f - QArray[x].MinQuality)/1.0f);
				RealPercent = ItemQuality * ChangeRate + QArray[x].MinQuality;
			}
			else
			{
				ChangeRate = ((QArray[x].MaxQuality - 1.0f)/1.0f);
				RealPercent = (ItemQuality - 1.0f)* ChangeRate + 1.0f;
			}

			// Calculate the field data
			if (QArray[x].Direction == 1)
			{
				NewFieldValue = (1.0f + (1.0f - RealPercent)) * FieldData;
			}
			else
			{
				NewFieldValue = RealPercent * FieldData;
			}

			// Client does not read below 1
			if (NewFieldValue < 1.0f)
				NewFieldValue = 1.0f;

			sprintf(IInstanceInfo, "%d:%4.2f^", FieldID,NewFieldValue);
			strcat(InstanceInfo, IInstanceInfo);
		}
	}

	if (InstanceInfo[0] != 0)
	{
		memcpy(myItem->InstanceInfo, InstanceInfo, sizeof(InstanceInfo));
		return true;
	}

	return false;
}

int Player::CargoAddItem(_Item * myItem)
{
	if (myItem->ItemTemplateID < 0)
	{
		return -1;
	}

	ItemBase * myItemBase = g_ItemBaseMgr->GetItem(myItem->ItemTemplateID);

	if (!myItemBase)
	{
		return -2;
	}

    u32 curTrade = myItem->TradeStack;
    u32 curStack = myItem->StackCount;
	u32 maxStack = myItemBase->MaxStack();
	u32 curPrice = (u32)myItem->Price;

    SendItemBase(myItem->ItemTemplateID);

	// Update the instance information
	QualityCalculator(myItem);

    // If this is a stackable item, check stacks
    if (maxStack > 1)
    {
	    for(u32 i=0; i<ShipIndex()->Inventory.GetCargoSpace(); i++)
	    {
		    if (ShipIndex()->Inventory.CargoInv.Item[i].GetItemTemplateID() == myItem->ItemTemplateID)
		    {
			    if (ShipIndex()->Inventory.CargoInv.Item[i].GetStackCount() + curStack > maxStack)
			    {
				    // There is SOME room in this slot
					u32 oldstack = ShipIndex()->Inventory.CargoInv.Item[i].GetStackCount();
                    u32 moved = maxStack - ShipIndex()->Inventory.CargoInv.Item[i].GetStackCount();
				    curStack -= moved;

                    ShipIndex()->Inventory.CargoInv.Item[i].AddTradeStack(curTrade < moved ? curTrade : moved);
                    curTrade -= moved;

				    ShipIndex()->Inventory.CargoInv.Item[i].SetStackCount(maxStack);
					//set average cost
					float average = ( (ShipIndex()->Inventory.CargoInv.Item[i].GetAveCost() * oldstack) + (float)(moved * curPrice) ) / (float)maxStack;
					ShipIndex()->Inventory.CargoInv.Item[i].SetAveCost(average);
					if (myItem->Price != 0)
					{
						ShipIndex()->Inventory.CargoInv.Item[i].SetPrice(myItem->Price);
					}
					SaveInventoryChange(i);
			    }
			    else
			    {
				    // Enough room here for the rest of the stack
					u32 oldstack = ShipIndex()->Inventory.CargoInv.Item[i].GetStackCount();
					//set average cost
					float average = ( (ShipIndex()->Inventory.CargoInv.Item[i].GetAveCost() * oldstack) + (float)(curStack * curPrice) ) / (float)(curStack + oldstack);
					ShipIndex()->Inventory.CargoInv.Item[i].SetAveCost(average);
					if (myItem->Price != 0)
					{
						ShipIndex()->Inventory.CargoInv.Item[i].SetPrice(myItem->Price);
					}

				    curStack += oldstack;
                    curTrade += ShipIndex()->Inventory.CargoInv.Item[i].GetTradeStack();
				    ShipIndex()->Inventory.CargoInv.Item[i].SetStackCount(curStack);
				    ShipIndex()->Inventory.CargoInv.Item[i].SetTradeStack(curTrade);
					SaveInventoryChange(i);
                    return 0;
			    }
		    }
	    }
    }

    // At this point, we need to fill empty slots with the item
	for(u32 i=0; i<ShipIndex()->Inventory.GetCargoSpace(); i++)
	{
		if (ShipIndex()->Inventory.CargoInv.Item[i].GetItemTemplateID() == -1)
		{
			if (curStack > maxStack)
			{
				// We have more than a full stack
				curStack -= maxStack;
				ShipIndex()->Inventory.CargoInv.Item[i].SetData(myItem);
				ShipIndex()->Inventory.CargoInv.Item[i].SetStackCount(maxStack);

                // The tradable count is the smaller amount of whats left tradable and max stack
                ShipIndex()->Inventory.CargoInv.Item[i].SetTradeStack(curTrade < maxStack ? curTrade : maxStack);
				ShipIndex()->Inventory.CargoInv.Item[i].SetAveCost((float)curPrice);

                // The SetTradeStack method converts negative numbers to zero so this is allowerd
				curTrade -= maxStack;

				SaveInventoryChange(i);
			}
			else
			{
				// Enough room here to finish adding this item
				ShipIndex()->Inventory.CargoInv.Item[i].SetData(myItem);
				ShipIndex()->Inventory.CargoInv.Item[i].SetStackCount(curStack);
				ShipIndex()->Inventory.CargoInv.Item[i].SetTradeStack(curTrade);
				ShipIndex()->Inventory.CargoInv.Item[i].SetAveCost((float)curPrice);
				SaveInventoryChange(i);
                return 0;
			}
		}
	}

    return -3;
}

void Player::CargoRemoveItem(long ItemID, long Stack)
{
    if (ItemID < 0 || Stack <= 0)
    {
        return;
    }

    long ItemStack;
    for (u32 i=0; i<ShipIndex()->Inventory.GetCargoSpace(); i++)
    {
        if (ShipIndex()->Inventory.CargoInv.Item[i].GetItemTemplateID() == ItemID)
        {
            ItemStack = ShipIndex()->Inventory.CargoInv.Item[i].GetStackCount();

            if (ItemStack <= Stack)
            {
                ShipIndex()->Inventory.CargoInv.Item[i].Empty();
                Stack -= ItemStack;
            }
            else
            {
                ItemStack -= Stack;
                ShipIndex()->Inventory.CargoInv.Item[i].SetStackCount(ItemStack);
                Stack = 0;
            }

			SaveInventoryChange(i);

            if (Stack == 0)
            {
                break;
            }
        }
    }

    if (Stack != 0)
    {
        LogMessage("RemoveTradeItem - Stack Remaining: %d\n",Stack);
    }
}

/* Moving an item from Source to Destination, check if they can stack */
/* Note: If modified, the source and destination stacks have to be inverted */

/* This currently does not check quality */
void Player::CheckStack(int MoveNum, _Item * Source, _Item * Destination)
{
    /* If the items have the same ID they can stack */
	if (Source->ItemTemplateID == Destination->ItemTemplateID)
	{
	    ItemBase * myItemBase = g_ItemBaseMgr->GetItem(Source->ItemTemplateID);

	    if (!myItemBase)
        {
		    return;
        }

        /* If the MoveNum has enough items to make a full stack at Destination */
		if (Destination->StackCount + MoveNum > myItemBase->MaxStack())
		{
            int moved = myItemBase->MaxStack() - Destination->StackCount;
			Source->StackCount -= moved;						
			Destination->StackCount = myItemBase->MaxStack();

            /* Now update the trade stacks */
            Destination->TradeStack += (Source->TradeStack < moved ? Source->TradeStack : moved);
            Source->TradeStack -= moved;

		}
        /* Otherwise the Destination can store the entire MoveNum ammount */
		else
		{
			Destination->StackCount += MoveNum;
			Source->StackCount -= MoveNum;

            /* Now update the trade stacks */
            Destination->TradeStack += (Source->TradeStack < MoveNum ? Source->TradeStack : MoveNum);
            Source->TradeStack -= MoveNum;
		}

        /* Switch the Source and Destination stack counts */
        u32 tmpStack = Destination->StackCount;
        Destination->StackCount = Source->StackCount;
        Source->StackCount = tmpStack;

        /* Now switch their trade stacks aswell */
        tmpStack = Destination->TradeStack;
        Destination->TradeStack = Source->TradeStack;
        Source->TradeStack = tmpStack;

        /* If weve moved all of the items out of the source, set it to an empty item */
		if (Destination->StackCount == 0)
		{
			memcpy(Destination, &g_ItemBaseMgr->EmptyItem, sizeof(_Item));
		}
	}
    /* If the destination is am empty item and we are moving a substack*/
	else if (Destination->ItemTemplateID == -1 && (u32)MoveNum < Source->StackCount)
	{
        /* Copy the source item to the destination */
        memcpy(Destination, Source, sizeof(_Item));

        Destination->StackCount = MoveNum;
		Source->StackCount -= MoveNum;

        /* Now update the trade stacks */
        Destination->TradeStack += (Source->TradeStack < MoveNum ? Source->TradeStack : MoveNum);
        Source->TradeStack -= MoveNum;

        /* Switch the Source and Destination stack counts */
        u32 tmpStack = Destination->StackCount;
        Destination->StackCount = Source->StackCount;
        Source->StackCount = tmpStack;

        /* Now switch their trade stacks aswell */
        tmpStack = Destination->TradeStack;
        Destination->TradeStack = Source->TradeStack;
        Source->TradeStack = tmpStack;

        /* If weve moved all of the items out of the source, set it to an empty item */
		if (Destination->StackCount == 0)
		{
			memcpy(Destination, &g_ItemBaseMgr->EmptyItem, sizeof(_Item));
		}
	}
}

void Player::SetPrices()
{
    u32 i;
    ItemBase * Item = 0;

    for (i=0; i<96; i++)
    {
		PlayerIndex()->SecureInv.Item[i].SetPrice(
			GetVenderBuyPrice(PlayerIndex()->SecureInv.Item[i].GetItemTemplateID())
			);
    }

	for (i=0; i<20; i++)
    {
		ShipIndex()->Inventory.EquipInv.EquipItem[i].SetPrice(
			GetVenderBuyPrice(ShipIndex()->Inventory.EquipInv.EquipItem[i].GetItemTemplateID())
			);
    }

	for (i=0; i<20; i++)
    {
		ShipIndex()->Inventory.AmmoInv.Item[i].SetPrice(
			GetVenderBuyPrice(ShipIndex()->Inventory.AmmoInv.Item[i].GetItemTemplateID())
			);
    }

    for (i=0; i<ShipIndex()->Inventory.GetCargoSpace(); i++)
    {
		ShipIndex()->Inventory.CargoInv.Item[i].SetPrice(
			GetVenderBuyPrice(ShipIndex()->Inventory.CargoInv.Item[i].GetItemTemplateID())
			);
    }
}

void Player::ClearPrices()
{
    u32 i;

    for (i=0; i<96; i++)
    {
        PlayerIndex()->SecureInv.Item[i].SetPrice(0);
    }

    for (i=0; i<20; i++)
    {
		ShipIndex()->Inventory.AmmoInv.Item[i].SetPrice(0);
    }

    for (i=0; i<20; i++)
    {
		ShipIndex()->Inventory.EquipInv.EquipItem[i].SetPrice(0);
    }

    for (i=0; i<ShipIndex()->Inventory.GetCargoSpace(); i++)
    {
        ShipIndex()->Inventory.CargoInv.Item[i].SetPrice(0);
    }
}
