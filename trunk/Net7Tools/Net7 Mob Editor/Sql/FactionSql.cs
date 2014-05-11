using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Text;
using System.Data;

namespace N7.Sql
{
    public class FactionSql
    {
        private DataTable factions;
        public FactionSql()
        {
            String factionQuery = "SELECT * FROM factions;";
            factions = Database.executeQuery(Database.DatabaseName.net7, factionQuery);
        }

        public String findNameByID(int factionID)
        {
            DataRow[] foundRows;
            String name;
            if (factionID > 0)
            {
                Console.Out.WriteLine(factionID);
                foundRows = factions.Select("faction_id = '" + factionID + "'");
                DataRow foundRow = foundRows[0];
                name = foundRow["name"].ToString();
            }
            else
            {
                name = "None";
            }
  
            return name;
        }

        public int findIDbyName(String name)
        {
            DataRow[] foundRows;
            int id;
            String name2 = name.Replace("'", "''");
            if (name != "None")
            {
                foundRows = factions.Select("name LIKE '" + name2 + "'");
                DataRow foundRow = foundRows[0];
                id = int.Parse(foundRow["faction_id"].ToString());
            }
            else
            {
                id = -1;
            }

            return id;
        }

        public DataTable getFactionTable()
        {
            return factions;
        }
    }
}
