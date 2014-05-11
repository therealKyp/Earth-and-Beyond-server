using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using MySql.Data.MySqlClient;
using TalkTreeEditor;


namespace Station_Tools
{
    public partial class Main : Form
    {
        private ContextMenu popUpMenu;

        private string m_DataBase = "net7";

        Control oldFormData = new Control();    // Saves old Data for resizing

        VenderTab Venders = new VenderTab();

        #region   - Varables for Stations -

        // Selected Node
        TreeNode SelectedNodeTree;              // This will hold the current Selected node
        // Current Values
        private string m_CurrentStationName;    // Current Station Name
        private int m_CurrentStationID;         // This holds the current StationID 0 if non selected
        private int m_CurrentStationType;       // This is the current Station Type
        private bool m_HasChanged;              // If this is set to true we need to ask to save the data
        private bool m_Loading;                 // Set when we are loading from SQL
        private int []m_VenderGroups = new int[400];  // List of the groups in combox

        private string m_TalkTreeData;          // TalkTree Data

        #endregion

         public enum IconType
        {
            // Icons
            Folder_Icon = 0,
            Station_Icon = 1,
            Terminal_Icon = 2,
            Room_Icon = 3,
            NPC_Icon = 4
        }

        public Main()
        {
            InitializeComponent();
            this.Text = "Station Editor - DASE " + System.Reflection.Assembly.GetExecutingAssembly().GetName().Version.Major.ToString() + "." + System.Reflection.Assembly.GetExecutingAssembly().GetName().Version.Minor.ToString() + " Build " + System.Reflection.Assembly.GetExecutingAssembly().GetName().Version.Revision.ToString();

            // Display Vender Tab
            tabPage2.Controls.Add(Venders);

            // Store this for resizing
            oldFormData.Height = this.Height;
            oldFormData.Width = this.Width;
        }

        private void Load_Icons()
        {
            // Load Image List for TreeView
            ImageList list = new ImageList();

            System.Drawing.Image StationImage = Image.FromFile(Application.StartupPath + "/ico/stations.gif");
            System.Drawing.Image TerminalImage = Image.FromFile(Application.StartupPath + "/ico/terminals.gif");
            System.Drawing.Image RoomImage = Image.FromFile(Application.StartupPath + "/ico/rooms.gif");
            System.Drawing.Image NPCImage = Image.FromFile(Application.StartupPath + "/ico/npc.gif");

            list.Images.Add(new Icon(Application.StartupPath + "/ico/folder.ico"));
            list.Images.Add(StationImage);
            list.Images.Add(TerminalImage);
            list.Images.Add(RoomImage);
            list.Images.Add(NPCImage);

            StationList.ImageList = list;
        }

        private void LoadStation_Click(object sender, EventArgs e)
        {
            // Check for saving
            // if cancle then drop out
            if (!CheckSave())
                return;

            // Station Name from CombBox
            try
            {
                string m_StationName = StationListBox.SelectedItem.ToString();  // Get Station Name Selected
                m_CurrentStationName = m_StationName;                           // Save Station Name
                LoadStationTree(m_StationName);                                 // Load station into tree
                // Reset Tabs
                RemoveAllTabs();                                                // Unset all tabs
            }
            catch
            {
                if (m_CurrentStationName == "")
                    MessageBox.Show("No station was selected please select a station!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private bool SqlLoadStarBases()
        {
            // Populate the Starbase Combox
            string m_QueryStr = "SELECT `starbases`.`starbase_id`,`starbases`.`sector_id`,`starbases`.`name`,`starbases`.`type`,`starbases`.`is_active`,`starbases`.`description`,`starbases`.`welcome_message`,`starbases`.`target_sector_object`, `starbases`.`faction_id` FROM `starbases`";
            MySqlConnection m_Connection = new MySqlConnection(SQLData.ConnStr(m_DataBase));
            MySqlCommand m_Command = new MySqlCommand(m_QueryStr, m_Connection);

            m_Loading = true;
            try
            {
                m_Connection.Open();
            }
            catch
            {
                MessageBox.Show("Couldn't Connect to MySQL Check your username/password!\n", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }
            m_Loading = false;
            
            MySqlDataReader m_Reader = m_Command.ExecuteReader();

            // Make sure the box is cleared
            StationListBox.Items.Clear();

            while (m_Reader.Read())
            {
                StationListBox.Items.Add(m_Reader.GetString(2));
            }
            return true;
        }

        private void RemoveAllTabs()
        {
            tabControl2.TabPages.Remove(NPCPage);
            tabControl2.TabPages.Remove(RoomPage);
            tabControl2.TabPages.Remove(TerminalPage);
            tabControl2.TabPages.Remove(StationPage);
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // Load Icons for Tree
            Load_Icons();

            // Unset all tabs
            RemoveAllTabs();

            // Load a station list of starbases
            if (!SqlLoadStarBases())
            {
                // Error connecting exiting program
                this.Close();
            }
            else
            {
                // Setup Station Tab
                SetupStationTab();
                // Set up terminal tab
                SetupTerminalTab();
                // Set up Room Tab
                SetupRoomTab();
                // Set up NPC Tab
                SetupNPCTab();
            }

            // No current Station
            m_CurrentStationID = 0;
            m_CurrentStationType = 0;
            m_CurrentStationName = "";
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

        #region  --- Station Functions ---

        #region ------------------------- Node Handeler -------------------------

        // When tree node is selected
        private void StationList_AfterSelect(object sender, TreeViewEventArgs e)
        {
            // Set Correct Tabs Up
            SetSelectedTabs();
        }

        private void SetSelectedTabs()
        {
            // If we are allready on this node dont change anything
            if (StationList.SelectedNode == SelectedNodeTree)
                return;

            // Check to see if we need to save
            // if cancel drop out
            if (!CheckSave())
            {
                // Revert to old node
                StationList.SelectedNode = SelectedNodeTree;
                return;
            }

            // Save selected node
            SelectedNodeTree = StationList.SelectedNode;


            // Set current Tabs

            // The partent Tabs are used to determan the holder
            // of the childs.  This way we can save the Terminal
            // and NPC ids in the tree
            if (StationList.SelectedNode.Name == "StationNode")
            {
                // Load Station Data
                LoadStationData();

                // If tab is not open open it and close the others
                if (!tabControl2.TabPages.Contains(StationPage))
                {
                    // Set StationPage on
                    tabControl2.TabPages.Remove(NPCPage);
                    tabControl2.TabPages.Remove(RoomPage);
                    tabControl2.TabPages.Remove(TerminalPage);
                    tabControl2.TabPages.Add(StationPage);
                }
            }
            else if (StationList.SelectedNode.Parent.Name == "StationNode")
            {
                // Load Room Data
                LoadRoomData();
                // If tab is not open open it and close the others
                if (!tabControl2.TabPages.Contains(RoomPage))
                {
                    // Set RoomPage on
                    tabControl2.TabPages.Remove(NPCPage);
                    tabControl2.TabPages.Add(RoomPage);
                    tabControl2.TabPages.Remove(TerminalPage);
                    tabControl2.TabPages.Remove(StationPage);
                }
            }
            else if (StationList.SelectedNode.Parent.Name == "TerminalNode")
            {
                // Load in the new Terminal Data
                LoadTerminalData();
                // If tab is not open open it and close the others
                if (!tabControl2.TabPages.Contains(TerminalPage))
                {
                    // Set TerminalPage on
                    tabControl2.TabPages.Remove(NPCPage);
                    tabControl2.TabPages.Remove(RoomPage);
                    tabControl2.TabPages.Add(TerminalPage);
                    tabControl2.TabPages.Remove(StationPage);
                }
            }
            else if (StationList.SelectedNode.Parent.Name == "NPCNode")
            {
                // Load NPC Data
                LoadNPCData();
                // If tab is not open open it and close the others
                if (!tabControl2.TabPages.Contains(NPCPage))
                {
                    // Set NPC Page on
                    tabControl2.TabPages.Add(NPCPage);
                    tabControl2.TabPages.Remove(RoomPage);
                    tabControl2.TabPages.Remove(TerminalPage);
                    tabControl2.TabPages.Remove(StationPage);
                }
            }
        }

        private void StationList_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                string AddNodeType = "";
                string DeleteNodeType = "";

                // See what we are to add/delete

                // The partent Tabs are used to determan the holder
                // of the childs.  This way we can save the Terminal
                // and NPC ids in the tree
                if (SelectedNodeTree.Name == "StationNode")
                {
                    AddNodeType = "Room";
                    DeleteNodeType = "Station";
                }
                else if (SelectedNodeTree.Name == "TerminalNode")
                {
                    AddNodeType = "Terminal";
                }
                else if (SelectedNodeTree.Name == "NPCNode")
                {
                    AddNodeType = "NPC";
                }
                else if (SelectedNodeTree.Parent.Name == "StationNode")
                {
                    DeleteNodeType = "Room";
                }
                else if (SelectedNodeTree.Parent.Name == "TerminalNode")
                {
                    DeleteNodeType = "Terminal";
                }
                else if (SelectedNodeTree.Parent.Name == "NPCNode")
                {
                    DeleteNodeType = "NPC";
                }

                // Create Popup Menu
                popUpMenu = new ContextMenu();

                // Set up popups
                if (AddNodeType != "")
                    popUpMenu.MenuItems.Add("Add " + AddNodeType, new EventHandler(PopUpMenuAdd));
                if (DeleteNodeType != "")
                    popUpMenu.MenuItems.Add("Delete " + DeleteNodeType, new EventHandler(PopUpMenuDelete));

                // Make sure we should atleast add or delete
                if (AddNodeType != "" || DeleteNodeType != "")
                    this.ContextMenu = popUpMenu;
                else
                    this.ContextMenu.Dispose();
            }
        }

        #endregion

        #region ------------------------- Popup Menu Handelers -------------------------

        // Pop up menu handler for Deleting
        private void PopUpMenuDelete(object sender, EventArgs e)
        {
            MenuItem mClicked = (MenuItem)sender;

            // Remove Station from Tree
            if (mClicked.Text == "Delete Station")
            {
                if (SelectedNodeTree.GetNodeCount(true) > 0)
                {
                    MessageBox.Show("You must delete all rooms before removing a starbase", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
                else
                {
                    Delete_StarBase();
                }
            }

            // Remove Room from Tree
            if (mClicked.Text == "Delete Room")
            {
                if (SelectedNodeTree.GetNodeCount(true) > 2)
                {
                    MessageBox.Show("You must delete all NPC's/Terminals before removing a room", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
                else
                {
                    Delete_Room();
                }
            }

            // Remove NPC from Tree
            if (mClicked.Text == "Delete NPC")
            {
                Delete_NPC();
            }

            // Remove Terminal from Tree
            if (mClicked.Text == "Delete Terminal")
            {
                Delete_Terminal();
            }

            // This removes the old popup menu
            this.ContextMenu.Dispose();
        }

        // Pop up menu handler for Adding
        private void PopUpMenuAdd(object sender, EventArgs e)
        {
            MenuItem mClicked = (MenuItem)sender;

            // Add a new terminal to the tree
            if (mClicked.Text == "Add Terminal")
            {
                int m_RoomID = Int32.Parse(SelectedNodeTree.Parent.Name);
                int m_Index = SelectedNodeTree.Nodes.Count;
                int m_TerminalID = Create_Terminal(m_RoomID, m_Index);

                if (m_TerminalID != 1)
                {
                    TreeNode TerminalLeaf = new TreeNode();
                    TerminalLeaf.Text = "Terminal " + m_Index;
                    TerminalLeaf.Name = m_TerminalID.ToString();
                    TerminalLeaf.ImageIndex = (int)IconType.Terminal_Icon;
                    TerminalLeaf.SelectedImageIndex = (int)IconType.Terminal_Icon;

                    SelectedNodeTree.Nodes.Add(TerminalLeaf);
                }
            }

            // Add a new NPC to the tree
            if (mClicked.Text == "Add NPC")
            {
                int m_RoomID = Int32.Parse(SelectedNodeTree.Parent.Name);
                int m_Index = SelectedNodeTree.Nodes.Count;
                int m_NPCID = Create_NPC(m_RoomID, m_Index);

                if (m_NPCID != -1)
                {
                    TreeNode NPCLeaf = new TreeNode();
                    NPCLeaf.Text = "NPC " + m_Index;
                    NPCLeaf.Name = m_NPCID.ToString();
                    NPCLeaf.ImageIndex = (int)IconType.NPC_Icon;
                    NPCLeaf.SelectedImageIndex = (int)IconType.NPC_Icon;

                    SelectedNodeTree.Nodes.Add(NPCLeaf);
                }
            }

            // Add a new room to the list
            if (mClicked.Text == "Add Room")
            {
                int m_RoomID = Create_Room();

                if (m_RoomID != -1)
                {
                    // Set up Room
                    TreeNode RoomTree = new TreeNode();
                    RoomTree.Text = "Room " + SelectedNodeTree.Nodes.Count;
                    RoomTree.Name = m_RoomID.ToString();
                    RoomTree.ImageIndex = (int)IconType.Room_Icon;
                    RoomTree.SelectedImageIndex = (int)IconType.Room_Icon;

                    // Set up Terminals Nodes
                    TreeNode TermNode = new TreeNode();
                    TermNode.Text = "Terminals";
                    TermNode.Name = "TerminalNode";
                    TermNode.ImageIndex = (int)IconType.Folder_Icon;
                    TermNode.SelectedImageIndex = (int)IconType.Folder_Icon;
                    // Add Terminals
                    RoomTree.Nodes.Add(TermNode);

                    // Set up NPC Nodes
                    TreeNode NPCNode = new TreeNode();
                    NPCNode.Text = "NPC's";
                    NPCNode.Name = "NPCNode";
                    NPCNode.ImageIndex = (int)IconType.Folder_Icon;
                    NPCNode.SelectedImageIndex = (int)IconType.Folder_Icon;
                    // Add Terminals
                    RoomTree.Nodes.Add(NPCNode);

                    // Add All Nodes
                    SelectedNodeTree.Nodes.Add(RoomTree);
                } else
                    MessageBox.Show("Error creating new room!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }

            // This removes the old popup menu
            this.ContextMenu.Dispose();
        }

        #endregion

        #region ------------------------- Use of Buttons -------------------------

        private void NewStation_Click(object sender, EventArgs e)
        {
            int Object_ID;

            // Remove all Stations
            StationList.Nodes.Clear();

            Object_ID = Create_Starbase();

            if (Object_ID == -1)
                MessageBox.Show("Error creating new station!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            else
            {
                TreeNode StationTree = new TreeNode();
                StationTree.Text = "New Station";
                StationTree.Name = "StationNode";
                StationTree.ImageIndex = (int)IconType.Station_Icon;
                StationTree.SelectedImageIndex = (int)IconType.Station_Icon;

                StationList.Nodes.Add(StationTree);                             // Add to Tree List
                StationListBox.Items.Add(StationTree.Text);                     // Add to dropdown list
                StationListBox.SelectedIndex = StationListBox.Items.Count - 1;
                // Save Current Values
                m_CurrentStationID = Object_ID;
                m_CurrentStationName = StationTree.Text;
                m_CurrentStationType = 0;
            }

        }

        private void Reload_Click(object sender, EventArgs e)
        {
            // Check for saving
            // if cancle then drop out
            if (!CheckSave())
                return;

            // Reload all stations
            RemoveAllTabs();
            SqlLoadStarBases();
        }

        #endregion

        #region  -------------------------   Save Tabs -------------------------
        private void MasterSave()
        {
            // Find open Tab
            if (tabControl2.TabPages.Contains(StationPage))
            {
                Save_Station();
            }
            else if (tabControl2.TabPages.Contains(RoomPage))
            {
                Save_Room();
            }
            else if (tabControl2.TabPages.Contains(TerminalPage))
            {
                Save_Terminals();
            }
            else if (tabControl2.TabPages.Contains(NPCPage))
            {
                Save_NPCS();
            }
        }

        private void Save_Station()
        {
            string m_SqlAdd = "UPDATE `starbases` SET ";

            m_SqlAdd += "`name` = \"" + StationName.Text + "\", ";
            m_SqlAdd += "`sector_id` = '" + StationSectorID.Text + "', ";
            m_SqlAdd += "`target_sector_object` = '" + StationObjectID.Text + "', ";
            m_SqlAdd += "`description` = \"" + StationDisc.Text + "\", ";
            m_SqlAdd += "`welcome_message` = \"" + StationWelcome.Text + "\", ";

            int FactionID = StationFaction.SelectedIndex + 1;

            m_SqlAdd += "`faction_id` = '" + FactionID.ToString() + "', ";
            m_SqlAdd += "`type` = '" + StationType.SelectedIndex.ToString() + "', ";
            m_SqlAdd += "`is_active` = '" + StationActive.SelectedIndex.ToString() + "'";

            m_SqlAdd += " WHERE `starbase_id` = '" + m_CurrentStationID + "'";

            // Connect to Database and update
            MySqlConnection m_Connection = new MySqlConnection(SQLData.ConnStr(m_DataBase));
            MySqlCommand m_Command = new MySqlCommand(m_SqlAdd, m_Connection);

            try
            {
                // Execute Command
                m_Connection.Open();
                m_Command.ExecuteNonQuery();
                m_Connection.Close();
            }
            catch (Exception Error)
            {
                MessageBox.Show("SQL Error: " + Error.Message);
            }
        }

        private void Save_Terminals()
        {
            string m_SqlAdd = "UPDATE `starbase_terminals` SET ";

            m_SqlAdd += "`location` = '" + TerminalLocation.SelectedIndex.ToString() + "', ";
            m_SqlAdd += "`type` = '" + TerminalType.SelectedIndex.ToString() + "', ";
            m_SqlAdd += "`attribute` = '" + TerminalAttribute.Text + "', ";
            m_SqlAdd += "`description` = \"" + TerminalDisc.Text + "\" ";

            m_SqlAdd += " WHERE `terminal_id` = '" + SelectedNodeTree.Name + "'";

            // Connect to Database and update
            MySqlConnection m_Connection = new MySqlConnection(SQLData.ConnStr(m_DataBase));
            MySqlCommand m_Command = new MySqlCommand(m_SqlAdd, m_Connection);

            try
            {
                // Execute Command
                m_Connection.Open();
                m_Command.ExecuteNonQuery();
                m_Connection.Close();
            }
            catch (Exception Error)
            {
                MessageBox.Show("SQL Error: " + Error.Message);
            }
        }

        private void Save_Room()
        {
            int m_RoomStyle = 0;

            m_RoomStyle += RoomDeviders.Checked? 1 : 0;
            m_RoomStyle += RoomRafters.Checked? 2 : 0;
            m_RoomStyle += RoomBar.Checked? 4 : 0;
            m_RoomStyle += RoomTables.Checked? 8 : 0;
            m_RoomStyle += RoomMonitors.Checked ? 16 : 0;
            m_RoomStyle += RoomEyecandy.Checked? 32 : 0;
            m_RoomStyle += RoomSmallroom.Checked? 64 : 0;
            m_RoomStyle += RoomAltroom.Checked? 128 : 0;
            m_RoomStyle += RoomFog.Checked? 256 : 0;

            string m_SqlAdd = "UPDATE `starbase_rooms` SET ";
            m_SqlAdd += "`type` = '" + RoomType.SelectedIndex + "', ";
            m_SqlAdd += "`style` = '" + m_RoomStyle + "', ";
            m_SqlAdd += "`fog_near` = '" + RoomFogNear.Text + "', ";
            m_SqlAdd += "`fog_far` = '" + RoomFogFar.Text + "', ";
            m_SqlAdd += "`fog_red` = '" + ColorBox.Color.R.ToString() + "', ";
            m_SqlAdd += "`fog_green` = '" + ColorBox.Color.G.ToString() + "', ";
            m_SqlAdd += "`fog_blue` = '" + ColorBox.Color.B.ToString() + "', ";
            m_SqlAdd += "`description` = \"" + RoomDiscription.Text + "\" ";
            m_SqlAdd += " WHERE `room_id` = '" + SelectedNodeTree.Name + "'";

            // Connect to Database and update
            MySqlConnection m_Connection = new MySqlConnection(SQLData.ConnStr(m_DataBase));
            MySqlCommand m_Command = new MySqlCommand(m_SqlAdd, m_Connection);

            try
            {
                // Execute Command
                m_Connection.Open();
                m_Command.ExecuteNonQuery();
                m_Connection.Close();
            }
            catch (Exception Error)
            {
                MessageBox.Show("SQL Error: " + Error.Message);
            }
        }

        private void Save_NPCS()
        {
            string m_NPCAdd, m_SqlAdd, TalkTree;

            // Translate Data so MySQL can read it
            TalkTree = m_TalkTreeData.Replace("\\", "\\\\");
            TalkTree = TalkTree.Replace("\"", "\\\"");
            TalkTree = TalkTree.Replace("'", "\\'");

            // Update NPC
            m_NPCAdd = "UPDATE `starbase_npcs` SET ";
            m_NPCAdd += "`first_name` = \"" + NPCFirstName.Text + "\", ";
            m_NPCAdd += "`last_name` = \"" + NPCLastName.Text + "\", ";
            m_NPCAdd += "`location` = '" + NPCLocation.SelectedIndex + "', ";
            m_NPCAdd += "`description` = \"" + NPCDiscription.Text + "\", ";
            m_NPCAdd += "`talk_tree_handle` = \"" + TalkTree + "\" ";
            m_NPCAdd += " WHERE `npc_Id` = '" + SelectedNodeTree.Name + "';";

            // Update Booths
            m_SqlAdd = "UPDATE `starbase_vendors` SET ";
            m_SqlAdd += "`level` = '" + NPCLevel.SelectedIndex + "', ";
            m_SqlAdd += "`booth_type` = '" + (NPCBoothType.SelectedIndex - 1) + "', ";
            m_SqlAdd += "`groupid` = '" + m_VenderGroups[VenderGroupBox.SelectedIndex] + "' ";
            m_SqlAdd += " WHERE `vendor_id` = '" + SelectedNodeTree.Name + "';";

            // Connect to Database and update
            MySqlConnection m_Connection = new MySqlConnection(SQLData.ConnStr(m_DataBase));
            MySqlCommand m_CommandNPC = new MySqlCommand(m_NPCAdd, m_Connection);
            MySqlCommand m_CommandBooth = new MySqlCommand(m_SqlAdd, m_Connection);
            
            try
            {
                // Execute Command
                m_Connection.Open();
                m_CommandNPC.ExecuteNonQuery();
                m_CommandBooth.ExecuteNonQuery();
                m_Connection.Close();
            }
            catch (Exception Error)
            {
                MessageBox.Show("SQL Error: " + Error.Message);
            }
        }

        private void Save_Click(object sender, EventArgs e)
        {
            // Check for saving
            // if cancle then drop out
            if (!CheckSave())
                return;
        }

        private bool CheckSave()
        {
            // See if anything has changed
            if (m_HasChanged)
            {
                // Display Dialog
                DialogResult Result = MessageBox.Show("Do you want to save your work?", "Save", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Exclamation);

                // Check result
                if (Result == DialogResult.Yes)
                {
                    // Save Data
                    MasterSave();
                }
                else if (Result == DialogResult.Cancel)
                {
                    // Cancel
                    return false;
                }
            }
            // Reset the flag if we saved/didnt save
            m_HasChanged = false;
            return true;
        }
        #endregion

        #region ------------------------- Station Tab Setup -------------------------

        private void SetupRoomTab()
        {
            // Reset all attributes
            RoomFogFar.Text = "";
            RoomFogNear.Text = "";
            RoomDiscription.Text = "";

            /*
             * Terminal Types in Order By ID
             */
            RoomType.Items.Clear();
            RoomType.Items.Add("Hangar");
            RoomType.Items.Add("Main Lobby");
            RoomType.Items.Add("Bazaar");
            RoomType.Items.Add("Lounge");
        }

        // Set up NPC tab
        private void SetupNPCTab()
        {
            // Reset all Text Attributes
            NPCDiscription.Text = "";
            NPCFirstName.Text = "";
            NPCLastName.Text = "";
            NPCAvatarID.Text = "";

            // NPC Location List
            NPCLocation.Items.Clear();
            // List of 15 locations
            for (int x = 0; x < 15; x++)
                NPCLocation.Items.Add(x.ToString());

            // NPC Level List
            NPCLevel.Items.Clear();
            // List of 0-9 Levels
            for (int x = 0; x <= 9; x++)
                NPCLevel.Items.Add(x.ToString());
            
            // Setup NPC Booth Type
            NPCBoothType.Items.Clear();
            NPCBoothType.Items.Add("No Booth");                // This is a -1 in the actual message
            NPCBoothType.Items.Add("Red (Weapons)");
            NPCBoothType.Items.Add("Light Blue ()");
            NPCBoothType.Items.Add("Gray  (Engines, Reactors, and Shields)");
            NPCBoothType.Items.Add("Purple (Consumables)");
            NPCBoothType.Items.Add("Yellow (Trade Goods)");
            NPCBoothType.Items.Add("Brown (Components)");
            NPCBoothType.Items.Add("Green (Ore)");


            // Load in Vender Groups
            // Populate the Starbase Combox
            string m_QueryStr = "SELECT `GroupID`,`GroupName` FROM `starbase_vender_groups`";

            MySqlDataReader m_Reader = ExcuteSQLQuery(m_QueryStr);

            int index = 0;
            VenderGroupBox.Items.Clear();

            VenderGroupBox.Items.Add("Not a Vender");
            m_VenderGroups[index] = -1;

            // Has no data yet
            if (m_Reader.HasRows)
            {
                while (m_Reader.Read())
                {
                    // Update Index
                    index++;
                    // Save data into a structure
                    m_VenderGroups[index] = m_Reader.GetInt32(0);      // Save Group ID
                    VenderGroupBox.Items.Add(m_Reader.GetString(1));   // Save Group Name
                }
            }



            // Factions
            NPCFaction.Items.Clear();
            // Query from Factions table to get the faction ID
            string m_FactionQuery = "SELECT `factions`.`name` FROM `factions` ORDER BY `factions`.`faction_id` ASC";
            // Connect to Database
            MySqlConnection m_Connection = new MySqlConnection(SQLData.ConnStr(m_DataBase));
            MySqlCommand m_Command = new MySqlCommand(m_FactionQuery, m_Connection);
            m_Connection.Open();

            // Read Data
            m_Reader = m_Command.ExecuteReader();
            while (m_Reader.Read())
            {
                NPCFaction.Items.Add(m_Reader.GetString(0));
            }
            m_Connection.Close();
        }

        // Set up terminal tab
        private void SetupTerminalTab()
        {
            // Reset all Text Attributes
            TerminalAttribute.Text = "0";
            TerminalDisc.Text = "";

            // Terminal Location List
            TerminalLocation.Items.Clear();
            // List of 15 locations
            for (int x = 0; x < 15; x++)
                TerminalLocation.Items.Add(x.ToString());

            // Set up terminal type list
            TerminalType.Items.Clear();
            TerminalType.Items.Add("Refining");
            TerminalType.Items.Add("Analyze");
            TerminalType.Items.Add("Manufacturing");
            TerminalType.Items.Add("Jobs");
            TerminalType.Items.Add("Intergalactic Net");
            TerminalType.Items.Add("Customize Ship");
            TerminalType.Items.Add("Customize Avatar");
            TerminalType.Items.Add("Training (Don't Use)");
        }

        // Set up Station Tab
        private void SetupStationTab()
        {
            StationName.Text = "";
            StationSectorID.Text = "";
            StationObjectID.Text = "";
            StationDisc.Text = "";
            StationWelcome.Text = "";

            // Load in types in order by ID
            StationType.Items.Clear();
            StationType.Items.Add("Terran");
            StationType.Items.Add("Jenquai");
            StationType.Items.Add("Progen");
            StationType.Items.Add("Solsec");
            StationType.Items.Add("Highport");
            StationType.Items.Add("Net7");
            StationType.Items.Add("PleasurePort");
            StationType.Items.Add("Pirates");
            StationType.Items.Add("Thule");

            // IsActive 
            StationActive.Items.Clear();
            StationActive.Items.Add("False");
            StationActive.Items.Add("True");

            // Factions
            StationFaction.Items.Clear();
            // Query from Factions table to get the faction ID
            string m_FactionQuery = "SELECT `factions`.`name` FROM `factions` ORDER BY `factions`.`faction_id` ASC";
            // Connect to Database
            MySqlConnection m_Connection = new MySqlConnection(SQLData.ConnStr(m_DataBase));
            MySqlCommand m_Command = new MySqlCommand(m_FactionQuery, m_Connection);
            m_Connection.Open();

            // Read Data
            MySqlDataReader m_Reader = m_Command.ExecuteReader();
            while (m_Reader.Read())
            {
                StationFaction.Items.Add(m_Reader.GetString(0));
            }
            m_Connection.Close();
        }
        #endregion

        #region   ------------------------- Create -------------------------
        private int Create_Starbase()
        {
            // Add StarBase
            string m_AddSql = "INSERT INTO `starbases` (`sector_id`,`name`,`type`,`is_active`,`description`,`welcome_message`,`target_sector_object`,`faction_id`)  VALUES ('0', 'New Station', '0', '0', 'Enter Description', 'Enter Welcome', '0', '1' );";
            string m_GetID = "SELECT LAST_INSERT_ID()";     // Grab the ID of last instered row
            int m_ObjectID = 0;

            // Connect to Database and update
            MySqlConnection m_Connection = new MySqlConnection(SQLData.ConnStr(m_DataBase));
            MySqlCommand m_CommandAdd = new MySqlCommand(m_AddSql, m_Connection);
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
                MessageBox.Show("SQL Error: " + Error.Message);
                return -1;
            }
            return m_ObjectID;
        }

        private int Create_Room()
        {
            // Add Room
            string m_AddSql = "INSERT INTO `starbase_rooms` (`type`,`style`,`fog_near`,`fog_far`,`description`,`starbase_id`) VALUES ('0', '0', '0', '0', 'Enter Description', '" + m_CurrentStationID + "');";
            string m_GetID = "SELECT LAST_INSERT_ID()";     // Grab the ID of last instered row
            int m_ObjectID = 0;

            // Connect to Database and update
            MySqlConnection m_Connection = new MySqlConnection(SQLData.ConnStr(m_DataBase));
            MySqlCommand m_CommandAdd = new MySqlCommand(m_AddSql, m_Connection);
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
                MessageBox.Show("SQL Error: " + Error.Message);
                return -1;
            }
            return m_ObjectID;
        }

        private int Create_Terminal(int RoomID, int Index)
        {
            // Add Room
            string m_AddSql = "INSERT INTO `starbase_terminals` (`location`,`type`,`attribute`,`description`,`room_id`,`terminal_index`) VALUES ('0', '0', '0', 'Enter Description', '" + RoomID + "', '" + Index + "');";
            string m_GetID = "SELECT LAST_INSERT_ID()";     // Grab the ID of last instered row
            int m_ObjectID = 0;

            // Connect to Database and update
            MySqlConnection m_Connection = new MySqlConnection(SQLData.ConnStr(m_DataBase));
            MySqlCommand m_CommandAdd = new MySqlCommand(m_AddSql, m_Connection);
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
                MessageBox.Show("SQL Error: " + Error.Message);
                return -1;
            }
            return m_ObjectID;
        }

        private int Create_NPC(int RoomID, int Index)
        {
            // Add Room
            string m_AddAvatar = "INSERT INTO `starbase_npc_avatar_templates` (`avatar_type`) VALUES ('1')";
            string m_AddVender = "INSERT INTO `starbase_vendors` (`level`,`booth_type`) VALUES ('0', '-1');";
            string m_AddSql = "INSERT INTO `starbase_npcs` (`first_name`,`last_name`,`location`,`faction_id`,`description`,`talk_tree_handle`,`room_id`,`npc_index`) VALUES ('change', 'me', '0', '1', 'Enter Discription', 'talktree', '" + RoomID + "', '" + Index + "');";
            string m_GetID = "SELECT LAST_INSERT_ID()";     // Grab the ID of last instered row
            int m_ObjectID = 0;





            // Connect to Database and update
            MySqlConnection m_Connection = new MySqlConnection(SQLData.ConnStr(m_DataBase));
            MySqlCommand m_CommandAvatar = new MySqlCommand(m_AddAvatar, m_Connection);
            MySqlCommand m_CommandVender = new MySqlCommand(m_AddVender, m_Connection);
            MySqlCommand m_CommandAdd = new MySqlCommand(m_AddSql, m_Connection);
            MySqlCommand m_CommandGetID = new MySqlCommand(m_GetID, m_Connection);

            try
            {
                // Execute Command
                m_Connection.Open();
                m_CommandAvatar.ExecuteNonQuery();                          // Add Avatar
                m_CommandVender.ExecuteNonQuery();                          // Add Vender
                m_CommandAdd.ExecuteNonQuery();                             // Add NPC
                MySqlDataReader m_Reader = m_CommandGetID.ExecuteReader();  // Get ID
                m_Reader.Read();                                            // Read in data
                m_ObjectID = m_Reader.GetInt32(0);
                m_Connection.Close();
            }
            catch (Exception Error)
            {
                MessageBox.Show("SQL Error: " + Error.Message);
                return -1;
            }
            return m_ObjectID;
        }
        #endregion

        #region   ------------------------- Delete Information -------------------------
        private void Delete_StarBase()
        {
            // Delete StarBase
            string m_DeleteStarBase = "DELETE FROM `starbases` WHERE starbase_id = '" + m_CurrentStationID + "';";

            SelectedNodeTree.Nodes.Clear();                     // Remove all sub nodes
            SelectedNodeTree.Remove();                          // Remove our own node


            // Connect to Database and update
            MySqlConnection m_Connection = new MySqlConnection(SQLData.ConnStr(m_DataBase));
            MySqlCommand m_CommandDelete = new MySqlCommand(m_DeleteStarBase, m_Connection);

            try
            {
                // Execute Command
                m_Connection.Open();
                m_CommandDelete.ExecuteNonQuery();         // Run Delete Query
                m_Connection.Close();
            }
            catch (Exception Error)
            {
                MessageBox.Show("SQL Error: " + Error.Message);
            }
        }

        private void Delete_Room()
        {
            // Delete Terminal
            string m_DeleteRoom = "DELETE FROM `starbase_rooms` WHERE room_id = '" + SelectedNodeTree.Name + "';";

            TreeNode RoomList = SelectedNodeTree.Parent;        // Get Parent
            SelectedNodeTree.Nodes.Clear();                     // Remove all sub nodes
            SelectedNodeTree.Remove();                          // Remove our own node

            int Nodes = 0;
            // Loop though all the nodes
            TreeNode TNChild = RoomList.FirstNode;      // Get the first node
            while (TNChild != null)                     // Do while not Null
            {
                TNChild.Text = "Room " + Nodes;         // Change the room Name
                TNChild = TNChild.NextNode;             // Get Next Node
                Nodes++;
            }


            // Connect to Database and update
            MySqlConnection m_Connection = new MySqlConnection(SQLData.ConnStr(m_DataBase));
            MySqlCommand m_CommandDelete = new MySqlCommand(m_DeleteRoom, m_Connection);

            try
            {
                // Execute Command
                m_Connection.Open();
                m_CommandDelete.ExecuteNonQuery();         // Run Delete Query
                m_Connection.Close();
            }
            catch (Exception Error)
            {
                MessageBox.Show("SQL Error: " + Error.Message);
            }
        }
        
        private void Delete_Terminal()
        {
            // Delete Terminal
            string m_DeleteTerm = "DELETE FROM `starbase_terminals` WHERE terminal_id = '" + SelectedNodeTree.Name + "';";
            string m_UpdateQuery;
            int[] m_TermUpdate = new int[15];        // Save up to 15 terminal updates


            TreeNode TerminalList = SelectedNodeTree.Parent;    // Get Parent
            SelectedNodeTree.Nodes.Clear();                     // Remove all sub nodes
            SelectedNodeTree.Remove();                          // Remove our own node


            int Nodes = 0;
            // Loop though all the nodes
            TreeNode TNChild = TerminalList.FirstNode;  // Get the first node
            while (TNChild != null)                     // Do while not Null
            {
                m_TermUpdate[Nodes] = Int32.Parse(TNChild.Name);        // This will be used to update the terminal Index
                TNChild.Text = "Terminal " + Nodes;     // Change the terminal Name
                TNChild = TNChild.NextNode;             // Get Next Node
                Nodes++;
            }

            // Connect to Database and update
            MySqlConnection m_Connection = new MySqlConnection(SQLData.ConnStr(m_DataBase));
            MySqlCommand m_CommandDelete = new MySqlCommand(m_DeleteTerm, m_Connection);
            MySqlCommand m_UpdateSQL;

            try
            {
                // Execute Command
                m_Connection.Open();
                m_CommandDelete.ExecuteNonQuery();         // Run Delete Query
                for(int x=0;x<Nodes;x++)                   // Rewrite the index to not skip any numbers
                {
                    m_UpdateQuery = "UPDATE `starbase_terminals` SET `terminal_index` = '" + x + "' WHERE `terminal_id` = '" + m_TermUpdate[x] + "';";
                    m_UpdateSQL = new MySqlCommand(m_UpdateQuery, m_Connection);
                    m_UpdateSQL.ExecuteNonQuery();
                }
                m_Connection.Close();
            }
            catch (Exception Error)
            {
                MessageBox.Show("SQL Error: " + Error.Message);
            }
        }

        private void Delete_NPC()
        {
            // Delete NPC
            string m_DeleteNpc = "DELETE FROM `starbase_npcs` WHERE npc_Id = '" + SelectedNodeTree.Name + "';";
            string m_DeleteNpcVender = "DELETE FROM `starbase_vendors` WHERE vendor_id = '" + SelectedNodeTree.Name + "';";
            string DeleteAvatar = "DELETE FROM `starbase_npc_avatar_templates` WHERE `avatar_template_id` = '" + SelectedNodeTree.Name + "';";
            string m_UpdateQuery;
            int[] m_NpcUpdate = new int[15];        // Save up to 15 NPC updates


            TreeNode NPCList = SelectedNodeTree.Parent;    // Get Parent
            SelectedNodeTree.Nodes.Clear();                     // Remove all sub nodes
            SelectedNodeTree.Remove();                          // Remove our own node

            //TODO: Delete Vender's also when they are implimented
            
            int Nodes = 0;
            // Loop though all the nodes
            TreeNode TNChild = NPCList.FirstNode;  // Get the first node
            while (TNChild != null)                     // Do while not Null
            {
                m_NpcUpdate[Nodes] = Int32.Parse(TNChild.Name);        // This will be used to update the npc Index
                TNChild = TNChild.NextNode;             // Get Next Node
                Nodes++;
            }

            // Connect to Database and update
            MySqlConnection m_Connection = new MySqlConnection(SQLData.ConnStr(m_DataBase));
            MySqlCommand m_CommandDelete = new MySqlCommand(m_DeleteNpc, m_Connection);
            MySqlCommand m_CommandDeleteVender = new MySqlCommand(m_DeleteNpcVender, m_Connection);
            MySqlCommand m_CommandDeleteAvatar = new MySqlCommand(DeleteAvatar, m_Connection);
            MySqlCommand m_UpdateSQL;

            try
            {
                // Execute Command
                m_Connection.Open();
                m_CommandDelete.ExecuteNonQuery();               // Run Delete Query
                m_CommandDeleteVender.ExecuteNonQuery();         // Run Delete Query
                m_CommandDeleteAvatar.ExecuteNonQuery();         // Run Delete Query

                for (int x = 0; x < Nodes; x++)                  // Rewrite the index to not skip any numbers
                {
                    m_UpdateQuery = "UPDATE `starbase_npcs` SET `npc_index` = '" + x + "' WHERE `npc_Id` = '" + m_NpcUpdate[x] + "';";
                    m_UpdateSQL = new MySqlCommand(m_UpdateQuery, m_Connection);
                    m_UpdateSQL.ExecuteNonQuery();
                }
                m_Connection.Close();
            }
            catch (Exception Error)
            {
                MessageBox.Show("SQL Error: " + Error.Message);
            }
        }
        #endregion

        private void DisplayStation(int StationType, int RoomIndex, bool SmallRoom, bool AltRoom)
        {

            // wait till station is loaded
            if (m_Loading)
                return;

            // Display correct Picture
            string m_SPrefix = "";
            string m_Body = "";
            string m_PostFix = "";

            // Get Prefix
            switch (StationType)
            {
                case 0:
                    m_SPrefix = "t";
                    break;
                case 1:
                    m_SPrefix = "j";
                    break;
                case 2:
                    m_SPrefix = "p";
                    break;
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                    m_SPrefix = "n";
                    break;
            }

            // Body
            switch (RoomIndex)
            {
                case 0:
                    m_Body = "sd01a";
                    break;
                case 1:
                    m_Body = "bar01_h";
                    break;
                case 2:
                    m_Body = "bar01_v";
                    break;
                case 3:
                    m_Body = "bar01";
                    break;
            }

            // This is for Non-Faction Stations only
            if (StationType >= 3 && RoomIndex == 0)
            {
                m_Body = "sd07a";
                m_SPrefix = "t";
            }

            // PostFix
            m_PostFix = SmallRoom ? "2" : "";
            m_PostFix = AltRoom ? "3" : m_PostFix;

            // Show Picture
            try
            {
                StationPicture.Image = Image.FromFile(Application.StartupPath + "/Stations/" + m_SPrefix + m_Body + m_PostFix + ".jpg");
            }
            catch
            {
                MessageBox.Show("Can't open station file \"" + m_SPrefix + m_Body + m_PostFix + "\"", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
        }

        #region   ------------------------- Load In Data to GUI -------------------------
        // Load Station Data on the current Station
        private void LoadStationData()
        {
            string m_StationQuery = "SELECT `starbases`.`starbase_id`,`starbases`.`sector_id`,`starbases`.`name`,`starbases`.`type`,`starbases`.`is_active`,`starbases`.`description`,`starbases`.`welcome_message`,`starbases`.`target_sector_object`,`starbases`.`faction_id` FROM `starbases` WHERE `starbases`.`name` = \"" + m_CurrentStationName + "\";";
            // Connect to Database
            MySqlConnection m_Connection = new MySqlConnection(SQLData.ConnStr(m_DataBase));
            MySqlCommand m_Command = new MySqlCommand(m_StationQuery, m_Connection);

            m_Loading = true;               // Set Loading Flag

            m_Connection.Open();

            // Read Data
            MySqlDataReader m_Reader = m_Command.ExecuteReader();

            // Read in first line
            m_Reader.Read();

            if (m_Reader.HasRows)
            {
                // Get all the data from SQL
                StationName.Text = m_Reader.GetString(2);
                StationID.Text = m_Reader.GetInt32(0).ToString();
                StationSectorID.Text = m_Reader.GetInt32(1).ToString();
                StationObjectID.Text = m_Reader.GetInt32(7).ToString();
                StationDisc.Text = m_Reader.GetString(5);
                StationWelcome.Text = m_Reader.GetString(6);                // Welcome message

                StationFaction.SelectedIndex = m_Reader.GetInt32(8) - 1;    // Load factions
                StationType.SelectedIndex = m_Reader.GetInt32(3);           // Load in station type
                StationActive.SelectedIndex = m_Reader.GetInt32(4);         // IsActive

                m_CurrentStationID = Int32.Parse(StationID.Text);           // Resave this info (due to a bug)
            }
            else
            {
                MessageBox.Show("Could not load station", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            m_Connection.Close();
            m_Loading = false;               // Unset Loading Flag
            // Reset Changed
            m_HasChanged = false;
        }

        // Load terminal Information
        private void LoadTerminalData()
        {
            // Get Terminal ID from Tree
            Int32 m_TerminalID = System.Convert.ToInt32(StationList.SelectedNode.Name, 10);

            // Query String
            string m_TerminalQuery = "SELECT `location`,`type`,`attribute`,`description` FROM `starbase_terminals` WHERE `terminal_id` = " + m_TerminalID;

            // Connect to Database
            MySqlConnection m_Connection = new MySqlConnection(SQLData.ConnStr(m_DataBase));
            MySqlCommand m_Command = new MySqlCommand(m_TerminalQuery, m_Connection);

            m_Loading = true;               // Set Loading Flag
            m_Connection.Open();

            // Read Data
            MySqlDataReader m_Reader = m_Command.ExecuteReader();

            // Read in first line
            m_Reader.Read();

            if (m_Reader.HasRows)
            {
                // Get all the data from SQL
                TerminalAttribute.Text = m_Reader.GetString(2);
                TerminalDisc.Text = m_Reader.GetString(3);
                // Terminal Location List
                TerminalLocation.SelectedIndex = m_Reader.GetInt32(0);
                // Set up terminal type list
                TerminalType.SelectedIndex = m_Reader.GetInt32(1);
            }

            m_Connection.Close();
            m_Loading = false;               // Unset Loading Flag
            // Reset Changed
            m_HasChanged = false;
        }

        // Load Room Information
        private void LoadRoomData()
        {
            // Get Terminal ID from Tree
            Int32 m_RoomID = Int32.Parse(StationList.SelectedNode.Name);

            // Query String
            string m_TerminalQuery = "SELECT `type`,`style`,`fog_near`,`fog_far`,`description`,`fog_red`,`fog_green`,`fog_blue` FROM `starbase_rooms` WHERE `room_id` = " + m_RoomID;

            // Connect to Database
            MySqlConnection m_Connection = new MySqlConnection(SQLData.ConnStr(m_DataBase));
            MySqlCommand m_Command = new MySqlCommand(m_TerminalQuery, m_Connection);

            m_Loading = true;               // Set Loading Flag
            m_Connection.Open();

            // Read Data
            MySqlDataReader m_Reader = m_Command.ExecuteReader();

            // Read in first line
            m_Reader.Read();

            if (m_Reader.HasRows)
            {
                int FRed, FGreen, FBlue;

                // Get all the data from SQL
                RoomDiscription.Text = m_Reader.GetString(4);
                RoomFogNear.Text = m_Reader.GetString(2);
                RoomFogFar.Text = m_Reader.GetString(3);
                RoomType.SelectedIndex = m_Reader.GetInt32(0);


                try
                {
                    FRed = m_Reader.GetInt32(5);    // Red
                    FGreen = m_Reader.GetInt32(6);    // Green
                    FBlue = m_Reader.GetInt32(7);    // Blue
                }
                catch
                {
                    // They cant be read if they are null
                    FRed = 0;
                    FGreen = 0;
                    FBlue = 0;
                }

                FogColorBox.BackColor = System.Drawing.Color.FromArgb(FRed, FGreen, FBlue);

                UInt32 m_RStyle = m_Reader.GetUInt32(1);
                // Parse Room Checks
                RoomDeviders.Checked = (m_RStyle & 0x001) > 0 ? true : false;
                RoomRafters.Checked = (m_RStyle & 0x002) > 0 ? true : false;
                RoomBar.Checked = (m_RStyle & 0x004) > 0 ? true : false;
                RoomTables.Checked = (m_RStyle & 0x008) > 0 ? true : false;
                RoomMonitors.Checked = (m_RStyle & 0x010) > 0 ? true : false;
                RoomEyecandy.Checked = (m_RStyle & 0x020) > 0 ? true : false;
                RoomSmallroom.Checked = (m_RStyle & 0x040) > 0 ? true : false;
                RoomAltroom.Checked = (m_RStyle & 0x080) > 0 ? true : false;
                RoomFog.Checked = (m_RStyle & 0x100) > 0 ? true : false;
            }
            // Display Station
            DisplayStation(m_CurrentStationType, m_Reader.GetInt32(0), RoomSmallroom.Checked, RoomAltroom.Checked);

            m_Connection.Close();
            m_Loading = false;               // Unset Loading Flag

            // Reset Changed
            m_HasChanged = false;
        }

        // Load Room Information
        private void LoadNPCData()
        {
            // Get Terminal ID from Tree
            Int32 m_NpcID = System.Convert.ToInt32(StationList.SelectedNode.Name, 10);

            // Query String
            string m_TerminalQuery = "SELECT `description`,`faction_id`,`location`,`last_name`,`first_name`,`level`,`booth_type`,`npc_Id`,`groupid`,`talk_tree_handle` FROM `starbase_npcs` Inner Join `starbase_vendors` ON `starbase_npcs`.`npc_Id` = `starbase_vendors`.`vendor_id` WHERE `npc_Id` = " + m_NpcID;
            string m_NPCTemplateQuery = "SELECT `avatar_version` FROM `starbase_npc_avatar_templates` WHERE `avatar_template_id` =  '" + m_NpcID + "'";

            // Connect to Database
            MySqlConnection m_Connection = new MySqlConnection(SQLData.ConnStr(m_DataBase));
            MySqlCommand m_Command = new MySqlCommand(m_TerminalQuery, m_Connection);
            m_Loading = true;               // Set Loading Flag
            m_Connection.Open();

            // Read Data
            MySqlDataReader m_Reader = m_Command.ExecuteReader();

            // Read in first line
            m_Reader.Read();

            if (m_Reader.HasRows)
            {
                // Get all the data from SQL
                NPCDiscription.Text = m_Reader.GetString(0);
                NPCFirstName.Text = m_Reader.GetString(4);
                NPCLastName.Text = m_Reader.GetString(3);
                NPCAvatarID.Text = m_Reader.GetString(7);
                // Set NPC Location
                NPCLocation.SelectedIndex = m_Reader.GetInt32(2);
                NPCFaction.SelectedIndex = m_Reader.GetInt32(1) - 1;
                // Set NPC Level
                NPCLevel.SelectedIndex = m_Reader.GetInt32(5);
                // Setup NPC Booth Type
                int BoothIndex = m_Reader.GetInt32(6);
                if (BoothIndex > 6)
                    BoothIndex = 6;
                NPCBoothType.SelectedIndex = BoothIndex + 1;

                MySqlDataReader NPCAvatar = ExcuteSQLQuery(m_NPCTemplateQuery);

                if (NPCAvatar.HasRows)
                {
                    NPCAvatar.Read();
                    if (NPCAvatar.IsDBNull(0))
                    {
                        AddAvatar.Text = "Create Avatar";
                        AddAvatar.ForeColor = System.Drawing.Color.Red;
                    }
                    else
                    {
                        AddAvatar.Text = "Change Avatar";
                        AddAvatar.ForeColor = System.Drawing.Color.Green;
                    }
                }
                else
                {
                    AddAvatar.Text = "Create Avatar";
                    AddAvatar.ForeColor = System.Drawing.Color.Red;
                }

                // Select the vender group
                int index = 0;
                foreach (int vgroup in m_VenderGroups)
                {
                    if (vgroup == m_Reader.GetInt32(8))       // Read in vender ID
                    {
                        VenderGroupBox.SelectedIndex = index;
                    }
                    index++;
                }
                // Save Talk Tree
                m_TalkTreeData = m_Reader.GetString(9);
            }
            m_Connection.Close();
            m_Loading = false;               // Set Loading Flag
            // Reset Changed
            m_HasChanged = false;
        }

        // Load Station into Tree (Everyting)
        private void LoadStationTree(string m_StationName)
        {
            string m_RoomQuery = "SELECT `starbase_rooms`.`room_id`, `starbase_rooms`.`starbase_id`, `starbases`.`type` FROM `starbases` Inner Join `starbase_rooms` ON `starbases`.`starbase_id` = `starbase_rooms`.`starbase_id` WHERE `starbases`.`name` =  \"" + m_StationName + "\"";
            string m_TerminalQuery;

            MySqlConnection m_Connection = new MySqlConnection(SQLData.ConnStr(m_DataBase));
            MySqlCommand m_Command = new MySqlCommand(m_RoomQuery, m_Connection);
            m_Loading = true;               // Set Loading Flag
            m_Connection.Open();


            // Unselect any item
            SelectedNodeTree = null;
            // Remove all Stations
            StationList.Nodes.Clear();
            // Add Station to List
            TreeNode StationTree = new TreeNode();
            StationTree.Text = m_StationName;
            StationTree.Name = "StationNode";
            StationTree.ImageIndex = (int)IconType.Station_Icon;
            StationTree.SelectedImageIndex = (int)IconType.Station_Icon;
            StationList.Nodes.Add(StationTree);

            MySqlDataReader m_Reader = m_Command.ExecuteReader();

            m_Reader.Read();

            if (m_Reader.HasRows)
            {
                // Station Type
                m_CurrentStationType = m_Reader.GetInt32(2);

                // Set Selected StationID
                m_CurrentStationID = m_Reader.GetInt32(1);
                // Save Station Name
                m_CurrentStationName = m_StationName;

                // Read in all the rooms for this Station
                int m_RIndex = 0;
                do
                {
                    int m_RoomID = m_Reader.GetInt32(0);    // RoomID

                    // Add Room
                    TreeNode RoomTree = new TreeNode();
                    RoomTree.Text = "Room " + m_RIndex;
                    RoomTree.Name = m_Reader.GetInt32(0).ToString();      // Save Room ID
                    RoomTree.ImageIndex = (int)IconType.Room_Icon;
                    RoomTree.SelectedImageIndex = (int)IconType.Room_Icon;
                    m_RIndex++;

                    // Set up Terminals Nodes
                    TreeNode TermNode = new TreeNode();
                    TermNode.Text = "Terminals";
                    TermNode.Name = "TerminalNode";
                    TermNode.ImageIndex = (int)IconType.Folder_Icon;
                    TermNode.SelectedImageIndex = (int)IconType.Folder_Icon;
                    // Add Terminals
                    RoomTree.Nodes.Add(TermNode);

                    // List the terminals
                    MySqlConnection m_TermConnection = new MySqlConnection(SQLData.ConnStr(m_DataBase));
                    m_TerminalQuery = "SELECT `starbase_terminals`.`terminal_id`,`starbase_terminals`.`location`,`starbase_terminals`.`type`,`starbase_terminals`.`attribute`,`starbase_terminals`.`description`,`starbase_terminals`.`room_id`,`starbase_terminals`.`terminal_index` FROM `starbase_terminals` WHERE `starbase_terminals`.`room_id` = " + m_RoomID;
                    MySqlCommand m_TermCommand = new MySqlCommand(m_TerminalQuery, m_TermConnection);
                    m_TermConnection.Open();
                    MySqlDataReader m_TermReader = m_TermCommand.ExecuteReader();
                    int m_TermID = 0;

                    while (m_TermReader.Read())
                    {
                        // Set up Terminals Nodes
                        TreeNode TermLeaf = new TreeNode();
                        TermLeaf.Text = "Terminals " + m_TermID;
                        TermLeaf.Name = m_TermReader.GetInt32(0).ToString();      // Save Terminal ID
                        TermLeaf.ImageIndex = (int)IconType.Terminal_Icon;
                        TermLeaf.SelectedImageIndex = (int)IconType.Terminal_Icon;
                        m_TermID++;

                        // Add Terminals
                        TermNode.Nodes.Add(TermLeaf);
                    }

                    m_TermConnection.Close();

                    // Set up NPC Nodes
                    TreeNode NPCNode = new TreeNode();
                    NPCNode.Text = "NPC's";
                    NPCNode.Name = "NPCNode";
                    NPCNode.ImageIndex = (int)IconType.Folder_Icon;
                    NPCNode.SelectedImageIndex = (int)IconType.Folder_Icon;
                    // Add Terminals
                    RoomTree.Nodes.Add(NPCNode);

                    // Add All Nodes as a brach off a Room
                    StationTree.Nodes.Add(RoomTree);

                    // List the NPCs
                    MySqlConnection m_NpcConnection = new MySqlConnection(SQLData.ConnStr(m_DataBase));
                    string m_NpcQuery = "SELECT `starbase_npcs`.`npc_Id`,`starbase_npcs`.`first_name`,`starbase_npcs`.`last_name`,`starbase_npcs`.`location`,`starbase_npcs`.`faction_id`,`starbase_npcs`.`description`,`starbase_npcs`.`talk_tree_handle`,`starbase_npcs`.`room_id`,`starbase_npcs`.`npc_index`,`starbase_vendors`.`vendor_id`,`starbase_vendors`.`level`,`starbase_vendors`.`booth_type` FROM `starbase_npcs` Inner Join `starbase_vendors` ON `starbase_npcs`.`npc_Id` = `starbase_vendors`.`vendor_id` WHERE `starbase_npcs`.`room_id` = " + m_RoomID;
                    MySqlCommand m_NpcCommand = new MySqlCommand(m_NpcQuery, m_NpcConnection);
                    m_NpcConnection.Open();
                    MySqlDataReader m_NpcReader = m_NpcCommand.ExecuteReader();

                    while (m_NpcReader.Read())
                    {
                        // Set up NPC Nodes
                        TreeNode NpcLeaf = new TreeNode();
                        NpcLeaf.Text = m_NpcReader.GetString(1) + " " + m_NpcReader.GetString(2);
                        NpcLeaf.Name = m_NpcReader.GetInt32(0).ToString();      // Save NPC ID
                        NpcLeaf.ImageIndex = (int)IconType.NPC_Icon;
                        NpcLeaf.SelectedImageIndex = (int)IconType.NPC_Icon;
                        m_TermID++;

                        // Add Terminals
                        NPCNode.Nodes.Add(NpcLeaf);
                    }
                    m_NpcConnection.Close();
                } while (m_Reader.Read());
            }
            m_Loading = false;               // Unset Loading Flag
        }
        #endregion

        private void Main_Resize(object sender, EventArgs e)
        {
            // Update the height for Bottom Tab
            tabControl2.Height = (this.Height - oldFormData.Height) + tabControl2.Height;

            // Update Picture Data
            StationPicture.Height = StationPicture.Height + (this.Height - oldFormData.Height);
            StationPicture.Width = StationPicture.Width + (this.Width - oldFormData.Width);

            // Store new values
            oldFormData.Width = this.Width;
            oldFormData.Height = this.Height;
        }

        #region ------------------ Has Changed Functions ---------------------------

        private void TerminalTab_Change(object sender, EventArgs e)
        {
            m_HasChanged = true;
        }

        private void StationTab_TextChanged(object sender, EventArgs e)
        {
            m_HasChanged = true;
        }

        private void NPCTab_TextChanged(object sender, EventArgs e)
        {
            m_HasChanged = true;
        }

        private void RoomTab_Change(object sender, EventArgs e)
        {
            m_HasChanged = true;
        }

        private void RoomStyle_Change(object sender, EventArgs e)
        {
            // Display Station
            DisplayStation(m_CurrentStationType, RoomType.SelectedIndex, RoomSmallroom.Checked, RoomAltroom.Checked);

            m_HasChanged = true;
        }

        #endregion

        private void Select_Color(object sender, EventArgs e)
        {
            ColorBox.Color = FogColorBox.BackColor;
            ColorBox.ShowDialog();
            FogColorBox.BackColor = System.Drawing.Color.FromArgb(ColorBox.Color.ToArgb());
            m_HasChanged = true;
        }

        private void EditTalkTree_Click(object sender, EventArgs e)
        {
            FrmTalkTree frmTalkTree = new FrmTalkTree();

            frmTalkTree.setConversation(m_TalkTreeData);
            frmTalkTree.ShowDialog();

            m_TalkTreeData = frmTalkTree.getConversation();
            m_HasChanged = true;
        }

        private void AddAvatar_Click(object sender, EventArgs e)
        {
            OpenFileDialog OpenAvatar = new OpenFileDialog();

            OpenAvatar.InitialDirectory = "C:\\Documents and Settings\\All Users\\Application Data\\Westwood Studios\\Earth and Beyond\\Character and Starship Creator";
            OpenAvatar.Filter = "Avatar Files (*.dat)|*.dat";
            OpenAvatar.Title = "Avatar File";
            OpenAvatar.FileName = "";

            if (OpenAvatar.ShowDialog() == DialogResult.OK)
            {
                Stream AFile = OpenAvatar.OpenFile();

                LoadAvatar AvatarLoad = new LoadAvatar(AFile);

                string AddAvatarQuery = "UPDATE `starbase_npc_avatar_templates` SET ";

                AddAvatarQuery += "`avatar_template_id` = '" + SelectedNodeTree.Name + "', ";
                AddAvatarQuery += "`avatar_type` = '" + AvatarLoad.avatarType + "', ";
                AddAvatarQuery += "`avatar_version` = '" + AvatarLoad.avaterVersion + "', ";
                AddAvatarQuery += "`race` = '" + AvatarLoad.race + "', ";
                AddAvatarQuery += "`profession` = '" + AvatarLoad.profession + "', ";
                AddAvatarQuery += "`gender` = '" + AvatarLoad.gender + "', ";
                AddAvatarQuery += "`mood_type` = '" + AvatarLoad.moodType + "', ";
                AddAvatarQuery += "`personality` = '" + AvatarLoad.personality + "', ";
                AddAvatarQuery += "`nlp` = '" + AvatarLoad.nlp + "', ";
                AddAvatarQuery += "`shirt_type` = '" + AvatarLoad.bodyType + "', ";
                AddAvatarQuery += "`pants_type` = '" + AvatarLoad.pantsType + "', ";
                AddAvatarQuery += "`head_type` = '" + AvatarLoad.headType + "', ";
                AddAvatarQuery += "`hair_type` = '" + AvatarLoad.hairNum + "', ";
                AddAvatarQuery += "`ear_type` = '" + AvatarLoad.earNum + "', ";
                AddAvatarQuery += "`goggle_num` = '" + AvatarLoad.goggleNum + "', ";
                AddAvatarQuery += "`beard_num` = '" + AvatarLoad.beardNum + "', ";
                AddAvatarQuery += "`weapon_hip_num` = '" + AvatarLoad.weaponHipNum + "', ";
                AddAvatarQuery += "`weapon_unique_num` = '" + AvatarLoad.weaponUniqueNum + "', ";
                AddAvatarQuery += "`weapon_back_num` = '" + AvatarLoad.weaponBackNum + "', ";
                AddAvatarQuery += "`head_texture_num` = '" + AvatarLoad.headTextureNum + "', ";
                AddAvatarQuery += "`tatoo_texture_num` = '" + AvatarLoad.tattooTextureNum + "', ";
                AddAvatarQuery += "`tatoo_x` = '" + AvatarLoad.tattooOffset[0] + "', ";
                AddAvatarQuery += "`tatoo_y` = '" + AvatarLoad.tattooOffset[1] + "', ";
                AddAvatarQuery += "`tatoo_z` = '" + AvatarLoad.tattooOffset[2] + "', ";
                AddAvatarQuery += "`hair_color_0` = '" + AvatarLoad.hairColor[0] + "', ";
                AddAvatarQuery += "`hair_color_1` = '" + AvatarLoad.hairColor[1] + "', ";
                AddAvatarQuery += "`hair_color_2` = '" + AvatarLoad.hairColor[2] + "', ";
                AddAvatarQuery += "`beard_color_0` = '" + AvatarLoad.beardColor[0] + "', ";
                AddAvatarQuery += "`beard_color_1` = '" + AvatarLoad.beardColor[1] + "', ";
                AddAvatarQuery += "`beard_color_2` = '" + AvatarLoad.beardColor[2] + "', ";
                AddAvatarQuery += "`eye_color_0` = '" + AvatarLoad.eyeColor[0] + "', ";
                AddAvatarQuery += "`eye_color_1` = '" + AvatarLoad.eyeColor[1] + "', ";
                AddAvatarQuery += "`eye_color_2` = '" + AvatarLoad.eyeColor[2] + "', ";
                AddAvatarQuery += "`skin_color_0` = '" + AvatarLoad.skinColor[0] + "', ";
                AddAvatarQuery += "`skin_color_1` = '" + AvatarLoad.skinColor[1] + "', ";
                AddAvatarQuery += "`skin_color_2` = '" + AvatarLoad.skinColor[2] + "', ";
                AddAvatarQuery += "`shirt1_color_0` = '" + AvatarLoad.shirtPrimaryColor[0] + "', ";
                AddAvatarQuery += "`shirt1_color_1` = '" + AvatarLoad.shirtPrimaryColor[1] + "', ";
                AddAvatarQuery += "`shirt1_color_2` = '" + AvatarLoad.shirtPrimaryColor[2] + "', ";
                AddAvatarQuery += "`shirt2_color_0` = '" + AvatarLoad.shirtSecondaryColor[0] + "', ";
                AddAvatarQuery += "`shirt2_color_1` = '" + AvatarLoad.shirtSecondaryColor[1] + "', ";
                AddAvatarQuery += "`shirt2_color_2` = '" + AvatarLoad.shirtSecondaryColor[2] + "', ";
                AddAvatarQuery += "`pants1_color_0` = '" + AvatarLoad.pantsPrimaryColor[0] + "', ";
                AddAvatarQuery += "`pants1_color_1` = '" + AvatarLoad.pantsPrimaryColor[1] + "', ";
                AddAvatarQuery += "`pants1_color_2` = '" + AvatarLoad.pantsPrimaryColor[2] + "', ";
                AddAvatarQuery += "`pants2_color_0` = '" + AvatarLoad.pantsSecondaryColor[0] + "', ";
                AddAvatarQuery += "`pants2_color_1` = '" + AvatarLoad.pantsSecondaryColor[1] + "', ";
                AddAvatarQuery += "`pants2_color_2` = '" + AvatarLoad.pantsSecondaryColor[2] + "', ";
                AddAvatarQuery += "`shirt1_metal` = '" + AvatarLoad.shirtPrimaryMetal + "', ";
                AddAvatarQuery += "`shirt2_metal` = '" + AvatarLoad.shirtSecondarymetal + "', ";
                AddAvatarQuery += "`pants1_metal` = '" + AvatarLoad.pantsPrimaryMetal + "', ";
                AddAvatarQuery += "`pants2_metal` = '" + AvatarLoad.pantsSecondaryMetal + "', ";
                AddAvatarQuery += "`body_weight0` = '" + AvatarLoad.bodyWeight[0] + "', ";
                AddAvatarQuery += "`body_weight1` = '" + AvatarLoad.bodyWeight[1] + "', ";
                AddAvatarQuery += "`body_weight2` = '" + AvatarLoad.bodyWeight[2] + "', ";
                AddAvatarQuery += "`body_weight3` = '" + AvatarLoad.bodyWeight[3] + "', ";
                AddAvatarQuery += "`body_weight4` = '" + AvatarLoad.bodyWeight[4] + "', ";
                AddAvatarQuery += "`head_weight0` = '" + AvatarLoad.headWeight[0] + "', ";
                AddAvatarQuery += "`head_weight1` = '" + AvatarLoad.headWeight[1] + "', ";
                AddAvatarQuery += "`head_weight2` = '" + AvatarLoad.headWeight[2] + "', ";
                AddAvatarQuery += "`head_weight3` = '" + AvatarLoad.headWeight[3] + "', ";
                AddAvatarQuery += "`head_weight4` = '" + AvatarLoad.headWeight[4] + "'";

                AddAvatarQuery += " WHERE `avatar_template_id` = '" + SelectedNodeTree.Name + "'";

                ExecuteSQL(AddAvatarQuery);     // Add new avatar


                // Change button text
                AddAvatar.Text = "Change Avatar";
                AddAvatar.ForeColor = System.Drawing.Color.Green;
            }
        }

        private void FindObject_Click(object sender, EventArgs e)
        {
            FindObject Obj = new FindObject();

            Obj.ShowDialog();

            // if they clicked ok
            if (Obj.GetOk())
            {
                StationObjectID.Text = Obj.GetStationID().ToString();
                StationSectorID.Text = Obj.GetSectorID().ToString();
            }
        }

        #region ----------- Drag Items --------------------------
        private void StationList_DragDrop(object sender, DragEventArgs e)
        {
            // Get the tree.
            TreeView tree = (TreeView)sender;
            TreeNode nodeSource = (TreeNode)e.Data.GetData(typeof(TreeNode));

            // Drag and drop denied by default.
            e.Effect = DragDropEffects.Move;

            // Is it a valid format?
            if (e.Data.GetData(typeof(TreeNode)) != null)
            {
                // Get the screen point.
                Point pt = new Point(e.X, e.Y);

                // Convert to a point in the TreeView's coordinate system.
                pt = tree.PointToClient(pt);

                // Is the mouse over a valid node?
                TreeNode node = tree.GetNodeAt(pt);

                // Make sure we are ok to drop this
                if (nodeSource.Parent.Name == node.Name && nodeSource.Parent.Name == "NPCNode")
                {
                    if (MessageBox.Show("Are you sure you want to move \"" + nodeSource.Text + "\" to \"" + node.Parent.Text + "\"?", "Conformation", MessageBoxButtons.YesNo, MessageBoxIcon.Information) == DialogResult.No)
                    {
                        // Exit if they do not conform
                        return;
                    }

                    string NPCRoomMove = "UPDATE `starbase_npcs` SET `room_id` = " + node.Parent.Name + " WHERE `npc_Id` = '" + nodeSource.Name + "';";

                    // Update NPC room
                    ExecuteSQL(NPCRoomMove);

                    // Copy new data
                    TreeNode NPCLeaf = new TreeNode();
                    NPCLeaf.Text = nodeSource.Text;
                    NPCLeaf.Name = nodeSource.Name;
                    NPCLeaf.ImageIndex = (int)IconType.NPC_Icon;
                    NPCLeaf.SelectedImageIndex = (int)IconType.NPC_Icon;
                    node.Nodes.Add(NPCLeaf);

                    // Delete old Data
                    nodeSource.Nodes.Clear();           // Remove sub nodes
                    nodeSource.Remove();                // Remove ourself
                }
            }
        }

        private void StationList_DragEnter(object sender, DragEventArgs e)
        {
            e.Effect = DragDropEffects.Move;
        }

        private void StationList_ItemDrag(object sender, ItemDragEventArgs e)
        {
            // Get the tree.
            TreeView tree = (TreeView)sender;

            if (e.Item.GetType().ToString() == "System.Windows.Forms.TreeNode")
            {

                TreeNode node = (TreeNode)e.Item;
                tree.SelectedNode = node;

                // Make sure we are not Null & we are a NPC
                if (node != null && node.Parent.Name == "NPCNode")
                {
                    tree.DoDragDrop(node, DragDropEffects.Move);
                }
                else
                {
                    tree.DoDragDrop(node, DragDropEffects.None);
                }
            }
            else
            {
                tree.DoDragDrop(e.Item.ToString(), DragDropEffects.None);
            }
        }

        private void StationList_DragOver(object sender, DragEventArgs e)
        {
            TreeView tree = (TreeView)sender;
            TreeNode node = (TreeNode)e.Data.GetData(typeof(TreeNode));

            // Check node to see if we can move to there
            if (node != null)
            {
                // Get the screen point.
                Point pt = new Point(e.X, e.Y);
                // Convert to a point in the TreeView's coordinate system.
                pt = tree.PointToClient(pt);
                // Is the mouse over a valid node?
                TreeNode MoveToNode = tree.GetNodeAt(pt);

                // If its a NPC Node we are dragging make sure we are droping to NPC Node
                if (node.Parent.Name == "NPCNode")
                {
                    // See if we are on a NPC Node to drop in
                    if (MoveToNode.Name == "NPCNode")
                    {
                        e.Effect = DragDropEffects.Move;
                    }
                    else
                    {
                        e.Effect = DragDropEffects.None;
                    }
                }


            }

        }
        #endregion

        #endregion


    }
}