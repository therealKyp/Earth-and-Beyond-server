using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Net.Sockets;
using System.Net;
using System.Diagnostics;
using Microsoft.Win32;
using LaunchNet7.Configuration;
using LaunchNet7.Patching;
using LaunchNet7.Updateing;
using System.Net.Security;
using System.Security.Authentication;
using System.Security.Cryptography.X509Certificates;

namespace LaunchNet7
{
    public partial class FormMain : Form
    {
        private LaunchSetting m_CurrentSetting = new LaunchSetting();
        private HostConfigurationElement m_LastSelectedServer;
        private BackgroundWorker m_Worker;
        private string m_FullErrorText;

        public FormMain()
        {
            InitializeComponent();
        }

        private void FormMain_Load(object sender, EventArgs e)
        {
            // Append Version
            this.Text += " v" + AssemblyFileInfo.Current.FileVersion.ToString(3);

            c_ToolStripStatusLabel.Text = String.Empty;
            c_Background.Image = LauncherResources.LaunchNet7;
            c_Status.Text = String.Empty;
            c_ServerStatus.Text = String.Empty;

            if (String.IsNullOrEmpty(Program.LauncherConfiguration.DefaultWebsite))
            {
                c_Background.Visible = true;
                c_Panel_WebBrowser.Visible = false;
            }
            else
            {
                DoDisplayWebpage(Program.LauncherConfiguration.DefaultWebsite);
            }

            DoRestoreWindowPosition();
            DoFillClientPath();
            DoCheckForUpdates();

            c_CheckBox_ClientDetours.Checked = Properties.Settings.Default.UseClientDetours;
            c_CheckBox_LocalCert.Checked = Properties.Settings.Default.UseLocalCert;

            if (String.IsNullOrEmpty(Properties.Settings.Default.AuthenticationPort) == false)
            {
                c_TextBox_Port.Text = Properties.Settings.Default.AuthenticationPort;
            }
            c_CheckBox_SecureAuthentication.Checked = Properties.Settings.Default.UseSecureAuthentication;
        }

        private void c_Button_Cancel_Click(object sender, EventArgs e)
        {
            if (m_Worker != null && m_Worker.IsBusy)
            {
                m_Worker.CancelAsync();
            }
            else
            {
                this.Close();
            }
        }

        private void c_Button_Browse_Click(object sender, EventArgs e)
        {
            DoBrowseClient();
        }

        private void c_ComboBox_Emulators_SelectedIndexChanged(object sender, EventArgs e)
        {
            DoDisplayHosts();
        }

        private bool TryGetSelectedEmulator(out ServerConfigurationElement server)
        {
            server = CurrentSelectedServer;
            return (server != null);
            //if (c_ComboBox_Emulators.SelectedIndex == -1)
            //{
            //    server = null;
            //    return false;
            //}
            
            //string emulatorName = c_ComboBox_Emulators.SelectedItem.ToString();
            //server = Program.Configuration.Servers[emulatorName];
            //return true;
        }

        private bool TryGetSelectedServer(out HostConfigurationElement host)
        {
            ServerConfigurationElement emulator;
            if (TryGetSelectedEmulator(out emulator) == false)
            {
                host = null;
                return false;
            }
            else if (c_ComboBox_Servers.SelectedIndex == -1 && m_LastSelectedServer == null)
            {
                host = null;
                return false;
            }
            else if (c_ComboBox_Servers.SelectedIndex == -1 && m_LastSelectedServer != null)
            {
                host = m_LastSelectedServer;
                return true;
            }

            string serverName = c_ComboBox_Servers.SelectedItem.ToString();
            host = emulator.Hosts.GetByHostName(serverName);
            return true;
        }
        private bool TryGetSelectedServer(out ServerConfigurationElement emulator, out HostConfigurationElement server)
        {
            if (TryGetSelectedEmulator(out emulator) == false)
            {
                server = null;
                return false;
            }
            else if (c_ComboBox_Servers.SelectedIndex == -1 && m_LastSelectedServer == null)
            {
                server = null;
                return false;
            }
            else if (c_ComboBox_Servers.SelectedIndex == -1 && m_LastSelectedServer != null)
            {
                server = m_LastSelectedServer;
                return true;
            }

            string serverName = c_ComboBox_Servers.SelectedItem.ToString();
            server = emulator.Hosts.GetByHostName(serverName);
            return true;
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

        private void c_ComboBox_Servers_SelectedIndexChanged(object sender, EventArgs e)
        {
            DoDisplayServerStatus();
        }

        private void c_Button_Check_Click(object sender, EventArgs e)
        {
            DoCheckServerStatus();
        }

        private void FormMain_FormClosing(object sender, FormClosingEventArgs e)
        {
            DoSaveWindowPosition();
        }

        private void c_Button_Play_Click(object sender, EventArgs e)
        {
            DoLaunch(m_CurrentSetting);
        }

        private void c_CheckBox_SecureAuthentication_CheckedChanged(object sender, EventArgs e)
        {
            HostConfigurationElement serverConfig;
            ServerConfigurationElement emulatorConfig;
            if (TryGetSelectedServer(out emulatorConfig, out serverConfig) == false) return;

            if (c_CheckBox_SecureAuthentication.Checked && serverConfig.SupportsSecureAuthentication)
            {
                c_TextBox_Port.Text = serverConfig.SecureAuthenticationPort.ToString();
            }
            else
            {
                c_TextBox_Port.Text = serverConfig.AuthenticationPort.ToString();
            }
        }

        private void c_ToolStripStatusLabel_Click(object sender, EventArgs e)
        {
            if (String.IsNullOrEmpty(m_FullErrorText)) return;
            MessageBox.Show(this, "Following errors occured while checking for updates:\n\n" + m_FullErrorText, Program.TitleForErrorMessages, MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        private void c_ToolStripMenuItem_Main_File_Quit_Click(object sender, EventArgs e)
        {
            DoQuit();
        }

        private void c_ToolStripMenuItem_Main_Help_CheckForUpdates_Click(object sender, EventArgs e)
        {
            DoCheckForUpdates();
        }

        private void c_ToolStripButton_Browser_OpenInNewWindow_Click(object sender, EventArgs e)
        {
            Process.Start(c_WebBrowser.Url.ToString());
        }

        private void c_ToolStripButton_WebBrowser_Back_Click(object sender, EventArgs e)
        {
            c_WebBrowser.GoBack();
        }

        private void c_ToolStripButton_WebBrowser_Forward_Click(object sender, EventArgs e)
        {
            c_WebBrowser.GoForward();
        }

        private void c_ToolStripMenuItem_Main_Launcher_CheckForUpdates_Click(object sender, EventArgs e)
        {
            DoCheckForUpdates();
        }

        private ServerConfigurationElement CurrentSelectedServer
        {
            get
            {
                if (c_ComboBox_Emulators.SelectedIndex == -1 || c_ComboBox_Emulators.Items.Count == 0)
                {
                    return null;
                }
                else
                {
                    return c_ComboBox_Emulators.SelectedItem as ServerConfigurationElement;
                }
            }
        }

        private void DoLaunch(LaunchSetting setting)
        {
            if (String.IsNullOrEmpty(m_CurrentSetting.ClientPath))
            {
                DisplayInformalMessage("The location of the client.exe could not be determined automatically. Please specify the location manually.");
                return;
            }

            int authenticationPort;
            if (Int32.TryParse(c_TextBox_Port.Text, out authenticationPort) == false)
            {
                DisplayWarningMessage("Please enter a valid port number.");
                c_TextBox_Port.SelectAll();
                c_TextBox_Port.Focus();
                return;
            }
            else if (authenticationPort <= 0 || authenticationPort > ushort.MaxValue)
            {
                DisplayWarningMessage("Please enter a valid port number between 1 and 65535.");
                c_TextBox_Port.SelectAll();
                c_TextBox_Port.Focus();
                return;
            }

            try
            {
                // Add Settings
                HostConfigurationElement serverConfig;
                ServerConfigurationElement emulatorConfig;
                if (TryGetSelectedServer(out emulatorConfig, out serverConfig) == false)
                {
                    DisplayWarningMessage("Please select an emulator/server.");
                    return;
                }

                setting.UseSecureAuthentication = c_CheckBox_SecureAuthentication.Checked;
                setting.AuthenticationPort = authenticationPort;

                if (c_ComboBox_Servers.SelectedIndex == -1)
                {
                    setting.Hostname = c_ComboBox_Servers.Text;
                }
                else
                {
                    setting.Hostname = serverConfig.Hostname;
                }

                setting.LaunchName = emulatorConfig.GetLaunchName();
                setting.UseClientDetours = c_CheckBox_ClientDetours.Checked;
                setting.UseLocalCert = c_CheckBox_LocalCert.Checked;
               

                // Save State
                Properties.Settings.Default.UseClientDetours = c_CheckBox_ClientDetours.Checked;
                Properties.Settings.Default.LastEmulatorName = emulatorConfig.Name;
                Properties.Settings.Default.UseLocalCert = c_CheckBox_LocalCert.Checked;

                if (c_ComboBox_Servers.Items.Count > 0 && c_ComboBox_Servers.SelectedIndex == -1)
                {
                    Properties.Settings.Default.LastServerName = c_ComboBox_Servers.Text;
                }
                else
                {
                    Properties.Settings.Default.LastServerName = serverConfig.Hostname;
                }
                Properties.Settings.Default.UseSecureAuthentication = c_CheckBox_SecureAuthentication.Checked;
                Properties.Settings.Default.AuthenticationPort = c_TextBox_Port.Text;
                Properties.Settings.Default.Save();

                // Launcher Server/Client
                Launcher launcher = new Launcher(setting);
                launcher.Launch();

                // Close Launcher
                this.Close();
            }
            catch (Exception e)
            {
                DisplayErrorMessage("Error while launching client.", e);
            }
        }

        private void DoDisplayServerStatus()
        {
            try
            {
                ServerConfigurationElement server;
                HostConfigurationElement host;
                if (TryGetSelectedServer(out server, out host) == false)
                {
                    c_ServerStatus.Text = String.Empty;
                    return;
                }

                if (host.SupportsSecureAuthentication)
                {
                    c_CheckBox_SecureAuthentication.Enabled = true;
                    c_CheckBox_SecureAuthentication.Checked = true;
                    c_TextBox_Port.Text = host.SecureAuthenticationPort.ToString();
                }
                else
                {
                    c_CheckBox_SecureAuthentication.Enabled = false;
                    c_CheckBox_SecureAuthentication.Checked = false;
                    c_TextBox_Port.Text = host.AuthenticationPort.ToString();
                }

                if (server.IsSinglePlayer)
                {
                    c_ServerStatus.Text = "READY";
                    c_Button_Check.Enabled = false;
                }
                else
                {
                    c_ServerStatus.Text = "CHECKING";
                    c_Button_Check.Enabled = true;
                    DoCheckServerStatus(host);
                }

                m_LastSelectedServer = host;
            }
            catch (Exception e)
            {
                Program.LogException(e);
                DisplayErrorMessage("Could not retrieve server status.", e);
                c_ServerStatus.Text = String.Empty;
            }
        }

        //TODO: check via UDP, simply do a MVAS login.
        private void DoCheckServerStatus()
        {
            //ServerConfigurationElement server = CurrentSelectedServer;
            //if (server == null) return;

            //HostConfigurationElement host;
            //if (TryGetSelectedServer(out server, out host) == false || server.IsSinglePlayer) return;
            //DoCheckServerStatus(host);

            //Don't allow people to keep spamming the server by repeatedly pressing the 'check' button if server has already reported 'ONLINE'.
            if (c_ServerStatus.Text != "ONLINE")
            {
                string hostName = c_ComboBox_Servers.Text;
                int port;
                if (Int32.TryParse(c_TextBox_Port.Text, out port) == false)
                {
                    c_ServerStatus.Text = String.Empty;
                    return;
                }
                port = 3809; // check Proxy listener now
                DoCheckServerStatus(hostName, port);
            }
        }
        private void DoCheckServerStatus(HostConfigurationElement host)
        {
            string hostName = host.Hostname;
            int port = 3809; //check the proxy listener, not SSL anymore.
            DoCheckServerStatus(hostName, port);
        }
        private void DoCheckServerStatus(string hostname, int port)
        {
            BackgroundWorker worker = new BackgroundWorker();
            worker.WorkerReportsProgress = false;
            worker.WorkerSupportsCancellation = false;
            worker.DoWork += delegate(object sender, DoWorkEventArgs e)
            {
                try
                {
                    Dns.GetHostAddresses(hostname);
                }
                catch
                {
                    e.Result = 1;
                }

                using (TcpClient client = new TcpClient())
                {
                    try
                    {
                        client.Connect(hostname, port);
                        e.Result = 0;
                    }
                    catch
                    {
                        e.Result = 2;
                    }
                    finally
                    {
                        client.Close();
                    }
                }
            };
            worker.ProgressChanged += delegate(object sender, ProgressChangedEventArgs e)
            {

            };
            worker.RunWorkerCompleted += delegate(object sender, RunWorkerCompletedEventArgs e)
            {
                if (e.Error != null)
                {
                    DisplayErrorMessage("Error on checking server status.", e.Error);
                    c_ServerStatus.Text = "OFFLINE";
                    return;
                }
                if (e.Cancelled)
                {
                    // TODO: Doing things on cancelation
                }
                else
                {
                    switch ((int)e.Result)
                    {
                        case 0:
                            c_ServerStatus.Text = "ONLINE";
                            break;

                        case 1:
                            c_ServerStatus.Text = "ERROR";
                            DisplayWarningMessage("Hostname cannot be resolved.");
                            c_ComboBox_Servers.SelectAll();
                            c_ComboBox_Servers.Focus();
                            break;

                        case 2:
                            c_ServerStatus.Text = "OFFLINE";
                            break;

                        default:
                            c_ServerStatus.Text = "???";
                            break;
                    }
                }
            };
            c_ServerStatus.Text = "CHECKING";
            worker.RunWorkerAsync();
        }
        
        private void DoDisplayHosts()
        {
            try
            {
                ServerConfigurationElement server = CurrentSelectedServer;
                if (server == null) return;

                // Lock Auth-Settings
                c_GroupBox_Authentication.Enabled = server.EnableAdvancedSettings;
                c_GroupBox_Misc.Enabled = server.EnableAdvancedSettings;

                int? selectedIndex = null;

                c_ComboBox_Servers.Items.Clear();
                foreach (HostConfigurationElement host in server.Hosts)
                {
                    int index = c_ComboBox_Servers.Items.Add(host.Hostname);
                    if (String.Equals(host.Hostname, Properties.Settings.Default.LastServerName, StringComparison.InvariantCultureIgnoreCase))
                    {
                        selectedIndex = index;
                    }
                }
                if (c_ComboBox_Servers.Items.Count > 0)
                {
                    c_ComboBox_Servers.SelectedIndex = selectedIndex.GetValueOrDefault(0);
                }
                else
                {
                    c_ComboBox_Servers.SelectedIndex = -1;
                }
            }
            catch (Exception e)
            {
                DisplayErrorMessage("Could not retrieve servers for emulator.", e);
            }
        }

        private void DoDisplayWebpage(string url)
        {
            c_Background.Visible = false;
            c_Panel_WebBrowser.Visible = true;
            try
            {
                c_WebBrowser.Navigate(url);
            }
            catch (Exception e)
            {
                Program.LogException(e);
                c_Background.Visible = true;
                c_Panel_WebBrowser.Visible = false;
                MessageBox.Show(this, e.Message, Program.TitleForErrorMessages, MessageBoxButtons.OK, MessageBoxIcon.Error);
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
                foreach (AutoUpdateTaskConfigurationElement updateTask in Program.LauncherConfiguration.AutoUpdate.AutoUpdateTasks)
                {
                    Updater updater = new Updater();
                    updater.Name = updateTask.Name;

                    // Get the client's dir
                    if (updater.Name == "Client")
                    {
                        updater.RootPath = Directory.GetParent(Path.GetDirectoryName(c_TextBox_Client.Text)).Parent.FullName + "\\";
                    }

                    updater.BaseUrl = updateTask.BaseUrl;
                    updater.VersionFileName = updateTask.VersionFileName;
                    updater.FileListFileName = updateTask.FileListName;
                    updater.VersionCompareMode = updateTask.VersionCompareMode;
                    updater.RestartOnUpdate = updateTask.RestartOnUpdate;
                    updater.ChangelogFileName = updateTask.ChangelogFileName;
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
                    c_Status.Text = e.UserState.ToString();
                }
                else
                {
                    c_ProgressBar.Value = e.ProgressPercentage;
                }
            };
            worker.RunWorkerCompleted += delegate(object sender, RunWorkerCompletedEventArgs e)
            {
                DoEnableUI();

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
                    DoFillServerList();

                    m_FullErrorText = String.Empty;

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
                                c_ToolStripStatusLabel_Version.Text = version;
                                c_ToolStripStatusLabel_Version.Visible = (version.Length > 0);
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

                        if (errorCount > 0)
                        {
                            m_FullErrorText = errorText.ToString();
                            c_ToolStripStatusLabel.Text = "Errors occured while checking for updates (click here for details)";
                            c_ToolStripStatusLabel.Image = LauncherResources.Warning_16x16x32;
                        }
                        else
                        {
                            m_FullErrorText = null;
                            c_ToolStripStatusLabel.Text = String.Empty;
                            c_ToolStripStatusLabel.Image = null;
                        }
                    }
                }

                DoDisplayDefaultStatus();
            };
            m_Worker = worker;
            DoDisableUI();
            worker.RunWorkerAsync();
        }

        private void DoRunSelfUpdate(string replaceFileName)
        {
            string temporaryFileName = Path.GetTempFileName();
            string temporaryExeFileName = Path.ChangeExtension(temporaryFileName, ".exe");
            if (File.Exists(temporaryExeFileName)) File.Delete(temporaryExeFileName);
            File.Move(temporaryFileName, temporaryExeFileName);
            File.WriteAllBytes(temporaryExeFileName, LauncherResources.ExeUpdater);
            Process.Start(temporaryExeFileName, String.Format("-exeFileName \"{0}\" -replaceFileName \"{1}\" -waitForPid {2}",
                Path.GetFullPath(Application.ExecutablePath),
                Path.GetFullPath(replaceFileName),
                Process.GetCurrentProcess().Id));
        }

        private void DoDisplayDefaultStatus()
        {
            c_Status.Text = "Please select a server and hit play.";
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

        private void DoRestoreWindowPosition()
        {
            Properties.Settings.Default.Properties["FormMainPosition"].DefaultValue = Point.Empty;
            Point position = (Point)Properties.Settings.Default.FormMainPosition;
            if (position != Point.Empty)
            {
                this.Top = position.Y;
                this.Left = position.X;
            }
        }

        private void DoFillServerList()
        {
            int? selectedIndex = null;

            c_ComboBox_Emulators.Items.Clear();
            foreach (ServerConfigurationElement server in Program.LauncherConfiguration.Servers)
            {
                int index = c_ComboBox_Emulators.Items.Add(server);
                if (String.Equals(server.Name, Properties.Settings.Default.LastEmulatorName, StringComparison.InvariantCultureIgnoreCase))
                {
                    selectedIndex = index;
                }
            }
            if (c_ComboBox_Emulators.Items.Count > 0)
            {
                c_ComboBox_Emulators.SelectedIndex = selectedIndex.GetValueOrDefault(0);
            }
            else
            {
                c_ComboBox_Emulators.SelectedIndex = -1;
            }
        }

        private void DoFillClientPath()
        {
            string clientPath = Properties.Settings.Default.ClientPath;
            if (String.IsNullOrEmpty(clientPath) || File.Exists(clientPath) == false)
            {
                clientPath = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles), "EA GAMES\\Earth & Beyond\\release\\client.exe");
                if (File.Exists(clientPath))
                {
                    c_TextBox_Client.Text = clientPath;
                }
                else
                {
                    clientPath = Path.GetFullPath(Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles), "..\\Program Files\\EA GAMES\\Earth & Beyond\\release\\client.exe"));
                    if (File.Exists(clientPath))
                    {
                        c_TextBox_Client.Text = clientPath;
                    }
                    else
                    {
                        DoBrowseClient();
                    }
                }
            }
            else
            {
                c_TextBox_Client.Text = clientPath;
            }

            if (String.IsNullOrEmpty(clientPath) == false)
            {
                m_CurrentSetting.ClientPath = clientPath;
                Properties.Settings.Default.ClientPath = clientPath;
                Properties.Settings.Default.Save();
            }
        }

        private void DoDisableUI()
        {
            c_MenuStrip_Main.Enabled = false;
            c_GroupBox_Authentication.Enabled = false;
            c_GroupBox_Misc.Enabled = false;
            c_GroupBox_Client.Enabled = false;
            c_GroupBox_Server.Enabled = false;
            c_Button_Play.Enabled = false;
        }

        private void DoEnableUI()
        {
            c_MenuStrip_Main.Enabled = true;
            c_GroupBox_Authentication.Enabled = true;
            c_GroupBox_Misc.Enabled = true;
            c_GroupBox_Client.Enabled = true;
            c_GroupBox_Server.Enabled = true;
            c_Button_Play.Enabled = true;
        }

        private void DoQuit()
        {
            this.Close();
        }

        private void DoBrowseClient()
        {
            string currentDirectory = Environment.CurrentDirectory;
            try
            {
                using (OpenFileDialog dlg = new OpenFileDialog())
                {
                    dlg.Title = "Select Earth & Beyond Client";
                    dlg.Filter = "Earth & Beyond Client (client.exe)|client.exe|All Files (*.*)|*.*";
                    dlg.Multiselect = false;
                    if (dlg.ShowDialog() == DialogResult.OK)
                    {
                        c_TextBox_Client.Text = dlg.FileName;
                        Properties.Settings.Default.ClientPath = dlg.FileName;
                        Properties.Settings.Default.Save();
                        m_CurrentSetting.ClientPath = dlg.FileName;
                    }
                }
            }
            finally
            {
                Environment.CurrentDirectory = currentDirectory;
            }
        }

        private void DoSaveWindowPosition()
        {
            Properties.Settings.Default.FormMainPosition = new Point(Left, Top);
            Properties.Settings.Default.Save();
        }

        private void c_ComboBox_Servers_TextUpdate(object sender, EventArgs e)
        {
            c_ServerStatus.Text = String.Empty;
            c_Button_Check.Enabled = true;
        }

        private void commonErrorsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("IExplore.exe", "http://forum.enb-emulator.com/index.php?s=&showtopic=3610&view=findpost&p=34465");
        }

        private void helpForumToolStripMenuItem_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("IExplore.exe", "http://forum.enb-emulator.com/index.php?showforum=175");
        }

        private void forumsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("IExplore.exe", "http://forum.enb-emulator.com/");
        }

        private void iNV300ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ServicePointManager.ServerCertificateValidationCallback = OnValidationCallback;

            HttpWebRequest req = (HttpWebRequest)WebRequest.Create("https://" + c_ComboBox_Servers.Text + ":" + c_TextBox_Port.Text + "/certificate.html");

            HttpWebResponse response = (HttpWebResponse)req.GetResponse();
        }

        public bool OnValidationCallback(object sender, X509Certificate cert, X509Chain chain, SslPolicyErrors errors)
        {
            if (errors.ToString() != "None")
            {
                MessageBox.Show("Please click yes on the next dialog box.\nThis will allow us to install the Net7 certificate.", "Certificate Install",
                    MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

                try
                {
                    X509Store Certificate = new X509Store(StoreName.Root);

                    X509Certificate2 cert2 = new X509Certificate2(cert);

                    Certificate.Open(OpenFlags.ReadWrite);

                    // Add Certificate
                    Certificate.Add(cert2);
                    Certificate.Close();
                }
                catch (Exception e)
                {
                    MessageBox.Show("Error installing certificate: " + e.ToString());
                }
            }
            else
            {
                MessageBox.Show("Certificate is installed!");
            }

            // Remove this message
            ServicePointManager.ServerCertificateValidationCallback = null;

            return true;
        }

        private void checkCertificateToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ServicePointManager.ServerCertificateValidationCallback = OnValidationCallback;

            HttpWebRequest req = (HttpWebRequest)WebRequest.Create("https://" + c_ComboBox_Servers.Text + ":" + c_TextBox_Port.Text + "/certificate.html");

            HttpWebResponse response = (HttpWebResponse)req.GetResponse();
        }

        private void advancedToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AdvancedSettings Settings = new AdvancedSettings();

            Settings.ShowDialog();
        }

        private void LocalCert_CheckedChanged(object sender, EventArgs e)
        {

        }
    }
}