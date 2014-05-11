using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using CommonTools;
using CommonTools.Database;

namespace TalkTreeEditor.Reply
{
    public partial class Flag : UserControl
    {
        private FrmTalkTree m_parentForm;
        private TreeNode m_treeNode;

        public Flag(FrmTalkTree parentForm)
        {
            m_parentForm = parentForm;
            InitializeComponent();
            m_treeNode = null;
            Enumeration.AddSortedByName<TalkTreeFlag>(guiTypeCbo);
            guiTypeCbo.SelectedIndex = 0;
        }

        public void setTreeNode(TreeNode treeNode)
        {
            m_treeNode = treeNode;
            TalkNode talkNode = (TalkNode)m_treeNode.Tag;
            guiValueTxt.Text = talkNode.id;
            TalkTreeFlag talkTreeFlag;
            if (Enumeration.TryParse<TalkTreeFlag>(talkNode.text, out talkTreeFlag))
            {
                guiTypeCbo.SelectedItem = talkTreeFlag;
            }
            else
            {
                guiTypeCbo.SelectedItem = TalkTreeFlag.More;
            }
        }

        public void UpdateData()
        {
            onTypeSelected(null, null);
        }

        private void onTypeSelected(object sender, EventArgs e)
        {
            guiValueTxt.Visible = false;
            guiValueCbo.Visible = false;
            TalkTreeFlag talkTreeFlag = (TalkTreeFlag)guiTypeCbo.SelectedItem;
            switch (talkTreeFlag)
            {
                case TalkTreeFlag.Mission_Goto_Stage:
                    guiValueCbo.Items.Clear();
                    if (m_parentForm.getStages() != null && m_parentForm.getStages().Count != 0)
                    {
                        guiValueCbo.Items.AddRange(m_parentForm.getStages().ToArray());
                        //guiValueCbo.Items.Add(CodeValue.Formatted(-2, "Repeatable mission"));
                        guiValueCbo.Items.Add(new CodeValue(-2, "Mission completed & is repeatable"));
                        if (guiValueTxt.Text.Length == 0)
                        {
                            guiValueCbo.SelectedIndex = 0;
                        }
                        else
                        {
                            guiValueCbo.SelectedItem = new CodeValue(Int32.Parse(guiValueTxt.Text));
                        }
                    }
                    guiValueCbo.Visible = true;
                    break;
                default:
                    guiValueTxt.Text = "";
                    break;
            }

            if (m_treeNode != null)
            {
                TalkNode talkNode = (TalkNode)m_treeNode.Tag;
                talkNode.text = ((int)talkTreeFlag).ToString();
                talkNode.id = guiValueTxt.Text;
                m_treeNode.Text = talkNode.ToString();
            }
        }

        private void onValueSelected(object sender, EventArgs e)
        {
            if (sender.GetType().Equals(guiValueCbo.GetType()))
            {
                CodeValue codeValue = (CodeValue)guiValueCbo.SelectedItem;
                guiValueTxt.Text = codeValue.code.ToString();
            }
            else
            {
                if (m_treeNode != null)
                {
                    TalkNode talkNode = (TalkNode)m_treeNode.Tag;
                    talkNode.id = guiValueTxt.Text;
                    m_treeNode.Text = talkNode.ToString();
                }
            }
        }
    }
}
