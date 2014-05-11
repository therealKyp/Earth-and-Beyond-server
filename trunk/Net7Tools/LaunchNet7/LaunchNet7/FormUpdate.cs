using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using LaunchNet7.Updateing;

namespace LaunchNet7
{
    public partial class FormUpdate : Form
    {
        public FormUpdate(Updater updater)
        {
            InitializeComponent();
            m_Updater = updater;
        }

        public Updater Updater
        {
            get { return m_Updater; }
        }
        private Updater m_Updater;

        public void ToggleDetails()
        {
            if (AreDetailsVisible)
            {
                this.Size = new Size(634, 319);
                c_Button_MoreInfo.Text = "<< Details";
                c_TextBox_Changelog.Visible = false;
                c_CheckBox_WordWrap.Visible = false;
            }
            else
            {
                this.Size = new Size(634, 500);
                c_Button_MoreInfo.Text = "Details >>";
                c_TextBox_Changelog.Visible = true;
                c_CheckBox_WordWrap.Visible = true;
            }
            AreDetailsVisible = !AreDetailsVisible;
        }

        public bool AreDetailsVisible
        {
            get { return m_AreDetailsVisible; }
            set { m_AreDetailsVisible = value; }
        }
        private bool m_AreDetailsVisible;
	

        private void FormUpdate_Load(object sender, EventArgs e)
        {
            this.Size = new Size(634, 319);
            c_PictureBox.Image = LauncherResources.Setup_48x48x32;

            c_Label_Application.Text = Updater.Name;
            c_Label_CurrentVersion.Text = Updater.CurrentVersionText;
            c_Label_NewVersion.Text = Updater.NewVersionText;

            c_LabelProgress1.Text = String.Empty;
            c_LabelProgress2.Text = String.Empty;

            c_TextBox_Changelog.DataBindings.Add("WordWrap", c_CheckBox_WordWrap, "Checked", false, DataSourceUpdateMode.OnPropertyChanged);


            if (String.IsNullOrEmpty(Updater.CurrentChangelog) == false)
            {
                c_Button_MoreInfo.Enabled = true;
                c_TextBox_Changelog.Text = Updater.CurrentChangelog;
                ToggleDetails();
            }
            else
            {
                c_CheckBox_WordWrap.Visible = false;
            }
        }

        private void c_Button_Cancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }

        private void c_Button_Update_Click(object sender, EventArgs e)
        {
            DoUpdateAsync();
        }

        private void DoUpdateAsync()
        {
            BackgroundWorker worker = new BackgroundWorker();
            worker.WorkerReportsProgress = true;
            worker.WorkerSupportsCancellation = false;
            worker.DoWork += delegate(object sender, DoWorkEventArgs e)
            {
                BackgroundWorker thisWorker = (BackgroundWorker)sender;

                m_Updater.UpdateWithWorker(thisWorker);
                if (worker.CancellationPending)
                {
                    e.Cancel = false;
                    return;
                }
            };
            worker.ProgressChanged += delegate(object sender, ProgressChangedEventArgs e)
            {
                switch (e.ProgressPercentage)
                {
                    case Updater.DisplayProgress1:
                        c_ProgressBar1.Value = ((Progress)e.UserState).ToProgressPercentage();
                        break;

                    case Updater.DisplayProgress2:
                        c_ProgressBar2.Value = ((Progress)e.UserState).ToProgressPercentage();
                        break;

                    case Updater.DisplayProgressText1:
                        c_LabelProgress1.Text = e.UserState.ToString();
                        break;

                    case Updater.DisplayProgressText2:
                        c_LabelProgress2.Text = e.UserState.ToString();
                        break;
                }
            };
            worker.RunWorkerCompleted += delegate(object sender, RunWorkerCompletedEventArgs e)
            {
                c_ProgressBar1.Visible = false;
                c_ProgressBar2.Visible = false;
                c_LabelProgress1.Text = String.Empty;
                c_LabelProgress2.Text = String.Empty;
                c_Button_Skip.Enabled = true;
                c_Button_Update.Enabled = true;

                if (e.Error != null)
                {
                    Program.LogException(e.Error);

                    string text;
                    if (e.Error is ApplicationException)
                    {
                        text = "Update failed. " + e.Error.Message;
                    }
                    else
                    {
                        text = "Update failed.\nDetails: " + e.Error.Message;
                    }
                    MessageBox.Show(this, text, Program.TitleForErrorMessages, MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
                if (e.Cancelled)
                {
                }
                else
                {
                    this.DialogResult = DialogResult.OK;
                    this.Close();
                }
            };
            c_ProgressBar1.Visible = true;
            c_ProgressBar2.Visible = true;
            c_Button_Skip.Enabled = false;
            c_Button_Update.Enabled = false;
            worker.RunWorkerAsync();
        }

        private void c_Button_Skip_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Ignore;
            this.Close();
        }

        private void c_Button_MoreInfo_Click(object sender, EventArgs e)
        {
            ToggleDetails();
        }
    }
}