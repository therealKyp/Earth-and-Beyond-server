using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

using N7.Sql;
using N7.GUI;
using N7.Utilities;
using N7.Props;

namespace N7
{
    public partial class mainFrm : Form
    {
        private MobsSQL mobs;
        private FactionSql factions;
        public static BaseAssetSQL baseAssets;
        public static MobItemsSQL mobItemsSql;
        public static ItemBaseSQL itemBase;
        private DataRow selectedRow;
        private DataGridViewRow selectedDataGridRow;
        public static DataRow newMobItem;
        private ListViewGroup equippedAmmoGroup;
        private ListViewGroup equippedDevicesGroup;
        private ListViewGroup equippedEnginesGroup;
        private ListViewGroup equippedReactorsGroup;
        private ListViewGroup equippedShieldsGroup;
        private ListViewGroup equippedMissileGroup;
        private ListViewGroup equippedProjectileGroup;
        private ListViewGroup equippedBeamGroup;
        private ListViewGroup inventoryAmmoGroup;
        private ListViewGroup inventoryDevicesGroup;
        private ListViewGroup inventoryEnginesGroup;
        private ListViewGroup inventoryReactorsGroup;
        private ListViewGroup inventoryShieldsGroup;
        private ListViewGroup inventoryMissileGroup;
        private ListViewGroup inventoryProjectileGroup;
        private ListViewGroup inventoryBeamGroup;
        private ListViewGroup inventoryComponentsGroup;
        private ListViewGroup inventoryOreGroup;
        private ListViewGroup inventoryMiscGroup;

        public mainFrm()
        {
            InitializeComponent();
        }

        private void mainFrm_Load(object sender, EventArgs e)
        {
            loadDataSources();
            setupTable();
            populateCombos();
        }

        #region Class Methods
        private void loadDataSources()
        {
            mobs = new MobsSQL();
            mobItemsSql = new MobItemsSQL();
            factions = new FactionSql();
            baseAssets = new BaseAssetSQL();
            itemBase = new ItemBaseSQL();
        }

        private void setupTable()
        {
            DataTable mt = mobs.getMobTable();
            object[] dataSource = new object[3];
            foreach (DataRow mr in mt.Rows)
            {
                int id = int.Parse(mr["mob_id"].ToString());
                String name = mr["name"].ToString();
                int level = int.Parse(mr["level"].ToString());

                dataSource[0] = id;
                dataSource[1] = name;
                dataSource[2] = level;

                dataGridView1.Rows.Add(dataSource);
            }
        }

        private void populateCombos()
        {
            //Populate Level Combo
            for (int i = 0; i < 67; i++)
            {
                levelCombo.Items.Add(i); 
            }

            //Populate Type combo
            typeCombo.Items.Add("Cybernetic");
            typeCombo.Items.Add("Structural");
            typeCombo.Items.Add("Organic_Red");
            typeCombo.Items.Add("Organic_Green");
            typeCombo.Items.Add("Crystalline");
            typeCombo.Items.Add("Energy");
            typeCombo.Items.Add("Rock Based");

            for (int i = -1; i < 67; i++)
            {
                comboBox1.Items.Add(i); 
            }

            //Populate Faction Combo
            factionCombo.Items.Add("Please Make A Selection");

            foreach (DataRow fr in factions.getFactionTable().Rows)
            {
                factionCombo.Items.Add(fr["name"].ToString());
            }
        }

        private void populateFields(int objectID)
        {
            equippedAmmoGroup = new ListViewGroup("Ammo", HorizontalAlignment.Left);
            equippedDevicesGroup = new ListViewGroup("Devices", HorizontalAlignment.Left);
            equippedEnginesGroup = new ListViewGroup("Engines", HorizontalAlignment.Left);
            equippedReactorsGroup = new ListViewGroup("Reactors", HorizontalAlignment.Left);
            equippedShieldsGroup = new ListViewGroup("Shields", HorizontalAlignment.Left);
            equippedMissileGroup = new ListViewGroup("Missile Weapon", HorizontalAlignment.Left);
            equippedProjectileGroup = new ListViewGroup("Projectile Weapon", HorizontalAlignment.Left);
            equippedBeamGroup = new ListViewGroup("Beam Weapon", HorizontalAlignment.Left);

            listView1.Groups.AddRange(new ListViewGroup[] { equippedAmmoGroup, equippedDevicesGroup, equippedEnginesGroup, 
                                        equippedReactorsGroup, equippedShieldsGroup, equippedMissileGroup, 
                                        equippedProjectileGroup, equippedBeamGroup});

            inventoryAmmoGroup = new ListViewGroup("Ammo", HorizontalAlignment.Left);
            inventoryDevicesGroup = new ListViewGroup("Devices", HorizontalAlignment.Left);
            inventoryEnginesGroup = new ListViewGroup("Engines", HorizontalAlignment.Left);
            inventoryReactorsGroup = new ListViewGroup("Reactors", HorizontalAlignment.Left);
            inventoryShieldsGroup = new ListViewGroup("Shields", HorizontalAlignment.Left);
            inventoryMissileGroup = new ListViewGroup("Missile Weapon", HorizontalAlignment.Left);
            inventoryProjectileGroup = new ListViewGroup("Projectile Weapon", HorizontalAlignment.Left);
            inventoryBeamGroup = new ListViewGroup("Beam Weapon", HorizontalAlignment.Left);
            inventoryComponentsGroup = new ListViewGroup("Components", HorizontalAlignment.Left);
            inventoryOreGroup = new ListViewGroup("Ore & Resources", HorizontalAlignment.Left);
            inventoryMiscGroup = new ListViewGroup("Misc. Loot", HorizontalAlignment.Left);

            listView2.Groups.AddRange(new ListViewGroup[] { inventoryAmmoGroup, inventoryDevicesGroup,
                    inventoryEnginesGroup, inventoryReactorsGroup, inventoryShieldsGroup,
                    inventoryMissileGroup, inventoryProjectileGroup, inventoryBeamGroup,
                    inventoryComponentsGroup, inventoryOreGroup, inventoryMiscGroup });

            selectedRow = mobs.getRowByID(objectID);

            //General Details.
            String name = selectedRow["name"].ToString();
            int level = int.Parse(selectedRow["level"].ToString());
            int type = int.Parse(selectedRow["type"].ToString());
            int intelligence = int.Parse(selectedRow["intelligence"].ToString());
            int bravery = int.Parse(selectedRow["bravery"].ToString());
            int factionID = int.Parse(selectedRow["faction_id"].ToString());
            int baseAssetID = int.Parse(selectedRow["base_asset_id"].ToString());
            int altruism = int.Parse(selectedRow["altruism"].ToString());
            int aggressiveness = int.Parse(selectedRow["aggressiveness"].ToString());
            float h = float.Parse(selectedRow["h"].ToString());
            float s = float.Parse(selectedRow["s"].ToString());
            float v = float.Parse(selectedRow["v"].ToString());
            float scale = float.Parse(selectedRow["scale"].ToString());
            String ai = selectedRow["ai"].ToString();

            nameText.Text = name;
            levelCombo.SelectedItem = level;
            baseAssetText.Text = baseAssetID.ToString();
            scaleText.Text = scale.ToString();
            aiText.Text = ai;

            AdobeColors.HSL hsl = new AdobeColors.HSL();
            hsl.H = h;
            hsl.S = s;
            hsl.L = v;
            cPicker1.Color = AdobeColors.HSL_to_RGB(hsl);

            switch (type)
            {
                case 0:
                    typeCombo.SelectedItem = "Cybernetic";
                    break;
                case 1:
                    typeCombo.SelectedItem = "Structural";
                    break;
                case 2:
                    typeCombo.SelectedItem = "Organic_Red";
                    break;
                case 3:
                    typeCombo.SelectedItem = "Organic_Green";
                    break;
                case 4:
                    typeCombo.SelectedItem = "Crystalline";
                    break;
                case 5:
                    typeCombo.SelectedItem = "Energy";
                    break;
                case 6:
                    typeCombo.SelectedItem = "Rock Based";
                    break;
            }

            String factionName = factions.findNameByID(factionID);
            if (factionName != "None")
            {
                factionCombo.SelectedItem = factionName;
            }
            else
            {
                factionCombo.SelectedItem = "Please Make A Selection";
            }

            //Equipped Items & Inventory
            DataRow[] mobItems = mobItemsSql.getRowsByID(objectID);

            listView1.Items.Clear();
            listView2.Items.Clear();

            for (int i = 0; i < mobItems.Length; i++)
            {
                int itemBaseID = int.Parse(mobItems[i]["item_base_id"].ToString());
                int itemType = int.Parse(mobItems[i]["type"].ToString());
                int itemUsage = int.Parse(mobItems[i]["usage_chance"].ToString());
                int itemDrop = int.Parse(mobItems[i]["drop_chance"].ToString());

                DataRow inDR = itemBase.getRowByID(itemBaseID);
                String itemName = inDR["name"].ToString();
                int itemCat = int.Parse(inDR["sub_category"].ToString());
                int asset_2d = int.Parse(inDR["2d_asset"].ToString());

                String imageFileName = baseAssets.getFileNameByID(asset_2d);

                if (itemType == 0)
                {
                    ListViewItem lvi1 = new ListViewItem(itemName, imageFileName);
                    lvi1.Tag = mobItems[i];
                    lvi1.ToolTipText = itemName;

                    switch (itemCat)
                    {
                        case 100:
                            lvi1.Group = equippedBeamGroup;
                            break;
                        case 101:
                            lvi1.Group = equippedProjectileGroup;
                            break;
                        case 102:
                            lvi1.Group = equippedMissileGroup;
                            break;
                        case 103:
                            lvi1.Group = equippedAmmoGroup;
                            break;
                        case 110:
                            lvi1.Group = equippedDevicesGroup;
                            break;
                        case 120:
                            lvi1.Group = equippedReactorsGroup;
                            break;
                        case 121:
                            lvi1.Group = equippedEnginesGroup;
                            break;
                        case 122:
                            lvi1.Group = equippedShieldsGroup;
                            break;
                    }
                    
                    listView1.Items.Add(lvi1);
                }
                else
                {
                    ListViewItem lvi2 = new ListViewItem(itemName, imageFileName);
                    lvi2.ToolTipText = itemName;
                    lvi2.Tag = mobItems[i];

                    switch (itemCat)
                    {
                        case -1:
                            lvi2.Group = inventoryMiscGroup;
                            break;
                        case 100:
                            lvi2.Group = inventoryBeamGroup;
                            break;
                        case 101:
                            lvi2.Group = inventoryProjectileGroup;
                            break;
                        case 102:
                            lvi2.Group = inventoryMissileGroup;
                            break;
                        case 103:
                            lvi2.Group = inventoryAmmoGroup;
                            break;
                        case 110:
                            lvi2.Group = inventoryDevicesGroup;
                            break;
                        case 120:
                            lvi2.Group = inventoryReactorsGroup;
                            break;
                        case 121:
                            lvi2.Group = inventoryEnginesGroup;
                            break;
                        case 122:
                            lvi2.Group = inventoryShieldsGroup;
                            break;
                        case 500:
                            lvi2.Group = inventoryComponentsGroup;
                            break;
                        case 800:
                            lvi2.Group = inventoryOreGroup;
                            break;
                    }

                    listView2.Items.Add(lvi2);
                }
            }

            //Skills

            setupMainImage(baseAssetID);
        }

        public static void setupMainImage(int baID)
        {
            pictureBox1.Refresh();
            pictureBox1.Update();

            String test = Application.StartupPath;
            if (baID != -1)
            {
                String imageFileName = baseAssets.getFileNameByID(baID);
                try
                {
                    String imagePath = test+"\\images\\" + imageFileName;
                    pictureBox1.Image = Image.FromFile(imagePath);
                }
                catch (Exception)
                {
                    String imagePath = test + "\\images\\naPic.JPG";
                    pictureBox1.Image = Image.FromFile(imagePath);
                }
            }
            else
            {
                String imagePath = test + "\\images\\naPic.JPG";
                pictureBox1.Image = Image.FromFile(imagePath);
            }
        }

        private void setupNewItem()
        {
            int itemBaseID = int.Parse(newMobItem["item_base_id"].ToString());
            int itemType = int.Parse(newMobItem["type"].ToString());
                int itemUsage = int.Parse(newMobItem["usage_chance"].ToString());
                int itemDrop = int.Parse(newMobItem["drop_chance"].ToString());

                DataRow inDR = itemBase.getRowByID(itemBaseID);
                String itemName = inDR["name"].ToString();
                int itemCat = int.Parse(inDR["sub_category"].ToString());
                int asset_2d = int.Parse(inDR["2d_asset"].ToString());

                String imageFileName = baseAssets.getFileNameByID(asset_2d);

                if (itemType == 0)
                {
                    ListViewItem lvi1 = new ListViewItem(itemName, imageFileName);
                    lvi1.Tag = newMobItem;
                    lvi1.ToolTipText = itemName;

                    switch (itemCat)
                    {
                        case 100:
                            lvi1.Group = equippedBeamGroup;
                            break;
                        case 101:
                            lvi1.Group = equippedProjectileGroup;
                            break;
                        case 102:
                            lvi1.Group = equippedMissileGroup;
                            break;
                        case 103:
                            lvi1.Group = equippedAmmoGroup;
                            break;
                        case 110:
                            lvi1.Group = equippedDevicesGroup;
                            break;
                        case 120:
                            lvi1.Group = equippedReactorsGroup;
                            break;
                        case 121:
                            lvi1.Group = equippedEnginesGroup;
                            break;
                        case 122:
                            lvi1.Group = equippedShieldsGroup;
                            break;
                    }

                    listView1.Items.Add(lvi1);
                }
                else
                {
                    ListViewItem lvi2 = new ListViewItem(itemName, imageFileName);
                    lvi2.ToolTipText = itemName;
                    lvi2.Tag = newMobItem;

                    switch (itemCat)
                    {
                        case -1:
                            lvi2.Group = inventoryMiscGroup;
                            break;
                        case 100:
                            lvi2.Group = inventoryBeamGroup;
                            break;
                        case 101:
                            lvi2.Group = inventoryProjectileGroup;
                            break;
                        case 102:
                            lvi2.Group = inventoryMissileGroup;
                            break;
                        case 103:
                            lvi2.Group = inventoryAmmoGroup;
                            break;
                        case 110:
                            lvi2.Group = inventoryDevicesGroup;
                            break;
                        case 120:
                            lvi2.Group = inventoryReactorsGroup;
                            break;
                        case 121:
                            lvi2.Group = inventoryEnginesGroup;
                            break;
                        case 122:
                            lvi2.Group = inventoryShieldsGroup;
                            break;
                        case 500:
                            lvi2.Group = inventoryComponentsGroup;
                            break;
                        case 800:
                            lvi2.Group = inventoryOreGroup;
                            break;
                    }

                    listView2.Items.Add(lvi2);
                }
        }
        #endregion

        #region EventsHandler Methods
        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AboutBox ab = new AboutBox();
            ab.Show();
        }

        private void datagridview1_rowEnter(object sender, DataGridViewCellEventArgs e)
        {
            propertyGrid1.SelectedObject = null;
            propertyGrid2.SelectedObject = null;
            int id = int.Parse(dataGridView1.Rows[e.RowIndex].Cells["mob_id"].Value.ToString());
            populateFields(id);
            selectedDataGridRow = dataGridView1.Rows[e.RowIndex];
        }

        private void button1_Click(object sender, EventArgs e)
        {
            MobBaseAssets mba = new MobBaseAssets(baseAssetText, pictureBox1);
            mba.ShowDialog(this);
        }

        private void toolStripButton4_Click(object sender, EventArgs e)
        {
            dataGridView1.Rows.Clear();
            setupTable();
        }

        private void listView1_MouseClick(object sender, MouseEventArgs e)
        {
            foreach (ListViewItem lvi in listView1.SelectedItems)
            {
                DataRow dr = (DataRow) lvi.Tag;
                int itemUsage = int.Parse(dr["usage_chance"].ToString());
                int itemDrop = int.Parse(dr["drop_chance"].ToString());
                int quantity = int.Parse(dr["qty"].ToString());

                EquippedEquipmentProps eep = new EquippedEquipmentProps();
                eep.UsageChance = itemUsage;
                eep.DropChance = itemDrop;
                eep.Quantity = quantity;

                propertyGrid1.SelectedObject = eep;
            }
            
        }

        private void listView2_MouseClick(object sender, MouseEventArgs e)
        {
            foreach (ListViewItem lvi in listView2.SelectedItems)
            {
                DataRow dr = (DataRow)lvi.Tag;
                int itemDrop = int.Parse(dr["drop_chance"].ToString());
                int quantity = int.Parse(dr["qty"].ToString());

                InventoryProps ip = new InventoryProps();
                ip.DropChance = itemDrop;
                ip.Quantity = quantity;

                propertyGrid2.SelectedObject = ip;
            }
        }

        private void refreshTableToolStripMenuItem_Click(object sender, EventArgs e)
        {
            dataGridView1.Rows.Clear();
            setupTable();
        }

        private void propertyGrid1_PropertyValueChanged(object sender, PropertyValueChangedEventArgs e)
        {
            String propName = e.ChangedItem.ToString();

            DataRow dr = null;
            foreach (ListViewItem lvi in listView1.SelectedItems)
            {
                dr = (DataRow)lvi.Tag;
            }

            if (propName.Contains("UsageChance"))
            {
                dr["usage_chance"] = e.ChangedItem.Value;
            }
            else if (propName.Contains("DropChance"))
            {
                dr["drop_chance"] = e.ChangedItem.Value;
            }
            else if (propName.Contains("Quantity"))
            {
                dr["qty"] = e.ChangedItem.Value;
            }
        }

        private void propertyGrid2_PropertyValueChanged(object sender, PropertyValueChangedEventArgs e)
        {
            String propName = e.ChangedItem.ToString();

            DataRow dr = null;
            foreach (ListViewItem lvi in listView2.SelectedItems)
            {
                dr = (DataRow)lvi.Tag;
            }

            if (propName.Contains("DropChance"))
            {
                dr["drop_chance"] = e.ChangedItem.Value;
            }
            else if (propName.Contains("Quantity"))
            {
                dr["qty"] = e.ChangedItem.Value;
            }

            if (dr.RowState != DataRowState.Modified)
            {
                dr.SetModified();
            }
        }

        private void nameText_TextChanged(object sender, CancelEventArgs e)
        {
            if (((TextBox)sender).Text == "")
            {
                MessageBox.Show("You must enter a name for the mob.", "Invalid Input", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                e.Cancel = true;
            }
            else
            {
                String name = ((TextBox)sender).Text.ToString();
                selectedRow["name"] = name;

                selectedDataGridRow.Cells[1].Value = name;
            }
        }

        private void nameText_TextChanged2(object sender, EventArgs e)
        {
            selectedRow["name"] = nameText.Text;
        }

        void baseAssetText_TextChanged(object sender, CancelEventArgs e)
        {
            if (int.Parse(((TextBox)sender).Text.ToString()) <= -1 || int.Parse(((TextBox)sender).Text.ToString()) > 2411)
            {
                MessageBox.Show("You must enter a valid base asset id.", "Invalid Input", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                e.Cancel = true;
            }
            else
            {
                int baseAssetid = int.Parse(((TextBox)sender).Text.ToString());
                selectedRow["base_asset_id"] = baseAssetid;
            }
        }

        void baseAssetText_TextChanged2(object sender, EventArgs e)
        {
            selectedRow["base_asset_id"] = baseAssetText.Text;
        }

        void scaleText_TextChanged(object sender, CancelEventArgs e)
        {
            try
            {
                float scale = float.Parse(((TextBox)sender).Text.ToString());
                selectedRow["scale"] = scale;
            }
            catch (Exception)
            {
                MessageBox.Show("You must enter a valid float.", "Invalid Input", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                e.Cancel = true;
            }
        }

        void scaleText_TextChanged2(object sender, EventArgs e)
        {
            selectedRow["scale"] = scaleText.Text;
        }

        void aiText_TextChanged(object sender, CancelEventArgs e)
        {
            String aiText = ((TextBox)sender).Text.ToString();
            selectedRow["ai"] = aiText;
        }

        void aiText_TextChanged2(object sender, EventArgs e)
        {
            selectedRow["ai"] = aiText.Text;
        }

        void typeCombo_SelectedValueChanged(object sender, CancelEventArgs e)
        {
            int type = ((ComboBox)sender).SelectedIndex;
            selectedRow["type"] = type;
        }

        void typeCombo_TextChanged2(object sender, EventArgs e)
        {
            selectedRow["type"] = typeCombo.SelectedIndex;
        }

        void factionCombo_SelectedValueChanged(object sender, CancelEventArgs e)
        {
            int factionID = ((ComboBox)sender).SelectedIndex;
            selectedRow["faction_id"] = factionID;
        }

        void factionCombo_TextChanged2(object sender, EventArgs e)
        {
            selectedRow["faction_id"] = factionCombo.SelectedIndex;
        }

        void levelCombo_SelectedValueChanged(object sender, CancelEventArgs e)
        {
            int level = ((ComboBox)sender).SelectedIndex;
            selectedRow["level"] = level;
            selectedDataGridRow.Cells[2].Value = level;
        }

        void levelCombo_TextChanged2(object sender, EventArgs e)
        {
            selectedRow["level"] = levelCombo.SelectedIndex;
        }

        void cPicker1_TextChanged(object sender, CancelEventArgs e)
        {
            Color c1 = ((LaMarvin.Windows.Forms.ColorPicker)sender).Color;
            AdobeColors.HSL hsl = AdobeColors.RGB_to_HSL(c1);
            selectedRow["h"] = hsl.H;
            selectedRow["s"] = hsl.S;
            selectedRow["v"] = hsl.L;
        }

        void cPicker1_TextChanged2(object sender, EventArgs e)
        {
            Color c1 = ((LaMarvin.Windows.Forms.ColorPicker)sender).Color;
            AdobeColors.HSL hsl = AdobeColors.RGB_to_HSL(c1);
            selectedRow["h"] = hsl.H;
            selectedRow["s"] = hsl.S;
            selectedRow["v"] = hsl.L;
        }

        private void newButton_Click(object sender, EventArgs e)
        {
            int id = mobs.newRecord();

            object[] dataSource = new object[3];
            dataSource[0] = id;
            dataSource[1] = "<New Object>";
            dataSource[2] = 0;

            dataGridView1.Rows.Add(dataSource);

            dataGridView1.Rows[dataGridView1.Rows.Count-1].Selected = true;
            dataGridView1.CurrentCell = dataGridView1.Rows[dataGridView1.Rows.Count-1].Cells[0];
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            newButton_Click(null, null);
        }

        private void deleteButton_Click(object sender, EventArgs e)
        {
            DialogResult dr = MessageBox.Show("Are you sure you want to delete this record?", "Record Deletion", 
                MessageBoxButtons.YesNo, MessageBoxIcon.Warning);
            switch (dr)
            {
                case DialogResult.Yes:

                    int id = int.Parse(selectedRow["mob_id"].ToString());
                    mobs.deleteRecord(id, selectedRow);

                    dataGridView1.Rows.Remove(selectedDataGridRow);

                    break;
                case DialogResult.No:
                    break;
            }
        }

        private void deleteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            deleteButton_Click(null, null);
        }

        private void saveButton_Click(object sender, EventArgs e)
        {
            
            mobs.updateRecord(selectedRow);

            foreach (ListViewItem lvi in listView1.Items)
            {
                DataRow mobItem = (DataRow) lvi.Tag;
                mobItemsSql.updateRecord(mobItem);
            }

            foreach (ListViewItem lvi in listView2.Items)
            {
                DataRow mobItem = (DataRow)lvi.Tag;
                mobItemsSql.updateRecord(mobItem);
            }
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            saveButton_Click(null, null);
        }

        private void newFromButton_Click(object sender, EventArgs e)
        {

            int id = mobs.newFromRecord(selectedRow);

            object[] dataSource = new object[3];
            dataSource[0] = id;
            dataSource[1] = selectedRow["name"].ToString();
            dataSource[2] = selectedRow["level"].ToString();

            dataGridView1.Rows.Add(dataSource);

            dataGridView1.Rows[dataGridView1.Rows.Count - 1].Selected = true;
            dataGridView1.CurrentCell = dataGridView1.Rows[dataGridView1.Rows.Count - 1].Cells[0];
        }

        private void copyRowToolStripMenuItem_Click(object sender, EventArgs e)
        {
            newFromButton_Click(null, null);
        }

        private void addToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ItemBaseAssets iba = new ItemBaseAssets(0, int.Parse(selectedRow["mob_id"].ToString()));
            iba.ShowDialog();

            if (newMobItem != null)
            {
                setupNewItem();
            }
        }

        private void addToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            ItemBaseAssets iba = new ItemBaseAssets(1, int.Parse(selectedRow["mob_id"].ToString()));
            iba.ShowDialog();

            if (newMobItem != null)
            {
                setupNewItem();
            }
        }

        private void deleteToolStripMenuItem3_Click(object sender, EventArgs e)
        {
            foreach (ListViewItem lvi in listView1.SelectedItems)
            {
                listView1.Items.Remove(lvi);
                propertyGrid1.SelectedObject = null;
                DataRow dr = (DataRow)lvi.Tag;
                mobItemsSql.deleteRecord(dr);
            }
        }

        private void deleteToolStripMenuItem4_Click(object sender, EventArgs e)
        {
            foreach (ListViewItem lvi in listView2.SelectedItems)
            {
                listView2.Items.Remove(lvi);
                propertyGrid2.SelectedObject = null;
                DataRow dr = (DataRow)lvi.Tag;
                mobItemsSql.deleteRecord(dr);
            }
        }

        private void comboBox1_SelectedValueChanged(object sender, EventArgs e)
        {
            dataGridView1.Rows.Clear();
            int comboText = comboBox1.SelectedIndex - 1;

            DataRow[] mt = mobs.getRowsBetween(comboText);
            object[] dataSource = new object[3];

            for (int i = 0; i < mt.Length; i++)
            {
                int id = int.Parse(mt[i]["mob_id"].ToString());
                String name = mt[i]["name"].ToString();
                int level = int.Parse(mt[i]["level"].ToString());

                dataSource[0] = id;
                dataSource[1] = name;
                dataSource[2] = level;

                dataGridView1.Rows.Add(dataSource); 
            }
        }
        #endregion 

        private void button2_Click(object sender, EventArgs e)
        {
            dataGridView1.Rows.Clear();

            String name = textBox1.Text;
            int searchType = comboBox2.SelectedIndex;
            String name2 = name.Replace("'", "''");

            String query = "";
            switch (searchType)
            {
                case 0:
                    query = "name = '" + name2 + "'";
                    break;
                case 1:
                    query = "name like '%" + name2 + "%'";
                    break;
            }

            DataRow[] mt = mobs.getRowsByNameQuery(query);
            object[] dataSource = new object[3];

            for (int i = 0; i < mt.Length; i++)
            {
                int id = int.Parse(mt[i]["mob_id"].ToString());
                String name3 = mt[i]["name"].ToString();
                int level = int.Parse(mt[i]["level"].ToString());

                dataSource[0] = id;
                dataSource[1] = name3;
                dataSource[2] = level;

                dataGridView1.Rows.Add(dataSource);
            }
        }
    }
}