using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Text;
using System.Data;

namespace N7.Sql
{
    public class SystemsSql
    {
        private DataTable systems;

        public SystemsSql()
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

        public String findRowNameByID(int id)
        {
            DataRow[] foundRows;
            foundRows = systems.Select("system_id='" + id + "'");
            DataRow foundRow = foundRows[0];
            String name = foundRow["name"].ToString();

            return name;
        }

        public int getIDFromName(String name)
        {
            DataRow[] foundRows;
            foundRows = systems.Select("name Like '" + name + "'");
            DataRow foundRow = foundRows[0];
            int id = int.Parse(foundRow["system_id"].ToString());

            return id;
        }

        public void updateRow(DataRow r)
        {
            String name = r["name"].ToString().Replace("'","''");
            String notes = r["notes"].ToString().Replace("'", "''");


            String systemQuery = "UPDATE systems SET name='"+name+"', galaxy_x='" + r["galaxy_x"].ToString() + "', " +
            "galaxy_y='" + r["galaxy_y"].ToString() + "', galaxy_z='" + r["galaxy_z"].ToString() + "', "+
            "color_r='" + r["color_r"].ToString() + "', color_g='" + r["color_g"].ToString() + "', " +
            "color_b='" + r["color_b"].ToString() + "', notes='" + notes + "' " +
            "where system_id='" + r["system_id"].ToString() + "';";

            try
            {
                Database.executeQuery(Database.DatabaseName.net7, systemQuery);
            }
            catch (Exception)
            {
                throw;
            }
        }

        public void newRow(DataRow nr)
        {
            String newSystemQuery = "INSERT INTO systems SET name='"+nr["name"].ToString()+"', "+
            "notes='"+nr["notes"].ToString()+"', galaxy_x='"+nr["galaxy_x"].ToString()+"', galaxy_y='"+nr["galaxy_y"].ToString()+"', "+
            "galaxy_z='"+nr["galaxy_z"].ToString()+"', color_r='"+nr["color_r"].ToString()+"', color_g='"+nr["color_g"].ToString()+"', "+
            "color_b='"+nr["color_b"].ToString()+"';";

            int lastInsertID = 0;
            try
            {
                Database.executeQuery(Database.DatabaseName.net7, newSystemQuery);

                DataTable tmp = Database.executeQuery(Database.DatabaseName.net7, "SELECT LAST_INSERT_ID()");
                foreach (DataRow z in tmp.Rows)
                {
                    lastInsertID = int.Parse(z["LAST_INSERT_ID()"].ToString());
                }
            }
            catch (Exception)
            {
                throw;
            }

            nr["system_id"] = lastInsertID;
        }
    }
}
