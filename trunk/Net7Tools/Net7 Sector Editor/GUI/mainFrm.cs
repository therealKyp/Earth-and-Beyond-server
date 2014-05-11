using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Threading;

using UMD.HCIL.Piccolo;
using UMD.HCIL.Piccolo.Event;
using UMD.HCIL.Piccolo.Nodes;
using UMD.HCIL.Piccolo.Util;

using N7.Sql;
using N7.Props;
using N7.GUI;
using N7.Utilities;

namespace N7
{
    public partial class mainFrm : Form
    {
        public static FactionSql factions;
        public static SectorsSql sectors;
        public static SystemsSql systems;
        public static MobsSQL mobs;
        public static BaseAssetSQL baseAssets;
        public static SectorObjectsSql sectorObjects;
        public static SectorWindow sectorWindow;
        public static SystemWindow systemWindow;
        public static int selectedObjectID;
        PDragEventHandler drag;
        PPanEventHandler pan;
        DataRow selectedSystemRow;
        public static int sectorID;
        private bool firstRun = true;
        
        public mainFrm()
        {
            InitializeComponent();
        }

        private void mainFrm_Load(object sender, EventArgs e)
        {
            pCanvas1.BackColor = Color.Black;
            factions = new FactionSql();
            systems = new SystemsSql();
            sectors = new SectorsSql();
            baseAssets = new BaseAssetSQL();
            mobs = new MobsSQL();

            TreeWindow treeWindow = new TreeWindow(systems.getSystemTable(), sectors.getSectorTable());
            treeView1.Nodes.AddRange(treeWindow.setupInitialTree());

            HE_GlobalVars._ListofFactions = new string[factions.getFactionTable().Rows.Count+1];
            int factionCount = 1;
            HE_GlobalVars._ListofFactions[0] = "None";
            DataTable factionTmp = factions.getFactionTable();
            foreach (DataRow r in factionTmp.Rows)
            {
                HE_GlobalVars._ListofFactions[factionCount] = r["name"].ToString();
                factionCount++;
            }
        }

        private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
        {
            checkIfChanged();

            SectorTab.Text = e.Node.Text;
            String selName = e.Node.Text.ToString();

            pCanvas1.Dispose();

            this.pCanvas1 = new UMD.HCIL.Piccolo.PCanvas();
            this.SectorTab.Controls.Add(this.pCanvas1);
            this.pCanvas1.AllowDrop = true;
            this.pCanvas1.BackColor = System.Drawing.Color.White;
            this.pCanvas1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pCanvas1.GridFitText = false;
            this.pCanvas1.Location = new System.Drawing.Point(0, 0);
            this.pCanvas1.Name = "pCanvas1";
            this.pCanvas1.RegionManagement = true;
            this.pCanvas1.Size = new System.Drawing.Size(554, 438);
            this.pCanvas1.TabIndex = 0;
            this.pCanvas1.Text = "pCanvas1";

            if (e.Node.Level == 0)
            {
                if (e.Node.Nodes.Count > 0)
                {
                    DataRow[] systemRow = systems.findRowsByName(selName);
                    String systemID = systemRow[0]["system_id"].ToString();
                    selectedSystemRow = systemRow[0];

                    //Populate Properties
                    SystemProps sp = new SystemProps();
                    sp.Name = selName;

                    int R = int.Parse(systemRow[0]["color_r"].ToString());
                    int G = int.Parse(systemRow[0]["color_g"].ToString());
                    int B = int.Parse(systemRow[0]["color_b"].ToString());
                    Color color = Color.FromArgb(R, G, B);
                    sp.Color = color;

                    sp.GalaxyX = float.Parse(systemRow[0]["galaxy_x"].ToString());
                    sp.GalaxyY = float.Parse(systemRow[0]["galaxy_y"].ToString());
                    sp.GalaxyZ = float.Parse(systemRow[0]["galaxy_z"].ToString());
                    sp.Notes = systemRow[0]["notes"].ToString();

                    DataRow[] sectorTable = sectors.getRowsBySystemID(systemID);

                    createPropertyGrid();
                    propertyGrid1.SelectedObject = sp;

                    systemWindow = new SystemWindow(pCanvas1, selName, sectorTable, propertyGrid1, selectedSystemRow);
                    propertyGrid1.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(systemWindow.pg_PropertyValueChanged);
                }
                else
                {
                    MessageBox.Show("Sorry, There are no sector's to display in the selected system! \n Please add a new sector to it, or select another system.");
                }
            }
            else if (e.Node.Level == 1)
            {
                DataRow[] sectorRow = sectors.findRowsByName(selName);
                sectorID = int.Parse(sectorRow[0]["sector_id"].ToString());
                createPropertyGrid();
                sectorWindow = new SectorWindow(pCanvas1, sectorRow, propertyGrid1, dataGridView1);
                createOptions(sectorWindow);
                sectorObjects = sectorWindow.getSectorObjectsSQL();
                propertyGrid1.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(sectorWindow.PropertyGrid1_PropertyValueChanged);

                dataGridView1.Rows.Clear();
                DataTable secObjects = sectorObjects.getSectorObject();
                object[] dataSource = new object[4];
                foreach (DataRow dr in secObjects.Rows)
                {
                    int id = int.Parse(dr["sector_object_id"].ToString());
                    String name = dr["name"].ToString();
                    int baID = int.Parse(dr["base_asset_id"].ToString());
                    int type = int.Parse(dr["type"].ToString());

                    dataSource[0] = id;
                    dataSource[1] = name;
                    dataSource[2] = baID;
                    dataSource[3] = type;

                    dataGridView1.Rows.Add(dataSource);
                }
            }
        }

        private void createPropertyGrid()
        {
            if (propertyGrid1 != null)
            {
                this.propertyGrid1.Dispose();
            }

            this.propertyGrid1 = new System.Windows.Forms.PropertyGrid();
            this.propertyGrid1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.propertyGrid1.Location = new System.Drawing.Point(1, 1);
            this.propertyGrid1.Name = "propertyGrid1";
            this.propertyGrid1.Size = new System.Drawing.Size(242, 279);
            this.propertyGrid1.TabIndex = 0;

            this.PropertiesDock.Controls.Add(this.propertyGrid1);
        }

        private void createOptions(SectorWindow sectorWindow)
        {
            if (Properties.Settings.Default.KeepVisualizationSetting == true)
            {
                if (optionsGui1 == null)
                {
                    this.optionsGui1 = new N7.GUI.OptionsGui(sectorWindow);
                    this.optionsGui1.AutoScroll = true;
                    this.optionsGui1.BackColor = System.Drawing.Color.Transparent;
                    this.optionsGui1.Dock = System.Windows.Forms.DockStyle.Fill;
                    this.optionsGui1.Location = new System.Drawing.Point(0, 0);
                    this.optionsGui1.Name = "optionsGui1";
                    this.optionsGui1.Size = new System.Drawing.Size(768, 97);
                    this.optionsGui1.TabIndex = 0;

                    this.sectorOptionsDock.Controls.Add(this.optionsGui1);
                }
                else
                {
                    optionsGui1.setSectorWindow(sectorWindow);
                }
            }
            else
            {
                if (optionsGui1 != null)
                {
                    this.optionsGui1.Dispose();
                }

                this.optionsGui1 = new N7.GUI.OptionsGui(sectorWindow);
                this.optionsGui1.AutoScroll = true;
                this.optionsGui1.BackColor = System.Drawing.Color.Transparent;
                this.optionsGui1.Dock = System.Windows.Forms.DockStyle.Fill;
                this.optionsGui1.Location = new System.Drawing.Point(0, 0);
                this.optionsGui1.Name = "optionsGui1";
                this.optionsGui1.Size = new System.Drawing.Size(768, 97);
                this.optionsGui1.TabIndex = 0;

                this.sectorOptionsDock.Controls.Add(this.optionsGui1);
            }

            optionsGui1.loadAll();
        }

        #region Toolbars and Menu Events
        private void checkIfChanged()
        {
            DataTable secT = sectors.getSectorTable();
            DataTable sysT = systems.getSystemTable();
            DataTable secObT = null;


            bool changed = false;

            foreach (DataRow dt in secT.Rows)
            {
                if (dt.RowState != DataRowState.Unchanged)
                {
                    changed = true;
                }
            }
            foreach (DataRow dt2 in sysT.Rows)
            {
                if (dt2.RowState != DataRowState.Unchanged)
                {
                    changed = true;
                }
            }
            if (sectorWindow != null)
            {
                secObT = sectorWindow.getSectorObjectsSQL().getSectorObject();

                foreach (DataRow secObRow in secObT.Rows)
                {
                    if (secObRow.RowState == DataRowState.Unchanged)
                    {
                        changed = true;
                    }
                }
            }

            if (changed == true)
            {
                DialogResult dr = MessageBox.Show("All your changes will be lost, Do want to save your changes before switching?",
                      "Loss of Information Warning", MessageBoxButtons.YesNo);
                switch (dr)
                {
                    case DialogResult.Yes:
                        save_Click(null, null);
                        break;
                    case DialogResult.No:
                        break;
                }
            }
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DataTable secT = sectors.getSectorTable();
            DataTable sysT = systems.getSystemTable();
            DataTable secObT = null;


            bool changed = false;

            foreach (DataRow dt in secT.Rows)
            {
                if (dt.RowState != DataRowState.Unchanged)
                {
                    changed = true;
                }
            }
            foreach (DataRow dt2 in sysT.Rows)
            {
                if (dt2.RowState != DataRowState.Unchanged)
                {
                    changed = true;
                }
            }
            if (sectorWindow != null)
            {
                secObT = sectorWindow.getSectorObjectsSQL().getSectorObject();

                foreach (DataRow secObRow in secObT.Rows)
                {
                    if (secObRow.RowState == DataRowState.Unchanged)
                    {
                        changed = true;
                    }
                }
            }

            if (changed == true)
            {
                DialogResult dr = MessageBox.Show("All your changes will be lost, Do want to save your changes before exiting?",
                      "Loss of Information Warning", MessageBoxButtons.YesNo);
                switch (dr)
                {
                    case DialogResult.Yes:
                        save_Click(sender, e);
                        break;
                    case DialogResult.No:
                        break;
                }
            }

            Application.Exit();
        }

        private void toolStripButton3_Click(object sender, EventArgs e)
        {
            if (pan != null)
            {
                pCanvas1.RemoveInputEventListener(pan);
            }
            pCanvas1.PanEventHandler = null;
            drag = new PDragEventHandler();
            pCanvas1.AddInputEventListener(drag);
        }

        private void toolStripButton4_Click(object sender, EventArgs e)
        {
            if (drag != null)
            {
                pCanvas1.RemoveInputEventListener(drag);
            }
            pan = new PPanEventHandler();
            pCanvas1.AddInputEventListener(pan);
        }

        private void save_Click(object sender, EventArgs e)
        {
            DataTable sysTmp = systems.getSystemTable();
            DataTable secTmp = sectors.getSectorTable();
            DataTable secObTmp = null;

            foreach (DataRow sysRow in sysTmp.Rows)
            {
                if (sysRow.RowState == DataRowState.Modified)
                {
                    sysRow.AcceptChanges();
                    systems.updateRow(sysRow);
                }
            }

            foreach (DataRow secRow in secTmp.Rows)
            {
                if (secRow.RowState == DataRowState.Modified)
                {
                    secRow.AcceptChanges();
                    sectors.updateRow(secRow);
                }
            }

            if (sectorWindow != null)
            {
                secObTmp = sectorWindow.getSectorObjectsSQL().getSectorObject();

                foreach (DataRow secObRow in secObTmp.Rows)
                {
                    if (secObRow.RowState == DataRowState.Modified)
                    {
                        secObRow.AcceptChanges();
                        sectorWindow.getSectorObjectsSQL().updateRow(secObRow);
                    }
                }
            }

            if (sectorWindow != null)
            {
                if (sectorWindow.getDeletedObjectsID().Count > 0)
                {
                    for (int i = 0; i < sectorWindow.getDeletedObjectsID().Count; i++)
                    {
                        int id = (int)sectorWindow.getDeletedObjectsID()[i];
                        int type = (int)sectorWindow.getDeletedObjectsType()[i];
                        sectorWindow.getSectorObjectsSQL().deleteRow(id, type);
                    }
                    sectorWindow.clearDeletedHashTables();
                }
            }
        }

        private void deleteSelected_Click(object sender, EventArgs e)
        {
            if (sectorWindow != null)
            {
                sectorWindow.deleteSelectedObject();
            }
            else
            {
                MessageBox.Show("Before Deleting an Object a Sector Must be openend \n and and object Selected. Please Try again.");
            }
        }

        private void systemToolStripMenuItem_Click(object sender, EventArgs e)
        {
            NewSystem newSystemDialog = new NewSystem(treeView1);
            newSystemDialog.ShowDialog();
        }

        private void sectorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (treeView1.SelectedNode == null)
            {
                MessageBox.Show("You Cannot add a Sector without first \n having a system selected. Please " +
                    "select a system and try again!");
            }
            else
            {
                if (treeView1.SelectedNode.Level == 0)
                {
                    NewSector newSectorDialog = new NewSector(treeView1);
                    newSectorDialog.ShowDialog();
                }
                else
                {
                    MessageBox.Show("You Cannot add a Sector without first \n having a system selected. Please " +
                    "select a system and try again!");
                }
            }
        }

        private void sectorObjectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            NewSectorObjectType newSectorObjectDialog = new NewSectorObjectType(treeView1, sectorWindow);
            newSectorObjectDialog.ShowDialog();
        }

        private void newToolbarClick_Click(object sender, EventArgs e)
        {
            NewFrm newfrm = new NewFrm(treeView1, sectorWindow);
            newfrm.ShowDialog();
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AboutBox2 aboutBox = new AboutBox2();
            aboutBox.ShowDialog();
        }

        private void settingsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Settings settings = new Settings();
            settings.Show(this);
        }

        private void datagridview1_rowEnter(object sender, DataGridViewCellEventArgs e)
        {
            if (firstRun != true)
            {
                int id = int.Parse(dataGridView1.Rows[e.RowIndex].Cells["id"].Value.ToString());
                sectorWindow.setSelected(id);

                //selectedDataGridRow = dataGridView1.Rows[e.RowIndex];
            }

            firstRun = false;
        }
        #endregion 
    }
}