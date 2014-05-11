using System;
using System.Collections.Generic;
using System.Text;
using System.Data;

namespace N7.Sql
{
    public class MobItemsSQL
    {
        private DataTable mobItems;

        public MobItemsSQL()
        {
            String mobsItemsQuery = "SELECT * FROM mob_items;";
            mobItems = Database.executeQuery(Database.DatabaseName.net7, mobsItemsQuery);
        }

        public DataRow[] getRowsByID(int objectID)
        {
            DataRow[] foundRows;
            foundRows = mobItems.Select("mob_id = '" + objectID + "'");

            return foundRows;
        }

        public DataTable getMobItemsTable()
        {
            return mobItems;
        }

        public void deleteRecord(DataRow dr)
        {
            String query = "DELETE FROM mob_items where mob_id='" + dr["mob_id"].ToString() + "' "+
                "and item_base_id='"+dr["item_base_id"].ToString()+"';";
            DataTable executeDelete = Database.executeQuery(Database.DatabaseName.net7, query);

            mobItems.Rows.Remove(dr);
        }

        public void updateRecord(DataRow dr)
        {
            String id = dr["mob_id"].ToString();
            String baID = dr["item_base_id"].ToString();
            String dc = dr["drop_chance"].ToString();
            String uc = dr["usage_chance"].ToString();
            String qty = dr["qty"].ToString();

            String query = "UPDATE mob_items SET drop_chance='"+dc+"', usage_chance='"+uc+"', qty='"+qty+"' "+
                "where mob_id='"+id+"' and item_base_id='"+baID+"';";
            DataTable executeDelete = Database.executeQuery(Database.DatabaseName.net7, query);
        }

        public void insertRecord(DataRow dr)
        {
            String id = dr["mob_id"].ToString();
            String baID = dr["item_base_id"].ToString();
            String dc = dr["drop_chance"].ToString();
            String uc = dr["usage_chance"].ToString();
            String qty = dr["qty"].ToString();
            String type = dr["type"].ToString();

            String query = "INSERT INTO mob_items SET mob_id='" + id + "', drop_chance='" + dc + "', usage_chance='" + uc + "', " +
                           "qty='" + qty + "', item_base_id='" + baID + "', type='" + type + "';";
            DataTable executeDelete = Database.executeQuery(Database.DatabaseName.net7, query);
        }
    }
}
