using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace N7.GUI
{
    public partial class Settings : Form
    {
        public Settings()
        {
            InitializeComponent();
        }

        private void Settings_Load(object sender, EventArgs e)
        {
            int visOptions = Convert.ToInt32(Properties.Settings.Default.KeepVisualizationSetting);
            int zoomSelection = Properties.Settings.Default.zoomSelection;

            comboBox1.SelectedIndex = visOptions;
            comboBox2.SelectedIndex = zoomSelection;
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            int visOptions = int.Parse(comboBox1.SelectedIndex.ToString());
            switch (visOptions)
            {
                case 0:
                    Properties.Settings.Default.KeepVisualizationSetting = false;
                    Properties.Settings.Default.Save();
                    break;
                case 1:
                    Properties.Settings.Default.KeepVisualizationSetting = true;
                    Properties.Settings.Default.Save();
                    break;
            }
        }

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
            int zoomSelection = int.Parse(comboBox2.SelectedIndex.ToString());
            switch (zoomSelection)
            {
                case 0:
                    Properties.Settings.Default.zoomSelection = 0;
                    Properties.Settings.Default.Save();
                    break;
                case 1:
                    Properties.Settings.Default.zoomSelection = 1;
                    Properties.Settings.Default.Save();
                    break;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
