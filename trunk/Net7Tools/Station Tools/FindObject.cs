using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using MySql.Data.MySqlClient;

namespace Station_Tools
{
    public partial class FindObject : Form
    {
        private string m_DataBase = "net7";
        private int m_StationID;
        private int m_SectorID;
        public bool m_Ok;

        public FindObject()
        {
            InitializeComponent();
            m_Ok = false;
        }

        private MySqlDataReader ExcuteSQLQuery(string SQLD)
        {
            // Populate the Starbase Combox
            MySqlConnection m_Connection = new MySqlConnection(SQLData.ConnStr(m_DataBase));
            MySqlCommand m_Command = new MySqlCommand(SQLD, m_Connection);
            MySqlDataReader ReadData = (null);

            try
            {
                m_Connection.Open();
                ReadData = m_Command.ExecuteReader();
            }
            catch (Exception Error)
            {
                MessageBox.Show("SQL Error: " + Error.Message, "SQL Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            return ReadData;
        }

        private int ExecuteSQL(string SQLD)
        {
            string m_GetID = "SELECT LAST_INSERT_ID()";     // Grab the ID of last instered row
            int m_ObjectID = 0;

            // Connect to Database and update
            MySqlConnection m_Connection = new MySqlConnection(SQLData.ConnStr(m_DataBase));
            MySqlCommand m_CommandAdd = new MySqlCommand(SQLD, m_Connection);
            MySqlCommand m_CommandGetID = new MySqlCommand(m_GetID, m_Connection);

            try
            {
                // Execute Command
                m_Connection.Open();
                m_CommandAdd.ExecuteNonQuery();
                MySqlDataReader m_Reader = m_CommandGetID.ExecuteReader();  // Get ID
                m_Reader.Read();                                            // Read in data
                m_ObjectID = m_Reader.GetInt32(0);
                m_Connection.Close();
            }
            catch (Exception Error)
            {
                MessageBox.Show("SQL Error: " + Error.Message, "SQL Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return -1;
            }

            return m_ObjectID;
        }

        private void FindObject_Load(object sender, EventArgs e)
        {
            // pull all of the data from the stations
            string StationQuery = "SELECT `sector_objects`.`sector_object_id`,`sector_objects`.`name`,`sectors`.`name`,`sector_objects`.`sector_id` FROM `sector_objects_starbases` Inner Join `sector_objects` ON `sector_objects_starbases`.`starbase_id` = `sector_objects`.`sector_object_id` Inner Join `sectors` ON `sector_objects`.`sector_id` = `sectors`.`sector_id` AND `sector_objects`.`sector_id` = `sectors`.`sector_id`";

            MySqlDataReader StationData = ExcuteSQLQuery(StationQuery);

            StationList.Rows.Clear();

            if (StationData.HasRows)
            {
                while (StationData.Read())
                {
                    StationList.Rows.Add(StationData.GetString(0), StationData.GetString(3), StationData.GetString(2), StationData.GetString(1));
                }
            }

        }

        public bool GetOk()
        {
            return m_Ok;
        }

        public int GetSectorID()
        {
            return m_SectorID;
        }

        public int GetStationID()
        {
            return m_StationID;
        }

        private void Ok_Click(object sender, EventArgs e)
        {
            m_StationID = Int32.Parse(StationList.CurrentRow.Cells[0].Value.ToString());
            m_SectorID = Int32.Parse(StationList.CurrentRow.Cells[1].Value.ToString());
            m_Ok = true;
            this.Close();
        }

        private void Cancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}