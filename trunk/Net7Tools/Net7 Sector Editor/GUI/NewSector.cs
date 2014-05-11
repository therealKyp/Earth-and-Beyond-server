using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using N7.Props;
using N7.Sql;

namespace N7.GUI
{
    public partial class NewSector : Form
    {
        private TreeView tree;
        private SectorProps sp;
        private SectorsSql sectorsSQL;

        public NewSector(TreeView t1)
        {
            tree = t1;
            sectorsSQL = mainFrm.sectors;
            InitializeComponent();
        }

        private void NewSector_Load(object sender, EventArgs e)
        {
            sp = new SectorProps();
            String selectedName = tree.SelectedNode.Text;
            int id = mainFrm.systems.getIDFromName(selectedName);
            sp.SystemID = id;
            sp.Width = 200000;
            sp.Height = 200000;
            sp.Depth = 50000;
            sp.Name = "<New Sector>";
            propertyGrid1.SelectedObject = sp;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            DataTable tmp = sectorsSQL.getSectorTable();

            bool idTaken = false;
            foreach(DataRow tr in tmp.Rows)
            {
                if (tr["sector_id"].ToString() == sp.SectorID.ToString())
                {
                    idTaken = true;
                }
                else if (sp.SectorID.ToString() == "0")
                {
                    idTaken = true;
                }
            }

            if (idTaken == true)
            {
                MessageBox.Show("Sorry, the entered sector Id is already taken. \n Please enter a unique one");
            }
            else
            {
                DataRow newSectorRow = tmp.NewRow();

                String name = sp.Name.Replace("'", "''");
                String notes = sp.Notes;
                String greetings = sp.Greetings;
                if (sp.Notes != null)
                {
                    notes = sp.Notes.Replace("'", "''");
                }
                if (sp.Greetings != null)
                {
                    greetings = sp.Greetings.Replace("'", "''");
                }
                newSectorRow["name"] = name;
                newSectorRow["notes"] = notes;
                newSectorRow["greetings"] = greetings;
                newSectorRow["sector_id"] = sp.SectorID;

                float xmin = -(sp.Width / 2);
                float xmax = sp.Width / 2;
                float ymin = -(sp.Height / 2);
                float ymax = sp.Height / 2;
                float zmin = -(sp.Depth / 2);
                float zmax = sp.Depth / 2;

                newSectorRow["x_min"] = xmin;
                newSectorRow["x_max"] = xmax;
                newSectorRow["y_min"] = ymin;
                newSectorRow["y_max"] = ymax;
                newSectorRow["z_min"] = zmin;
                newSectorRow["z_max"] = zmax;
                newSectorRow["grid_x"] = sp.GridX;
                newSectorRow["grid_y"] = sp.GridY;
                newSectorRow["grid_z"] = sp.GridZ;
                newSectorRow["fog_near"] = sp.FogNear;
                newSectorRow["fog_far"] = sp.FogFar;
                newSectorRow["debris_mode"] = sp.DebrisMode;
                newSectorRow["light_backdrop"] = sp.LightBackdrop;
                newSectorRow["fog_backdrop"] = sp.FogBackdrop;
                newSectorRow["swap_backdrop"] = sp.SwapBackdrop;
                newSectorRow["backdrop_fog_near"] = sp.BackdropFogNear;
                newSectorRow["backdrop_fog_far"] = sp.BackdropFogFar;
                newSectorRow["max_tilt"] = sp.MaxTilt;
                newSectorRow["auto_level"] = sp.AutoLevel;
                newSectorRow["impulse_rate"] = sp.ImpulseRate;
                newSectorRow["decay_velocity"] = sp.DecayVelocity;
                newSectorRow["decay_spin"] = sp.DecaySpin;
                newSectorRow["backdrop_asset"] = sp.BackdropAsset;
                newSectorRow["system_id"] = sp.SystemID;
                newSectorRow["galaxy_x"] = sp.GalaxyX;
                newSectorRow["galaxy_y"] = sp.GalaxyY;
                newSectorRow["galaxy_z"] = sp.GalaxyZ;

                tmp.Rows.Add(newSectorRow);

                String systemName = mainFrm.systems.findRowNameByID(int.Parse(newSectorRow["system_id"].ToString()));

                //Add to treeNode
                foreach (TreeNode r in tree.Nodes)
                {
                    if (r.Text == systemName)
                    {
                        TreeNode tmpTNode = new TreeNode(name);
                        r.Nodes.Add(tmpTNode);
                    }
                }

                //Add to database
                sectorsSQL.newRow(newSectorRow);

                //Add to graphic screen.
                mainFrm.systemWindow.newSector(newSectorRow);

                this.Close();
            }
        }
    }
}