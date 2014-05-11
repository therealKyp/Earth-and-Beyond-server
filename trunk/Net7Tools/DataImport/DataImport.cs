using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using CommonTools.Database;

namespace DataImport
{
    public partial class DataImport : Form
    {

        public DataImport()
        {
            InitializeComponent();
            this.Text = this.Text + " " + CommonTools.Gui.LoginData.ApplicationVersion;
        }

        /// <summary>
        /// Display the form and load the mission specified via setMissionXml()
        /// </summary>
        /// <param name="e"></param>
        protected override void OnShown(EventArgs e)
        {
            base.OnShown(e);
            CommonTools.Enumeration.AddSortedByName<Net7.Tables>(guiTableCbo);
            if (guiTableCbo.Items.Count != 0)
            {
                guiTableCbo.SelectedIndex = 0;
            }
        }

        private void onBrowse(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            if (dlg.ShowDialog() == DialogResult.OK)
            {
                guiFileTxt.Text = dlg.FileName;
            }
        }

        private void onImport(object sender, EventArgs e)
        {
            DB.Instance.importValues((Net7.Tables)guiTableCbo.SelectedItem, guiFileTxt.Text);
        }

        private void onClose(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
