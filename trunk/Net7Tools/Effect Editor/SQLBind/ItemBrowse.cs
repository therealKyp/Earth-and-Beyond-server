using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using MySql.Data.MySqlClient;

namespace SQLBind
{
    public partial class ItemBrowse : Form
    {
        private string m_DataBase = "net7";

        private string[] g_ItemType = { "System","Weapon","Shields","Sensor","Ejector","Turret","Engine","Reactor","Controler","Robot","Ammo","Devices","System","Base","Beam Weapon","Missile Launcher","Projectile Weapon","Countermesure","Over Rides","All" };

        private List<string> g_Description = new List<string>();

        private int g_ItemBaseID = -1;


        public ItemBrowse()
        {
            InitializeComponent();

            ItemTypeQuery.SelectedIndex = 0;
            ItemLevelQuery.SelectedIndex = 0;
        }

        private MySqlDataReader ExcuteSQLQuery(string SQLD)
        {
            // Populate the Starbase Combox
            MySqlConnection m_Connection = new MySqlConnection(DB.GetLoginStr());
            MySqlCommand m_Command = new MySqlCommand(SQLD, m_Connection);
            m_Connection.Open();

            return m_Command.ExecuteReader();
        }

        private int ExecuteSQL(string SQLD)
        {
            string m_GetID = "SELECT LAST_INSERT_ID()";     // Grab the ID of last instered row
            int m_ObjectID = 0;

            // Connect to Database and update
            MySqlConnection m_Connection = new MySqlConnection(DB.GetLoginStr());
            MySqlCommand m_CommandAdd = new MySqlCommand(SQLD, m_Connection);
            MySqlCommand m_CommandGetID = new MySqlCommand(m_GetID, m_Connection);
            m_Connection.Open();

            try
            {
                // Execute Command
                m_CommandAdd.ExecuteNonQuery();
                MySqlDataReader m_Reader = m_CommandGetID.ExecuteReader();  // Get ID
                m_Reader.Read();                                            // Read in data
                m_ObjectID = m_Reader.GetInt32(0);
            }
            catch (Exception Error)
            {
                MessageBox.Show("SQL Error: " + Error.Message, "SQL Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return -1;
            }

            return m_ObjectID;
        }

        public DialogResult ShowDialog(bool Multi)
        {
            if (Multi)
            {
                ItemList.MultiSelect = true;
            }
            else
            {
                ItemList.MultiSelect = false;
            }
            return this.ShowDialog();
        }

        public int GetNumSelected()
        {
            if (ItemList.MultiSelect)
            {
                // Get Number of selected Items
                return ItemList.SelectedRows.Count;
            }
            else
            {
                return 0;
            }
        }

        public int GetSelecteItemID(int Row)
        {
            if (ItemList.MultiSelect)
            {
                // Get the row ItemID
                return Int32.Parse(ItemList.SelectedRows[Row].Cells[1].Value.ToString());
            }
            else
            {
                return 0;
            }
        }

        public string GetSelecteItemName(int Row)
        {
            if (ItemList.MultiSelect)
            {
                // Get the row ItemID
                return ItemList.SelectedRows[Row].Cells[2].Value.ToString();
            }
            else
            {
                return "";
            }
        }



        public int GetItemBase()
        {
            if (!ItemList.MultiSelect)
            {
                // Send ItemID to caller
                return g_ItemBaseID;
            }
            else
            {
                return 0;
            }
        }

        private void ErrorMsg(string Msg)
        {
            MessageBox.Show(Msg, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        private void AddSelected_Click(object sender, EventArgs e)
        {
            int Id = ItemList.CurrentRow.Index;

            // Make sure we have a row selected
            if (Id == -1)
            {
                ErrorMsg("You must select a row!");
                return;
            }

            if (!ItemList.MultiSelect)
            {
                // Get Index From first cell (Allows sorting)
                int Index = Int32.Parse(ItemList.Rows[Id].Cells[0].Value.ToString());

                // Get Item ID from selected item
                g_ItemBaseID = Int32.Parse(ItemList.Rows[Index].Cells[1].Value.ToString());
            }

            // Close form
            this.Close();
        }

        private void Search_Click(object sender, EventArgs e)
        {
            bool LastQuery = false;
            string m_QueryStr = "SELECT `id`,`level`,`category`,`sub_category`,`type`,`price`,`max_stack`,`name`,`description`,`manufacturer` FROM `item_base` WHERE ";

            // Decide what type of query to do
            if (ItemNameQuery.Text != "")
            {
                m_QueryStr += "`item_base`.`name` like '%" + ItemNameQuery.Text + "%'";
                LastQuery = true;
            }

            if (ItemLevelQuery.SelectedIndex != 0)
            {
                if (LastQuery)
                {
                    m_QueryStr += " and ";
                }
                m_QueryStr += "`item_base`.`level` = '" + ItemLevelQuery.SelectedIndex.ToString() + "'";
                LastQuery = true;
            }

            if (ItemTypeQuery.SelectedIndex != 0)
            {
                if (LastQuery)
                {
                    m_QueryStr += " and ";
                }
                m_QueryStr += "`item_base`.`type` = '" + (ItemTypeQuery.SelectedIndex - 1) + "'";
                LastQuery = true;
            }

            if (!LastQuery)
            {
                ErrorMsg("You need to specify atleast one critera to search on!");
                return;
            }

            // Exeucte Query and get result
            MySqlDataReader m_Query = ExcuteSQLQuery(m_QueryStr);

            // Make sure we have rows before processing
            if (!m_Query.HasRows)
            {
                ErrorMsg("No results from your query");
                return;
            }

            // Clear array
            g_Description.Clear();

            // Clearn items in box
            ItemList.Rows.Clear();

            // This value allows sorting and keeping tack of items
            int ID = 0;

            // Read all the items in the query
            while (m_Query.Read())
            {
                // Add discription to array
                g_Description.Add(m_Query.GetString(8));
                // Add to List of items
                ItemList.Rows.Add(ID, m_Query.GetInt32(0), m_Query.GetString(7), g_ItemType[m_Query.GetInt32(4)], m_Query.GetInt32(1), m_Query.GetInt32(5));
                ID++;
            }
            RowCount.Text = "Number of Results: " + ItemList.RowCount.ToString();
        }

        private void ItemList_SelectionChanged(object sender, EventArgs e)
        {
            int Id = ItemList.CurrentRow.Index;

            // Make sure we have a row selected
            if (Id == -1)
            {
                return;
            }

            // Get Index From first cell (Allows sorting)
            int Index = Int32.Parse(ItemList.Rows[Id].Cells[0].Value.ToString());

            // Make sure we are in range
            if (g_Description.Count <= Index)
            {
                return;
            }

            // Display discription
            ItemDescription.Text = g_Description[Index];
        }

        private void CloseWindow_Click(object sender, EventArgs e)
        {
            ItemList.MultiSelect = false;
            this.Close();
        }
    }
}