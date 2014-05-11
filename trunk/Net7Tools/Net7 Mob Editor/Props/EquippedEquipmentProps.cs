using System;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel;

namespace N7.Props
{
    [DefaultPropertyAttribute("UsageChance")]
    class EquippedEquipmentProps
    {
        private int usage_chance;
        private int drop_chance;
        private int quantity;

        [CategoryAttribute("Item Settings"), DescriptionAttribute("The chance that this item is used by the mob.")]
        public int UsageChance
        {
            get{ return usage_chance; }
            set { usage_chance = value; }
        }

        [CategoryAttribute("Item Settings"), DescriptionAttribute("The chance that this item will be dropped by the mob as a loot item for players.")]
        public int DropChance
        {
            get { return drop_chance; }
            set { drop_chance = value; }
        }

        [CategoryAttribute("Item Settings"), DescriptionAttribute("The maximum amount of this item that can be equipped at any given time.")]
        public int Quantity
        {
            get { return quantity; }
            set { quantity = value; }
        }
    }
}
