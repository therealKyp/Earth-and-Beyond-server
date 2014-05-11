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
    public partial class SoundEffects : Form
    {
        public IWindowsFormsEditorService _wfes;
        public int selectedID;
        public int pgID;

        public SoundEffects()
        {
            InitializeComponent();
        }

        private void SoundEffects_Load(object sender, EventArgs e)
        {
            String query = "SELECT * FROM effects where effect_class='SoundEffect' order by description;";
            DataTable effects = Database.executeQuery(Database.DatabaseName.net7, query);

            dataGridView1.DataSource = effects;

            //Todo: Select the row of the current sound effect id (pgID);
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
