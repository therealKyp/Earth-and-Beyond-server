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

        public DataTable getMobTable()
        {
            return mobs;
        }
    }
}
