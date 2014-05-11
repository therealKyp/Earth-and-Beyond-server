using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Text;
using System.Data;

namespace N7.Sql
{
    class Sectors
    {
        private DataTable sectors;

        public Sectors()
        {
           // String sectorQuery = "SELECT * FROM sectors left join systems on sectors.system_id = systems.system_id "
           // +"order by sectors.system_id;";
            String sectorQuery = "SELECT * FROM sectors order by system_id, name";
            sectors = Database.executeQuery(Database.DatabaseName.net7, sectorQuery);
        }

        public DataTable getSectorTable()
        {
            return sectors;
        }

        public DataRow[] findRowsByName(String name)
        {
            DataRow[] foundRows;
            foundRows = sectors.Select("name Like '"+name+"'");

            return foundRows;
        }

        public DataRow[] getRowsBySystemID(String systemID)
        {
            DataRow[] foundRows;
            foundRows = sectors.Select("system_id = '" + systemID + "'");

            return foundRows;
        }
    }
}
