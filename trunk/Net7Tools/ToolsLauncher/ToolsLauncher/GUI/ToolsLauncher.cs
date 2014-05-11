using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;
using System.IO;

using ToolsLauncher.Updateing;

namespace ToolsLauncher
{
    public partial class ToolsLauncher : Form
    {
        List<FileLinkList> m_LinkList = new List<FileLinkList>();
        List<FtpWindow> m_FtpWindows = new List<FtpWindow>();

        // List of all the FTP locations
        List<FtpAddy> m_FtpLocation = new List<FtpAddy>();
        List<ToolStripMenuItem> FtpUrlList = new List<ToolStripMenuItem>();

        IRCMessenger IrcMsg = new IRCMessenger();

        private BackgroundWorker m_Worker;

        public ToolsLauncher()
        {
            InitializeComponent();

            // This is a list of the exe files with the button links
            m_LinkList.Add(new FileLinkList("N7 Mob Editor.exe", MobEdit));
            m_LinkList.Add(new FileLinkList("Station Tools.exe", StationEdit));
            m_LinkList.Add(new FileLinkList("Net7 Sector Editor.exe", SectorEdit));
            m_LinkList.Add(new FileLinkList("Net7 Item Editor.exe", ItemEdit));
            m_LinkList.Add(new FileLinkList("Effect Editor.exe", EffectEdit));
            m_LinkList.Add(new FileLinkList("MissionEditor.exe", MissionEdit));
            m_LinkList.Add(new FileLinkList("ToolsPatcher", ToolsPatch));
            m_LinkList.Add(new FileLinkList("N7 Mob Editor.exe", MobEditorMenu));
            m_LinkList.Add(new FileLinkList("Station Tools.exe", StationEditorMenu));
            m_LinkList.Add(new FileLinkList("Net7 Sector Editor.exe", SectorEditorMenu));
            m_LinkList.Add(new FileLinkList("Net7 Item Editor.exe", ItemEditorMenu));
            m_LinkList.Add(new FileLinkList("Effect Editor.exe", EffectEditorMenu));
            m_LinkList.Add(new FileLinkList("MissionEditor.exe", MissionEditorMenu));
            m_LinkList.Add(new FileLinkList("ToolsPatcher", Update));

            // FTP Locations
            m_FtpLocation.Add(new FtpAddy("ftp://ENBCC:Edp0Wp4Lf8PYVkMH74Cnz6G68eKB7Cp240pLPEoum5bJqIxpKeIKVOYa9nq98499CmbRAR5VLNwsAAdyPi7TGEQEmSveWXXk@net-7.org/", "Content Dev FTP"));

            // List FTP's on Menu bar
            CreateFtpLinks();
        }

        // Find an Exe File for an Object
        private String FindExeFile(Object Object)
        {
            if (Object.GetType() == typeof(Button))
            {
                foreach (FileLinkList LList in m_LinkList)
                {
                    if (LList.BObject == Object)
                    {
                        return LList.ExeFile;
                    }
                }
            }
            else if (Object.GetType() == typeof(ToolStripMenuItem))
            {
                foreach (FileLinkList LList in m_LinkList)
                {
                    if (LList.MObject == Object)
                    {
                        return LList.ExeFile;
                    }
                }
            }
            return "";
        }

        private void ClickIcon(object sender, EventArgs e)
        {
            Object SenderObject = null;

            if (sender.GetType() == typeof(Button))
            {
                SenderObject = (Button)sender;
            }
            else if(sender.GetType() == typeof(ToolStripMenuItem))
            {
                SenderObject = (ToolStripMenuItem)sender;
            }

            ProcessStartInfo info = new ProcessStartInfo();
            String ExeFile = FindExeFile(SenderObject);

            if (ExeFile == "ToolsPatcher")
            {
                // Dont launch a program
                DoCheckForUpdates();
                ExeFile = "";
            }

            // Make sure we have a valid exe file
            if (ExeFile != "")
            {
                info.WorkingDirectory = Directory.GetCurrentDirectory();
                info.FileName = Path.Combine(info.WorkingDirectory, ExeFile);

                if (File.Exists(info.FileName))
                {
                    // Start the process
                    Process.Start(info);
                }
            }
        }

#region FTP Clicking
        private void CreateFtpLinks()
        {
            foreach (FtpAddy FtpItem in m_FtpLocation)
            {
                // Create and setup proerties
                ToolStripMenuItem FtpLink = new ToolStripMenuItem(FtpItem.Name);
                FtpLink.Size = new System.Drawing.Size(152, 22);
                FtpLink.Click += new System.EventHandler(this.loadFTPToolStripMenuItem_Click);

                FtpUrlList.Add(FtpLink);
            }

            // Display list
            loadFTPToolStripMenuItem.DropDownItems.AddRange(FtpUrlList.ToArray());
        }

        private void loadFTPToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ToolStripMenuItem Item = (ToolStripMenuItem)sender;

            // See if any windows closed
            FTPWindowClose();


            // Find the url from the name
            foreach(FtpAddy ftpLoc in m_FtpLocation)
            {
                if (Item.Text == ftpLoc.Name)
                {
                    // Open FTP window
                    m_FtpWindows.Add(new FtpWindow(ftpLoc.Location, ftpLoc.Name));
                }
            }
        }

        public void FTPWindowClose()
        {
            List<FtpWindow> DeadWindows = new List<FtpWindow>();

            // remove window from list
            foreach (FtpWindow ftpWin in m_FtpWindows)
            {
                if (ftpWin.m_WindowDead)
                {
                    // find all dead windows
                    DeadWindows.Add(ftpWin);
                    break;
                }
            }
            // clear all dead windows
            foreach (FtpWindow ftpwin in DeadWindows)
            {
                m_FtpWindows.Remove(ftpwin);
            }
        }
#endregion

        private void messengerToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // Show IRC Messenger
            if (IrcMsg == null)
            {
                IrcMsg = new IRCMessenger();
            }
            IrcMsg.Show();
        }

        private void ToolsLauncher_FormClosing(object sender, FormClosingEventArgs e)
        {
            IrcMsg.Close();
            foreach (FtpWindow FtpWin in m_FtpWindows)
            {
                FtpWin.Close();
            }
        }

        private void ToolsLauncher_Resize(object sender, System.EventArgs e)
        {
            if (FormWindowState.Minimized == WindowState)
            {
                this.ShowInTaskbar = false;
                Hide();
                Maximize.Enabled = true;
                Minimize.Enabled = false;
                this.WindowState = FormWindowState.Normal;
            }
        }

        private void notifyIcon1_DoubleClick(object sender,
                                     System.EventArgs e)
        {
            Show();
            this.ShowInTaskbar = true;
            WindowState = FormWindowState.Normal;
        }

        private void Quit_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void Maximize_Click(object sender, EventArgs e)
        {
            Show();
            this.ShowInTaskbar = true;
            WindowState = FormWindowState.Normal;
            Maximize.Enabled = false;
            Minimize.Enabled = true;
        }

        private void Minimize_Click(object sender, EventArgs e)
        {
            if (FormWindowState.Minimized == WindowState)
                this.ShowInTaskbar = false;
            Hide();
            Maximize.Enabled = true;
            Minimize.Enabled = false;
        }

        private void ToolsLauncher_Load(object sender, EventArgs e)
        {
            int screenHeight = Screen.PrimaryScreen.WorkingArea.Height;
            int screenWidth = Screen.PrimaryScreen.WorkingArea.Width;

            int finalY = (screenHeight - this.Size.Height) - 20;
            int finalX =(screenWidth - this.Size.Width) - 20;

            this.Location = new Point(finalX, finalY);

            // Check for updates on logs
            DoCheckForUpdates();
        }

        private void settingsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SettingsFrm s1 = new SettingsFrm();
            s1.Show(this);
        }

        private void LaunchN7_Click(object sender, EventArgs e)
        {
            String LaunchNet7Path = Properties.Settings.Default.LaunchNet7Path;

            ProcessStartInfo info = new ProcessStartInfo();
            info.WorkingDirectory = LaunchNet7Path+"/";
            info.FileName = Path.Combine(LaunchNet7Path, "LaunchNet7.exe");

            Console.Out.WriteLine(info.WorkingDirectory);

            if (File.Exists(info.FileName))
            {
                // Start the process
                Process.Start(info);
            }
            else
            {
                MessageBox.Show("Sorry we could not find LaunchNet7.exe"
                            + " please set your path in the settings.");
            }
        }


        private void DoCheckForUpdates()
        {
            BackgroundWorker worker = new BackgroundWorker();
            worker.WorkerReportsProgress = true;
            worker.WorkerSupportsCancellation = true;
            worker.DoWork += delegate(object sender, DoWorkEventArgs e)
            {
                BackgroundWorker thisWorker = (BackgroundWorker)sender;

                thisWorker.ReportProgress(0, "Checking Configuration");
                List<Updater> updaterList = new List<Updater>();

                // Updater Task
                {
                    Updater updater = new Updater();

                    updater.BaseUrl = "http://toolspatch.net-7.org/";
                    updater.VersionFileName = "Version.txt";
                    updater.FileListFileName = "Files.txt";
                    updater.VersionCompareMode = VersionCompareMode.SameText;
                    updater.RestartOnUpdate = true;
                    updater.ChangelogFileName = "";
                    updaterList.Add(updater);
                }

                Progress progress = new Progress(0, updaterList.Count * 3);
                foreach (Updater updater in updaterList)
                {
                    if (thisWorker.CancellationPending)
                    {
                        e.Cancel = true;
                        return;
                    }

                    thisWorker.ReportProgress(progress.ToProgressPercentage());
                    thisWorker.ReportProgress(0, "Checking Updates For " + updater.Name);
                    switch (updater.CheckForUpdates())
                    {
                        case UpdateCheckResult.DownloadRequired:
                        case UpdateCheckResult.UpdateAvailable:
                            {
                                progress.Increment();
                                thisWorker.ReportProgress(progress.ToProgressPercentage());
                                thisWorker.ReportProgress(0, String.Format("Downloading Changelog for '{0}'", updater.Name));
                                updater.DownloadPatchlog();

                                progress.Increment();
                                thisWorker.ReportProgress(progress.ToProgressPercentage());
                                thisWorker.ReportProgress(0, "Updating " + updater.Name);
                                DialogResult? result = (DialogResult?)this.Invoke(new DoDisplayUpdateCallback(DoDisplayUpdate), updater);
                                if (result.GetValueOrDefault() == DialogResult.Cancel)
                                {
                                    e.Cancel = true;
                                    return;
                                }

                                progress.Increment();
                                thisWorker.ReportProgress(progress.ToProgressPercentage());

                                if (updater.SelfUpdateDetected && updater.SelfUpdateItem.CheckStatus != UpdateCheckStatus.Ok)
                                {
                                    DoRunSelfUpdate(updater.SelfUpdateItem.TargetFileName);
                                    e.Cancel = true;
                                    return;
                                }
                            }
                            break;

                        default:
                            progress.Increment();
                            progress.Increment();
                            progress.Increment();
                            thisWorker.ReportProgress(progress.ToProgressPercentage());
                            break;
                    }
                }
                e.Result = updaterList;
            };
            worker.ProgressChanged += delegate(object sender, ProgressChangedEventArgs e)
            {
                if (e.UserState != null)
                {
                    //c_Status.Text = e.UserState.ToString();
                }
                else
                {
                    //c_ProgressBar.Value = e.ProgressPercentage;
                }
            };
            worker.RunWorkerCompleted += delegate(object sender, RunWorkerCompletedEventArgs e)
            {
                if (e.Error != null)
                {
                    DisplayErrorMessage("Error on client validation.", e.Error);
                    this.Close();
                }
                else if (e.Cancelled)
                {
                    this.Close();
                }
                else
                {
                    List<Updater> updaters = e.Result as List<Updater>;
                    if (updaters != null)
                    {
                        int errorCount = 0;
                        StringBuilder errorText = new StringBuilder();
                        foreach (Updater updater in updaters)
                        {
                            if (String.Equals(updater.Name, "Net7", StringComparison.InvariantCultureIgnoreCase))
                            {
                                string version = String.Empty;
                                if (String.IsNullOrEmpty(updater.CurrentVersionText) == false)
                                {
                                    version += "Current Version: " + updater.CurrentVersionText;
                                }
                                if (String.Equals(updater.CurrentVersionText, updater.NewVersionText, StringComparison.InvariantCultureIgnoreCase) == false &&
                                    String.IsNullOrEmpty(updater.NewVersionText) == false)
                                {
                                    if (version.Length > 0) version += ", ";
                                    version += "Server Version: " + updater.NewVersionText;
                                }
                            }

                            if (updater.CheckResult != UpdateCheckResult.Timeout &&
                                updater.CheckResult != UpdateCheckResult.Error) continue;

                            errorCount++;

                            if (errorText.Length > 0) errorText.Append("\n\n");
                            if (updater.CheckResult == UpdateCheckResult.Timeout)
                            {
                                errorText.Append("Timeout while checking updates for '" + updater.Name + "'");
                            }
                            else if (updater.CheckResult == UpdateCheckResult.Error)
                            {
                                errorText.Append("Error while checking updates for '" + updater.Name + "'");
                            }

                            if (updater.Error != null)
                            {
                                errorText.Append("\nDetails: " + updater.Error.Message);
                            }
                        }
                    }
                }
            };
            m_Worker = worker;
            worker.RunWorkerAsync();
        }

        private delegate DialogResult DoDisplayUpdateCallback(Updater updater);
        private DialogResult DoDisplayUpdate(Updater updater)
        {
            if (updater == null) return DialogResult.None;

            using (FormUpdate dlg = new FormUpdate(updater))
            {
                return dlg.ShowDialog(this);
            }
        }

        private void DoRunSelfUpdate(string replaceFileName)
        {
            string temporaryFileName = Path.GetTempFileName();
            string temporaryExeFileName = Path.ChangeExtension(temporaryFileName, ".exe");
            if (File.Exists(temporaryExeFileName)) File.Delete(temporaryExeFileName);
            File.Move(temporaryFileName, temporaryExeFileName);
            File.WriteAllBytes(temporaryExeFileName, Resources.ExeUpdater);
            System.Diagnostics.Process.Start(temporaryExeFileName, String.Format("-exeFileName \"{0}\" -replaceFileName \"{1}\" -waitForPid {2}",
                Path.GetFullPath(Application.ExecutablePath),
                Path.GetFullPath(replaceFileName),
                System.Diagnostics.Process.GetCurrentProcess().Id));
        }

        private void DisplayErrorMessage(string message, Exception e)
        {
            MessageBox.Show(this, message + "\nDetails: " + e.Message, Program.Title + " - Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        private void DisplayWarningMessage(string message)
        {
            MessageBox.Show(this, message, Program.Title + " - Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
        }

        private void DisplayInformalMessage(string message)
        {
            MessageBox.Show(this, message, Program.Title + " - Information", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }
    }
}
