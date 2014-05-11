using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Collections;

using N7.SQL;

namespace N7.GUI
{
    public partial class mainFrm : Form
    {
        private FactionsSQL factions;
        private FactionMatrixSQL factionMatrix;
        private DataGridViewRow selectedDataGridRow;
        private Hashtable props = new Hashtable();
        private DataRow facRow;

        public mainFrm()
        {
            InitializeComponent();
        }

        private void mainFrm_Load(object sender, EventArgs e)
        {
            factions = new FactionsSQL();
            factionMatrix = new FactionMatrixSQL();

            object[] dataSource = new object[2];
            for (int i = 0; i < factions.getFactionTable().Rows.Count; i++)
			{
			    dataSource[0] = factions.getFactionTable().Rows[i]["faction_id"].ToString();
                dataSource[1] = factions.getFactionTable().Rows[i]["name"].ToString();

                dataGridView1.Rows.Add(dataSource); 
			}
        }

        private void populateFields(int id)
        {
            facRow = factions.getRowByID(id);
            nameText.Text = facRow["name"].ToString();
            descriptionText.Text = facRow["description"].ToString();
            pdaText.Text = facRow["PDA_text"].ToString();

            DataRow[] fmRows = factionMatrix.getRowsByID(id);
            for (int i = 0; i < fmRows.Length; i++)
            {
                DataRow dr = fmRows[i];
                int fmID = int.Parse(dr["id"].ToString());
                int factionID = id;
                int feID = int.Parse(dr["faction_entry_id"].ToString());
                int baseValue = int.Parse(dr["base_value"].ToString());
                int currentValue = int.Parse(dr["current_value"].ToString());
                int rf = int.Parse(dr["reward_faction"].ToString());
                Boolean rewardFaction = Convert.ToBoolean(rf);

                FactionMatrixProps fmp = new FactionMatrixProps();
                fmp.ID = fmID;
                fmp.FactionID = id;
                fmp.FactionEntryID = feID;
                fmp.BaseValue = baseValue;
                fmp.CurrentValue = currentValue;
                fmp.RewardFaction = rewardFaction;

                DataRow r = factions.getRowByID(feID);
                listBox1.Items.Add(r["name"].ToString());
                props.Add(r["name"].ToString(), fmp);
            }
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Database.closeDB();
            Application.Exit();
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AboutBox ab = new AboutBox();
            ab.ShowDialog();
        }

        private void datagridview1_rowEnter(object sender, DataGridViewCellEventArgs e)
        {
            listBox1.Items.Clear();
            propertyGrid1.SelectedObject = null;
            props.Clear();
            int id = int.Parse(dataGridView1.Rows[e.RowIndex].Cells["FactionID"].Value.ToString());
            selectedDataGridRow = dataGridView1.Rows[e.RowIndex];
            populateFields(id);
        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            FactionMatrixProps fmp = (FactionMatrixProps) props[listBox1.SelectedItem.ToString()];
            propertyGrid1.SelectedObject = fmp;
        }

        private void toolStripButton5_Click(object sender, EventArgs e)
        {
            dataGridView1.Rows.Clear();
            factions = new FactionsSQL();

            object[] dataSource = new object[2];
            for (int i = 0; i < factions.getFactionTable().Rows.Count; i++)
            {
                dataSource[0] = factions.getFactionTable().Rows[i]["faction_id"].ToString();
                dataSource[1] = factions.getFactionTable().Rows[i]["name"].ToString();

                dataGridView1.Rows.Add(dataSource);
            }
        }

        private void refreshTableToolStripMenuItem_Click(object sender, EventArgs e)
        {
            toolStripButton5_Click(null, null);
        }

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            DataRow tmp = factions.newRecord();
            object[] newRow = new object[2];
            newRow[0] = tmp["faction_id"].ToString();
            newRow[1] = tmp["name"].ToString();

            int newRowId = int.Parse(tmp["faction_id"].ToString());

            foreach (DataRow r in factions.getFactionTable().Rows)
            {
                int existingRowID = int.Parse(r["faction_id"].ToString());
                if (newRowId != existingRowID)
                {
                    DataRow dr = factionMatrix.newRecord(newRowId, existingRowID);

                    FactionMatrixProps fmp = new FactionMatrixProps();
                    fmp.ID = int.Parse(dr["id"].ToString());
                    fmp.FactionID = int.Parse(dr["faction_id"].ToString());
                    fmp.FactionEntryID = int.Parse(dr["faction_entry_id"].ToString());
                    fmp.BaseValue = int.Parse(dr["base_value"].ToString());
                    fmp.CurrentValue = int.Parse(dr["current_value"].ToString());
                    fmp.RewardFaction = bool.Parse(dr["reward_faction"].ToString());

                    listBox1.Items.Clear();
                    props.Clear();
                    listBox1.Items.Add(r["name"].ToString());
                    props.Add(r["name"].ToString(), fmp);
                }
            }

            dataGridView1.Rows.Add(newRow);
            dataGridView1.Rows[dataGridView1.Rows.Count - 1].Selected = true;
            dataGridView1.CurrentCell = dataGridView1.Rows[dataGridView1.Rows.Count - 1].Cells[0];
        }

        private void toolStripButton3_Click(object sender, EventArgs e)
        {
            DialogResult dr = MessageBox.Show("Are you sure you want to delete this record?", "Record Deletion",
                MessageBoxButtons.YesNo, MessageBoxIcon.Warning);
            switch (dr)
            {
                case DialogResult.Yes:

                    int id = int.Parse(facRow["faction_id"].ToString());
                    factions.deleteRecord(id, facRow);
                    factionMatrix.deleteRecord(id);

                    dataGridView1.Rows.Remove(selectedDataGridRow);

                    break;
                case DialogResult.No:
                    break;
            }
        }

        private void toolStripButton2_Click(object sender, EventArgs e)
        {
            factions.updateRecord(facRow);
            int factionID = int.Parse(facRow["faction_id"].ToString());
            factionMatrix.updateRecord(factionID);
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            toolStripButton1_Click(null, null);
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            toolStripButton2_Click(null, null);
        }

        private void deleteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            toolStripButton3_Click(null, null);
        }

        private void nameText_TextChanged(object sender, EventArgs e)
        {
            String name = nameText.Text;
            String name2 = name.Replace("'","''");

            facRow["name"] = name2;
        }

        private void descriptionText_TextChanged(object sender, EventArgs e)
        {
            String descr = descriptionText.Text;
            String descr2 = descr.Replace("'", "''");

            facRow["description"] = descr2;
        }

        private void pdaText_TextChanged(object sender, EventArgs e)
        {
            facRow["PDA_text"] = pdaText.Text;
        }

        private void propertyGrid1_PropertyValueChanged(object sender, PropertyValueChangedEventArgs e)
        {
            String propName = e.ChangedItem.ToString();

            FactionMatrixProps fmp = (FactionMatrixProps)props[listBox1.SelectedItem.ToString()];
            DataRow dr = factionMatrix.getRowByID(fmp.ID);

            if (propName.Contains("BaseValue"))
            {
                dr["base_value"] = fmp.BaseValue;
            }
            else if (propName.Contains("RewardFaction"))
            {
                dr["reward_faction"] = fmp.RewardFaction;
            }
        }
    }
}
