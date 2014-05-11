using System;
using System.Collections.Generic;
using System.Text;
using System.Data;

namespace N7.Sql
{
    public class ItemBaseSQL
    {
        private DataTable itemBase;
        public ItemBaseSQL()
        {
            String ibQuery = "SELECT id, level, name, sub_category, 2d_asset FROM item_base order by name;";
            itemBase = Database.executeQuery(Database.DatabaseName.net7, ibQuery);
        }

        public DataTable getItemBaseTable()
        {
            return itemBase;
        }

        public DataRow getRowByID(int objectID)
        {
            DataRow[] foundRows;
            foundRows = itemBase.Select("id = '" + objectID + "'");
            DataRow foundRow = foundRows[0];

            return foundRow;
        }

        public DataRow getRowBy2dAsset(int objectID)
        {
            DataRow[] foundRows;
            foundRows = itemBase.Select("2d_asset = '" + objectID + "'");
            DataRow foundRow = foundRows[0];

            return foundRow;
        }

        public DataRow[] getRowsByCategory(int subcatID)
        {
            DataRow[] foundRows;
            foundRows = itemBase.Select("sub_category='" + subcatID + "'");
            return foundRows;
        }
    }
}
