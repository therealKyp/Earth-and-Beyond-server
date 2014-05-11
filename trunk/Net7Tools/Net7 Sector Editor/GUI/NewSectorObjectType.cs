using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace N7.GUI
{
    public partial class NewSectorObjectType : Form
    {
        private TreeView tree;
        private SectorWindow secWin;

        public NewSectorObjectType(TreeView t1, SectorWindow s1)
        {
            tree = t1;
            secWin = s1;
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.Hide();
            if (tree.SelectedNode == null)
            {
                MessageBox.Show("You Cannot add a Sector Object without first \n having a sector selected. Please " +
                    "select a sector and try again!");
            }
            else
            {
                if (tree.SelectedNode.Parent != null)
                {
                    NewSectorObject newSectorObject = new NewSectorObject(comboBox1.SelectedItem.ToString(), tree, secWin);
                    newSectorObject.ShowDialog();
                }
                else
                {
                    MessageBox.Show("You Cannot add a Sector Object without first \n having a sector selected. Please " +
                    "select a sector and try again!");
                }
            }
        }

        private void NewSectorObjectType_Load(object sender, EventArgs e)
        {
            comboBox1.Items.Add("Mobs (Type 0)");
            comboBox1.Items.Add("Planets (Type 3)");
            comboBox1.Items.Add("Stargates (Type 11)");
            comboBox1.Items.Add("Starbases (Type 12)");
            comboBox1.Items.Add("Decorations (Type 37)");
            comboBox1.Items.Add("Harvestables (Type 38)");
        }
    }
}