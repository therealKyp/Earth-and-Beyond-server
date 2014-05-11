using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.SqlClient;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using MySql.Data.MySqlClient;

using SQLBind.Tools;

namespace SQLBind
{
    public partial class Form1 : Form
    {
        private MySqlConnection m_conn;
        private int m_EffectID;
        EditItem ItemEditor;

        public Form1()
        {
            InitializeComponent();

            ItemEditor = new EditItem();

            DB.Connect();
            m_conn = DB.GetConnection();
            m_EffectID = 0;

            // Buffs
            EffectBuff.Items.AddRange(GetBuffList());

            // Stats
            VarStat1.Items.AddRange(GetStatsList());
            VarStat2.Items.AddRange(GetStatsList());
            VarStat3.Items.AddRange(GetStatsList());
            ConstStat1.Items.AddRange(GetStatsList());
            ConstStat2.Items.AddRange(GetStatsList());      

            // Types
            VarType1.Items.AddRange(GetVarableType());
            VarType2.Items.AddRange(GetVarableType());
            VarType3.Items.AddRange(GetVarableType());
            ConstType1.Items.AddRange(GetVarableType());
            ConstType2.Items.AddRange(GetVarableType());

            // Set defaults
            EffectType.SelectedIndex = 0;
            VarStat1.SelectedIndex = 0;
            VarStat2.SelectedIndex = 0;
            VarStat3.SelectedIndex = 0;
            VarType1.SelectedIndex = 0;
            VarType2.SelectedIndex = 0;
            VarType3.SelectedIndex = 0;
            ConstType1.SelectedIndex = 0;
            ConstType2.SelectedIndex = 0;
            ConstStat1.SelectedIndex = 0;
            ConstStat2.SelectedIndex = 0;
        }

        private void LoadEffect(int ID)
        {
            m_EffectID = ID;

            // Pull down data
            DataTable dataTable = new DataTable();
            String query = "SELECT item_effect_base.EffectType,item_effect_base.Name,item_effect_base.Description,item_effect_base.Tooltip,item_effect_base.flag1,item_effect_base.flag2,item_effect_base.Constant1Value,item_effect_base.Constant1Stat,item_effect_base.Constant1Type,item_effect_base.Constant2Value,item_effect_base.Constant2Stat,item_effect_base.Constant2Type,item_effect_base.Var1Stat,item_effect_base.Var1Type,item_effect_base.Var2Stat,item_effect_base.Var2Type,item_effect_base.Var3Stat,item_effect_base.Var3Type,item_effect_base.VisualEffect,item_effect_base.Buff_Name FROM `item_effect_base` WHERE `item_effect_base`.`EffectID` =  '" + m_EffectID + "'";
            MySqlDataAdapter mySqlDataAdapter = new MySqlDataAdapter(query, m_conn);

            mySqlDataAdapter.Fill(dataTable);

            // Load Data
            if (dataTable.Rows.Count > 0)
            {
                DataRow r = dataTable.Rows[0];
                EffectType.SelectedIndex = Int32.Parse(r["EffectType"].ToString());
                EffectName.Text = r["Name"].ToString();
                EffectToolTip.Text = r["Tooltip"].ToString();
                EffectDesc.Text = r["Description"].ToString();

                // Load in target type
                // BitSets for the flags
                TFriend.Checked = ((int) r["Flag1"] & (1 << 4)) > 0;
                TEnemy.Checked = ((int)r["Flag1"] & (1 << 5)) > 0;
                TGroupM.Checked = ((int)r["Flag1"] & (1 << 6)) > 0;
                RequireT.Checked = ((int)r["Flag2"] & 1) > 0;


                // Load in buff
                EffectBuff.SelectedIndex = EffectBuff.Items.IndexOf(r["Buff_Name"].ToString());

                // Load Values
                VarStat1.SelectedIndex = VarStat1.Items.IndexOf(r["Var1Stat"].ToString());
                VarStat2.SelectedIndex = VarStat2.Items.IndexOf(r["Var2Stat"].ToString());
                VarStat3.SelectedIndex = VarStat3.Items.IndexOf(r["Var3Stat"].ToString());
                ConstStat1.SelectedIndex = ConstStat1.Items.IndexOf(r["Constant1Stat"].ToString());
                ConstStat2.SelectedIndex = ConstStat2.Items.IndexOf(r["Constant2Stat"].ToString());

                // Var Types
                VarType1.SelectedIndex = Int32.Parse(r["Var1Type"].ToString());
                VarType2.SelectedIndex = Int32.Parse(r["Var2Type"].ToString());
                VarType3.SelectedIndex = Int32.Parse(r["Var3Type"].ToString());
                ConstType1.SelectedIndex = Int32.Parse(r["Constant1Type"].ToString());
                ConstType2.SelectedIndex = Int32.Parse(r["Constant2Type"].ToString());

                // Load Data
                ConstValue1.Text = r["Constant1Value"].ToString();
                ConstValue2.Text = r["Constant2Value"].ToString();
                
            }
        }

        private string[] GetStatsList()
        {
            List<string> codeValueList = new List<string>();

            // Pull down data
            DataTable dataTable = new DataTable();
            String query = "SELECT Stat_Name FROM `item_effect_stats`";
            MySqlDataAdapter mySqlDataAdapter = new MySqlDataAdapter(query, m_conn);

            mySqlDataAdapter.Fill(dataTable);

            foreach (DataRow r in dataTable.Rows)
            {
                codeValueList.Add(r["Stat_Name"].ToString());
            }

            return codeValueList.ToArray();
        }

        private string[] GetBuffList()
        {
            List<string> codeValueList = new List<string>();

            // Pull down data
            DataTable dataTable = new DataTable();
            String query = "SELECT buff_name FROM buffs";
            MySqlDataAdapter mySqlDataAdapter = new MySqlDataAdapter(query, m_conn);

            mySqlDataAdapter.Fill(dataTable);

            foreach (DataRow r in dataTable.Rows)
            {
                codeValueList.Add(r["buff_name"].ToString());
            }

            return codeValueList.ToArray();
        }

        private static CodeValue[] GetVarableType()
        {
            List<CodeValue> codeValueList = new List<CodeValue>();

            codeValueList.Add(CodeValue.Formatted(0, "Not Used"));
            codeValueList.Add(CodeValue.Formatted(1, "Increase Value"));
            codeValueList.Add(CodeValue.Formatted(2, "Increase Percent"));
            codeValueList.Add(CodeValue.Formatted(3, "Decrease Value"));
            codeValueList.Add(CodeValue.Formatted(4, "Decrease Percent"));
            codeValueList.Add(CodeValue.Formatted(5, "Duration"));

            return codeValueList.ToArray();
        }

        private void EffectType_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (Int32.Parse(EffectType.SelectedIndex.ToString()) == 0)
            {
                TargetTypeGrp.Enabled = false;
            }
            else
            {
                TargetTypeGrp.Enabled = true;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            EffectSearch NSearch = new EffectSearch();

            NSearch.ShowDialog();

            if (NSearch.GetEffectID() != -1)
            {
                LoadEffect(NSearch.GetEffectID());
            }

        }

        private void Save_Click(object sender, EventArgs e)
        {
            string SQLData = "UPDATE `item_effect_base` SET ";
            int Flag1 = 0, Flag2 = 0;

            // BitSets for the flags
            Flag1 += TFriend.Checked ? 1 << 4 : 0;
            Flag1 += TEnemy.Checked ? 1 << 5 : 0;
            Flag1 += TGroupM.Checked ? 1 << 6 : 0;

            Flag2 += RequireT.Checked ? 1 : 0;
            Flag2 += RequireT.Checked ? 0 : 1 << 1;

            // Make sure it has a value
            if (VisualEffect.Text == "")
            {
                VisualEffect.Text = "0";
            }

            // Main Data
            SQLData += "`EffectType` = '" + EffectType.SelectedIndex + "',";
            SQLData += "`Name` = '" + EffectName.Text + "',";
            SQLData += "`Tooltip` = '" + EffectToolTip.Text + "',";
            SQLData += "`Description` = '" + EffectDesc.Text + "',";

            SQLData += "`Flag1` = '" + Flag1.ToString() + "',";
            SQLData += "`Flag2` = '" + Flag2.ToString() + "',";

            SQLData += "`Buff_Name` = '" + EffectBuff.Text + "',";
            SQLData += "`VisualEffect` = '" + VisualEffect.Text + "',";

            // Load Values
            SQLData += "`Var1Stat` = '" + VarStat1.Text + "',";
            SQLData += "`Var2Stat` = '" + VarStat2.Text + "',";
            SQLData += "`Var3Stat` = '" + VarStat3.Text + "',";
            SQLData += "`Constant1Stat` = '" + ConstStat1.Text + "',";
            SQLData += "`Constant2Stat` = '" + ConstStat2.Text + "',";

            // Var Types
            SQLData += "`Var1Type` = '" + VarType1.SelectedIndex + "',";
            SQLData += "`Var2Type` = '" + VarType2.SelectedIndex + "',";
            SQLData += "`Var3Type` = '" + VarType3.SelectedIndex + "',";
            SQLData += "`Constant1Type` = '" + ConstType1.SelectedIndex + "',";
            SQLData += "`Constant2Type` = '" + ConstType2.SelectedIndex + "',";

            // Load Data
            SQLData += "`Constant1Value` = '" + ConstValue1.Text + "',";
            SQLData += "`Constant2Value` = '" + ConstValue2.Text + "' ";

            SQLData += "WHERE `EffectID` = '" + m_EffectID + "'";

            DataTable dataTable = new DataTable();
            MySqlDataAdapter mySqlDataAdapter = new MySqlDataAdapter(SQLData, m_conn);
            mySqlDataAdapter.Fill(dataTable);
        }

        private void NewEffect_Click(object sender, EventArgs e)
        {
            // Add a new query
            DataTable dataTable = new DataTable();
            String query = "INSERT INTO `item_effect_base` (item_effect_base.EffectType,item_effect_base.Name,item_effect_base.Description,item_effect_base.Tooltip,item_effect_base.flag1,item_effect_base.flag2,item_effect_base.Constant1Value,item_effect_base.Constant1Stat,item_effect_base.Constant1Type,item_effect_base.Constant2Value,item_effect_base.Constant2Stat,item_effect_base.Constant2Type,item_effect_base.Var1Stat,item_effect_base.Var1Type,item_effect_base.Var2Stat,item_effect_base.Var2Type,item_effect_base.Var3Stat,item_effect_base.Var3Type,item_effect_base.Buff_Name) values (0,'none','none','none',0,0,0,'NO_STAT',0,0,'NO_STAT',0,'NO_STAT',0,'NO_STAT',0,'NO_STAT',0,'BUFF_NONE')";
            String GetIDQuery = "select last_insert_id()";
            MySqlDataAdapter mySqlDataAdapter = new MySqlDataAdapter(query, m_conn);
            MySqlDataAdapter mySqlDataGetID = new MySqlDataAdapter(GetIDQuery, m_conn);

            mySqlDataAdapter.Fill(dataTable);
            mySqlDataGetID.Fill(dataTable);

            // Get ID
            if (dataTable.Rows.Count > 0)
            {
                DataRow r = dataTable.Rows[0];

                LoadEffect(Int32.Parse(r[0].ToString()));   // Get ID of new item
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            // Display new box
            ItemEditor.ShowDialog();
        }
    }
}
