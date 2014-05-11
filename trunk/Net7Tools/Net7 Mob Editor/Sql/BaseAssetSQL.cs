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

        public String getFileNameByID(int id)
        {
            DataRow[] foundRows;
            foundRows = baseAssets.Select("base_id='" + id + "'");

            String name = foundRows[0]["filename"].ToString();
            String cat_name = foundRows[0]["main_cat"].ToString();
            String newName = "null";

            //Check to see if we have a file name, and substring it to create our new filename.
            if (name.Contains(".w3d") || name.Contains(".W3D") || name.Contains(".tga") || name.Contains(".TGA"))
            {
                String cutName = name.Remove(name.Length - 4);
                newName = cutName + ".jpg";
            }

            return newName;
        }
    }
}
