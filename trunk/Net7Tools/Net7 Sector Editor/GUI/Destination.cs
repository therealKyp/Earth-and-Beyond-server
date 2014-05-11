using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Windows.Forms.Design;

namespace N7.GUI
{
    public partial class Destination : Form
    {

        public IWindowsFormsEditorService _wfes;
        public int selectedID;
        public int pgID;

        public Destination()
        {
            InitializeComponent();
        }

        private void Destination_Load(object sender, EventArgs e)
        {

        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            int type = comboBox1.SelectedIndex;
            String query = "";

            switch (type)
            {
                case 0:
                    query = "SELECT sector_id, name FROM sectors "+
                        "order by name";
                    break;
                case 1:
                    int sectorID = mainFrm.sectorID;
                    query = "SELECT sector_object_id, name FROM sector_objects " +
                        "where sector_id='"+sectorID+"' order by name";
                    break;
            }

            DataTable select = Database.executeQuery(Database.DatabaseName.net7, query);
            dataGridView1.DataSource = select;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            selectedID = pgID;
            this.Close();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            foreach (DataGridViewRow dgvr in dataGridView1.SelectedRows)
            {
                selectedID = int.Parse(dgvr.Cells[0].Value.ToString());
            }
            this.Close();
        }
    }
}
