using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using MySql.Data.MySqlClient;

namespace SQLBind
{
    public partial class EffectSearch : Form
    {
        private ContextMenu popUpMenu;
        private int m_ClickRow = 0;

        private MySqlConnection m_Conn = null;
        int IDEffect = -1;
        bool m_Ok = false;

        public EffectSearch()
        {
            InitializeComponent();
        }

        private void EffectSearch_Load(object sender, EventArgs e)
        {
            m_Conn = DB.GetConnection();

            // Pull down data
            DataTable dataTable = new DataTable();
            String query = "SELECT `EffectID`,`item_effect_base`.`Description`,`item_effect_base`.`Tooltip` FROM `item_effect_base`";
            MySqlDataAdapter mySqlDataAdapter = new MySqlDataAdapter(query, m_Conn);

            mySqlDataAdapter.Fill(dataTable);

            EffectList.DataSource = dataTable;
        }

        public int GetEffectID()
        {
            IDEffect = Int32.Parse(EffectList.SelectedRows[0].Cells[0].Value.ToString());
            return m_Ok? IDEffect : -1;
        }

        private void OkBt_Click(object sender, EventArgs e)
        {
            m_Ok = true;
            this.Close();
        }

        private void CancelBt_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void EffectList_CellMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                m_ClickRow = e.RowIndex;

                popUpMenu = new ContextMenu();

                try
                {
                    this.ContextMenu.Dispose();
                }
                catch
                {
                }

                popUpMenu.MenuItems.Add("Delete Effect", new EventHandler(PopUpMenuDelete));
                this.ContextMenu = popUpMenu;
            }            
        }

        private void PopUpMenuDelete(object sender, EventArgs e)
        {
            MenuItem mClicked = (MenuItem)sender;

            if (mClicked.Text == "Delete Effect")
            {
                String EffectDesc = EffectList.Rows[m_ClickRow].Cells[1].Value.ToString();
                if (MessageBox.Show("Are you sure you want to delete '" + EffectDesc + "'?", "Delete Conformation", MessageBoxButtons.YesNo, MessageBoxIcon.Warning) == DialogResult.Yes)
                {
                    // Delete the row now
                    DataTable dataTable = new DataTable();
                    String DelQuery = "DELETE FROM `item_effect_base` WHERE `EffectID` = '" + EffectList.Rows[m_ClickRow].Cells[0].Value.ToString() + "';";
                    MySqlDataAdapter DeleteSqlDataA = new MySqlDataAdapter(DelQuery, m_Conn);
                    DeleteSqlDataA.Fill(dataTable);


                    // Pull down data
                    String query = "SELECT `EffectID`,`item_effect_base`.`Description`,`item_effect_base`.`Tooltip` FROM `item_effect_base`";
                    MySqlDataAdapter mySqlDataAdapter = new MySqlDataAdapter(query, m_Conn);

                    mySqlDataAdapter.Fill(dataTable);

                    EffectList.DataSource = dataTable;

                }
            }
            this.ContextMenu.Dispose();
        }
    }
}
