// ItemBaseParser.cpp

#include "ItemBaseImporter.h"
#include "xmlParser/xmlParser.h"

void ItemBaseImporter::ImportXML(char * FileName)
{
    int m_StartCount = m_ItemCount;

    ItemBaseStruct * m_ItemData = 0;
    ItemEffect * m_Effect = 0;

    XMLNode m_ItemBaseXML;
    XMLNode m_Item;
    XMLNode m_ItemFields;
    XMLNode m_ItemField;
    XMLNode m_ItemEffects;
    XMLNode m_ItemEffect;
    XMLNode m_ItemEffectDescVars;
    XMLNode m_ItemEffectDescVar;

    int ItemFieldType;
    int ItemCount, ItemFieldCount, ItemEffectCount, ItemEffectDescVarCount;

    m_ItemBaseXML = XMLNode::openFileHelper(FileName);

    ItemCount = m_ItemBaseXML.nChildNode("ItemBase");

    // Loop through the entire itembase
    for (int i=0; i<ItemCount; i++)
    {
        m_Item = m_ItemBaseXML.getChildNode("ItemBase",i);
        
        m_ItemData = &m_Data[m_ItemCount++];

        m_ItemData->m_ItemTemplateID = intVal(m_Item.getAttribute("ItemTemplateID"));
        m_ItemData->m_Category = intVal(m_Item.getAttribute("Category"));
        m_ItemData->m_SubCategory = intVal(m_Item.getAttribute("Subcategory"));
        m_ItemData->m_ItemType = intVal(m_Item.getAttribute("ItemType"));
        
        //m_ItemData->m_GameBaseAsset = intVal(m_Item.getAttribute("GameBasset"));
        m_ItemData->m_GameBaseAsset = intVal(m_Item.getAttribute("GameBaseAsset"));
                
        m_ItemData->m_IconBaseAsset = intVal(m_Item.getAttribute("IconBaseAsset"));
        m_ItemData->m_TechLevel = intVal(m_Item.getAttribute("TechLevel"));
        m_ItemData->m_Cost = intVal(m_Item.getAttribute("Cost"));
        m_ItemData->m_MaxStack = intVal(m_Item.getAttribute("MaxStack"));

        //m_ItemData->m_UseEffect = intVal(m_Item.getAttribute("EffectUsage"));
        m_ItemData->m_UseEffect = intVal(m_Item.getAttribute("UseEffect"));
        m_ItemData->m_Flags = intVal(m_Item.getAttribute("Flags"));

        strcpy(m_ItemData->m_Name, strVal(m_Item.getChildNode("Name").getText()));
        strcpy(m_ItemData->m_Description, strVal(m_Item.getChildNode("Description").getText()));
        strcpy(m_ItemData->m_Manufacturer, strVal(m_Item.getChildNode("Manufacturer").getText()));

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
                strcpy(m_ItemData->m_ItemFields[ItemFieldType].sData, strVal(m_ItemField.getText()));
				m_ItemData->m_ItemFields[ItemFieldType].HasData = true;
                break;

            case 0x00 : // #0  = ????
            case 0x09 : // #9  = Energy Usage
            case 0x0A : // #10 = Energy Drain
            case 0x14 : // #20 = Reactor Recharge Rate
            case 0x15 : // #21 = Weapon Reload
            case 0x17 : // #23 = Shield Usage
            case 0x19 : // #25 = Shield Drain
            case 0x1A : // #26 = Shield Recharge Rate
            case 0x22 : // #34 = Engine Freewarp Drain
            case 0x24 : // #36 = Terminal Override Skill Increase
            case 0x25 : // #37 = Terminal Override Crit Increase
                m_ItemData->m_ItemFields[ItemFieldType].fData = floatVal(m_ItemField.getText());
				m_ItemData->m_ItemFields[ItemFieldType].HasData = true;
                break;

            case 0x02 : // #2  = ????
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
            case 0x1D : // #29 = Engine Signature
            case 0x1E : // #30 = ????
            case 0x1F : // #31 = Engine Speed
            case 0x20 : // #32 = Trade Level Requirement
            case 0x21 : // #33 = Engine Warp Speed
            case 0x23 : // #35 = Terminal Override Flags
                m_ItemData->m_ItemFields[ItemFieldType].iData = intVal(m_ItemField.getText());
				m_ItemData->m_ItemFields[ItemFieldType].HasData = true;
                break;

            default :
                printf("Uknown ItemField: %d Item: %d", ItemFieldType, m_ItemData->m_ItemTemplateID);
                return;
            }

            m_ItemData->m_NumItemFields++;
        }

        /* Activatable Effects */
        m_ItemEffects = m_Item.getChildNode("ActivatableEffects");

        ItemEffectCount = m_ItemEffects.nChildNode("Effect");

        for (int k=0; k<ItemEffectCount; k++)
        {
            m_Effect = &m_ItemData->m_ActivatableEffects[m_ItemData->m_NumActivatableEffects++];
            m_ItemEffect = m_ItemEffects.getChildNode("Effect", k);

            m_Effect->Flag1 = intVal(m_ItemEffect.getAttribute("flag1"));
            m_Effect->Flag2 = intVal(m_ItemEffect.getAttribute("flag2"));

            strcpy(m_Effect->Name, strVal(m_ItemEffect.getChildNode("Name").getText()));
            strcpy(m_Effect->Description, strVal(m_ItemEffect.getChildNode("Description").getText()));
            strcpy(m_Effect->Tooltip, strVal(m_ItemEffect.getChildNode("Tooltip").getText()));
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

        ItemEffectCount = m_ItemEffects.nChildNode("Effect");

        for (int l=0; l<ItemEffectCount; l++)
        {
            m_Effect = &m_ItemData->m_EquipableEffects[m_ItemData->m_NumEquipableEffects++];
            m_ItemEffect = m_ItemEffects.getChildNode("Effect", l);

            m_Effect->Flag1 = intVal(m_ItemEffect.getAttribute("flag1"));
            m_Effect->Flag2 = intVal(m_ItemEffect.getAttribute("flag2"));

            strcpy(m_Effect->Name, strVal(m_ItemEffect.getChildNode("Name").getText()));
            strcpy(m_Effect->Description, strVal(m_ItemEffect.getChildNode("Description").getText()));
            strcpy(m_Effect->Tooltip, strVal(m_ItemEffect.getChildNode("Tooltip").getText()));
            m_Effect->VisualEffect = intVal(m_ItemEffect.getChildNode("VisualEffect").getText());

            m_ItemEffectDescVars = m_ItemEffect.getChildNode("DescriptionVars");
            ItemEffectDescVarCount = m_ItemEffectDescVars.nChildNode("Float");

            for (int x=0; x<ItemEffectDescVarCount; x++)
            {
                m_ItemEffectDescVar = m_ItemEffectDescVars.getChildNode("Float", x);
                m_Effect->DescVar[m_Effect->DescVarCount++] = floatVal(m_ItemEffectDescVar.getText());
            }
        }

    }

    long m_ItemsFound = m_ItemCount - m_StartCount;

    printf("XML Parsing complete, found %d items!\n\n", m_ItemsFound);

}