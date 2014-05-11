using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using CommonTools.Database;

namespace Net7_Tools.Database
{
    class Restrictions
    {
        private static CheckBox m_terran;
        private static CheckBox m_jenquai;
        private static CheckBox m_progen;
        private static Label m_race;
        private static CheckBox m_warrior;
        private static CheckBox m_trader;
        private static CheckBox m_explorer;
        private static Label m_profession;
        private static Boolean m_suspendValueChanged = false;

        public static void setRestrictions(CheckBox terran,
                                           CheckBox jenquai,
                                           CheckBox progen,
                                           Label race,
                                           CheckBox warrior,
                                           CheckBox trader,
                                           CheckBox explorer,
                                           Label profession)
        {
            m_terran = terran;
            m_terran.CheckedChanged += new EventHandler(raceRestrictionChanged);

            m_jenquai = jenquai;
            m_jenquai.CheckedChanged += new EventHandler(raceRestrictionChanged);

            m_progen = progen;
            m_progen.CheckedChanged += new EventHandler(raceRestrictionChanged);

            m_race = race;

            m_warrior = warrior;
            m_warrior.CheckedChanged += new EventHandler(professionRestrictionChanged);
            
            m_trader = trader;
            m_trader.CheckedChanged += new EventHandler(professionRestrictionChanged);
            
            m_explorer = explorer;
            m_explorer.CheckedChanged += new EventHandler(professionRestrictionChanged);

            m_profession = profession;
        }

        enum Profession { None = 0, Warrior = 1, Trader = 2, Explorer = 4 };
        enum Race { None = 0, Terran = 1, Jenquai = 2, Progen = 4 };

        public static Boolean displayField(System.Data.DataRow dataRow, Enum raceField, Enum professionField)
        {
            Int32 race = dataRow == null ? 0 : ColumnData.GetInt32(dataRow, raceField);
            m_suspendValueChanged = true;
            m_terran.Checked = (race & (int)Race.Terran) != 0;
            m_jenquai.Checked = (race & (int)Race.Jenquai) != 0;
            m_progen.Checked = (race & (int)Race.Progen) != 0;

            Int32 profession = dataRow == null ? 0 : ColumnData.GetInt32(dataRow, professionField);
            m_warrior.Checked = (profession & (int)Profession.Warrior) != 0;
            m_trader.Checked = (profession & (int)Profession.Trader) != 0;
            m_explorer.Checked = (profession & (int)Profession.Explorer) != 0;
            m_suspendValueChanged = false;

            setRaceDescription(getRace());
            setProfessionDescription(getProfession());

            return true;
        }

        public static Boolean updateField(Boolean newRecord, System.Data.DataRow dataRow, Enum raceField, Enum professionField)
        {
            Int32 race = getRace();
            String columnName = ColumnData.GetName(raceField);
            dataRow[columnName] = race;

            Int32 profession = getProfession();
            dataRow[ColumnData.GetName(professionField)] = profession;
            return true;
        }

        public static Boolean enableField(Boolean enabled)
        {
            m_suspendValueChanged = true;
            m_terran.Enabled = enabled;
            m_jenquai.Enabled = enabled;
            m_progen.Enabled = enabled;
            m_warrior.Enabled = enabled;
            m_trader.Enabled = enabled;
            m_explorer.Enabled = enabled;
            m_suspendValueChanged = false;

            setRaceDescription(getRace());
            setProfessionDescription(getProfession());

/*
            m_terran.Enabled = enabled;
            m_jenquai.Enabled = enabled;
            m_progen.Enabled = enabled;
            m_warrior.Enabled = enabled;
            m_trader.Enabled = enabled;
            m_explorer.Enabled = enabled;

            if (enabled == false)
            {
                m_suspendValueChanged = true;
                m_terran.Checked = false;
                m_jenquai.Checked = false;
                m_progen.Checked = false;
                m_warrior.Checked = false;
                m_trader.Checked = false;
                m_explorer.Checked = false;
                m_suspendValueChanged = false;

                setRaceDescription(getRace());
                setProfessionDescription(getProfession());
            }
*/
            return true;
        }

        public static Int32 getRace()
        {
            Int32 race = 0;
            if (m_terran.Checked)
            {
                race |= (int) Race.Terran;
            }
            if (m_jenquai.Checked)
            {
                race |= (int)Race.Jenquai;
            }
            if (m_progen.Checked)
            {
                race |= (int)Race.Progen;
            }
            return race;
        }

        public static Int32 getProfession()
        {
            Int32 profession = 0;
            if (m_warrior.Checked)
            {
                profession |= (int)Profession.Warrior;
            }
            if (m_trader.Checked)
            {
                profession |= (int)Profession.Trader;
            }
            if (m_explorer.Checked)
            {
                profession |= (int)Profession.Explorer;
            }
            return profession;
        }

        private static void raceRestrictionChanged(object sender, EventArgs e)
        {
            if (!m_suspendValueChanged)
            {
                Int32 race = getRace();
                setRaceDescription(race);
            }
        }

        private static void professionRestrictionChanged(object sender, EventArgs e)
        {
            if (!m_suspendValueChanged)
            {
                Int32 profession = getProfession();
                setProfessionDescription(profession);
            }
        }

        private static void setRaceDescription(Int32 race)
        {
            String description = "";
            switch(race)
            {
                case 0:
                    description = "Unrestricted";
                    break;
                case 1:
                    description = "Terran restricted";
                    break;
                case 2:
                    description = "Jenquai restricted";
                    break;
                case 3:
                    description = "Progen only";
                    break;
                case 4:
                    description = "Progen restricted";
                    break;
                case 5:
                    description = "Jenquai only";
                    break;
                case 6:
                    description = "Terran only";
                    break;
                case 7:
                    description = "***Every Race is Restricted***";
                    break;
            }
            m_race.Text = "Result: " + description;
        }

        private static void setProfessionDescription(Int32 profession)
        {
            String description = "";
            switch (profession)
            {
                case 0:
                    description = "Unrestricted";
                    break;
                case 1:
                    description = "Warrior restricted";
                    break;
                case 2:
                    description = "Trader restricted";
                    break;
                case 3:
                    description = "Explorer only";
                    break;
                case 4:
                    description = "Explorer restricted";
                    break;
                case 5:
                    description = "Trader only";
                    break;
                case 6:
                    description = "Warrior only";
                    break;
                case 7:
                    description = "***Every Profession is Restricted***";
                    break;
            }
            m_profession.Text = "Result: " + description;
        }
    }
}
