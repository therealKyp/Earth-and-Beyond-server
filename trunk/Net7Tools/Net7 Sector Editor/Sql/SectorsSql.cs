using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Text;
using System.Data;

namespace N7.Sql
{
    public class SectorsSql
    {
        private DataTable sectors;

        public SectorsSql()
        {
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
            String name2 = name.Replace("'", "''");
            foundRows = sectors.Select("name Like '"+name2+"'");

            return foundRows;
        }

        public DataRow[] getRowsBySystemID(String systemID)
        {
            DataRow[] foundRows;
            foundRows = sectors.Select("system_id = '" + systemID + "'");

            return foundRows;
        }

        public DataTable queryBySystemID(String systemID)
        {
            String sectorQuery = "SELECT * FROM sectors where system_id='"+systemID+"' order by name";
            DataTable tmp = Database.executeQuery(Database.DatabaseName.net7, sectorQuery);

            return tmp;
        }

        public int getIDFromName(String name)
        {
            DataRow[] foundRows;
            foundRows = sectors.Select("name Like '" + name + "'");
            DataRow foundRow = foundRows[0];
            int id = int.Parse(foundRow["sector_id"].ToString());

            return id;
        }

        public void updateRow(DataRow r)
        {

            //Retrieve Properties from sql row.
            String sectorName = r["name"].ToString().Replace("'","''");
            String greetings = r["greetings"].ToString().Replace("'","''");
            String notes = r["notes"].ToString().Replace("'","''");

            int lightbackdrop = Convert.ToInt32(r["light_backdrop"]);
            int swapbackdrop = Convert.ToInt32(r["swap_backdrop"]);
            int fogbackdrop = Convert.ToInt32(r["fog_backdrop"]);
            int autolevel = Convert.ToInt32(r["auto_level"]);

            String sectorQuery = "UPDATE sectors SET name='"+sectorName+"', x_min='"+r["x_min"].ToString()+"', x_max='"+r["x_max"].ToString()+"', "+
            "y_min='"+r["y_min"].ToString()+"', y_max='"+r["y_max"].ToString()+"', z_min='"+r["z_min"].ToString()+"', "+
            "z_max='"+r["z_max"].ToString()+"', grid_x='"+r["grid_x"].ToString()+"', grid_y='"+r["grid_y"].ToString()+"', "+
            "grid_z='"+r["grid_z"].ToString()+"', fog_near='"+r["fog_near"].ToString()+"', fog_far='"+r["fog_far"].ToString()+"', "+
            "debris_mode='"+r["debris_mode"].ToString()+"', light_backdrop='"+lightbackdrop+"', "+
            "fog_backdrop='"+fogbackdrop+"', swap_backdrop='"+swapbackdrop+"', "+
            "backdrop_fog_near='"+r["backdrop_fog_near"].ToString()+"', backdrop_fog_far='"+r["backdrop_fog_far"].ToString()+"', "+
            "max_tilt='"+r["max_tilt"].ToString()+"', auto_level='"+autolevel+"', impulse_rate='"+r["impulse_rate"].ToString()+"', "+
            "decay_velocity='"+r["decay_velocity"].ToString()+"', decay_spin='"+r["decay_spin"].ToString()+"', "+
            "backdrop_asset='"+r["backdrop_asset"].ToString()+"', greetings='"+greetings+"', notes='"+notes+"', "+
            "system_id='"+r["system_id"].ToString()+"', galaxy_x='"+r["galaxy_x"].ToString()+"', galaxy_y='"+r["galaxy_y"].ToString()+"', "+
            "galaxy_z='"+r["galaxy_z"].ToString()+"', sector_type='+"+r["sector_type"].ToString()+"' where sector_id='"+r["sector_id"].ToString()+"';";

            try
            {
                Database.executeQuery(Database.DatabaseName.net7, sectorQuery);
            }
            catch (Exception)
            {
                throw;
            }
        }

        public void newRow(DataRow r)
        {
            String sectorName = r["name"].ToString().Replace("'", "''");
            String greetings = r["greetings"].ToString().Replace("'", "''");
            String notes = r["notes"].ToString().Replace("'", "''");

            int lightbackdrop = Convert.ToInt32(r["light_backdrop"]);
            int swapbackdrop = Convert.ToInt32(r["swap_backdrop"]);
            int fogbackdrop = Convert.ToInt32(r["fog_backdrop"]);
            int autolevel = Convert.ToInt32(r["auto_level"]);

            String sectorQuery = "INSERT INTO sectors SET sector_id='"+r["sector_id"]+"', name='" + sectorName + "', x_min='" + r["x_min"].ToString() + "', x_max='" + r["x_max"].ToString() + "', " +
            "y_min='" + r["y_min"].ToString() + "', y_max='" + r["y_max"].ToString() + "', z_min='" + r["z_min"].ToString() + "', " +
            "z_max='" + r["z_max"].ToString() + "', grid_x='" + r["grid_x"].ToString() + "', grid_y='" + r["grid_y"].ToString() + "', " +
            "grid_z='" + r["grid_z"].ToString() + "', fog_near='" + r["fog_near"].ToString() + "', fog_far='" + r["fog_far"].ToString() + "', " +
            "debris_mode='" + r["debris_mode"].ToString() + "', light_backdrop='" + lightbackdrop + "', " +
            "fog_backdrop='" + fogbackdrop + "', swap_backdrop='" + swapbackdrop + "', " +
            "backdrop_fog_near='" + r["backdrop_fog_near"].ToString() + "', backdrop_fog_far='" + r["backdrop_fog_far"].ToString() + "', " +
            "max_tilt='" + r["max_tilt"].ToString() + "', auto_level='" + autolevel + "', impulse_rate='" + r["impulse_rate"].ToString() + "', " +
            "decay_velocity='" + r["decay_velocity"].ToString() + "', decay_spin='" + r["decay_spin"].ToString() + "', " +
            "backdrop_asset='" + r["backdrop_asset"].ToString() + "', greetings='" + greetings + "', notes='" + notes + "', " +
            "system_id='" + r["system_id"].ToString() + "', galaxy_x='" + r["galaxy_x"].ToString() + "', galaxy_y='" + r["galaxy_y"].ToString() + "', " +
            "galaxy_z='" + r["galaxy_z"].ToString() + "', sector_type='+"+r["sector_type"].ToString()+"';";

            try
            {
                Database.executeQuery(Database.DatabaseName.net7, sectorQuery);
            }
            catch (Exception)
            {
                throw;
            }
        }
    }
}
