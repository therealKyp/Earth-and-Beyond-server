using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace ToolsLauncher
{
    public partial class SettingsFrm : Form
    {
        public SettingsFrm()
        {
            InitializeComponent();
        }

        private void Settings_Load(object sender, EventArgs e)
        {
            String LaunchNet7Path = Properties.Settings.Default.LaunchNet7Path;

            textBox1.Text = LaunchNet7Path;
        }

        private void button1_Click(object sender, EventArgs e)
        {

            Properties.Settings.Default.LaunchNet7Path = textBox1.Text;
            Properties.Settings.Default.Save();

            this.Close();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            DialogResult result = this.folderBrowserDialog1.ShowDialog();
            if (result == DialogResult.OK)
            {
                string selectedFolder = this.folderBrowserDialog1.SelectedPath;
                textBox1.Text = selectedFolder;
            }
        }
    }
}
