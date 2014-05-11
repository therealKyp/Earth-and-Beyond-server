using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace TalkTreeEditor
{
    public partial class EditTalkTree : Form
    {

        private string m_TreeData;

        public EditTalkTree()
        {
            InitializeComponent();
        }

        public void SetTalkTreeData(string Data)
        {
            m_TreeData = Data;
            TalkTreeData.Text = Data;
        }

        public string GetTalkTreeData()
        {
            return m_TreeData;
        }

        private void SaveTalkTree_Click(object sender, EventArgs e)
        {
            m_TreeData = TalkTreeData.Text;
            this.Close();
        }
    }
}