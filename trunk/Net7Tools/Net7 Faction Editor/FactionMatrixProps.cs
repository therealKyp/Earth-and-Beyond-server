using System;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel;

namespace N7
{
    [DefaultPropertyAttribute("BaseValue")]
    class FactionMatrixProps
    {
        private int id;
        private int faction_id;
        private int faction_entry_id;
        private int base_value;
        private int current_value;
        private bool pda_text;

        [BrowsableAttribute(false)]
        public int ID
        {
            get { return id; }
            set { id = value; }
        }

        [BrowsableAttribute(false)]
        public int FactionID
        {
            get { return faction_id; }
            set { faction_id = value; }
        }

        [BrowsableAttribute(false)]
        public int FactionEntryID
        {
            get { return faction_entry_id; }
            set { faction_entry_id = value; }
        }

        [CategoryAttribute("Faction Matrix Props"), DescriptionAttribute("The base like / dislike value for this faction. + and - allowed.")]
        public int BaseValue
        {
            get { return base_value; }
            set { base_value = value; }
        }

        [CategoryAttribute("Faction Matrix Props"), ReadOnlyAttribute(true), DescriptionAttribute("The current like / dislike value of the faction in the live server.")]
        public int CurrentValue
        {
            get { return current_value; }
            set { current_value = value; }
        }

        [CategoryAttribute("Faction Matrix Props"), DescriptionAttribute("Faction for the currently selected faction Increases or descreases if you kill the root faction. True for Increases, False for Decreases.")]
        public bool RewardFaction
        {
            get { return pda_text; }
            set { pda_text = value; }
        }
    }
}
