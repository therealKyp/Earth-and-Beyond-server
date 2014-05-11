using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data;

using Gajatko.IniFiles;

namespace EnBIniParser
{
    class BaseAsset
    {

        public BaseAsset()
        {
            String query = "SELECT * FROM assets;";
            DataTable insert = Database.executeQuery(Database.DatabaseName.net7, query);

            foreach (DataRow r in insert.Rows)
            {
                String filename = r["filename"].ToString();
                int base_id = int.Parse(r["base_id"].ToString());

                if (filename.Contains(".w3d") || filename.Contains(".W3D") || filename.Contains("null")
                    || filename.Contains("NULL") || filename.Contains(".tga") || filename.Contains(".TGA"))
                {
                    //do nothing.
                }
                else
                {
                    String query2 = "UPDATE assets SET filename='" + filename + ".w3d' where base_id='" + base_id + "';";

                    DataTable update = Database.executeQuery(Database.DatabaseName.net7, query2);
                    Console.Out.WriteLine(query2);
                }   
            }

            Console.Out.WriteLine("***** Finished *****");
        }
    }
}
