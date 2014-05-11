#include "ItemBaseImporter.h"
#include "stdio.h"

ItemBaseImporter::ItemBaseImporter(long MaxItems)
{
    m_Data = new ItemBaseStruct[MaxItems];

    if (!m_Data)
    {
        printf("Could not allocate %d items. Terminating.\n", MaxItems);
        exit(1);
    }

    memset(m_Data, 0, sizeof(ItemBaseStruct) * MaxItems);

    m_MaxItems = MaxItems;
    m_ItemCount = 0;
}

ItemBaseImporter::~ItemBaseImporter()
{
    if (m_Data)
    {
        delete [] m_Data;
    }
}

void ItemBaseImporter::ConflictCheck()
{
    for (int x=0; x<m_ItemCount; x++)
    {
        for (int y=0; y<m_ItemCount; y++)
        {
            if (x != y)
            {
                if (m_Data[x].m_ItemTemplateID == m_Data[y].m_ItemTemplateID)
                {
                    printf("Item %d has same TemplateID as item %d\n", x, y);
                    printf("Name of %d : `%s`\n", x, m_Data[x].m_Name);
                    printf("Name of %d : `%s`\n\n", y, m_Data[y].m_Name);
                }
                else if (!strcmpi(m_Data[x].m_Name, m_Data[y].m_Name))
                {
                    printf("Item %d has same Name as item %d\n", x, y);
                    printf("Name: `%s`\n\n", m_Data[x].m_Name);
                }
            }
        }
    }
}

void ItemBaseImporter::Normalize()
{
    ConflictCheck();

    static int PriorityTable[] =
    {
        120, 121, 122, 
        100, 101, 102, 103, 
        110, 111,
        140, 141, 142,
        150, 151, 152, 153,
        160, 161, 162, 163,
        170, 171, 172, 173,
        180, 181, 182, 183,
        200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211,
        -1
    };

    int PriorityCount = sizeof(PriorityTable) / sizeof(int);
    int SortedIndex = 0;
    int CurCat = 0;

    //We need to allocate a new array
    ItemBaseStruct * SortedBase = new ItemBaseStruct[m_ItemCount];
    if (!SortedBase)
    {
        printf("Could not allocate %d items. Terminating.\n", m_ItemCount);
        exit(1);
    }

    //Sort along the priorities
    for (int p=0; p<PriorityCount; p++)
    {
        CurCat = PriorityTable[p];

        for (int i=0; i<m_ItemCount; i++)
        {
            if (m_Data[i].m_ItemTemplateID != -1 && m_Data[i].m_SubCategory == CurCat)
            {
                memcpy(&SortedBase[SortedIndex++], &m_Data[i], sizeof(ItemBaseStruct));
                m_Data[i].m_ItemTemplateID = -1;
            }
        }
    }

    //Add everything that wasnt in the priority table
    for (int i=0; i<m_ItemCount; i++)
    {
        if (m_Data[i].m_ItemTemplateID != -1)
        {
            memcpy(&SortedBase[SortedIndex++], &m_Data[i], sizeof(ItemBaseStruct));
        }
    }

    printf("Started: %d Ended: %d\n", m_ItemCount, SortedIndex);

    delete [] m_Data;

    m_Data = SortedBase;

    //Now check for bad data
    for (int j=0; j<m_ItemCount; j++)
    {
        if (m_Data[j].m_GameBaseAsset <= 0)
            printf("ItemID %d has invalid GameBaseAsset\n", m_Data[j].m_ItemTemplateID); 

        if (m_Data[j].m_IconBaseAsset <= 0)
            printf("ItemID %d has invalid IconBaseAsset\n", m_Data[j].m_ItemTemplateID);

        if (m_Data[j].m_Category == -1 && m_Data[j].m_SubCategory >= 100 && m_Data[j].m_SubCategory < 130)
        {
            printf("ItemID %d had invalid Category\n", m_Data[j].m_ItemTemplateID);
            m_Data[j].m_Category = m_Data[j].m_SubCategory / 10;
        }

        if (m_Data[j].m_SubCategory == -1 && m_Data[j].m_Category >= 10 && m_Data[j].m_Category <= 12)
            printf("ItemID %d has invalid SubCategory. Must be fixed manually\n", m_Data[j].m_ItemTemplateID);
    }
}

bool ItemBaseImporter::WriteFlatFile(char * FileName)
{
    bool success = false;

    FILE * f = fopen(FileName, "wb");

    if (f)
    {
        if (fwrite(&m_ItemCount, 1, sizeof(m_ItemCount), f) == sizeof(m_ItemCount))
        {
            if (fwrite(m_Data, 1, sizeof(ItemBaseStruct) * m_ItemCount, f) == sizeof(ItemBaseStruct) * m_ItemCount)
            {
                success = true;
            }
        }

        fclose(f);
    }

    return success;
}

bool ItemBaseImporter::ReadFlatFile(char * FileName)
{
    bool success = false;

    FILE * f = fopen(FileName, "rb");

    if (f)
    {
        if (fread(&m_ItemCount, 1, sizeof(m_ItemCount), f) == sizeof(m_ItemCount))
        {
            if (m_Data)
            {
                delete [] m_Data;
            }

            m_Data = new ItemBaseStruct[m_ItemCount];

            if (fread(m_Data, 1, sizeof(ItemBaseStruct) * m_ItemCount, f) == sizeof(ItemBaseStruct) * m_ItemCount)
            {
                success = true;
            }
        }

        fclose(f);
    }

    return success;
}

int ItemBaseImporter::GetFirstID(long ID)
{
    for (int i=0; i<m_ItemCount; i++)
    {
        if (m_Data[i].m_ItemTemplateID == ID)
        {
            return i;
        }
    }

    return m_ItemCount;
}

int ItemBaseImporter::GetFirstName(char * Name)
{
    for (int i=0; i<m_ItemCount; i++)
    {
        if (!strcmpi(m_Data[i].m_Name, Name))
        {
            return i;
        }
    }

    return m_ItemCount;
}    

void ItemBaseImporter::ExportXML(char * FileName)
{
    //the '/t' charachters can be replaced for spaces but are used to make the code more readable

    FILE * f = fopen(FileName, "w");

    if (f)
    {
        char buffer[256];

        for (int idx=0; idx<m_ItemCount; idx++)
        {

            fprintf(f,"<ItemBase ItemTemplateID=\"%d\" Category=\"%d\" Subcategory=\"%d\" ItemType=\"%d\"\n\t\tGameBaseAsset=\"%d\"  IconBaseAsset=\"%d\" TechLevel=\"%d\" Cost=\"%d\"\n\t\tMaxStack=\"%d\" UseEffect=\"%d\" Flags=\"%d\">\n",
                m_Data[idx].m_ItemTemplateID, m_Data[idx].m_Category, m_Data[idx].m_SubCategory, m_Data[idx].m_ItemType, m_Data[idx].m_GameBaseAsset, m_Data[idx].m_IconBaseAsset, m_Data[idx].m_TechLevel, m_Data[idx].m_Cost, m_Data[idx].m_MaxStack, m_Data[idx].m_UseEffect, m_Data[idx].m_Flags);
            
            fprintf(f, "\t<Name>%s</Name>\n", m_Data[idx].m_Name);

            if (strlen(m_Data[idx].m_Description) > 0)
                fprintf(f, "\t<Description>%s</Description>\n", m_Data[idx].m_Description);

            if (strlen(m_Data[idx].m_Manufacturer) > 0)
                fprintf(f, "\t<Manufacturer>%s</Manufacturer>\n", m_Data[idx].m_Manufacturer);

            fprintf(f, "\t<ItemFields>\n");
            
            for (int i=0; i<ITEM_BASE_MAX_ITEM_FIELDS; i++)
            {
                if (m_Data[idx].m_ItemFields[i].HasData)
                {
                    switch (i)
                    {
                    case 0x01 : // #1  = Ammo Parent
                    case 0x0B : // #11 = Requires (0x0B) Level (ItemLVL)
                    case 0x0D : // #13 = Item Type (Subdescription)
                    case 0x1B : // #27 = String for 0x1C
                        // short string
                        strcpy(buffer, m_Data[idx].m_ItemFields[i].sData);
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
                        // 4-byte float
                        sprintf(buffer, "%f", m_Data[idx].m_ItemFields[i].fData);
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
                        // 4-byte integer
                        sprintf(buffer, "%d", m_Data[idx].m_ItemFields[i].iData);
                        break;
                    default :
                        printf("Write XML File: Unknown Item Field: %d\n", i);
                        break;
                    }

                    fprintf(f, "\t\t<Item type=\"%d\">%s</Item>\n", i, buffer);
                }
            }

            fprintf(f, "\t</ItemFields>\n");

            //Activatable Effects
            if (m_Data[idx].m_NumActivatableEffects > 0)
            {
                fprintf(f, "\t<ActivatableEffects>\n");

                for (int j=0; j<m_Data[idx].m_NumActivatableEffects; j++)
                {
                    fprintf(f, "\t\t<Effect flag1=\"%d\" flag2=\"%d\">\n", m_Data[idx].m_ActivatableEffects[j].Flag1, m_Data[idx].m_ActivatableEffects[j].Flag2);
                    fprintf(f, "\t\t\t<Name>%s</Name>\n", m_Data[idx].m_ActivatableEffects[j].Name);
                    fprintf(f, "\t\t\t<Description>%s</Description>\n", m_Data[idx].m_ActivatableEffects[j].Description);
                    fprintf(f, "\t\t\t<Tooltip>%s</Tooltip>\n", m_Data[idx].m_ActivatableEffects[j].Tooltip);

                    if (m_Data[idx].m_ActivatableEffects[j].VisualEffect > 0)
                        fprintf(f, "\t\t\t<VisualEffect>%d</VisualEffect>\n", m_Data[idx].m_ActivatableEffects[j].VisualEffect);

                    fprintf(f, "\t\t\t<DescriptionVars>\n");

                    for (int k=0; k<m_Data[idx].m_ActivatableEffects[j].DescVarCount; k++)
                    {
                        fprintf(f, "\t\t\t\t<Float>%f</Float>\n", m_Data[idx].m_ActivatableEffects[j].DescVar[k]);
                    }

                    fprintf(f, "\t\t\t</DescriptionVars>\n");
                    fprintf(f, "\t\t</Effect>\n");
                }

                fprintf(f, "\t</ActivatableEffects>\n");
            }


            //Equipable Effects
            if (m_Data[idx].m_NumEquipableEffects > 0)
            {
                fprintf(f, "\t<EquipableEffects>\n");

                for (int j=0; j<m_Data[idx].m_NumEquipableEffects; j++)
                {
                    fprintf(f, "\t\t<Effect flag1=\"%d\" flag2=\"%d\">\n", m_Data[idx].m_EquipableEffects[j].Flag1, m_Data[idx].m_EquipableEffects[j].Flag2);
                    fprintf(f, "\t\t\t<Name>%s</Name>\n", m_Data[idx].m_EquipableEffects[j].Name);
                    fprintf(f, "\t\t\t<Description>%s</Description>\n", m_Data[idx].m_EquipableEffects[j].Description);
                    fprintf(f, "\t\t\t<Tooltip>%s</Tooltip>\n", m_Data[idx].m_EquipableEffects[j].Tooltip);

                    if (m_Data[idx].m_EquipableEffects[j].VisualEffect > 0)
                        fprintf(f, "\t\t\t<VisualEffect>%d</VisualEffect>\n", m_Data[idx].m_EquipableEffects[j].VisualEffect);

                    fprintf(f, "\t\t\t<DescriptionVars>\n");

                    for (int k=0; k<m_Data[idx].m_EquipableEffects[j].DescVarCount; k++)
                    {
                        fprintf(f, "\t\t\t\t<Float>%f</Float>\n", m_Data[idx].m_EquipableEffects[j].DescVar[k]);
                    }

                    fprintf(f, "\t\t\t</DescriptionVars>\n");
                    fprintf(f, "\t\t</Effect>\n");
                }

                fprintf(f, "\t</EquipableEffects>\n");
            }

            fprintf(f, "</ItemBase>\n\n");
        }

        fclose(f);

        printf("Item XML Export succesful. %d items written\n\n", m_ItemCount);
    }
    else
    {
        printf("Unable to open %d for writing\n", FileName);
    }
    
}
