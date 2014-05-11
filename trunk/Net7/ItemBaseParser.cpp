// ItemBaseParser.cpp

#include "Net7.h"

#ifndef USE_MYSQL_ITEMS

#include "ItemBaseParser.h"
#include "StringManager.h"
#include "xmlParser/xmlParser.h"

bool ItemBaseParser::LoadItemBase(ItemBase ** GlobalDB)
{
    printf("Parsing ItemBase.xml\n");

    ItemBaseData * m_Data = new ItemBaseData;
    ItemEffect * m_Effect = 0;

    XMLNode m_ItemBaseXML;
    XMLNode m_Item;
    XMLNode m_ItemFields;
    XMLNode m_ItemField;
    XMLNode m_ItemEffects;
    XMLNode m_ItemEffect;
    XMLNode m_ItemEffectDescVars;
    XMLNode m_ItemEffectDescVar;

    int ItemID, ItemFieldType;
    int ItemCount, ItemFieldCount, ItemEffectCount, ItemEffectDescVarCount;

    {
        char orig_path[MAX_PATH];
        GetCurrentDirectory(sizeof(orig_path), orig_path);
        SetCurrentDirectory(SERVER_DATABASE_PATH);

        m_ItemBaseXML = XMLNode::openFileHelper("ItemBase.xml");

        SetCurrentDirectory(orig_path);
    }

    ItemCount = m_ItemBaseXML.nChildNode("ItemBase");

    // Loop through the entire itembase
    for (int i=0; i<ItemCount; i++)
    {
        m_Item = m_ItemBaseXML.getChildNode("ItemBase",i);
        
        ItemID = intVal(m_Item.getAttribute("ItemTemplateID"));

        if (GlobalDB[ItemID])
        {
            printf("ItemBaseParser - Item with ID: %d already exists\n", ItemID);
            return false;
        }
        else
        {
            if (GlobalDB[ItemID] = new ItemBase())
            {
                memset(m_Data, 0, sizeof(ItemBaseData));
                m_Data->ItemTemplateID = ItemID;
            }
            else
            {
                printf("ItemBaseParser - Unable to allocate memory for ItemBase! ItemCount: %d\n", i);
                return false;
            }
        }

        m_Data->Category = intVal(m_Item.getAttribute("Category"));
        m_Data->SubCategory = intVal(m_Item.getAttribute("Subcategory"));
        m_Data->ItemType = intVal(m_Item.getAttribute("ItemType"));
        m_Data->GameBaseAsset = intVal(m_Item.getAttribute("GameBaseAsset"));
        m_Data->IconBaseAsset = intVal(m_Item.getAttribute("IconBaseAsset"));
        m_Data->TechLevel = intVal(m_Item.getAttribute("TechLevel"));
        m_Data->Cost = intVal(m_Item.getAttribute("Cost"));
        m_Data->MaxStack = intVal(m_Item.getAttribute("MaxStack"));
        m_Data->UseEffect = intVal(m_Item.getAttribute("UseEffect"));
        m_Data->Flags = intVal(m_Item.getAttribute("Flags"));

        m_Data->Name = g_StringMgr->GetStr(m_Item.getChildNode("Name").getText());
        m_Data->Description = g_StringMgr->GetStr(m_Item.getChildNode("Description").getText());
        m_Data->Manufacturer = g_StringMgr->GetStr(m_Item.getChildNode("Manufacturer").getText());

        m_ItemFields = m_Item.getChildNode("ItemFields");
        ItemFieldCount = m_ItemFields.nChildNode("Item");

        for (int j=0; j<ItemFieldCount; j++)
        {
            m_ItemField = m_ItemFields.getChildNode("Item", j);
            ItemFieldType = intVal(m_ItemField.getAttribute("type"));

            switch (ItemFieldType)
            {
            case 0x01 : // #1  = Ammo Parent
            case 0x0B : // #11 = Requires (0x0B) Level (ItemLVL)
            case 0x0D : // #13 = Item Type (Subdescription)
            case 0x1B : // #27 = String for 0x1C
                m_Data->ItemFields[ItemFieldType].sData = g_StringMgr->GetStr(m_ItemField.getText());
				m_Data->ItemFields[ItemFieldType].HasData = true;
                break;

            case 0x00 : // #0  = ????
            case 0x09 : // #9  = Energy Usage
            case 0x0A : // #10 = Energy Drain
            case 0x14 : // #20 = Reactor Recharge Rate
            case 0x15 : // #21 = Weapon Reload
            case 0x17 : // #23 = Shield Usage
            case 0x19 : // #25 = Shield Drain
            case 0x1A : // #26 = Shield Recharge Rate
            case 0x1D : // #29 = Engine Signature
            case 0x22 : // #34 = Engine Freewarp Drain
            case 0x24 : // #36 = Terminal Override Skill Increase
            case 0x25 : // #37 = Terminal Override Crit Increase
                m_Data->ItemFields[ItemFieldType].fData = floatVal(m_ItemField.getText());
				m_Data->ItemFields[ItemFieldType].HasData = true;
                break;

            case 0x02 : // #2  = Autofire Box
            case 0x03 : // #3  = Profession Restriction
            case 0x04 : // #4  = Combat Level Requirement
            case 0x05 : // #5  = Weapon Damage
            case 0x06 : // #6  = Weapon Damage Type
            case 0x07 : // #7  = Effect Range
            case 0x08 : // #8  = Effect Radius
            case 0x0C : // #12 = Explore Level Requirement
            case 0x0E : // #14 = Overall Level Requirement
            case 0x0F : // #15 = Missle Manuverability
            case 0x10 : // #16 = Reactor Capacity
            case 0x11 : // #17 = Lore Requirements (1 - Projen Restricted, 2 - Jenq Restricted) 
            case 0x12 : // #18 = Race Requirement
            case 0x13 : // #19 = Weapon Range
            case 0x16 : // #22 = Rounds Per Shot
            case 0x18 : // #24 = Shield Capacity
            case 0x1C : // #28 = (0x1B) Level Requirement
            case 0x1E : // #30 = ????
            case 0x1F : // #31 = Engine Speed
            case 0x20 : // #32 = Trade Level Requirement
            case 0x21 : // #33 = Engine Warp Speed
            case 0x23 : // #35 = Terminal Override Flags
                m_Data->ItemFields[ItemFieldType].iData = intVal(m_ItemField.getText());
				m_Data->ItemFields[ItemFieldType].HasData = true;
                break;

            default :
                printf("Unknown ItemField: %d Item: %d", ItemFieldType, ItemID);
                return false;
            }

            m_Data->ItemFieldCount++;
        }

        /* Activatable Effects */
        m_ItemEffects = m_Item.getChildNode("ActivatableEffects");

        /*
        m_Data->ActivatableEffects.UnknownData.Unknown1 = intVal(m_ItemEffects.getAttribute("unknown1"));
        m_Data->ActivatableEffects.UnknownData.Unknown2 = intVal(m_ItemEffects.getAttribute("unknown2"));
        m_Data->ActivatableEffects.UnknownData.Unknown3 = intVal(m_ItemEffects.getAttribute("unknown3"));
        m_Data->ActivatableEffects.UnknownData.Unknown4 = intVal(m_ItemEffects.getAttribute("unknown4"));
        */

        ItemEffectCount = m_ItemEffects.nChildNode("Effect");

        for (int k=0; k<ItemEffectCount; k++)
        {
            m_Effect = &m_Data->ActivatableEffects.Effects[m_Data->ActivatableEffects.Count++];
            m_ItemEffect = m_ItemEffects.getChildNode("Effect", k);

            m_Effect->Flag1 = intVal(m_ItemEffect.getAttribute("flag1"));
            m_Effect->Flag2 = intVal(m_ItemEffect.getAttribute("flag2"));

            m_Effect->Name = g_StringMgr->GetStr(m_ItemEffect.getChildNode("Name").getText());
            m_Effect->Description = g_StringMgr->GetStr(m_ItemEffect.getChildNode("Description").getText());
            m_Effect->Tooltip = g_StringMgr->GetStr(m_ItemEffect.getChildNode("Tooltip").getText());
            m_Effect->VisualEffect = intVal(m_ItemEffect.getChildNode("VisualEffect").getText());

            m_ItemEffectDescVars = m_ItemEffect.getChildNode("DescriptionVars");
            ItemEffectDescVarCount = m_ItemEffectDescVars.nChildNode("Float");

            for (int x=0; x<ItemEffectDescVarCount; x++)
            {
                m_ItemEffectDescVar = m_ItemEffectDescVars.getChildNode("Float", x);
                m_Effect->DescVar[m_Effect->DescVarCount++] = floatVal(m_ItemEffectDescVar.getText());
            }
        }

        /* Equipable Effects */
        m_ItemEffects = m_Item.getChildNode("EquipableEffects");

        /*
        m_Data->EquipableEffects.UnknownData.Unknown1 = intVal(m_ItemEffects.getAttribute("unknown1"));
        m_Data->EquipableEffects.UnknownData.Unknown1 = intVal(m_ItemEffects.getAttribute("unknown2"));
        m_Data->EquipableEffects.UnknownData.Unknown1 = intVal(m_ItemEffects.getAttribute("unknown3"));
        m_Data->EquipableEffects.UnknownData.Unknown1 = intVal(m_ItemEffects.getAttribute("unknown4"));
        */

        ItemEffectCount = m_ItemEffects.nChildNode("Effect");

        for (int l=0; l<ItemEffectCount; l++)
        {
            m_Effect = &m_Data->EquipableEffects.Effects[m_Data->EquipableEffects.Count++];
            m_ItemEffect = m_ItemEffects.getChildNode("Effect", l);

            m_Effect->Flag1 = intVal(m_ItemEffect.getAttribute("flag1"));
            m_Effect->Flag2 = intVal(m_ItemEffect.getAttribute("flag2"));

            m_Effect->Name = g_StringMgr->GetStr(m_ItemEffect.getChildNode("Name").getText());
            m_Effect->Description = g_StringMgr->GetStr(m_ItemEffect.getChildNode("Description").getText());
            m_Effect->Tooltip = g_StringMgr->GetStr(m_ItemEffect.getChildNode("Tooltip").getText());
            m_Effect->VisualEffect = intVal(m_ItemEffect.getChildNode("VisualEffect").getText());

            m_ItemEffectDescVars = m_ItemEffect.getChildNode("DescriptionVars");
            ItemEffectDescVarCount = m_ItemEffectDescVars.nChildNode("Float");

            for (int x=0; x<ItemEffectDescVarCount; x++)
            {
                m_ItemEffectDescVar = m_ItemEffectDescVars.getChildNode("Float", x);
                m_Effect->DescVar[m_Effect->DescVarCount++] = floatVal(m_ItemEffectDescVar.getText());
            }
        }

        // Assign the data to this item
        GlobalDB[ItemID]->SetData(m_Data);
    }

    delete m_Data;

    return true;
}

#endif

