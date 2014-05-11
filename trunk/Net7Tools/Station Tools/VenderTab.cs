using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using MySql.Data.MySqlClient;


namespace Station_Tools
{
    public partial class VenderTab : UserControl
    {
        private string m_DataBase = "net7";

        const bool m_PreSqlItems = false;

        private ItemBrowse BrowseItems;

        #region   - Varables for Venders -

        struct VenderGroup
        {
            public int GroupID;
            public string GroupName;
        }

        private List<VenderGroup> VGroups = new List<VenderGroup>();

        int CurrentEditingItem = -1;
        int VenderGroupID = 0;
        //bool VenderUpdate = false;
        bool VenderItemUpdate = false;

        #endregion

        public VenderTab()
        {
            InitializeComponent();
            BrowseItems = new ItemBrowse();
        }

        private void VenderTab_Load(object sender, EventArgs e)
        {
            if (!SqlLoadVenders())
            {
                DataError("Error Loading Data from SQL");
            }
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

        #region   ------------------------- Vender Tab -------------------------

        private bool SqlLoadVenders()
        {
            // Populate the Starbase Combox
            string m_QueryStr = "SELECT `starbase_vender_groups`.`GroupID`,`starbase_vender_groups`.`GroupName` FROM `starbase_vender_groups`";

            MySqlDataReader m_Reader = ExcuteSQLQuery(m_QueryStr);

            // Make sure the box is cleared
            Vender_SelectGroup.Items.Clear();

            VGroups.Clear();        // Reset all data

            // Has no data yet
            if (!m_Reader.HasRows)
                return true;

            int index = 0;
            VenderGroup VenderGroupData;

            while (m_Reader.Read())
            {
                VenderGroupData = new VenderGroup();

                // Save data into a structure
                VenderGroupData.GroupID = m_Reader.GetInt32(0);      // Save Group ID
                VenderGroupData.GroupName = m_Reader.GetString(1);   // Save Group Name
                // Put into List
                Vender_SelectGroup.Items.Add(m_Reader.GetString(1));

                VGroups.Add(VenderGroupData);       // Add to List
                // Update Index
                index++;
            }
            return true;
        }

        private void DataError(string ErrorMsg)
        {
            MessageBox.Show(ErrorMsg, "Data Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        private bool Vender_Valadate()
        {
            // Validate the Data
            if (Vender_NewItemID.Text == "")
            {
                DataError("You need to fill in a Item ID");
                return false;
            }

            if (Vender_Quanity.Text == "")
                Vender_Quanity.Text = "-1";     // Make it unlimited

            if (Vender_SellPrice.Text == "")
                Vender_SellPrice.Text = "-1";     // Use default price

            if (Vender_BuyPrice.Text == "")
                Vender_BuyPrice.Text = "-1";     // Use default price

            return true;
        }

        private bool Vender_Save()
        {
            // If no item drop out
            if (VenderGroupID == 0)
                return false;

            // See if anything has changed
            if (VenderItemUpdate)
            {
                // Display Dialog
                DialogResult Result = MessageBox.Show("Do you want to save your work?", "Save", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Exclamation);

                // Check result
                if (Result == DialogResult.Yes)
                {
                    // Save Data
                    Vender_SaveItems();
                }
                else if (Result == DialogResult.Cancel)
                {
                    // Cancel
                    return false;
                }
            }
            // Reset the flag if we saved/didnt save
            VenderItemUpdate = false;
            return true;
        }

        private void Vender_SaveItems()
        {
            int CurrentIndex = 0;

            if (VenderGroupID == 0)
            {
                DataError("You must select a group!");
                return;
            }

            // Find item in list
            for (int index = 0; index < Vender_ItemLists.Rows.Count; index++)
            {
                if (Int32.Parse(Vender_ItemLists.Rows[index].Cells[0].Value.ToString()) == CurrentEditingItem)
                {
                    CurrentIndex = index;
                }
            }

            string ItemName = "";
            if (!m_PreSqlItems)
            {
                // Read in Items and add to list
                string m_SqlQuery = "SELECT `item_base`.`name` FROM `item_base` WHERE `item_base`.`id` =  '" + Vender_NewItemID.Text + "'";

                MySqlDataReader m_Reader = ExcuteSQLQuery(m_SqlQuery);

                // Make sure we have data
                if (m_Reader.HasRows)
                {
                    m_Reader.Read();
                    ItemName = m_Reader.GetString(0);
                }
            }

            // Remove old Data
            Vender_ItemLists.Rows.RemoveAt(CurrentIndex);

            // Insert new Data
            Vender_ItemLists.Rows.Insert(CurrentIndex, CurrentEditingItem, ItemName, Vender_NewItemID.Text, Vender_SellPrice.Text,
                Vender_BuyPrice.Text, Vender_Quanity.Text);

            string m_AddSql = "UPDATE `starbase_vender_inventory` SET ";

            m_AddSql += "`itemid` = '" + Vender_NewItemID.Text + "',";
            m_AddSql += "`sell_price` = '" + Vender_SellPrice.Text + "',";
            m_AddSql += "`buy_price` = '" + Vender_BuyPrice.Text + "',";
            m_AddSql += "`quanity` = '" + Vender_Quanity.Text + "'";
            m_AddSql += " WHERE `id` = '" + CurrentEditingItem + "';";

            ExecuteSQL(m_AddSql);

            VenderItemUpdate = false;
        }

        private void Vender_SaveItem_Click(object sender, EventArgs e)
        {
            // Valadate Data
            if (!Vender_Valadate())
                return;

            // Save Item
            Vender_SaveItems();
        }

        private void Vender_EditItem_Funct()
        {
            // Make sure we allways save or ask before changing data
            if (!Vender_Save())
                return;

            // If no item drop out
            if (VenderGroupID == 0)
                return;

            // Make sure we have a valid row
            if (Vender_ItemLists.CurrentRow == null)
                return;

            int CurrentIndex = Vender_ItemLists.CurrentRow.Index;

            // Copy Data to text boxes
            CurrentEditingItem = Int32.Parse(Vender_ItemLists.Rows[CurrentIndex].Cells[0].Value.ToString());

            Vender_NewItemID.Text = Vender_ItemLists.Rows[CurrentIndex].Cells[2].Value.ToString();
            Vender_SellPrice.Text = Vender_ItemLists.Rows[CurrentIndex].Cells[3].Value.ToString();
            Vender_BuyPrice.Text = Vender_ItemLists.Rows[CurrentIndex].Cells[4].Value.ToString();
            Vender_Quanity.Text = Vender_ItemLists.Rows[CurrentIndex].Cells[5].Value.ToString();

            // We dont want to ask to save unless the user has changed data
            VenderItemUpdate = false;
        }

        private void Vender_EditItem_Click(object sender, DataGridViewCellEventArgs e)
        {
            Vender_EditItem_Funct();
        }

        private void Vender_EditItem_Click(object sender, EventArgs e)
        {
            Vender_EditItem_Funct();
        }

        private void Vender_DelItem_Click(object sender, EventArgs e)
        {
            // If no item drop out
            if (VenderGroupID == 0)
                return;

            // Make sure we have someting selected
            if (Vender_ItemLists.SelectedRows.Count < 0)
                return;

            // Display Dialog
            DialogResult Result = MessageBox.Show("Are you sure you want to delete this item?", "Delete Confermation", MessageBoxButtons.YesNo, MessageBoxIcon.Exclamation);

            // Check result
            if (Result == DialogResult.No)
            {
                // Cancel
                return;
            }

            // Make sure we have a valid row
            if (Vender_ItemLists.CurrentRow == null)
                return;

            string m_AddSql = "";
            int Count = Vender_ItemLists.SelectedRows.Count;
            int ICount = 0;

            // Delete all items in group
            while(Count > 0 && ICount < Count)
            {
                ICount++;
                m_AddSql += "DELETE FROM `starbase_vender_inventory` WHERE `id` = '" + Vender_ItemLists.SelectedRows[0].Cells[0].Value.ToString() + "';";

                // Remove Row from Data Block
                Vender_ItemLists.Rows.RemoveAt(Vender_ItemLists.SelectedRows[0].Index);
            }
            ExecuteSQL(m_AddSql);
        }

        private void Vender_NewItem_Click(object sender, EventArgs e)
        {
            // Make sure we allways save or ask before changing data
            if (!Vender_Save())
                return;

            // If no item drop out
            if (VenderGroupID == 0)
            {
                DataError("You must select a group!");
                return;
            }

            string m_AddSql = "INSERT INTO `starbase_vender_inventory` (`groupid`, `itemid`,`sell_price`,`buy_price`,`quanity`)  VALUES ('" + VenderGroupID + "', '0', '0', '0', '0');";

            CurrentEditingItem = ExecuteSQL(m_AddSql);

            // Add item to grid
            int CurrentIndex = Vender_ItemLists.Rows.Add(CurrentEditingItem.ToString(), "", "0", "0", "0", "-1");

            // Copy Data to text boxes
            Vender_NewItemID.Text = Vender_ItemLists.Rows[CurrentIndex].Cells[2].Value.ToString();
            Vender_SellPrice.Text = Vender_ItemLists.Rows[CurrentIndex].Cells[3].Value.ToString();
            Vender_BuyPrice.Text = Vender_ItemLists.Rows[CurrentIndex].Cells[4].Value.ToString();
            Vender_Quanity.Text = Vender_ItemLists.Rows[CurrentIndex].Cells[5].Value.ToString();

            // Unselect other rows
            Vender_ItemLists.CurrentCell = Vender_ItemLists.Rows[CurrentIndex].Cells[2];
            Vender_ItemLists.Rows[CurrentIndex].Selected = true;

            // We dont want the loaded data to make us save unless the user edits an item
            VenderItemUpdate = false;
        }

        private void Vender_QuanityUnlimited_CheckedChanged(object sender, EventArgs e)
        {
            // Set/Unset unlimited
            Vender_Quanity.Enabled = !Vender_Quanity.Enabled;

            if (Vender_Quanity.Enabled == false)
            {
                Vender_Quanity.Text = "-1";
            }
            else
            {
                Vender_Quanity.Text = "0";
            }
        }

        private void Vender_ItemUpdate(object sender, EventArgs e)
        {
            // If no item drop out
            if (VenderGroupID == 0)
                return;

            // Set flag to let us know that the item has been changed
            VenderItemUpdate = true;
        }

        private void Vender_BrowseItems_Click(object sender, EventArgs e)
        {
            // TODO: Launch a new window to search for items

            if (!m_PreSqlItems)
            {
                BrowseItems.ShowDialog(false);

                string ItemIDNum = BrowseItems.GetItemBase().ToString();

                // Pull Item Number
                if (ItemIDNum != "-1")
                {
                    Vender_NewItemID.Text = ItemIDNum;
                }
            }
        }

        #region --- Groups ---

        private void Vender_AddGroup_Click(object sender, EventArgs e)
        {
            string m_AddSql = "INSERT INTO `starbase_vender_groups` (`GroupName`,`SellMultiplyer`,`BuyMultiplyer`,`BuyOnlyList`)  VALUES (";

            if (Vender_SellMult.Text == "")
                Vender_SellMult.Text = "0";

            if (Vender_BuyMult.Text == "")
                Vender_BuyMult.Text = "0";

            if (Vender_GroupName.Text == "")
            {
                DataError("You must specify a group name!");
                return;
            }

            string GroupName = Vender_GroupName.Text;

            GroupName = GroupName.Replace("\\", "\\\\");
            GroupName = GroupName.Replace("\"", "\\\"");
            //GroupName = GroupName.Replace("'", "\\'");

            int BuyOnlyChecked = Vender_BuyList.Checked ? 1 : 0;

            m_AddSql += "\"" + GroupName + "\",";
            m_AddSql += "'" + Vender_SellMult.Text + "',";
            m_AddSql += "'" + Vender_BuyMult.Text + "',";
            m_AddSql += "'" + BuyOnlyChecked.ToString() + "');";

            VenderGroupID = ExecuteSQL(m_AddSql);

            // Reload data from SQL
            Vender_ReloadGroup_Click(null, null);
        }

        private void Vender_UpdateName_Click(object sender, EventArgs e)
        {
            // If no item drop out
            if (VenderGroupID == 0)
                return;

            if (Vender_SellMult.Text == "")
                Vender_SellMult.Text = "0";

            if (Vender_BuyMult.Text == "")
                Vender_BuyMult.Text = "0";

            if (Vender_GroupName.Text == "")
            {
                DataError("You must specify a group name!");
                return;
            }

            string m_AddSql = "UPDATE `starbase_vender_groups` SET ";

            int BuyOnlyChecked = Vender_BuyList.Checked ? 1 : 0;

            m_AddSql += "`GroupName` = \"" + Vender_GroupName.Text + "\",";
            m_AddSql += "`SellMultiplyer` = '" + Vender_SellMult.Text + "',";
            m_AddSql += "`BuyMultiplyer` = '" + Vender_BuyMult.Text + "',";
            m_AddSql += "`BuyOnlyList` = '" + BuyOnlyChecked.ToString() + "'";
            m_AddSql += " WHERE `GroupID` = '" + VenderGroupID + "';";

            ExecuteSQL(m_AddSql);

            // Reload data from SQL
            Vender_ReloadGroup_Click(null, null);
        }

        private void Vender_DeleteGroup_Click(object sender, EventArgs e)
        {
            // If no item drop out
            if (VenderGroupID == 0)
                return;

            // Display Dialog
            DialogResult Result = MessageBox.Show("Are you sure you want to delete this group?", "Delete Confermation", MessageBoxButtons.YesNo, MessageBoxIcon.Exclamation);

            // Check result
            if (Result == DialogResult.No)
            {
                // Cancel
                return;
            }

            if (Vender_SelectGroup.SelectedIndex < 0)
                return;

            // Delete Group
            string m_AddSql = "DELETE FROM `starbase_vender_groups`";
            m_AddSql += " WHERE `GroupID` = '" + VGroups[Vender_SelectGroup.SelectedIndex].GroupID + "';";
            ExecuteSQL(m_AddSql);

            // Delete all items in group
            m_AddSql = "DELETE FROM `starbase_vender_inventory`";
            m_AddSql += " WHERE `groupid` = '" + VGroups[Vender_SelectGroup.SelectedIndex].GroupID + "';";
            ExecuteSQL(m_AddSql);

            // Make sure all venders in this group are unset from selling
            m_AddSql = "UPDATE `starbase_vendors` SET `groupid` = '-1'";
            m_AddSql += " WHERE `groupid` = '" + VGroups[Vender_SelectGroup.SelectedIndex].GroupID + "';";
            ExecuteSQL(m_AddSql);

            // Reload data from SQL
            Vender_ReloadGroup_Click(null, null);
        }

        private void Vender_LoadGroup_Click(object sender, EventArgs e)
        {
            if (Vender_SelectGroup.SelectedIndex == -1)
                return;

            // Set Group ID
            VenderGroupID = VGroups[Vender_SelectGroup.SelectedIndex].GroupID;

            // Read data to GUI
            string m_SqlQuery = "SELECT `GroupName`, `SellMultiplyer`, `BuyMultiplyer`,`BuyOnlyList` FROM `starbase_vender_groups` WHERE `starbase_vender_groups`.`GroupID` =  '" + VenderGroupID + "'";
            MySqlDataReader m_Reader = ExcuteSQLQuery(m_SqlQuery);

            if (m_Reader.Read())
            {
                Vender_GroupName.Text = m_Reader.GetString(0);
                Vender_SellMult.Text = m_Reader.GetInt32(1).ToString();
                Vender_BuyMult.Text = m_Reader.GetInt32(2).ToString();
                Vender_BuyList.Checked = m_Reader.GetInt32(3) == 1 ? true : false;
            }
            else
            {
                MessageBox.Show("Error Reading Data in!", "SQL Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            // Clear item list
            Vender_ItemLists.Rows.Clear();

            // Read in Items and add to list
            if (!m_PreSqlItems)
            {
                m_SqlQuery = "SELECT `starbase_vender_inventory`.`id`,`groupid`,`itemid`,`sell_price`,`buy_price`,`quanity`,`item_base`.`name` FROM `starbase_vender_inventory` Inner Join `item_base` ON `starbase_vender_inventory`.`itemid` = `item_base`.`id` WHERE `groupid` = '" + VenderGroupID + "'";
            }
            else
            {
                m_SqlQuery = "SELECT `starbase_vender_inventory`.`id`,`groupid`,`itemid`,`sell_price`,`buy_price`,`quanity` FROM `starbase_vender_inventory` WHERE `groupid` = '" + VenderGroupID + "'";
            }
            m_Reader = ExcuteSQLQuery(m_SqlQuery);

            // Make sure we have data
            if (!m_Reader.HasRows)
                return;

            while (m_Reader.Read())
            {
                if (!m_PreSqlItems)
                {
                    Vender_ItemLists.Rows.Add(m_Reader.GetInt32(0), m_Reader.GetString(6), m_Reader.GetInt32(2), m_Reader.GetInt32(3), m_Reader.GetInt32(4), m_Reader.GetInt32(5));
                }
                else
                {
                    Vender_ItemLists.Rows.Add(m_Reader.GetInt32(0), "", m_Reader.GetInt32(2), m_Reader.GetInt32(3), m_Reader.GetInt32(4), m_Reader.GetInt32(5));
                }
            }

        }

        private void Vender_ReloadGroup_Click(object sender, EventArgs e)
        {
            // If no item drop out
            if (VenderGroupID == 0)
                return;

            Vender_SelectGroup.Text = "";

            // Reload all data
            SqlLoadVenders();

            // Reset all values
            CurrentEditingItem = -1;
            VenderGroupID = 0;
            //VenderUpdate = false;
            VenderItemUpdate = false;

            //Reset GUI
            Vender_ItemLists.Rows.Clear();
        }

        #endregion

        #region --- Sell Boxes ---
        private void IBSell_CheckedChanged(object sender, EventArgs e)
        {
            // Check/Unchuck
            DNSell.Checked = false;
            VSP.Checked = false;
            Vender_SellPrice.Enabled = false;
            Vender_SellPrice.Text = "0";
        }

        private void DNSell_CheckedChanged(object sender, EventArgs e)
        {
            // Check/Unchuck
            IBSell.Checked = false;
            VSP.Checked = false;
            Vender_SellPrice.Enabled = false;
            Vender_SellPrice.Text = "-1";
        }

        private void VSP_CheckedChanged(object sender, EventArgs e)
        {
            IBSell.Checked = false;
            DNSell.Checked = false;
            Vender_SellPrice.Enabled = true;
            Vender_SellPrice.Text = "";
        }
        #endregion

        #region --- Buy Boxes ---

        private void DNBuy_CheckedChanged(object sender, EventArgs e)
        {
            IBBuy.Checked = false;
            VBP.Checked = false;
            Vender_BuyPrice.Enabled = false;
            Vender_BuyPrice.Text = "-1";
        }

        private void IBBuy_CheckedChanged(object sender, EventArgs e)
        {
            VBP.Checked = false;
            DNBuy.Checked = false;
            Vender_BuyPrice.Enabled = false;
            Vender_BuyPrice.Text = "0";
        }

        private void VBP_CheckedChanged(object sender, EventArgs e)
        {
            IBBuy.Checked = false;
            DNBuy.Checked = false;
            Vender_BuyPrice.Enabled = true;
            Vender_BuyPrice.Text = "";
        }
        #endregion

        private void MultAdd_Click(object sender, EventArgs e)
        {
            List<int> SelectedItems = new List<int>();
            List<string> SelecteItemsName = new List<string>();

            // If no item drop out
            if (VenderGroupID == 0)
            {
                DataError("You must select a group!");
                return;
            }

            BrowseItems.ShowDialog(true);

            int ItemCount = BrowseItems.GetNumSelected();

            // Retrive all items and store into array
            if (ItemCount > 0)
            {
                SelectedItems.Clear();
                for (int Ix = 0; Ix < ItemCount; Ix++)
                {
                    SelectedItems.Add(BrowseItems.GetSelecteItemID(Ix));
                    SelecteItemsName.Add(BrowseItems.GetSelecteItemName(Ix));
                }
            }

            // Send all the items to the server
            for (int Index = 0; Index < ItemCount; Index++)
            {
                int CItemID = SelectedItems[Index];

                string m_AddSql = "INSERT INTO `starbase_vender_inventory` (`groupid`, `itemid`,`sell_price`,`buy_price`,`quanity`)  VALUES ('" + VenderGroupID + "', '" + CItemID + "', '0', '0', '-1');";

                CurrentEditingItem = ExecuteSQL(m_AddSql);

                // Add item to grid
                int CurrentIndex = Vender_ItemLists.Rows.Add(CurrentEditingItem.ToString(), SelecteItemsName[Index], CItemID.ToString(), "0", "0", "-1");

                // Copy Data to text boxes
                Vender_NewItemID.Text = Vender_ItemLists.Rows[CurrentIndex].Cells[2].Value.ToString();
                Vender_SellPrice.Text = Vender_ItemLists.Rows[CurrentIndex].Cells[3].Value.ToString();
                Vender_BuyPrice.Text = Vender_ItemLists.Rows[CurrentIndex].Cells[4].Value.ToString();
                Vender_Quanity.Text = Vender_ItemLists.Rows[CurrentIndex].Cells[5].Value.ToString();

                // Unselect other rows
                Vender_ItemLists.CurrentCell = Vender_ItemLists.Rows[CurrentIndex].Cells[2];
                Vender_ItemLists.Rows[CurrentIndex].Selected = true;
            }

            // We dont want the loaded data to make us save unless the user edits an item
            VenderItemUpdate = false;
        }

        #endregion
    }
}
