using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace N7.GUI
{
    public partial class NewFrm : Form
    {
        private TreeView tree;
        private SectorWindow sw;

        public NewFrm(TreeView t1, SectorWindow s1)
        {
            tree = t1;
            sw = s1;
            InitializeComponent();
        }

        private void NewFrm_Load(object sender, EventArgs e)
        {
            comboBox1.Items.Add("System");
            comboBox1.Items.Add("Sector");
            comboBox1.Items.Add("Sector Object");
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            try
            {
                this.Hide();

                if (comboBox1.SelectedItem.ToString() == "System")
                {
                    NewSystem newSystem = new NewSystem(tree);
                    newSystem.ShowDialog();
                }
                else if (comboBox1.SelectedItem.ToString() == "Sector")
                {
                    NewSector newSector = new NewSector(tree);
                    newSector.ShowDialog();
                }
                else if (comboBox1.SelectedItem.ToString() == "Sector Object")
                {
                    NewSectorObjectType newSectorObject = new NewSectorObjectType(tree, sw);
                    newSectorObject.ShowDialog();
                }

                this.Close();
            }
            catch (Exception)
            {
                MessageBox.Show("Please Select an option from the dropdown menu.");
            }
        }
    }
}