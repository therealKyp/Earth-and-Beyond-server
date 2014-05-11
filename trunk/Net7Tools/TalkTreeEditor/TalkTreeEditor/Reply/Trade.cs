using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace TalkTreeEditor.Reply
{
    public partial class Trade : UserControl
    {
        private TreeNode m_treeNode;

        public Trade()
        {
            InitializeComponent();
            m_treeNode = null;
        }

        public void setTreeNode(TreeNode treeNode)
        {
            m_treeNode = treeNode;
            TalkNode talkNode = (TalkNode)m_treeNode.Tag;
            guiQuantityTxt.Text = talkNode.id;
        }

        private void onTextChanged(object sender, EventArgs e)
        {
            if (m_treeNode != null)
            {
                TalkNode talkNode = (TalkNode)m_treeNode.Tag;
                talkNode.id = guiQuantityTxt.Text;
                m_treeNode.Text = talkNode.ToString();
            }
        }
    }
}
