using System;
using System.Collections.Generic;
using System.Text;

namespace Net7_Tools
{
    public static class Items
	{
        public enum ItemCategory 
		{
			//Equipment
			Weapon = 10,
			Device = 11,
			Core_Item = 12,
			Consumable = 13,

			//Components
			Electronic_Item = 50,
			Reactor_Component = 51,
			Fabricated_Item = 52,
			Weapon_Component = 53,
			Ammo_Component = 54,

			//Ores
			Refined_Resource = 80,
            Raw_Resource = 81,

			//Trade
			Trade_Good = 90,

			//Other
			Looted_Item = -1,
            Invalid = 0 // Used to display erronous data
		}

/*
        public static IGui.EditorType getEditorType(ItemCategory itemCategory)
        {
            switch (itemCategory)
            {
            }
        }
*/

        public static string CategoryLookup(ItemCategory itemCategory)
        {
            return itemCategory.ToString().Replace('_', ' ');
        }

        public static String[] GetCategoriesByName()
        {
            String[] categories = Enum.GetNames(typeof(ItemCategory));
            Array.Sort(categories);
            return categories;
        }

        public static String[] SortEnumByName(System.Type enumeration)
        {
            String[] enumNames = Enum.GetNames(enumeration);
            Array.Sort(enumNames);
            return enumNames;
        }

        public static Boolean isComponent(ItemCategory itemCategory)
        {
            return itemCategory.Equals(Items.ItemCategory.Ammo_Component)
                || itemCategory.Equals(Items.ItemCategory.Consumable)
                || itemCategory.Equals(Items.ItemCategory.Electronic_Item)
                || itemCategory.Equals(Items.ItemCategory.Fabricated_Item)
                || itemCategory.Equals(Items.ItemCategory.Invalid)
                || itemCategory.Equals(Items.ItemCategory.Looted_Item)
                || itemCategory.Equals(Items.ItemCategory.Raw_Resource)
                || itemCategory.Equals(Items.ItemCategory.Reactor_Component)
                || itemCategory.Equals(Items.ItemCategory.Refined_Resource)
                || itemCategory.Equals(Items.ItemCategory.Trade_Good)
                || itemCategory.Equals(Items.ItemCategory.Weapon_Component);
        }

        public enum ItemSubCategory
		{
			//Weapon
			Beam_Weapon = 100,
			Projectile_Launcher = 101,
			Missile_Launcher = 102,
			Ammo = 103,

			//Device
			Device = 110,
			Drone = 111,

			//Core
			Reactor = 120,
			Engine = 121,
			Shield = 122,

			//Consumable
			Consumable = 130,

			//Electronic Components
			Software = 140,
			Electronic_Item = 141,
			Computer = 142,

			//Reactors
			Power_Converter = 150,
			Power_Coupling = 151,
			Power_Core = 152,
			Power_Generator = 153,

			//Fabricated
			Casing = 160,
			Engine_Frame = 161,
			Drone_Frame = 162,
			Mount = 163,

			//Weapon
			Firing_Mechanism = 170,
			Ammunition_Feeder = 171,
			Beam_Optic = 172,
			Weapon_Barrel = 173,

			//Ammo
			Ammunition_Slug = 180,
			Warhead = 181,
			Propellant = 182,
			Shell_Casing = 183,

			//Refined
			Optic_Gem = 200,
			Metal = 201,
			Conductor = 202,
			Alloy = 203,
			Radioactive = 204,
			Catalyst = 205,
			Hydrocarbon = 206,
			Silicate = 207,
			Magnetic = 208,
			Gemstone = 209,
			Gas = 210,
			Core_Metal = 211,

			//Other
			Other = -1,
            Invalid = 0 // Used to display erronous data
		}

        public static frmItems.EditorType getEditorType(ItemSubCategory itemSubCategory)
        {
            switch (itemSubCategory)
            {
                case ItemSubCategory.Ammo:
                    return frmItems.EditorType.Items_Ammo;
                case ItemSubCategory.Beam_Weapon:
                    return frmItems.EditorType.Items_Beam;
                case ItemSubCategory.Device:
                    return frmItems.EditorType.Items_Device;
                case ItemSubCategory.Engine:
                    return frmItems.EditorType.Items_Engine;
                case ItemSubCategory.Missile_Launcher:
                    return frmItems.EditorType.Items_Missile;
                case ItemSubCategory.Projectile_Launcher:
                    return frmItems.EditorType.Items_Projectile;
                case ItemSubCategory.Reactor:
                    return frmItems.EditorType.Items_Reactor;
                case ItemSubCategory.Shield:
                    return frmItems.EditorType.Items_Shield;
            }
            return frmItems.EditorType.Items;
        }

        public static string SubCategoryLookup(ItemSubCategory SubCat)
		{
			return SubCat.ToString().Replace('_',' ');
		}


	}
}
