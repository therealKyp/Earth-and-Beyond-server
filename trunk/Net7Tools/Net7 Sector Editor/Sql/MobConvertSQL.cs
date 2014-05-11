using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Text;
using System.Data;

namespace N7.Sql
{
    class MobConvertSQL
    {

        public MobConvertSQL()
        {
            String oldMobQuery = "SELECT * FROM mob;";
            DataTable oldMobs = Database.executeQuery(Database.DatabaseName.tmp_enbemulator, oldMobQuery);

            foreach (DataRow r in oldMobs.Rows)
            {
                //New Schema
                String name = "name='" + r["name"].ToString().Replace("'","''") + "', ";
                String level = "level='" + r["level"].ToString() + "', ";
                String hull_hitpoints = "hull_hitpoints='" + r["hull"].ToString() + "', ";
                String dps = "dps='" + 0 + "', ";
                String fire_rate = "fire_rate='" + 0 + "', ";
                String shield_hitpoints = "shield_hitpoints='" + r["shield"].ToString() + "', ";
                String shield_recovery_delay = "shield_recovery_delay='" + 0 + "', ";
                String shield_recovery_rate = "shield_recovery_rate='" + r["shield_recharge"].ToString() + "', ";
                String intelligence = "intelligence='" + r["intelligence"].ToString() + "', ";
                String bravery = "bravery='" + r["bravery"].ToString() + "', ";
                String type = "type='" + r["type"].ToString() + "', ";
                String explosive_modifier = "explosive_modifier='" + 0 + "', ";
                String chemical_modifier = "chemical_modifier='" + 0 + "', ";
                String impact_modifier = "impact_modifier='" + 0 + "', ";
                String emp_modifier = "emp_modifier='" + 0 + "', ";
                String energy_modifier = "energy_modifier='" + 0 + "', ";
                String plasma_modifier = "plasma_modifier='" + 0 + "', ";
                String loot_group_id = "loot_group_id='" + -1 + "', ";
                String faction_id = "faction_id='" + r["faction_fk"].ToString() + "', ";
                String skills_group_id = "skills_group_id='" + -1 + "', ";

                //---Possibly add to new schema

                String base_asset_id = "base_asset_id='" + -1 +"', ";
                if (r["3d_id"].ToString() != "")
                {
                    base_asset_id = "base_asset_id='" + r["3d_id"].ToString() + "', ";
                }
                String speed = "speed='" + r["speed"].ToString() + "', ";
                String altruism = "altruism='" + r["altruism"].ToString() + "', ";
                String aggressiveness = "aggressiveness='" + r["aggressiveness"].ToString() + "', ";
                String reactor_charge = "reactor_charge='" + r["reactor"].ToString() + "', ";
                String reactor_recovery_delay = "reactor_recovery_delay='" + 0 + "', ";
                String reactor_recharge_rate = "reactor_recharge_rate='" + r["reactor_recharge"].ToString() + "'";

                String newMobInsert = "INSERT INTO mob_base SET " + name + level + hull_hitpoints + dps + fire_rate +
                    shield_hitpoints + shield_recovery_delay + shield_recovery_rate + intelligence + bravery +
                    type + explosive_modifier + chemical_modifier + impact_modifier + emp_modifier + energy_modifier +
                    plasma_modifier + loot_group_id + faction_id + skills_group_id + base_asset_id + speed + altruism +
                    aggressiveness + reactor_charge + reactor_recovery_delay + reactor_recharge_rate + ";";

                Console.Out.WriteLine(newMobInsert);
                DataTable newMobs = Database.executeQuery(Database.DatabaseName.net7, newMobInsert);
            }
        }
    }
}
