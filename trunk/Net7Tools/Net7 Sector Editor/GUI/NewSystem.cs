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
    public partial class NewSystem : Form
    {
        private SystemsSql _systemSQL;
        private SystemProps sp;
        private TreeView tree;

        public NewSystem(TreeView t1)
        {
            tree = t1;
            _systemSQL = mainFrm.systems;
            InitializeComponent();
        }

        private void NewSystem_Load(object sender, EventArgs e)
        {
            sp = new SystemProps();
            sp.Name = "<New System>";
            sp.Color = Color.Black;
            propertyGrid1.SelectedObject = sp;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            DataTable tmp = _systemSQL.getSystemTable();
            DataRow newSystemRow = tmp.NewRow();

            String name = sp.Name.Replace("'", "''");

            String notes = "";
            if (sp.Notes != null)
            {
                notes = sp.Notes.Replace("'", "''");
            }
            
            newSystemRow["name"] = name;
            newSystemRow["notes"] = notes;
            newSystemRow["galaxy_x"] = sp.GalaxyX;
            newSystemRow["galaxy_y"] = sp.GalaxyY;
            newSystemRow["galaxy_z"] = sp.GalaxyZ;
            newSystemRow["color_r"] = sp.Color.R;
            newSystemRow["color_g"] = sp.Color.G;
            newSystemRow["color_b"] = sp.Color.B;

            tmp.Rows.Add(newSystemRow);

            TreeNode tmpTNode = new TreeNode(name);
            tree.Nodes.Add(tmpTNode);

            _systemSQL.newRow(newSystemRow);
            this.Close();
        }
    }
}