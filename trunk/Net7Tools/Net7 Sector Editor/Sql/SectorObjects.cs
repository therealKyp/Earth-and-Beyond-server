using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Text;
using System.Data;

namespace N7.Sql
{
    class SectorObjects
    {
        private DataTable sectorObjects;
        private String sectorID;
        private DataTable specificSecObject;

        public SectorObjects(String sectorName)
        {
            String sectorIDQuery = "SELECT sector_id FROM sectors where name='"+sectorName+"';";
            DataTable tmp = Database.executeQuery(Database.DatabaseName.net7, sectorIDQuery);

            foreach (DataRow r in tmp.Rows)
            {
                sectorID = r["sector_id"].ToString();
                String soQuery = "SELECT * FROM sector_objects left join sector_nav_points on " +
                             "sector_objects.sector_object_id = sector_nav_points.sector_object_id " +
                             "where sector_objects.sector_id='" + sectorID + "' order by sector_objects.type;";
                sectorObjects = Database.executeQuery(Database.DatabaseName.net7, soQuery);
            }
            tmp.Dispose();
        }

        public DataTable getSectorObject()
        {
            return sectorObjects;
        }

        public DataTable specificSectorObjectTable(int type)
        {
            switch (type)
            {
                case 0:
                    
                    break;
                case 3:
                    
                    break;
                case 11:
                    String sosecQuery = "SELECT stargate_id,classSpecific,faction_id FROM" +
                    "sector_objects_stargates left join sector_objects on " +
                    "sector_objects.sector_object_id = sector_objects_stargates.stargate_id " +
                    "where sector_objects.sector_id='" + sectorID + "';";
                    specificSecObject = Database.executeQuery(Database.DatabaseName.net7, sosecQuery);
                    break;
                case 12:
                    
                    break;
                case 37:
                    
                    break;
                case 38:
                    
                    break; 
            }
            return specificSecObject;
        }
    }
}
