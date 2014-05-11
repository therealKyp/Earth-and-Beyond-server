namespace LaunchNet7
{
    partial class FormMain
    {
        /// <summary>
        /// Erforderliche Designervariable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Verwendete Ressourcen bereinigen.
        /// </summary>
        /// <param name="disposing">True, wenn verwaltete Ressourcen gelöscht werden sollen; andernfalls False.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Vom Windows Form-Designer generierter Code

        /// <summary>
        /// Erforderliche Methode für die Designerunterstützung.
        /// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FormMain));
            this.c_Button_Play = new System.Windows.Forms.Button();
            this.c_Button_Cancel = new System.Windows.Forms.Button();
            this.c_ProgressBar = new System.Windows.Forms.ProgressBar();
            this.c_Status = new System.Windows.Forms.Label();
            this.c_GroupBox_Client = new System.Windows.Forms.GroupBox();
            this.c_Button_Browse = new System.Windows.Forms.Button();
            this.c_TextBox_Client = new System.Windows.Forms.TextBox();
            this.c_GroupBox_Server = new System.Windows.Forms.GroupBox();
            this.c_ServerStatus = new System.Windows.Forms.Label();
            this.c_ComboBox_Servers = new System.Windows.Forms.ComboBox();
            this.c_Button_Check = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.c_ComboBox_Emulators = new System.Windows.Forms.ComboBox();
            this.c_GroupBox_Authentication = new System.Windows.Forms.GroupBox();
            this.label1 = new System.Windows.Forms.Label();
            this.c_TextBox_Port = new System.Windows.Forms.TextBox();
            this.c_CheckBox_SecureAuthentication = new System.Windows.Forms.CheckBox();
            this.c_CheckBox_ClientDetours = new System.Windows.Forms.CheckBox();
            this.c_GroupBox_Misc = new System.Windows.Forms.GroupBox();
            this.c_CheckBox_LocalCert = new System.Windows.Forms.CheckBox();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.c_ToolStripStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.c_ToolStripStatusLabel_Filler = new System.Windows.Forms.ToolStripStatusLabel();
            this.c_ToolStripStatusLabel_Version = new System.Windows.Forms.ToolStripStatusLabel();
            this.c_Panel_WebBrowser = new System.Windows.Forms.Panel();
            this.c_WebBrowser = new System.Windows.Forms.WebBrowser();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.c_ToolStripButton_WebBrowser_Back = new System.Windows.Forms.ToolStripButton();
            this.c_ToolStripButton_WebBrowser_Forward = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.c_ToolStripButton_Browser_OpenInNewWindow = new System.Windows.Forms.ToolStripButton();
            this.c_Background = new System.Windows.Forms.PictureBox();
            this.c_ToolStripMenuItem_Main_Launcher = new System.Windows.Forms.ToolStripMenuItem();
            this.c_ToolStripMenuItem_Main_Launcher_CheckForUpdates = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.c_ToolStripMenuItem_Main_Launcher_Quit = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpPageToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.commonErrorsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpForumToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.forumsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.checkCertificateToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.iNV300ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.advancedToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.c_MenuStrip_Main = new System.Windows.Forms.MenuStrip();
            this.c_GroupBox_Client.SuspendLayout();
            this.c_GroupBox_Server.SuspendLayout();
            this.c_GroupBox_Authentication.SuspendLayout();
            this.c_GroupBox_Misc.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.c_Panel_WebBrowser.SuspendLayout();
            this.toolStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.c_Background)).BeginInit();
            this.c_MenuStrip_Main.SuspendLayout();
            this.SuspendLayout();
            // 
            // c_Button_Play
            // 
            this.c_Button_Play.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.c_Button_Play.Enabled = false;
            this.c_Button_Play.Location = new System.Drawing.Point(677, 596);
            this.c_Button_Play.Name = "c_Button_Play";
            this.c_Button_Play.Size = new System.Drawing.Size(75, 23);
            this.c_Button_Play.TabIndex = 7;
            this.c_Button_Play.Text = "&Play";
            this.c_Button_Play.UseVisualStyleBackColor = true;
            this.c_Button_Play.Click += new System.EventHandler(this.c_Button_Play_Click);
            // 
            // c_Button_Cancel
            // 
            this.c_Button_Cancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.c_Button_Cancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.c_Button_Cancel.Location = new System.Drawing.Point(12, 596);
            this.c_Button_Cancel.Name = "c_Button_Cancel";
            this.c_Button_Cancel.Size = new System.Drawing.Size(75, 23);
            this.c_Button_Cancel.TabIndex = 5;
            this.c_Button_Cancel.Text = "&Cancel";
            this.c_Button_Cancel.UseVisualStyleBackColor = true;
            this.c_Button_Cancel.Click += new System.EventHandler(this.c_Button_Cancel_Click);
            // 
            // c_ProgressBar
            // 
            this.c_ProgressBar.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.c_ProgressBar.Location = new System.Drawing.Point(93, 596);
            this.c_ProgressBar.Name = "c_ProgressBar";
            this.c_ProgressBar.Size = new System.Drawing.Size(578, 23);
            this.c_ProgressBar.TabIndex = 6;
            // 
            // c_Status
            // 
            this.c_Status.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.c_Status.AutoEllipsis = true;
            this.c_Status.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.c_Status.Location = new System.Drawing.Point(12, 567);
            this.c_Status.Margin = new System.Windows.Forms.Padding(3);
            this.c_Status.Name = "c_Status";
            this.c_Status.Padding = new System.Windows.Forms.Padding(3);
            this.c_Status.Size = new System.Drawing.Size(740, 23);
            this.c_Status.TabIndex = 4;
            this.c_Status.Text = "[Status]";
            this.c_Status.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // c_GroupBox_Client
            // 
            this.c_GroupBox_Client.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.c_GroupBox_Client.Controls.Add(this.c_Button_Browse);
            this.c_GroupBox_Client.Controls.Add(this.c_TextBox_Client);
            this.c_GroupBox_Client.Enabled = false;
            this.c_GroupBox_Client.Location = new System.Drawing.Point(12, 27);
            this.c_GroupBox_Client.Name = "c_GroupBox_Client";
            this.c_GroupBox_Client.Size = new System.Drawing.Size(740, 50);
            this.c_GroupBox_Client.TabIndex = 0;
            this.c_GroupBox_Client.TabStop = false;
            this.c_GroupBox_Client.Text = "Client";
            // 
            // c_Button_Browse
            // 
            this.c_Button_Browse.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.c_Button_Browse.Location = new System.Drawing.Point(659, 17);
            this.c_Button_Browse.Name = "c_Button_Browse";
            this.c_Button_Browse.Size = new System.Drawing.Size(75, 23);
            this.c_Button_Browse.TabIndex = 1;
            this.c_Button_Browse.Text = "&Browse";
            this.c_Button_Browse.UseVisualStyleBackColor = true;
            this.c_Button_Browse.Click += new System.EventHandler(this.c_Button_Browse_Click);
            // 
            // c_TextBox_Client
            // 
            this.c_TextBox_Client.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.c_TextBox_Client.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.c_TextBox_Client.Location = new System.Drawing.Point(9, 22);
            this.c_TextBox_Client.Name = "c_TextBox_Client";
            this.c_TextBox_Client.ReadOnly = true;
            this.c_TextBox_Client.Size = new System.Drawing.Size(644, 13);
            this.c_TextBox_Client.TabIndex = 0;
            // 
            // c_GroupBox_Server
            // 
            this.c_GroupBox_Server.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.c_GroupBox_Server.Controls.Add(this.c_ServerStatus);
            this.c_GroupBox_Server.Controls.Add(this.c_ComboBox_Servers);
            this.c_GroupBox_Server.Controls.Add(this.c_Button_Check);
            this.c_GroupBox_Server.Controls.Add(this.label3);
            this.c_GroupBox_Server.Controls.Add(this.label2);
            this.c_GroupBox_Server.Controls.Add(this.c_ComboBox_Emulators);
            this.c_GroupBox_Server.Enabled = false;
            this.c_GroupBox_Server.Location = new System.Drawing.Point(12, 83);
            this.c_GroupBox_Server.Name = "c_GroupBox_Server";
            this.c_GroupBox_Server.Size = new System.Drawing.Size(423, 81);
            this.c_GroupBox_Server.TabIndex = 1;
            this.c_GroupBox_Server.TabStop = false;
            this.c_GroupBox_Server.Text = "Server";
            // 
            // c_ServerStatus
            // 
            this.c_ServerStatus.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.c_ServerStatus.Location = new System.Drawing.Point(244, 46);
            this.c_ServerStatus.Name = "c_ServerStatus";
            this.c_ServerStatus.Size = new System.Drawing.Size(81, 21);
            this.c_ServerStatus.TabIndex = 4;
            this.c_ServerStatus.Text = "[Status]";
            this.c_ServerStatus.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // c_ComboBox_Servers
            // 
            this.c_ComboBox_Servers.FormattingEnabled = true;
            this.c_ComboBox_Servers.Location = new System.Drawing.Point(64, 46);
            this.c_ComboBox_Servers.Name = "c_ComboBox_Servers";
            this.c_ComboBox_Servers.Size = new System.Drawing.Size(174, 21);
            this.c_ComboBox_Servers.TabIndex = 3;
            this.c_ComboBox_Servers.SelectedIndexChanged += new System.EventHandler(this.c_ComboBox_Servers_SelectedIndexChanged);
            this.c_ComboBox_Servers.TextUpdate += new System.EventHandler(this.c_ComboBox_Servers_TextUpdate);
            // 
            // c_Button_Check
            // 
            this.c_Button_Check.Location = new System.Drawing.Point(331, 46);
            this.c_Button_Check.Name = "c_Button_Check";
            this.c_Button_Check.Size = new System.Drawing.Size(75, 23);
            this.c_Button_Check.TabIndex = 5;
            this.c_Button_Check.Text = "&Check";
            this.c_Button_Check.UseVisualStyleBackColor = true;
            this.c_Button_Check.Click += new System.EventHandler(this.c_Button_Check_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(29, 49);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(29, 13);
            this.label3.TabIndex = 2;
            this.label3.Text = "&Host";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(20, 22);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(38, 13);
            this.label2.TabIndex = 0;
            this.label2.Text = "&Server";
            // 
            // c_ComboBox_Emulators
            // 
            this.c_ComboBox_Emulators.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.c_ComboBox_Emulators.FormattingEnabled = true;
            this.c_ComboBox_Emulators.Location = new System.Drawing.Point(64, 19);
            this.c_ComboBox_Emulators.Name = "c_ComboBox_Emulators";
            this.c_ComboBox_Emulators.Size = new System.Drawing.Size(174, 21);
            this.c_ComboBox_Emulators.TabIndex = 1;
            this.c_ComboBox_Emulators.SelectedIndexChanged += new System.EventHandler(this.c_ComboBox_Emulators_SelectedIndexChanged);
            // 
            // c_GroupBox_Authentication
            // 
            this.c_GroupBox_Authentication.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.c_GroupBox_Authentication.Controls.Add(this.label1);
            this.c_GroupBox_Authentication.Controls.Add(this.c_TextBox_Port);
            this.c_GroupBox_Authentication.Controls.Add(this.c_CheckBox_SecureAuthentication);
            this.c_GroupBox_Authentication.Enabled = false;
            this.c_GroupBox_Authentication.Location = new System.Drawing.Point(441, 83);
            this.c_GroupBox_Authentication.Name = "c_GroupBox_Authentication";
            this.c_GroupBox_Authentication.Size = new System.Drawing.Size(163, 81);
            this.c_GroupBox_Authentication.TabIndex = 2;
            this.c_GroupBox_Authentication.TabStop = false;
            this.c_GroupBox_Authentication.Text = "Authentication";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 22);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(29, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Port:";
            // 
            // c_TextBox_Port
            // 
            this.c_TextBox_Port.Location = new System.Drawing.Point(41, 19);
            this.c_TextBox_Port.Name = "c_TextBox_Port";
            this.c_TextBox_Port.Size = new System.Drawing.Size(47, 20);
            this.c_TextBox_Port.TabIndex = 1;
            // 
            // c_CheckBox_SecureAuthentication
            // 
            this.c_CheckBox_SecureAuthentication.AutoSize = true;
            this.c_CheckBox_SecureAuthentication.Location = new System.Drawing.Point(41, 45);
            this.c_CheckBox_SecureAuthentication.Name = "c_CheckBox_SecureAuthentication";
            this.c_CheckBox_SecureAuthentication.Size = new System.Drawing.Size(114, 17);
            this.c_CheckBox_SecureAuthentication.TabIndex = 2;
            this.c_CheckBox_SecureAuthentication.Text = "Secure Auth (SSL)";
            this.c_CheckBox_SecureAuthentication.UseVisualStyleBackColor = true;
            this.c_CheckBox_SecureAuthentication.CheckedChanged += new System.EventHandler(this.c_CheckBox_SecureAuthentication_CheckedChanged);
            // 
            // c_CheckBox_ClientDetours
            // 
            this.c_CheckBox_ClientDetours.AutoSize = true;
            this.c_CheckBox_ClientDetours.Location = new System.Drawing.Point(18, 18);
            this.c_CheckBox_ClientDetours.Name = "c_CheckBox_ClientDetours";
            this.c_CheckBox_ClientDetours.Size = new System.Drawing.Size(92, 17);
            this.c_CheckBox_ClientDetours.TabIndex = 0;
            this.c_CheckBox_ClientDetours.Text = "Client Detours";
            this.c_CheckBox_ClientDetours.UseVisualStyleBackColor = true;
            // 
            // c_GroupBox_Misc
            // 
            this.c_GroupBox_Misc.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.c_GroupBox_Misc.Controls.Add(this.c_CheckBox_LocalCert);
            this.c_GroupBox_Misc.Controls.Add(this.c_CheckBox_ClientDetours);
            this.c_GroupBox_Misc.Enabled = false;
            this.c_GroupBox_Misc.Location = new System.Drawing.Point(610, 83);
            this.c_GroupBox_Misc.Name = "c_GroupBox_Misc";
            this.c_GroupBox_Misc.Size = new System.Drawing.Size(142, 81);
            this.c_GroupBox_Misc.TabIndex = 3;
            this.c_GroupBox_Misc.TabStop = false;
            this.c_GroupBox_Misc.Text = "Misc";
            // 
            // c_CheckBox_LocalCert
            // 
            this.c_CheckBox_LocalCert.AutoSize = true;
            this.c_CheckBox_LocalCert.Location = new System.Drawing.Point(18, 42);
            this.c_CheckBox_LocalCert.Name = "c_CheckBox_LocalCert";
            this.c_CheckBox_LocalCert.Size = new System.Drawing.Size(74, 17);
            this.c_CheckBox_LocalCert.TabIndex = 1;
            this.c_CheckBox_LocalCert.Text = "Local Cert";
            this.c_CheckBox_LocalCert.UseVisualStyleBackColor = true;
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.c_ToolStripStatusLabel,
            this.c_ToolStripStatusLabel_Filler,
            this.c_ToolStripStatusLabel_Version});
            this.statusStrip1.Location = new System.Drawing.Point(0, 637);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(764, 22);
            this.statusStrip1.TabIndex = 9;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // c_ToolStripStatusLabel
            // 
            this.c_ToolStripStatusLabel.Name = "c_ToolStripStatusLabel";
            this.c_ToolStripStatusLabel.Size = new System.Drawing.Size(46, 17);
            this.c_ToolStripStatusLabel.Text = "[Status]";
            this.c_ToolStripStatusLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.c_ToolStripStatusLabel.Click += new System.EventHandler(this.c_ToolStripStatusLabel_Click);
            // 
            // c_ToolStripStatusLabel_Filler
            // 
            this.c_ToolStripStatusLabel_Filler.Name = "c_ToolStripStatusLabel_Filler";
            this.c_ToolStripStatusLabel_Filler.Size = new System.Drawing.Size(703, 17);
            this.c_ToolStripStatusLabel_Filler.Spring = true;
            // 
            // c_ToolStripStatusLabel_Version
            // 
            this.c_ToolStripStatusLabel_Version.BorderSides = System.Windows.Forms.ToolStripStatusLabelBorderSides.Left;
            this.c_ToolStripStatusLabel_Version.Name = "c_ToolStripStatusLabel_Version";
            this.c_ToolStripStatusLabel_Version.Size = new System.Drawing.Size(54, 17);
            this.c_ToolStripStatusLabel_Version.Text = "[Version]";
            this.c_ToolStripStatusLabel_Version.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.c_ToolStripStatusLabel_Version.Visible = false;
            // 
            // c_Panel_WebBrowser
            // 
            this.c_Panel_WebBrowser.Controls.Add(this.c_WebBrowser);
            this.c_Panel_WebBrowser.Controls.Add(this.toolStrip1);
            this.c_Panel_WebBrowser.Location = new System.Drawing.Point(12, 173);
            this.c_Panel_WebBrowser.Name = "c_Panel_WebBrowser";
            this.c_Panel_WebBrowser.Size = new System.Drawing.Size(740, 380);
            this.c_Panel_WebBrowser.TabIndex = 12;
            this.c_Panel_WebBrowser.Visible = false;
            // 
            // c_WebBrowser
            // 
            this.c_WebBrowser.Dock = System.Windows.Forms.DockStyle.Fill;
            this.c_WebBrowser.Location = new System.Drawing.Point(0, 25);
            this.c_WebBrowser.MinimumSize = new System.Drawing.Size(20, 20);
            this.c_WebBrowser.Name = "c_WebBrowser";
            this.c_WebBrowser.Size = new System.Drawing.Size(740, 355);
            this.c_WebBrowser.TabIndex = 1;
            // 
            // toolStrip1
            // 
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.c_ToolStripButton_WebBrowser_Back,
            this.c_ToolStripButton_WebBrowser_Forward,
            this.toolStripSeparator1,
            this.c_ToolStripButton_Browser_OpenInNewWindow});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(740, 25);
            this.toolStrip1.TabIndex = 0;
            this.toolStrip1.Text = "c_ToolStrip_Webbrowser";
            // 
            // c_ToolStripButton_WebBrowser_Back
            // 
            this.c_ToolStripButton_WebBrowser_Back.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.c_ToolStripButton_WebBrowser_Back.Image = global::LaunchNet7.LauncherResources.NavBack;
            this.c_ToolStripButton_WebBrowser_Back.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.c_ToolStripButton_WebBrowser_Back.Name = "c_ToolStripButton_WebBrowser_Back";
            this.c_ToolStripButton_WebBrowser_Back.Size = new System.Drawing.Size(23, 22);
            this.c_ToolStripButton_WebBrowser_Back.Text = "Back";
            this.c_ToolStripButton_WebBrowser_Back.Click += new System.EventHandler(this.c_ToolStripButton_WebBrowser_Back_Click);
            // 
            // c_ToolStripButton_WebBrowser_Forward
            // 
            this.c_ToolStripButton_WebBrowser_Forward.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.c_ToolStripButton_WebBrowser_Forward.Image = global::LaunchNet7.LauncherResources.NavForward;
            this.c_ToolStripButton_WebBrowser_Forward.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.c_ToolStripButton_WebBrowser_Forward.Name = "c_ToolStripButton_WebBrowser_Forward";
            this.c_ToolStripButton_WebBrowser_Forward.Size = new System.Drawing.Size(23, 22);
            this.c_ToolStripButton_WebBrowser_Forward.Text = "Forward";
            this.c_ToolStripButton_WebBrowser_Forward.Click += new System.EventHandler(this.c_ToolStripButton_WebBrowser_Forward_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // c_ToolStripButton_Browser_OpenInNewWindow
            // 
            this.c_ToolStripButton_Browser_OpenInNewWindow.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.c_ToolStripButton_Browser_OpenInNewWindow.Image = ((System.Drawing.Image)(resources.GetObject("c_ToolStripButton_Browser_OpenInNewWindow.Image")));
            this.c_ToolStripButton_Browser_OpenInNewWindow.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.c_ToolStripButton_Browser_OpenInNewWindow.Name = "c_ToolStripButton_Browser_OpenInNewWindow";
            this.c_ToolStripButton_Browser_OpenInNewWindow.Size = new System.Drawing.Size(113, 22);
            this.c_ToolStripButton_Browser_OpenInNewWindow.Text = "Open in New Window";
            this.c_ToolStripButton_Browser_OpenInNewWindow.Click += new System.EventHandler(this.c_ToolStripButton_Browser_OpenInNewWindow_Click);
            // 
            // c_Background
            // 
            this.c_Background.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.c_Background.Location = new System.Drawing.Point(12, 173);
            this.c_Background.MaximumSize = new System.Drawing.Size(740, 380);
            this.c_Background.Name = "c_Background";
            this.c_Background.Size = new System.Drawing.Size(740, 380);
            this.c_Background.TabIndex = 0;
            this.c_Background.TabStop = false;
            // 
            // c_ToolStripMenuItem_Main_Launcher
            // 
            this.c_ToolStripMenuItem_Main_Launcher.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.c_ToolStripMenuItem_Main_Launcher_CheckForUpdates,
            this.toolStripMenuItem1,
            this.c_ToolStripMenuItem_Main_Launcher_Quit});
            this.c_ToolStripMenuItem_Main_Launcher.Name = "c_ToolStripMenuItem_Main_Launcher";
            this.c_ToolStripMenuItem_Main_Launcher.Size = new System.Drawing.Size(63, 20);
            this.c_ToolStripMenuItem_Main_Launcher.Text = "&Launcher";
            // 
            // c_ToolStripMenuItem_Main_Launcher_CheckForUpdates
            // 
            this.c_ToolStripMenuItem_Main_Launcher_CheckForUpdates.Name = "c_ToolStripMenuItem_Main_Launcher_CheckForUpdates";
            this.c_ToolStripMenuItem_Main_Launcher_CheckForUpdates.Size = new System.Drawing.Size(191, 22);
            this.c_ToolStripMenuItem_Main_Launcher_CheckForUpdates.Text = "Check For Updates ...";
            this.c_ToolStripMenuItem_Main_Launcher_CheckForUpdates.Click += new System.EventHandler(this.c_ToolStripMenuItem_Main_Launcher_CheckForUpdates_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(188, 6);
            // 
            // c_ToolStripMenuItem_Main_Launcher_Quit
            // 
            this.c_ToolStripMenuItem_Main_Launcher_Quit.Name = "c_ToolStripMenuItem_Main_Launcher_Quit";
            this.c_ToolStripMenuItem_Main_Launcher_Quit.Size = new System.Drawing.Size(191, 22);
            this.c_ToolStripMenuItem_Main_Launcher_Quit.Text = "&Quit";
            this.c_ToolStripMenuItem_Main_Launcher_Quit.Click += new System.EventHandler(this.c_ToolStripMenuItem_Main_File_Quit_Click);
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.helpPageToolStripMenuItem,
            this.toolStripSeparator2,
            this.checkCertificateToolStripMenuItem,
            this.iNV300ToolStripMenuItem,
            this.advancedToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(40, 20);
            this.helpToolStripMenuItem.Text = "&Help";
            // 
            // helpPageToolStripMenuItem
            // 
            this.helpPageToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.commonErrorsToolStripMenuItem,
            this.helpForumToolStripMenuItem,
            this.forumsToolStripMenuItem});
            this.helpPageToolStripMenuItem.Name = "helpPageToolStripMenuItem";
            this.helpPageToolStripMenuItem.Size = new System.Drawing.Size(179, 22);
            this.helpPageToolStripMenuItem.Text = "H&elp...";
            // 
            // commonErrorsToolStripMenuItem
            // 
            this.commonErrorsToolStripMenuItem.Name = "commonErrorsToolStripMenuItem";
            this.commonErrorsToolStripMenuItem.Size = new System.Drawing.Size(158, 22);
            this.commonErrorsToolStripMenuItem.Text = "&Common Errors";
            this.commonErrorsToolStripMenuItem.Click += new System.EventHandler(this.commonErrorsToolStripMenuItem_Click);
            // 
            // helpForumToolStripMenuItem
            // 
            this.helpForumToolStripMenuItem.Name = "helpForumToolStripMenuItem";
            this.helpForumToolStripMenuItem.Size = new System.Drawing.Size(158, 22);
            this.helpForumToolStripMenuItem.Text = "He&lp Forum";
            this.helpForumToolStripMenuItem.Click += new System.EventHandler(this.helpForumToolStripMenuItem_Click);
            // 
            // forumsToolStripMenuItem
            // 
            this.forumsToolStripMenuItem.Name = "forumsToolStripMenuItem";
            this.forumsToolStripMenuItem.Size = new System.Drawing.Size(158, 22);
            this.forumsToolStripMenuItem.Text = "&Forums";
            this.forumsToolStripMenuItem.Click += new System.EventHandler(this.forumsToolStripMenuItem_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(176, 6);
            // 
            // checkCertificateToolStripMenuItem
            // 
            this.checkCertificateToolStripMenuItem.Name = "checkCertificateToolStripMenuItem";
            this.checkCertificateToolStripMenuItem.Size = new System.Drawing.Size(179, 22);
            this.checkCertificateToolStripMenuItem.Text = "Check Certificate...";
            this.checkCertificateToolStripMenuItem.Click += new System.EventHandler(this.checkCertificateToolStripMenuItem_Click);
            // 
            // iNV300ToolStripMenuItem
            // 
            this.iNV300ToolStripMenuItem.Name = "iNV300ToolStripMenuItem";
            this.iNV300ToolStripMenuItem.Size = new System.Drawing.Size(179, 22);
            this.iNV300ToolStripMenuItem.Text = "&INV-300";
            this.iNV300ToolStripMenuItem.Click += new System.EventHandler(this.iNV300ToolStripMenuItem_Click);
            // 
            // advancedToolStripMenuItem
            // 
            this.advancedToolStripMenuItem.Name = "advancedToolStripMenuItem";
            this.advancedToolStripMenuItem.Size = new System.Drawing.Size(179, 22);
            this.advancedToolStripMenuItem.Text = "&Advanced Settings";
            this.advancedToolStripMenuItem.Click += new System.EventHandler(this.advancedToolStripMenuItem_Click);
            // 
            // c_MenuStrip_Main
            // 
            this.c_MenuStrip_Main.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.c_MenuStrip_Main.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.c_ToolStripMenuItem_Main_Launcher,
            this.helpToolStripMenuItem});
            this.c_MenuStrip_Main.Location = new System.Drawing.Point(0, 0);
            this.c_MenuStrip_Main.Name = "c_MenuStrip_Main";
            this.c_MenuStrip_Main.Size = new System.Drawing.Size(764, 24);
            this.c_MenuStrip_Main.TabIndex = 10;
            this.c_MenuStrip_Main.Text = "menuStrip1";
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.CancelButton = this.c_Button_Cancel;
            this.ClientSize = new System.Drawing.Size(764, 659);
            this.Controls.Add(this.c_Panel_WebBrowser);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.c_MenuStrip_Main);
            this.Controls.Add(this.c_GroupBox_Misc);
            this.Controls.Add(this.c_Background);
            this.Controls.Add(this.c_GroupBox_Authentication);
            this.Controls.Add(this.c_GroupBox_Server);
            this.Controls.Add(this.c_GroupBox_Client);
            this.Controls.Add(this.c_ProgressBar);
            this.Controls.Add(this.c_Status);
            this.Controls.Add(this.c_Button_Cancel);
            this.Controls.Add(this.c_Button_Play);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.c_MenuStrip_Main;
            this.MaximizeBox = false;
            this.MinimumSize = new System.Drawing.Size(770, 686);
            this.Name = "FormMain";
            this.Text = "LaunchNet7";
            this.Load += new System.EventHandler(this.FormMain_Load);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FormMain_FormClosing);
            this.c_GroupBox_Client.ResumeLayout(false);
            this.c_GroupBox_Client.PerformLayout();
            this.c_GroupBox_Server.ResumeLayout(false);
            this.c_GroupBox_Server.PerformLayout();
            this.c_GroupBox_Authentication.ResumeLayout(false);
            this.c_GroupBox_Authentication.PerformLayout();
            this.c_GroupBox_Misc.ResumeLayout(false);
            this.c_GroupBox_Misc.PerformLayout();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.c_Panel_WebBrowser.ResumeLayout(false);
            this.c_Panel_WebBrowser.PerformLayout();
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.c_Background)).EndInit();
            this.c_MenuStrip_Main.ResumeLayout(false);
            this.c_MenuStrip_Main.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox c_Background;
        private System.Windows.Forms.Button c_Button_Play;
        private System.Windows.Forms.Button c_Button_Cancel;
        private System.Windows.Forms.ProgressBar c_ProgressBar;
        private System.Windows.Forms.Label c_Status;
        private System.Windows.Forms.GroupBox c_GroupBox_Client;
        private System.Windows.Forms.TextBox c_TextBox_Client;
        private System.Windows.Forms.Button c_Button_Browse;
        private System.Windows.Forms.GroupBox c_GroupBox_Server;
        private System.Windows.Forms.ComboBox c_ComboBox_Servers;
        private System.Windows.Forms.Button c_Button_Check;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox c_ComboBox_Emulators;
        private System.Windows.Forms.Label c_ServerStatus;
        private System.Windows.Forms.GroupBox c_GroupBox_Authentication;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox c_TextBox_Port;
        private System.Windows.Forms.CheckBox c_CheckBox_SecureAuthentication;
        private System.Windows.Forms.CheckBox c_CheckBox_ClientDetours;
        private System.Windows.Forms.GroupBox c_GroupBox_Misc;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel c_ToolStripStatusLabel;
        private System.Windows.Forms.ToolStripStatusLabel c_ToolStripStatusLabel_Version;
        private System.Windows.Forms.ToolStripStatusLabel c_ToolStripStatusLabel_Filler;
        private System.Windows.Forms.Panel c_Panel_WebBrowser;
        private System.Windows.Forms.WebBrowser c_WebBrowser;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton c_ToolStripButton_Browser_OpenInNewWindow;
        private System.Windows.Forms.ToolStripButton c_ToolStripButton_WebBrowser_Back;
        private System.Windows.Forms.ToolStripButton c_ToolStripButton_WebBrowser_Forward;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem c_ToolStripMenuItem_Main_Launcher;
        private System.Windows.Forms.ToolStripMenuItem c_ToolStripMenuItem_Main_Launcher_CheckForUpdates;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem c_ToolStripMenuItem_Main_Launcher_Quit;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpPageToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem commonErrorsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpForumToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem forumsToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripMenuItem checkCertificateToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem iNV300ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem advancedToolStripMenuItem;
        private System.Windows.Forms.MenuStrip c_MenuStrip_Main;
        private System.Windows.Forms.CheckBox c_CheckBox_LocalCert;
    }
}