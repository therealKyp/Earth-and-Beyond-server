using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace ToolsLauncher
{
    public partial class PrivateMessage : Form
    {
        public String m_PrivateNick;
        public String m_MyNick;
        public bool m_Closing = false;

        private IRCMessenger m_ParentMsg = null;

        public PrivateMessage()
        {
            InitializeComponent();
        }

        private void SendMessage()
        {
            ReciveBox.AppendText(m_MyNick + ": " + SendBox.Text + "\n");
            m_ParentMsg.SendMsg(m_PrivateNick, SendBox.Text);
            SendBox.Clear();
            // Auto scroll
            ReciveBox.SelectionStart = ReciveBox.TextLength;
            ReciveBox.Focus();
            ReciveBox.ScrollToCaret();
            SendBox.Focus();
        }

        public void SetParent(IRCMessenger Parnt)
        {
            m_ParentMsg = Parnt;
        }

        public void ReciveMessage(String msg)
        {
            ReciveBox.AppendText(m_PrivateNick + ": " + msg + "\n");
            // Auto scroll
            ReciveBox.SelectionStart = ReciveBox.TextLength;
            ReciveBox.Focus();
            ReciveBox.ScrollToCaret();
            SendBox.Focus();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (SendBox.Text != "\n")
            {
                SendMessage();
            }
            else
            {
                SendBox.Clear();
            }
        }

        private void PrivateMessage_FormClosing(object sender, FormClosingEventArgs e)
        {
            m_Closing = true;
        }

        private void SendBox_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (SendBox.Text != "\n")
            {
                if (e.KeyChar == '\r')
                {
                    SendBox.Text = SendBox.Text.TrimEnd('\n');
                    SendMessage();
                }
            }
            else
            {
                SendBox.Clear();
            }
        }

        private void PrivateMessage_Enter(object sender, EventArgs e)
        {
            SendBox.Focus();
        }

    }
}
