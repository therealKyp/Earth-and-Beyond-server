using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using MySql.Data.MySqlClient;

namespace SQLBind
{
    public partial class EditItem : Form
    {
        ItemBrowse ItemBrowser;
        int m_CurrentItem;
        int m_CurrentContainer;

        string[] OutputStr = new string[5];

        struct EffectList
        {
            public string EffectDescList;
            public string EffectToolList;
            public int EffectID;
        };

        // Lists of data from SQL
        private List<EffectList> m_EffectList = new List<EffectList>();
        // List of EffectData
        private List<int> m_EffectIDNum = new List<int>();
        private List<EffectComboHandel> m_BoxList = new List<EffectComboHandel>();


        private MySqlDataReader ExcuteSQLQuery(string SQLD)
        {
            // Populate the Starbase Combox
            MySqlConnection m_Connection = new MySqlConnection(DB.GetLoginStr());
            MySqlCommand m_Command = new MySqlCommand(SQLD, m_Connection);
            MySqlDataReader data = (null);
            m_Connection.Open();

            try
            {
                data = m_Command.ExecuteReader();
            }
            catch( Exception e )
            {
                MessageBox.Show("Error executing mysql: " + SQLD, e.ToString(), MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            return data;
        }

        private int ExecuteSQL(string SQLD)
        {
            string m_GetID = "SELECT LAST_INSERT_ID()";     // Grab the ID of last instered row
            int m_ObjectID = 0;

            // Connect to Database and update
            MySqlConnection m_Connection = new MySqlConnection(DB.GetLoginStr());
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

        public EditItem()
        {
            // Set up data
            InitializeComponent();
            ItemBrowser = new ItemBrowse();
            LoadEffects();

            // Fill box's
            ManageEffectBox(Effect1, Effect1String, Effect1Var1, Effect1Var2, Effect1Var3);
            ManageEffectBox(Effect2, Effect2String, Effect2Var1, Effect2Var2, Effect2Var3);
            ManageEffectBox(Effect3, Effect3String, Effect3Var1, Effect3Var2, Effect3Var3);
            ManageEffectBox(Effect4, Effect4String, Effect4Var1, Effect4Var2, Effect4Var3);
            ManageEffectBox(Effect5, Effect5String, Effect5Var1, Effect5Var2, Effect5Var3);

            FillComboBoxs();
            EnableEffects(false);
        }

        private void EnableEffects(bool Enable)
        {
            for (int x = 0; x < m_BoxList.Count; x++)
            {
                m_BoxList[x].EnableFields(Enable);
            }
        }

        // Fill Data into all handled Combo box's
        private void FillComboBoxs()
        {
            for (int x = 0; x < m_BoxList.Count; x++)
            {
                m_BoxList[x].EffectBox.Items.AddRange(GetEffectList());
            }
        }

        // Setup a combo box for handeling
        private void ManageEffectBox(ComboBox cbo, Label effString, TextBox Var1, TextBox Var2, TextBox Var3)
        {
            EffectComboHandel BoxHandler = new EffectComboHandel();
            BoxHandler.EffectBox = cbo;
            BoxHandler.EffectString = effString;
            BoxHandler.Var1 = Var1;
            BoxHandler.Var2 = Var2;
            BoxHandler.Var3 = Var3;
            /*
            BoxHandler.Var1Lable = Var1Label;
            BoxHandler.Var2Lable = Var2Label;
            BoxHandler.Var3Lable = Var3Label;
            */
            m_BoxList.Add(BoxHandler);

            // Set up propertys
            cbo.DropDownStyle = ComboBoxStyle.DropDownList;

            // Set up events for box's
            Var1.TextChanged += new EventHandler(VarChanged);
            Var2.TextChanged += new EventHandler(VarChanged);
            Var3.TextChanged += new EventHandler(VarChanged);
            cbo.SelectedIndexChanged += new EventHandler(EffectChange);


        }

        private void DisplayString(int Index)
        {
            string Data;
            string FormatStr;

            m_BoxList[Index].Var1.Text = (m_BoxList[Index].Var1.Text == "" ? "0" : m_BoxList[Index].Var1.Text);
            m_BoxList[Index].Var2.Text = (m_BoxList[Index].Var2.Text == "" ? "0" : m_BoxList[Index].Var2.Text);
            m_BoxList[Index].Var3.Text = (m_BoxList[Index].Var3.Text == "" ? "0" : m_BoxList[Index].Var3.Text);

            // Load in data
            float Val1 = float.Parse(m_BoxList[Index].Var1.Text);
            float Val2 = float.Parse(m_BoxList[Index].Var2.Text);
            float Val3 = float.Parse(m_BoxList[Index].Var3.Text);

            Data = m_EffectList[m_BoxList[Index].EffectBox.SelectedIndex].EffectToolList;

            OutputStr[Index] = "";

            // Find and replace all old C++
            // printf with new format options
            // NOTE: This is for ENB Emulator types ONLY!
            int VarNum = 0;
            for(int x=0;x<Data.Length;x++)
            {
                FormatStr = "";

                // See if we found the start of the string
                if ((x+6 <= Data.Length) && ("%value" == Data.Substring(x, 6)))
                {
                    x+=6;   // Add 6 to our string
                    for(int z=x;z<Data.Length-1;z++)
                    {
                        // We have found the end of the format string
                        if (Data.Substring(z,2) == "f%")
                        {
                            // Save the data
                            FormatStr = Data.Substring(x, z-x);
                            x += z - x;
                            break;
                        }
                    }
                    // Format string now      
                    for(int y=0;y<FormatStr.Length;y++)
                    {
                        if (FormatStr.Substring(y,1) == ".")
                        {
                            OutputStr[Index] += "{" + VarNum.ToString() + ":";
                            int Before = Int32.Parse(FormatStr.Substring(0, y));
                            int After = Int32.Parse(FormatStr.Substring(y + 1, FormatStr.Length - y - 1));
                            for(int j=0;j<Before;j++)
                            {
                                OutputStr[Index] += "0";
                            }
                            OutputStr[Index] += ".";
                            for(int j=0;j<After;j++)
                            {
                                OutputStr[Index] += "0";
                            }
                            break;
                        }
                    }
                    OutputStr[Index] += "}";
                    x += 2;     // Jump over the f%
                    VarNum++;
                } else {
                    // Save parsed string
                    OutputStr[Index] += Data.Substring(x,1);
                }
            }

            // Enable correct box's
            if (VarNum > 0)
                m_BoxList[Index].Var1.Enabled = true;
            else
                m_BoxList[Index].Var1.Enabled = false;

            if (VarNum > 1)
                m_BoxList[Index].Var2.Enabled = true;
            else
                m_BoxList[Index].Var2.Enabled = false;

            if (VarNum > 2)
                m_BoxList[Index].Var3.Enabled = true;
            else
                m_BoxList[Index].Var3.Enabled = false;

            m_BoxList[Index].EffectString.Text = String.Format(OutputStr[Index], Val1, Val2, Val3);
        }

        // When an effect is changed update String
        private void EffectChange(object sender, EventArgs e)
        {
            // TODO: Update string/Remove any extra box's
            ComboBox caller = (ComboBox)sender;

            for (int x = 0; x < m_BoxList.Count; x++)
            {
                // Find the box that was updated
                if (m_BoxList[x].EffectBox.Name == caller.Name)
                {

                    // Disable values if you select no buffs
                    if (m_EffectList[m_BoxList[x].EffectBox.SelectedIndex].EffectID == -1)
                    {
                        m_BoxList[x].Var1.Enabled = false;
                        m_BoxList[x].Var2.Enabled = false;
                        m_BoxList[x].Var3.Enabled = false;
                    }

                    // Display string
                    DisplayString(x);
                }
            }
        }

        private void VarChanged(object sender, EventArgs e)
        {
            TextBox caller = (TextBox)sender;

            for (int x = 0; x < m_BoxList.Count; x++)
            {
                // Find the box that was updated
                if (m_BoxList[x].Var1.Name == caller.Name || m_BoxList[x].Var2.Name == caller.Name || m_BoxList[x].Var3.Name == caller.Name)
                {
                    if (OutputStr[x] == null)
                    {
                        break;
                    }

                    m_BoxList[x].Var1.Text = (m_BoxList[x].Var1.Text == "" ? "0" : m_BoxList[x].Var1.Text);
                    m_BoxList[x].Var2.Text = (m_BoxList[x].Var2.Text == "" ? "0" : m_BoxList[x].Var2.Text);
                    m_BoxList[x].Var3.Text = (m_BoxList[x].Var3.Text == "" ? "0" : m_BoxList[x].Var3.Text);

                    // Load in data
                    float Val1 = float.Parse(m_BoxList[x].Var1.Text);
                    float Val2 = float.Parse(m_BoxList[x].Var2.Text);
                    float Val3 = float.Parse(m_BoxList[x].Var3.Text);

                    m_BoxList[x].EffectString.Text = String.Format(OutputStr[x], Val1, Val2, Val3);
                }
            }
        }

        // -------- Load in Data for combox from SQL --------

        private string[] GetEffectList()
        {
            List<string> EffList = new List<string>();

            foreach(EffectList Effect in m_EffectList)
            {
                EffList.Add(Effect.EffectDescList);
            }

            return EffList.ToArray();
        }

        private void LoadEffects()
        {
            // Pull down data
            DataTable dataTable = new DataTable();
            MySqlConnection m_conn = new MySqlConnection(DB.GetLoginStr());
            String query = "SELECT Description,Tooltip,Constant1Value,Constant2Value,EffectID FROM item_effect_base";
            MySqlDataAdapter mySqlDataAdapter = new MySqlDataAdapter(query, m_conn);

            mySqlDataAdapter.Fill(dataTable);

            foreach (DataRow r in dataTable.Rows)
            {
                EffectList EffectAdd = new EffectList();

                EffectAdd.EffectDescList = r["Description"].ToString();
                EffectAdd.EffectID = (int)r["EffectID"];
                EffectAdd.EffectToolList = r["Tooltip"].ToString();

                // Add to the list
                m_EffectList.Add(EffectAdd);
            }
        }

        // Find an Effect ID
        private int FindEffectID(int EffectID)
        {
            int Index = 0;

            foreach (EffectList Eff in m_EffectList)
            {
                if (Eff.EffectID == EffectID)
                {
                    return Index;
                }
                Index++;
            }

            return -1;
        }

        // ----------------------------------------

        private void BrowseItems_Click(object sender, EventArgs e)
        {
            // Load up dialog box
            ItemBrowser.ShowDialog();

            // See if we got an item
            if (ItemBrowser.GetItemBase() != 0)
            {
                // If so save it and send it to the text box
                m_CurrentItem = ItemBrowser.GetItemBase();
                ItemIDBox.Text = m_CurrentItem.ToString();
                // Load in item data
                LoadItemData();
            }
        }

        private void LoadItem_Click(object sender, EventArgs e)
        {
            m_CurrentItem = Int32.Parse(ItemIDBox.Text);
            LoadItemData();
        }

        private void SaveEffect(int EffectNum)
        {
            string SQLData;

            if (m_EffectIDNum.Count == 0)
                return;

            //MessageBox.Show("EffectID: " + m_EffectIDNum[EffectNum].ToString() + "Select: " + m_BoxList[EffectNum].EffectBox.SelectedIndex.ToString());

            // See if we need to delete the effect
            if (m_EffectIDNum[EffectNum] != 0 && m_BoxList[EffectNum].EffectBox.SelectedIndex == 0)
            {
                //MessageBox.Show("Deleting Data!");
                // Delete the data if needed
                SQLData = "DELETE FROM `item_effects` WHERE `ItemEffectID` = '" + m_EffectIDNum[EffectNum] + "'";

                // Remove the EffectID
                m_EffectIDNum[EffectNum] = 0;
            }
            else
            {
                // If there is no data dont record it
                if (m_BoxList[EffectNum].EffectBox.SelectedIndex == 0 || m_EffectIDNum[EffectNum] == m_BoxList[EffectNum].EffectBox.SelectedIndex)
                {
                    return;
                }

                if (m_EffectIDNum[EffectNum] == 0)
                {
                    SQLData = "INSERT INTO `item_effects` SET ";
                    SQLData += "`ItemID` = '" + m_CurrentItem + "',";
                }
                else
                {
                    SQLData = "UPDATE `item_effects` SET ";
                }

                // Fix and make sure all data is atleast a 0
                m_BoxList[EffectNum].Var1.Text = m_BoxList[EffectNum].Var1.Text == "" ? "0" : m_BoxList[EffectNum].Var1.Text;
                m_BoxList[EffectNum].Var2.Text = m_BoxList[EffectNum].Var2.Text == "" ? "0" : m_BoxList[EffectNum].Var2.Text;
                m_BoxList[EffectNum].Var3.Text = m_BoxList[EffectNum].Var3.Text == "" ? "0" : m_BoxList[EffectNum].Var3.Text;

                // Main Data
                SQLData += "`item_effect_base_id` = '" + m_EffectList[m_BoxList[EffectNum].EffectBox.SelectedIndex].EffectID + "',";
                SQLData += "`Var1Data` = '" + m_BoxList[EffectNum].Var1.Text + "',";
                SQLData += "`Var2Data` = '" + m_BoxList[EffectNum].Var2.Text + "',";
                SQLData += "`Var3Data` = '" + m_BoxList[EffectNum].Var3.Text + "' ";

                if (m_EffectIDNum[EffectNum] != 0)
                {
                    SQLData += "WHERE `ItemID` = '" + m_CurrentItem + "' AND `ItemEffectID` = '" + m_EffectIDNum[EffectNum] + "'";
                }
                //MessageBox.Show("MySQL", SQLData);
            }


            int EffID = ExecuteSQL(SQLData);

            // Save EffectID
            if (m_EffectIDNum[EffectNum] == 0)
            {
                m_EffectIDNum[EffectNum] = EffID;
            }
        }

        private void SaveItemData()
        {
            string SQLData;

            // if we havent loaded any item dont save!
            if (m_EffectIDNum.Count == 0)
                return;

            // Save all the Effect Data
            for (int x = 0; x < 5; x++)
            {
                SaveEffect(x);
            }

            // Save Effect Container
            if (m_CurrentContainer == 0)
            {
                SQLData = "INSERT INTO `item_effect_container` SET ";
                SQLData += "`ItemID` = '" + m_CurrentItem + "', ";
            }
            else
            {
                SQLData = "UPDATE `item_effect_container` SET ";
            }

            SQLData += "`RechargeTime` = '" + CoolDown.Text + "', ";
            SQLData += "`EnergyUse` = '" + EnergyUse.Text + "', ";
            SQLData += "`_Range` = '" + Range.Text + "', ";
            SQLData += "`EquipEffect` = '1'";

            if (m_CurrentContainer != 0)
            {
                SQLData += " WHERE `ItemID` = '" + m_CurrentItem + "' AND `EffectContainerID` = '" + m_CurrentContainer + "'";
            }

            int ContainID = ExecuteSQL(SQLData);

            // Save ContainerID
            if (m_CurrentContainer == 0)
            {
                m_CurrentContainer = ContainID;
            }


        }

        private void LoadItemData()
        {
            // Load in item name
            MySqlDataReader ItemQuery = ExcuteSQLQuery("SELECT name FROM item_base WHERE id = '" + m_CurrentItem + "'");

            // See if we found data
            if (ItemQuery.HasRows)
            {
                ItemQuery.Read();
                ItemName.Text = ItemQuery.GetString(0);
            }

            // remove all data
            m_EffectIDNum.Clear();

            // Set it all to 0
            for (int y = 0; y < 5; y++)
            {
                m_BoxList[y].EffectBox.SelectedIndex = 0;
            }

            // Find all Effects on this item
            DataTable dataTable = new DataTable();
            MySqlConnection conn = new MySqlConnection(DB.GetLoginStr());
            String query = "SELECT ItemEffectID, item_effect_base_id, Var1Data, Var2Data, Var3Data FROM item_effects WHERE ItemID = '" + m_CurrentItem + "'";
            MySqlDataAdapter mySqlDataAdapter = new MySqlDataAdapter(query, conn);

            mySqlDataAdapter.Fill(dataTable);

            int x = 0;
            foreach (DataRow r in dataTable.Rows)
            {
                m_BoxList[x].EffectBox.Enabled = true;

                m_EffectIDNum.Add((int) r["ItemEffectID"]);
                int EffID = (int) r["item_effect_base_id"];
                m_BoxList[x].EffectBox.SelectedIndex = FindEffectID(EffID);
                m_BoxList[x].Var1.Text = r["Var1Data"].ToString();
                m_BoxList[x].Var2.Text = r["Var2Data"].ToString();
                m_BoxList[x].Var3.Text = r["Var3Data"].ToString();
                x++;
                // Make sure we dont go over the count #
                if (x >= m_BoxList.Count)
                {
                    break;
                }
            }
            // Fill rest with 0's
            for (; x < 5; x++)
            {
                m_BoxList[x].EffectBox.Enabled = true;
                m_EffectIDNum.Add(0);
            }

            // Zero this out, if we cant find a container when we save
            // we will create a new one
            m_CurrentContainer = 0;

            // Find containers for Activateable Only
            DataTable ContainerDataTable = new DataTable();
            MySqlConnection ContainConn = new MySqlConnection(DB.GetLoginStr());
            String ContainerQuery = "SELECT EffectContainerID,RechargeTime,Unknown2,_Range,Unknown4, EnergyUse FROM Item_effect_container WHERE EquipEffect = '1' AND ItemID = '" + m_CurrentItem + "'";
            MySqlDataAdapter ContainermySqlDataAdapter = new MySqlDataAdapter(ContainerQuery, ContainConn);

            ContainermySqlDataAdapter.Fill(ContainerDataTable);

            if (ContainerDataTable.Rows.Count > 0)
            {
                // Read in the data (will only have 1 row)
                DataRow r = ContainerDataTable.Rows[0];

                m_CurrentContainer = (int)r["EffectContainerID"];
                Range.Text = r["_Range"].ToString();
                CoolDown.Text = r["RechargeTime"].ToString();
                EnergyUse.Text = r["EnergyUse"].ToString();
            }
            else
            {
                Range.Text = "0";
                CoolDown.Text = "0";
                EnergyUse.Text = "0";
            }


            // Enable effects to be changed
            //EnableEffects(true);
        }

        private void Save_Click(object sender, EventArgs e)
        {
            // Save data
            SaveItemData();
        }
    }
}
