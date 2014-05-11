using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Text;
using System.Data;
namespace N7.Sql
{
    public class BaseAssetSQL
    {
        private DataTable baseAssets;

        public BaseAssetSQL()
        {
            String baQuery = "SELECT * FROM assets;";
            baseAssets = Database.executeQuery(Database.DatabaseName.net7, baQuery);
        }

        public DataTable getAssetsTable()
        {
            return baseAssets;
        }

        public DataRow[] getRowsbyCategory(String name)
        {
            DataRow[] foundRows;
            String name2 = name.Replace("'", "''");
            foundRows = baseAssets.Select("main_cat LIKE '" + name2 + "'");
            return foundRows;
        }
    }
}
