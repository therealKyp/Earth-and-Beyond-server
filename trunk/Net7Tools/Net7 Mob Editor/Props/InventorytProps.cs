using System;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel;

namespace N7.Props
{
    [DefaultPropertyAttribute("DropChance")]
    class InventoryProps
    {
        private int drop_chance;
        private int quantity;

        [CategoryAttribute("Item Settings"), DescriptionAttribute("The chance that this item will be dropped by the mob as a loot item for players.")]
        public int DropChance
        {
            get { return drop_chance; }
            set { drop_chance = value; }
        }

        [CategoryAttribute("Item Settings"), DescriptionAttribute("The total amount of this item that the mob has in inventory.")]
        public int Quantity
        {
            get { return quantity; }
            set { quantity = value; }
        }
    }
}
