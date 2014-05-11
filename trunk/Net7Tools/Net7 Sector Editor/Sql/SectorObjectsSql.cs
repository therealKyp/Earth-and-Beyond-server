using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Text;
using System.Data;

namespace N7.Sql
{
    public class SectorObjectsSql
    {
        private DataTable sectorObjects;
        private String sectorID;

        public SectorObjectsSql(String sectorName)
        {
            String sectorName2 = sectorName.Replace("'", "''");
            String sectorIDQuery = "SELECT sector_id FROM sectors where name='"+sectorName2+"';";

            DataTable tmp = Database.executeQuery(Database.DatabaseName.net7, sectorIDQuery);

            foreach (DataRow r in tmp.Rows)
            {
                sectorID = r["sector_id"].ToString();
                String soQuery = "SELECT * FROM sector_objects left join sector_nav_points"+
                                    " on sector_objects.sector_object_id = sector_nav_points.sector_object_id" +
                                    " left join sector_objects_harvestable" +
                                    " on sector_objects.sector_object_id = sector_objects_harvestable.resource_id" +
                                    " left join sector_objects_planets" +
                                    " on sector_objects.sector_object_id = sector_objects_planets.planet_id" +
                                    " left join sector_objects_starbases" +
                                    " on sector_objects.sector_object_id = sector_objects_starbases.starbase_id" +
                                    " left join sector_objects_stargates" +
                                    " on sector_objects.sector_object_id = sector_objects_stargates.stargate_id" +
                                    " left join sector_objects_mob" +
                                    " on sector_objects.sector_object_id = sector_objects_mob.mob_id" +
                                    " where sector_objects.sector_id='" + sectorID + "' order by sector_objects.type;";
                sectorObjects = Database.executeQuery(Database.DatabaseName.net7, soQuery);
            }
            tmp.Dispose();
        }

        public void updateRow(DataRow r)
        {
            int isHuge = Convert.ToInt32(r["is_huge"]);
            int air = Convert.ToInt32(r["appears_in_radar"]);
            int type = int.Parse(r["type"].ToString());

            String name = r["name"].ToString().Replace("'","''");

            String navPointQuery = "UPDATE sector_nav_points SET nav_type='"+r["nav_type"].ToString()+"', "+
            "signature='"+r["signature"].ToString()+"', is_huge='"+isHuge+"', "+
            "base_xp='" + r["base_xp"].ToString() + "', exploration_range='" + r["exploration_range"].ToString() + "' " +
            "where sector_object_id='" + r["sector_object_id"].ToString() +"';";

            String sectorObQuery = "UPDATE sector_objects SET base_asset_id='" + r["base_asset_id"].ToString() + "', " +
            "h='" + r["h"].ToString() + "', s='" + r["s"].ToString() + "' ,v='" + r["v"].ToString() + "', " +
            "type='" + r["type"].ToString() + "', scale='" + r["scale"].ToString() + "', position_x='" + r["position_x"].ToString() + "', " +
            "position_y='" + r["position_y"].ToString() + "', position_z='" + r["position_z"].ToString() + "', " +
            "orientation_u='" + r["orientation_u"].ToString() + "', orientation_v='" + r["orientation_v"].ToString() + "', " +
            "orientation_w='" + r["orientation_w"].ToString() + "', orientation_z='" + r["orientation_z"].ToString() + "', " +
            "name='" + name + "', appears_in_radar='" + air + "', " +
            "radar_range='" + r["radar_range"].ToString() + "', gate_to='" + r["gate_to"].ToString() + "', " +
            "sound_effect_id='" + r["sound_effect_id"].ToString() + "', sound_effect_range='" + r["sound_effect_range"].ToString() + "' " +
            "where sector_object_id='" + r["sector_object_id"].ToString() + "';";

            try
            {
                Database.executeQuery(Database.DatabaseName.net7, navPointQuery);
                Database.executeQuery(Database.DatabaseName.net7, sectorObQuery);

                String specialQuery = "";
                switch (type)
                {
                    case 0:
                        
                        int delayed_spawn = Convert.ToInt32(r["delayed_spawn"]); 
                        specialQuery = "UPDATE sector_objects_mob SET "+
                        "mob_count='" + r["mob_count"].ToString() + "', " +
                        "mob_spawn_radius='" + r["mob_spawn_radius"].ToString() + "', "+
                        "respawn_time='" + r["respawn_time"].ToString() + "', " +
                        "delayed_spawn='" + delayed_spawn + "' " +
                        "where mob_id='" + r["mob_id"].ToString() + "';";
                        break;
                    case 3:
                        int isLandable = Convert.ToInt32(r["is_landable"]);

                        specialQuery = "UPDATE sector_objects_planets SET orbit_id='" + r["orbit_id"].ToString() + "', orbit_dist='" + r["orbit_dist"].ToString() + "', " +
                        "orbit_angle='" + r["orbit_angle"].ToString() + "', orbit_rate='" + r["orbit_rate"].ToString() + "', rotate_angle='" + r["rotate_angle"].ToString() + "', " +
                        "rotate_rate='" + r["rotate_rate"].ToString() + "', tilt_angle='" + r["tilt_angle"].ToString() + "', is_landable='" + isLandable + "' " +
                        "where planet_id='" + r["planet_id"].ToString() + "';";
                        break;
                    case 11: //stargate
                        int isClassSpecific = Convert.ToInt32(r["classSpecific"]);

                        specialQuery = "UPDATE sector_objects_stargates SET classSpecific='" + isClassSpecific + "', " +
                        "faction_id='" + r["faction_id"].ToString() + "' where stargate_id='" + r["stargate_id"].ToString() + "';";
                        break;
                    case 12: //starbase
                        int isCapShip = Convert.ToInt32(r["capShip"]);
                        int isDockable = Convert.ToInt32(r["dockable"]);

                        specialQuery = "UPDATE sector_objects_starbases SET capShip='"+isCapShip+"', "+
                        "dockable='" + isDockable + "' where starbase_id='" + r["starbase_id"].ToString() + "';";
                        break;
                    case 38:
                        specialQuery = "UPDATE sector_objects_harvestable SET level='" + r["level"].ToString() + "', " +
                        "field='" + r["field"].ToString() + "', res_count='" + r["res_count"].ToString() + "', " +
                        "spawn_radius='" + r["spawn_radius"].ToString() + "', pop_rock_chance='" + r["pop_rock_chance"].ToString() + "', " +
                        "max_field_radius='" + r["max_field_radius"].ToString() + "' " +
                        "where resource_id='" + r["resource_id"].ToString() + "';";
                        break;
                }

                if (type != 37)
                {
                    Database.executeQuery(Database.DatabaseName.net7, specialQuery);
                }
            }
            catch (Exception e)
            {
                MessageBox.Show(e + "\n" + e.StackTrace);
            }
        }

        public void deleteRow(int id, int type)
        {
            String navPointQuery = "DELETE FROM sector_nav_points where sector_object_id='"+id+"';";
            String sectorObQuery = "DELETE FROM sector_objects where sector_object_id='" + id + "';";

            String specialQuery = "";
            String mobGroupQuery = "";
            String harvGroupQuery = "";
            switch (type)
            {
                case 0:
                    specialQuery = "DELETE FROM sector_objects_mob where mob_id='" + id + "';";
                    mobGroupQuery = "DELETE FROM mob_spawn_group where spawn_group_id='" + id + "';";
                    break;
                case 3:
                    specialQuery = "DELETE FROM sector_objects_planets where planet_id='" + id + "';";
                    break;
                case 11: //stargate
                    specialQuery = "DELETE FROM sector_objects_stargates where stargate_id='" + id + "';";
                    break;
                case 12: //starbase
                    specialQuery = "DELETE FROM sector_objects_starbases where starbase_id='" + id + "';";
                    break;
                case 38:
                    specialQuery = "DELETE FROM sector_objects_harvestable where resource_id='" + id + "';";
                    harvGroupQuery = "DELETE FROM sector_objects_harvestable_restypes where group_id='" + id + "';";
                    mobGroupQuery = "DELETE FROM mob_spawn_group where spawn_group_id='" + id + "';";
                    break;
            }

            try
            {
                if (type == 0 || type == 38)
                {
                    Database.executeQuery(Database.DatabaseName.net7, mobGroupQuery);
                }
                if (type == 38)
                {
                    Database.executeQuery(Database.DatabaseName.net7, harvGroupQuery);
                }
                if (type != 37)
                {
                    Database.executeQuery(Database.DatabaseName.net7, specialQuery);
                }

                Database.executeQuery(Database.DatabaseName.net7, navPointQuery);
                Database.executeQuery(Database.DatabaseName.net7, sectorObQuery);
            }
            catch (Exception)
            {
                
                throw;
            }
        }

        public void newRow(DataRow r)
        {
            int isHuge = Convert.ToInt32(r["is_huge"]);
            int air = Convert.ToInt32(r["appears_in_radar"]);
            int type = int.Parse(r["type"].ToString());

            String name = r["name"].ToString().Replace("'", "''");
            Console.Out.WriteLine(r["sound_effect_id"].ToString());

            String sectorObQuery = "INSERT INTO sector_objects SET base_asset_id='" + r["base_asset_id"].ToString() + "', " +
            "h='" + r["h"].ToString() + "', s='" + r["s"].ToString() + "' ,v='" + r["v"].ToString() + "', " +
            "type='" + r["type"].ToString() + "', scale='" + r["scale"].ToString() + "', position_x='" + r["position_x"].ToString() + "', " +
            "position_y='" + r["position_y"].ToString() + "', position_z='" + r["position_z"].ToString() + "', " +
            "orientation_u='" + r["orientation_u"].ToString() + "', orientation_v='" + r["orientation_v"].ToString() + "', " +
            "orientation_w='" + r["orientation_w"].ToString() + "', orientation_z='" + r["orientation_z"].ToString() + "', " +
            "name='" + name + "', appears_in_radar='" + air + "', sector_id='"+r["sector_id"].ToString()+"', " +
            "radar_range='" + r["radar_range"].ToString() + "', gate_to='" + r["gate_to"].ToString() + "', "+
            "sound_effect_id='" + r["sound_effect_id"].ToString() + "', sound_effect_range='" + r["sound_effect_range"].ToString() + "';";

            int lastInsertID = 0;
            try
            {
                Database.executeQuery(Database.DatabaseName.net7, sectorObQuery);

                DataTable tmp = Database.executeQuery(Database.DatabaseName.net7, "SELECT LAST_INSERT_ID()");
                foreach (DataRow z in tmp.Rows)
                {
                    lastInsertID = int.Parse(z["LAST_INSERT_ID()"].ToString());
                }
            }
            catch (Exception)
            {
                throw;
            }

            String navPointQuery = "INSERT INTO sector_nav_points SET sector_object_id='" + lastInsertID + "', nav_type='" + r["nav_type"].ToString() + "', " +
                "signature='" + r["signature"].ToString() + "', is_huge='" + isHuge + "', sector_id='" + r["sector_id"].ToString() + "', " +
                "base_xp='" + r["base_xp"].ToString() + "', exploration_range='" + r["exploration_range"].ToString() + "';";

            try
            {
                Database.executeQuery(Database.DatabaseName.net7, navPointQuery);

                String specialQuery = "";
                switch (type)
                {
                    case 0:
                        int delayed_spawn = Convert.ToInt32(r["delayed_spawn"]); 
                        specialQuery = "INSERT INTO sector_objects_mob SET mob_id='" + lastInsertID + "', " +
                        "mob_count='" + r["mob_count"].ToString() + "', " +
                        "mob_spawn_radius='" + r["mob_spawn_radius"].ToString() + "', " +
                        "respawn_time='" + r["respawn_time"].ToString() + "', " +
                        "delayed_spawn='" + delayed_spawn + "';";
                        break;
                    case 3:
                        int isLandable = Convert.ToInt32(r["is_landable"]);

                        specialQuery = "INSERT INTO sector_objects_planets SET planet_id='" + lastInsertID + "', orbit_id='" + r["orbit_id"].ToString() + "', orbit_dist='" + r["orbit_dist"].ToString() + "', " +
                        "orbit_angle='" + r["orbit_angle"].ToString() + "', orbit_rate='" + r["orbit_rate"].ToString() + "', rotate_angle='" + r["rotate_angle"].ToString() + "', " +
                        "rotate_rate='" + r["rotate_rate"].ToString() + "', tilt_angle='" + r["tilt_angle"].ToString() + "', is_landable='" + isLandable + "';";
                        break;
                    case 11: //stargate
                        int isClassSpecific = Convert.ToInt32(r["classSpecific"]);

                        specialQuery = "INSERT INTO sector_objects_stargates SET stargate_id='" + lastInsertID + "', classSpecific='" + isClassSpecific + "', " +
                        "faction_id='" + r["faction_id"].ToString() + "';";
                        break;
                    case 12: //starbase
                        int isCapShip = Convert.ToInt32(r["capShip"]);
                        int isDockable = Convert.ToInt32(r["dockable"]);

                        specialQuery = "INSERT INTO sector_objects_starbases SET starbase_id='" + lastInsertID + "', capShip='" + isCapShip + "', " +
                        "dockable='" + isDockable + "';";
                        break;
                    case 38:
                        specialQuery = "INSERT INTO sector_objects_harvestable SET resource_id='" + lastInsertID + "', level='" + r["level"].ToString() + "', " +
                        "field='" + r["field"].ToString() + "', res_count='" + r["res_count"].ToString() + "', " +
                        "spawn_radius='" + r["spawn_radius"].ToString() + "', pop_rock_chance='" + r["pop_rock_chance"].ToString() + "', " +
                        "max_field_radius='" + r["max_field_radius"].ToString() + "';";
                        break;
                }
                if (type != 37)
                {
                    Database.executeQuery(Database.DatabaseName.net7, specialQuery);
                }

                r["sector_object_id"] = lastInsertID;
                r.AcceptChanges();
            }
            catch (Exception)
            {
                throw;
            }
        }

        public DataTable getSectorObject()
        {
            return sectorObjects;
        }
    }
}
