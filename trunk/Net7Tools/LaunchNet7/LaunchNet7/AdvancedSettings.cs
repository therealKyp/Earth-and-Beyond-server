using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using LaunchNet7.Configuration;

namespace LaunchNet7
{
    public partial class AdvancedSettings : Form
    {
        public AdvancedSettings()
        {
            InitializeComponent();

            StreamReader tr = new StreamReader(Path.GetDirectoryName(Properties.Settings.Default.ClientPath) + "\\authlogin.log");
            AuthLog.Text = tr.ReadToEnd();
            tr.Close();
        }

        private void ForceUpdate_Click(object sender, EventArgs e)
        {
            foreach (AutoUpdateTaskConfigurationElement updateTask in Program.LauncherConfiguration.AutoUpdate.AutoUpdateTasks)
            {
                File.Delete(updateTask.VersionFileName);
            }
        }
    }
}
