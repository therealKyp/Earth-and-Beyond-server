using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Text;
using System.Data;

namespace N7.Sql
{
    class Systems
    {
        private DataTable systems;

        public Systems()
        {
            String systemsQuery = "Select * from systems";
            systems = Database.executeQuery(Database.DatabaseName.net7, systemsQuery);
        }

        public DataTable getSystemTable()
        {
            return systems;
        }

        public DataRow[] findRowsByName(String name)
        {
            DataRow[] foundRows;
            foundRows = systems.Select("name Like '" + name + "'");

            return foundRows;
        }
    }
}
