using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data;

namespace N7.SQL
{
    class FactionMatrixSQL
    {
        private DataTable factionMatrix;

        public FactionMatrixSQL()
        {
            String query = "SELECT * FROM faction_matrix order by faction_id, faction_entry_id;";
            factionMatrix = Database.executeQuery(query);
        }

        public DataTable getFactionMatrixTable()
        {
            return factionMatrix;
        }

        public DataRow[] getRowsByID(int selectedRowID)
        {
            DataRow[] foundRows;
            foundRows = factionMatrix.Select("faction_id = '" + selectedRowID + "'");

            return foundRows;
        }

        public DataRow getRowByID(int selectedRowID)
        {
            DataRow[] foundRows;
            foundRows = factionMatrix.Select("id = '" + selectedRowID + "'");
            DataRow foundRow = foundRows[0];

            return foundRow;
        }

        public DataRow newRecord(int newID, int existingID)
        {

            String query = "INSERT INTO faction_matrix SET faction_id='" + newID + "', " +
                           "faction_entry_id='" + existingID + "', base_value='0', reward_faction=0;";
            
            DataTable executeInsert = Database.executeQuery(query);
            DataTable lastInsert = Database.executeQuery("SELECT last_insert_id() from faction_matrix;");

            int lastInsertID = int.Parse(lastInsert.Rows[0]["last_insert_id()"].ToString());

            DataRow newRow = factionMatrix.NewRow();
            newRow["id"] = lastInsertID;
            newRow["faction_id"] = newID;
            newRow["faction_entry_id"] = existingID;
            newRow["base_value"] = 0;
            newRow["current_value"] = 0;
            newRow["reward_faction"] = 0;

            factionMatrix.Rows.Add(newRow);

            newRow.AcceptChanges();
            factionMatrix.AcceptChanges();

            return newRow;
        }

        public void deleteRecord(int id)
        {
            String query = "DELETE FROM faction_matrix where faction_id='" + id + "';";
            DataTable executeDelete = Database.executeQuery(query);

            DataRow[] tmp = getRowsByID(id);
            for (int i = 0; i < tmp.Length; i++)
			{
			    DataRow tmp2 = tmp[i];

                factionMatrix.Rows.Remove(tmp2);
			}
        }

        public void updateRecord(int factionID)
        {
            DataRow[] dr = getRowsByID(factionID);
            for (int i = 0; i < dr.Length; i++)
            {
                DataRow r = dr[i];
                int ID = int.Parse(r["id"].ToString());
                int _factionID = int.Parse(r["faction_id"].ToString());
                int factionEntryID = int.Parse(r["faction_entry_id"].ToString());
                int baseValue = int.Parse(r["base_value"].ToString());
                int currentValue = int.Parse(r["current_value"].ToString());
                int rewardFaction = int.Parse(r["reward_faction"].ToString());

                String query = "UPDATE faction_matrix SET faction_id='" + _factionID + "', " +
                    "faction_entry_id='"+factionEntryID+"', base_value='"+baseValue+"', "+
                    "current_value='"+currentValue+"', reward_faction='"+rewardFaction+"' where id='"+ID+"';";
                DataTable executeUpdate = Database.executeQuery(query);
            }
        }
    }
}
