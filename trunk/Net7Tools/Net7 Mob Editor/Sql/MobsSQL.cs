using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Text;
using System.Data;

namespace N7.Sql
{
    public class MobsSQL
    {
        private DataTable mobs;

        public MobsSQL()
        {
            String mobsQuery = "SELECT * FROM mob_base order by name, level;";
            mobs = Database.executeQuery(Database.DatabaseName.net7, mobsQuery);
        }

        public DataRow getRowByID(int selectedRowID)
        {
            DataRow[] foundRows;
            foundRows = mobs.Select("mob_id = '" + selectedRowID + "'");
            DataRow foundRow = foundRows[0];

            return foundRow;
        }

        public DataRow[] getRowsByNameQuery(String query)
        {
            DataRow[] foundRows;
            foundRows = mobs.Select(query);

            return foundRows;
        }

        public DataRow[] getRowsBetween(int level)
        {
            DataRow[] foundRows;
            foundRows = mobs.Select("level = '" + level + "'");
            return foundRows;
        }

        public DataTable getMobTable()
        {
            return mobs;
        }

        public int newRecord()
        {
            String name = "<New Mob>";
            int level = 1;
            int type = 0;
            int altruism = 0;
            int aggressiveness = 0;
            int bravery = 0;
            int intelligence = 0;
            int factionID = 0;
            int baseAssetID = -1;
            float h = 0;
            float s = 0;
            float v = 0;
            float scale = 1;
            String ai = "";

            String query = "INSERT INTO mob_base SET name='"+name+"', level='"+level+"', type='"+type+"', "+
                "altruism='"+altruism+"', aggressiveness='"+aggressiveness+"', bravery='"+bravery+"', intelligence='"+intelligence+"', "+
                "faction_id='"+factionID+"', base_asset_id='"+baseAssetID+"', h='"+h+"', s='"+s+"', v='"+v+"', scale='"+scale+"', ai='"+ai+"';";
            
            DataTable executeInsert = Database.executeQuery(Database.DatabaseName.net7, query);
            DataTable lastInsert = Database.executeQuery(Database.DatabaseName.net7, "SELECT last_insert_id() from mob_base;");

            int lastInsertID = int.Parse(lastInsert.Rows[0]["last_insert_id()"].ToString());

            DataRow newRow = mobs.NewRow();
            newRow["mob_id"] = lastInsertID;
            newRow["name"] = "<New Mob>";
            newRow["level"] = 1;
            newRow["type"] = 0;
            newRow["altruism"] = 0;
            newRow["aggressiveness"] = 0;
            newRow["bravery"] = 0;
            newRow["intelligence"] = 0;
            newRow["faction_id"] = 0;
            newRow["base_asset_id"] = -1;
            newRow["h"] = 0;
            newRow["s"] = 0;
            newRow["v"] = 0;
            newRow["scale"] = 1;
            newRow["ai"] = "";

            mobs.Rows.Add(newRow);

            newRow.AcceptChanges();
            mobs.AcceptChanges();

            return lastInsertID;
        }

        public void deleteRecord(int id, DataRow dr)
        {
            String query = "DELETE FROM mob_base where mob_id='"+id+"';";
            DataTable executeDelete = Database.executeQuery(Database.DatabaseName.net7, query);

            mobs.Rows.Remove(dr);
        }

        public void updateRecord(DataRow dr)
        {
            int mobID = int.Parse(dr["mob_id"].ToString());
            int level = int.Parse(dr["level"].ToString());
            int type = int.Parse(dr["type"].ToString());
            int altruism = int.Parse(dr["altruism"].ToString());
            int aggressiveness = int.Parse(dr["aggressiveness"].ToString());
            int bravery = int.Parse(dr["bravery"].ToString());
            int intelligence = int.Parse(dr["intelligence"].ToString());
            int factionID = int.Parse(dr["faction_id"].ToString());
            int baseAssetID = int.Parse(dr["base_asset_id"].ToString());
            float h = float.Parse(dr["h"].ToString());
            float s = float.Parse(dr["s"].ToString());
            float v = float.Parse(dr["v"].ToString());
            float scale = float.Parse(dr["scale"].ToString());
            String ai = dr["ai"].ToString();

            String name = dr["name"].ToString();
            String name2 = name.Replace("'", "''");

            String query = "UPDATE mob_base SET name='" + name2 + "', level='" + level + "', type='" + type + "', " +
                "altruism='" + altruism + "', aggressiveness='" + aggressiveness + "', bravery='" + bravery + "', intelligence='" + intelligence + "', " +
                "faction_id='" + factionID + "', base_asset_id='" + baseAssetID + "', h='" + h + "', s='" + s + "', v='" + v + "', scale='" + scale + "', ai='" + ai + "' "+
                "where mob_id='"+mobID+"';";

            DataTable executeUpdate = Database.executeQuery(Database.DatabaseName.net7, query);
        }

        public int newFromRecord(DataRow dr)
        {
            int mobID = int.Parse(dr["mob_id"].ToString());
            int level = int.Parse(dr["level"].ToString());
            int type = int.Parse(dr["type"].ToString());
            int altruism = int.Parse(dr["altruism"].ToString());
            int aggressiveness = int.Parse(dr["aggressiveness"].ToString());
            int bravery = int.Parse(dr["bravery"].ToString());
            int intelligence = int.Parse(dr["intelligence"].ToString());
            int factionID = int.Parse(dr["faction_id"].ToString());
            int baseAssetID = int.Parse(dr["base_asset_id"].ToString());
            float h = float.Parse(dr["h"].ToString());
            float s = float.Parse(dr["s"].ToString());
            float v = float.Parse(dr["v"].ToString());
            float scale = float.Parse(dr["scale"].ToString());
            String ai = dr["ai"].ToString();

            String name = dr["name"].ToString();
            String name2 = name.Replace("'", "''");

            String query = "INSERT INTO mob_base SET name='" + name2 + "', level='" + level + "', type='" + type + "', " +
                "altruism='" + altruism + "', aggressiveness='" + aggressiveness + "', bravery='" + bravery + "', intelligence='" + intelligence + "', " +
                "faction_id='" + factionID + "', base_asset_id='" + baseAssetID + "', h='" + h + "', s='" + s + "', v='" + v + "', scale='" + scale + "', ai='" + ai + "';";

            DataTable executeInsert = Database.executeQuery(Database.DatabaseName.net7, query);
            DataTable lastInsert = Database.executeQuery(Database.DatabaseName.net7, "SELECT last_insert_id() from mob_base;");

            int lastInsertID = int.Parse(lastInsert.Rows[0]["last_insert_id()"].ToString());

            DataRow newRow = mobs.NewRow();
            newRow["mob_id"] = lastInsertID;
            newRow["name"] = name2;
            newRow["level"] = level;
            newRow["type"] = type;
            newRow["altruism"] = altruism;
            newRow["aggressiveness"] = aggressiveness;
            newRow["bravery"] = bravery;
            newRow["intelligence"] = intelligence;
            newRow["faction_id"] = factionID;
            newRow["base_asset_id"] = baseAssetID;
            newRow["h"] = h;
            newRow["s"] = s;
            newRow["v"] = v;
            newRow["scale"] = scale;
            newRow["ai"] = ai;

            mobs.Rows.Add(newRow);

            newRow.AcceptChanges();
            mobs.AcceptChanges();

            return lastInsertID;
        }

    }
}
