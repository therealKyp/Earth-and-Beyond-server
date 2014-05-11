using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace TalkTreeEditor.Reply
{
    public partial class Branch : UserControl
    {
        private FrmTalkTree m_parentForm;
        private TreeNode m_treeNode;

        public Branch(FrmTalkTree parentForm)
        {
            m_parentForm = parentForm;
            InitializeComponent();
            m_treeNode = null;
        }

        public void setTreeNode(TreeNode treeNode)
        {
            m_treeNode = treeNode;
            TalkNode talkNode = (TalkNode)m_treeNode.Tag;
            String id = talkNode.id;
            String text = talkNode.text;
            guiGotoTxt.Text = id;
            guiReplyTxt.Text = text;
        }

        private void onTextChanged(object sender, EventArgs e)
        {
            if (m_treeNode != null)
            {
                TalkNode talkNode = (TalkNode)m_treeNode.Tag;
                talkNode.text = guiReplyTxt.Text;
                talkNode.id = guiGotoTxt.Text;
                m_treeNode.Text = talkNode.ToString();
            }
        }

        private void onGoto(object sender, EventArgs e)
        {
            m_parentForm.onNodeGoto(guiGotoTxt.Text);
        }

    }
}
