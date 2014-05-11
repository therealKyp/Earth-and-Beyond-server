using System;
using System.Collections.Generic;
using System.Text;

namespace N7.Sql
{
    class Navs
    {
        private DataTable navs;

        public Navs()
        {
            String navQuery = "SELECT * FROM sectors order by system_id, name";
            navs = Database.executeQuery(Database.DatabaseName.net7, navQuery);
        }

        public DataTable getSectorTable()
        {
            return navs;
        }
    }
}
