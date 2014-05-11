using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Windows.Forms.Design;

using N7.Sql;

namespace N7.GUI
{
    public partial class MobGroup : Form
    {
        public IWindowsFormsEditorService _wfes;
        private DataTable mobsTable;
        private DataTable groupMobsTable;
        private int _id;

        public MobGroup()
        {
            DataTable tmp = Database.executeQuery(Database.DatabaseName.net7, "SELECT Auto_increment FROM information_schema.tables WHERE table_name='sector_objects' and table_schema='net7';");
            int autoID = 0;
            foreach (DataRow z in tmp.Rows)
            {
                autoID = int.Parse(z["Auto_increment"].ToString());
            }

            DataTable tmp2 = Database.executeQuery(Database.DatabaseName.net7, "SELECT sector_object_id FROM sector_objects where sector_object_id='" + mainFrm.selectedObjectID + "';");
            int id = 0;
            foreach (DataRow z in tmp2.Rows)
            {
                id = int.Parse(z["sector_object_id"].ToString());
                Console.Out.WriteLine(id);
            }

            if (id != 0)
            {
                _id = mainFrm.selectedObjectID;
            }
            else
            {
                _id = autoID;
            }

            InitializeComponent();
        }

        private void MobGroup_Load(object sender, EventArgs e)
        {
            mobsTable = mainFrm.mobs.getMobTable();
            dataGridView1.DataSource = mobsTable;
            dataGridView1.SelectionMode = DataGridViewSelectionMode.FullRowSelect;

            String mobsQuery = "SELECT * FROM mob_spawn_group where spawn_group_id='"+_id+"';";
            groupMobsTable = Database.executeQuery(Database.DatabaseName.net7, mobsQuery);

            DataTable tmp = new DataTable();
            tmp = mobsTable;

            foreach (DataRow r in groupMobsTable.Rows)
            {
                String mob_id = r["mob_id"].ToString();

                foreach (DataGridViewRow row in dataGridView1.Rows)
                {
                    String mobID = dataGridView1.Rows[row.Index].Cells["mob_id"].Value.ToString();
                    if(mobID == mob_id)
                    {
                        String[] rowData = new String[2];
                        rowData[0] = dataGridView1.Rows[row.Index].Cells["mob_id"].Value.ToString();
                        rowData[1] = dataGridView1.Rows[row.Index].Cells["name"].Value.ToString();
                        dataGridView2.Rows.Add(rowData);
                    }
                }
            }
            dataGridView2.SelectionMode = DataGridViewSelectionMode.FullRowSelect;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            foreach (DataGridViewRow row in dataGridView1.SelectedRows)
            {
                int index = 0;
                String[] rowData = new String[2];
                rowData[0] = dataGridView1.Rows[row.Index].Cells["mob_id"].Value.ToString();
                rowData[1] = dataGridView1.Rows[row.Index].Cells["name"].Value.ToString();

                String mob_id;
                mob_id = dataGridView1.Rows[row.Index].Cells["mob_id"].Value.ToString();

                for (int i = 0; i < dataGridView2.Rows.Count-1; i++)
                {
                    String prevMobID = dataGridView2.Rows[i].Cells["id"].Value.ToString();
                    if (mob_id == prevMobID)
                    {
                        index++;
                    }
                }

                try
                {
                    String insert = "INSERT INTO mob_spawn_group SET spawn_group_id='" + _id + "', mob_id='" + rowData[0] + "', group_index='"+index+"';";
                    DataTable saved = Database.executeQuery(Database.DatabaseName.net7, insert);

                    dataGridView2.Rows.Add(rowData);
                    dataGridView2.Update();
                }
                catch (Exception)
                {
                    throw;
                }
            }
        }

        private void button4_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            int index = 0;
            foreach (DataGridViewRow r in dataGridView2.SelectedRows)
            {
                String mob_id;
                mob_id = dataGridView2.Rows[r.Index].Cells["id"].Value.ToString();

                for (int i = r.Index+1; i < dataGridView2.Rows.Count-1; i++)
                {
                    String prevMobID = dataGridView2.Rows[i].Cells["id"].Value.ToString();
                    if (mob_id == prevMobID)
                    {
                        index++;
                    }
                }

                for (int i2 = 0; i2 < r.Index; i2++)
                {
                    String prevMobID = dataGridView2.Rows[i2].Cells["id"].Value.ToString();
                    if (mob_id == prevMobID)
                    {
                        index++;
                    }
                }

                try
                {
                    String remove = "DELETE FROM mob_spawn_group where spawn_group_id='" + _id + "' and mob_id='" + mob_id + "' and group_index='" + index + "';";
                    Console.Out.WriteLine(remove);
                    DataTable saved = Database.executeQuery(Database.DatabaseName.net7, remove);
                    dataGridView2.Rows.Remove(r);
                }
                catch (Exception)
                {
                    throw;
                }
            }
        }
    }
}