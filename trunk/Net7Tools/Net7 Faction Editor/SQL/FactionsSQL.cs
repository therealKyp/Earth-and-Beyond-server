using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Text;
using System.Data;

namespace N7.SQL
{
    public class FactionsSQL
    {
        private DataTable factions;

        public FactionsSQL()
        {
            String query = "SELECT * FROM factions order by name;";
            factions = Database.executeQuery(query);
        }

        public DataRow getRowByID(int selectedRowID)
        {
            DataRow[] foundRows;
            foundRows = factions.Select("faction_id = '" + selectedRowID + "'");
            DataRow foundRow = foundRows[0];

            return foundRow;
        }

        public DataTable getFactionTable()
        {
            return factions;
        }

        public DataRow newRecord()
        {
            String name = "<New Faction>";
            String description = "";

            String query = "INSERT INTO factions SET name='"+name+"', description='"+description+"';";
            
            DataTable executeInsert = Database.executeQuery(query);
            DataTable lastInsert = Database.executeQuery("SELECT last_insert_id() from factions;");

            int lastInsertID = int.Parse(lastInsert.Rows[0]["last_insert_id()"].ToString());

            DataRow newRow = factions.NewRow();
            newRow["faction_id"] = lastInsertID;
            newRow["name"] = "<New Faction>";
            newRow["description"] = "";
            newRow["PDA_text"] = "";

            factions.Rows.Add(newRow);

            newRow.AcceptChanges();
            factions.AcceptChanges();

            return newRow;
        }

        public void deleteRecord(int id, DataRow dr)
        {
            String query = "DELETE FROM factions where faction_id='"+id+"';";
            DataTable executeDelete = Database.executeQuery(query);

            factions.Rows.Remove(dr);
        }

        public void updateRecord(DataRow dr)
        {
            int factionID = int.Parse(dr["faction_id"].ToString());
            String name = dr["name"].ToString();
            String nameNew = name.Replace("'", "''");
            String description = dr["description"].ToString();
            String descriptionNew = description.Replace("'", "''");
            String pda_text = dr["PDA_text"].ToString();

            String query = "UPDATE factions SET name='" + name + "', description='"+descriptionNew+"', " +
                "PDA_text='" + pda_text + "' where faction_id='" + factionID + "';";

            DataTable executeUpdate = Database.executeQuery(query);
        }
    }
}
