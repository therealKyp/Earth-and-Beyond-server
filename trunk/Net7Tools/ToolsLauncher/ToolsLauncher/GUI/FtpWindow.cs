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
    public partial class FtpWindow : Form
    {
        public bool m_WindowDead = false;


        public FtpWindow(string FTPUrl, string Name)
        {
            InitializeComponent();
            // Set the browse to the url
            this.Text = Name;
            FtpBrowser.Navigate(FTPUrl);
            Show();
        }

        private void FtpWindow_FormClosed(object sender, FormClosedEventArgs e)
        {
            // Call parent and get this removed for the list
            m_WindowDead = true;
        }
    }
}
