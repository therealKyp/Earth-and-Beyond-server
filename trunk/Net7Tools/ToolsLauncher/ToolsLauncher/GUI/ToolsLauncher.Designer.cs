namespace ToolsLauncher
{
    partial class ToolsLauncher
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                this.notifyIcon1.Dispose(); //we dispose our tray icon here
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ToolsLauncher));
            this.MobEdit = new System.Windows.Forms.Button();
            this.StationEdit = new System.Windows.Forms.Button();
            this.MissionEdit = new System.Windows.Forms.Button();
            this.ItemEdit = new System.Windows.Forms.Button();
            this.SectorEdit = new System.Windows.Forms.Button();
            this.EffectEdit = new System.Windows.Forms.Button();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fTPToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadFTPToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.messengerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.settingsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.LaunchN7 = new System.Windows.Forms.Button();
            this.ToolsPatch = new System.Windows.Forms.Button();
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.Launch = new System.Windows.Forms.ToolStripMenuItem();
            this.EffectEditorMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.ItemEditorMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.MissionEditorMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.MobEditorMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.SectorEditorMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.StationEditorMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.LaunchN7Menu = new System.Windows.Forms.ToolStripMenuItem();
            this.Update = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.Minimize = new System.Windows.Forms.ToolStripMenuItem();
            this.Maximize = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.Quit = new System.Windows.Forms.ToolStripMenuItem();
            this.notifyIcon1 = new System.Windows.Forms.NotifyIcon(this.components);
            this.menuStrip1.SuspendLayout();
            this.contextMenuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // MobEdit
            // 
            this.MobEdit.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.MobEdit.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(64)))), ((int)(((byte)(0)))));
            this.MobEdit.Image = ((System.Drawing.Image)(resources.GetObject("MobEdit.Image")));
            this.MobEdit.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.MobEdit.Location = new System.Drawing.Point(12, 170);
            this.MobEdit.Name = "MobEdit";
            this.MobEdit.Size = new System.Drawing.Size(227, 40);
            this.MobEdit.TabIndex = 3;
            this.MobEdit.Text = "Mob Editor";
            this.MobEdit.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.MobEdit.UseVisualStyleBackColor = true;
            this.MobEdit.Click += new System.EventHandler(this.ClickIcon);
            // 
            // StationEdit
            // 
            this.StationEdit.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.StationEdit.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(64)))), ((int)(((byte)(0)))));
            this.StationEdit.Image = ((System.Drawing.Image)(resources.GetObject("StationEdit.Image")));
            this.StationEdit.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.StationEdit.Location = new System.Drawing.Point(12, 262);
            this.StationEdit.Name = "StationEdit";
            this.StationEdit.Size = new System.Drawing.Size(227, 40);
            this.StationEdit.TabIndex = 5;
            this.StationEdit.Text = "Station Editor";
            this.StationEdit.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.StationEdit.UseVisualStyleBackColor = true;
            this.StationEdit.Click += new System.EventHandler(this.ClickIcon);
            // 
            // MissionEdit
            // 
            this.MissionEdit.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.MissionEdit.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(64)))), ((int)(((byte)(0)))));
            this.MissionEdit.Image = ((System.Drawing.Image)(resources.GetObject("MissionEdit.Image")));
            this.MissionEdit.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.MissionEdit.Location = new System.Drawing.Point(12, 124);
            this.MissionEdit.Name = "MissionEdit";
            this.MissionEdit.Size = new System.Drawing.Size(227, 40);
            this.MissionEdit.TabIndex = 2;
            this.MissionEdit.Text = "Mission Editor";
            this.MissionEdit.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.MissionEdit.UseVisualStyleBackColor = true;
            this.MissionEdit.Click += new System.EventHandler(this.ClickIcon);
            // 
            // ItemEdit
            // 
            this.ItemEdit.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ItemEdit.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(64)))), ((int)(((byte)(0)))));
            this.ItemEdit.Image = ((System.Drawing.Image)(resources.GetObject("ItemEdit.Image")));
            this.ItemEdit.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.ItemEdit.Location = new System.Drawing.Point(12, 78);
            this.ItemEdit.Name = "ItemEdit";
            this.ItemEdit.Size = new System.Drawing.Size(227, 40);
            this.ItemEdit.TabIndex = 1;
            this.ItemEdit.Text = "Item Editor";
            this.ItemEdit.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.ItemEdit.UseVisualStyleBackColor = true;
            this.ItemEdit.Click += new System.EventHandler(this.ClickIcon);
            // 
            // SectorEdit
            // 
            this.SectorEdit.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.SectorEdit.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(64)))), ((int)(((byte)(0)))));
            this.SectorEdit.Image = ((System.Drawing.Image)(resources.GetObject("SectorEdit.Image")));
            this.SectorEdit.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.SectorEdit.Location = new System.Drawing.Point(12, 216);
            this.SectorEdit.Name = "SectorEdit";
            this.SectorEdit.Size = new System.Drawing.Size(227, 40);
            this.SectorEdit.TabIndex = 4;
            this.SectorEdit.Text = "Sector Editor";
            this.SectorEdit.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.SectorEdit.UseVisualStyleBackColor = true;
            this.SectorEdit.Click += new System.EventHandler(this.ClickIcon);
            // 
            // EffectEdit
            // 
            this.EffectEdit.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.EffectEdit.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(64)))), ((int)(((byte)(0)))));
            this.EffectEdit.Image = ((System.Drawing.Image)(resources.GetObject("EffectEdit.Image")));
            this.EffectEdit.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.EffectEdit.Location = new System.Drawing.Point(12, 32);
            this.EffectEdit.Name = "EffectEdit";
            this.EffectEdit.Size = new System.Drawing.Size(227, 40);
            this.EffectEdit.TabIndex = 0;
            this.EffectEdit.Text = "Effect Editor";
            this.EffectEdit.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.EffectEdit.UseVisualStyleBackColor = true;
            this.EffectEdit.Click += new System.EventHandler(this.ClickIcon);
            // 
            // menuStrip1
            // 
            this.menuStrip1.BackColor = System.Drawing.SystemColors.ButtonFace;
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fTPToolStripMenuItem,
            this.messengerToolStripMenuItem,
            this.settingsToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(250, 24);
            this.menuStrip1.TabIndex = 7;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fTPToolStripMenuItem
            // 
            this.fTPToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadFTPToolStripMenuItem});
            this.fTPToolStripMenuItem.Name = "fTPToolStripMenuItem";
            this.fTPToolStripMenuItem.Size = new System.Drawing.Size(39, 20);
            this.fTPToolStripMenuItem.Text = "FTP";
            // 
            // loadFTPToolStripMenuItem
            // 
            this.loadFTPToolStripMenuItem.Name = "loadFTPToolStripMenuItem";
            this.loadFTPToolStripMenuItem.Size = new System.Drawing.Size(123, 22);
            this.loadFTPToolStripMenuItem.Text = "Load FTP";
            this.loadFTPToolStripMenuItem.Click += new System.EventHandler(this.loadFTPToolStripMenuItem_Click);
            // 
            // messengerToolStripMenuItem
            // 
            this.messengerToolStripMenuItem.Name = "messengerToolStripMenuItem";
            this.messengerToolStripMenuItem.Size = new System.Drawing.Size(76, 20);
            this.messengerToolStripMenuItem.Text = "Messenger";
            this.messengerToolStripMenuItem.Click += new System.EventHandler(this.messengerToolStripMenuItem_Click);
            // 
            // settingsToolStripMenuItem
            // 
            this.settingsToolStripMenuItem.Name = "settingsToolStripMenuItem";
            this.settingsToolStripMenuItem.Size = new System.Drawing.Size(61, 20);
            this.settingsToolStripMenuItem.Text = "Settings";
            this.settingsToolStripMenuItem.Click += new System.EventHandler(this.settingsToolStripMenuItem_Click);
            // 
            // LaunchN7
            // 
            this.LaunchN7.Image = ((System.Drawing.Image)(resources.GetObject("LaunchN7.Image")));
            this.LaunchN7.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.LaunchN7.Location = new System.Drawing.Point(12, 310);
            this.LaunchN7.Name = "LaunchN7";
            this.LaunchN7.Size = new System.Drawing.Size(94, 30);
            this.LaunchN7.TabIndex = 6;
            this.LaunchN7.Text = "Launch Net7";
            this.LaunchN7.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.LaunchN7.UseVisualStyleBackColor = true;
            this.LaunchN7.Click += new System.EventHandler(this.LaunchN7_Click);
            // 
            // ToolsPatch
            // 
            this.ToolsPatch.Image = ((System.Drawing.Image)(resources.GetObject("ToolsPatch.Image")));
            this.ToolsPatch.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.ToolsPatch.Location = new System.Drawing.Point(112, 310);
            this.ToolsPatch.Name = "ToolsPatch";
            this.ToolsPatch.Size = new System.Drawing.Size(127, 30);
            this.ToolsPatch.TabIndex = 7;
            this.ToolsPatch.Text = "Check For Updates";
            this.ToolsPatch.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.ToolsPatch.UseVisualStyleBackColor = true;
            this.ToolsPatch.Click += new System.EventHandler(this.ClickIcon);
            // 
            // contextMenuStrip1
            // 
            this.contextMenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.Launch,
            this.Update,
            this.toolStripSeparator3,
            this.Minimize,
            this.Maximize,
            this.toolStripSeparator2,
            this.Quit});
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.ShowImageMargin = false;
            this.contextMenuStrip1.Size = new System.Drawing.Size(100, 126);
            // 
            // Launch
            // 
            this.Launch.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.EffectEditorMenu,
            this.ItemEditorMenu,
            this.MissionEditorMenu,
            this.MobEditorMenu,
            this.SectorEditorMenu,
            this.StationEditorMenu,
            this.toolStripSeparator1,
            this.LaunchN7Menu});
            this.Launch.Name = "Launch";
            this.Launch.Size = new System.Drawing.Size(99, 22);
            this.Launch.Text = "Launch";
            // 
            // EffectEditorMenu
            // 
            this.EffectEditorMenu.Image = ((System.Drawing.Image)(resources.GetObject("EffectEditorMenu.Image")));
            this.EffectEditorMenu.Name = "EffectEditorMenu";
            this.EffectEditorMenu.Size = new System.Drawing.Size(149, 22);
            this.EffectEditorMenu.Text = "Effect Editor";
            this.EffectEditorMenu.Click += new System.EventHandler(this.ClickIcon);
            // 
            // ItemEditorMenu
            // 
            this.ItemEditorMenu.Image = ((System.Drawing.Image)(resources.GetObject("ItemEditorMenu.Image")));
            this.ItemEditorMenu.Name = "ItemEditorMenu";
            this.ItemEditorMenu.Size = new System.Drawing.Size(149, 22);
            this.ItemEditorMenu.Text = "Item Editor";
            this.ItemEditorMenu.Click += new System.EventHandler(this.ClickIcon);
            // 
            // MissionEditorMenu
            // 
            this.MissionEditorMenu.Image = ((System.Drawing.Image)(resources.GetObject("MissionEditorMenu.Image")));
            this.MissionEditorMenu.Name = "MissionEditorMenu";
            this.MissionEditorMenu.Size = new System.Drawing.Size(149, 22);
            this.MissionEditorMenu.Text = "Mission Editor";
            this.MissionEditorMenu.Click += new System.EventHandler(this.ClickIcon);
            // 
            // MobEditorMenu
            // 
            this.MobEditorMenu.Image = ((System.Drawing.Image)(resources.GetObject("MobEditorMenu.Image")));
            this.MobEditorMenu.Name = "MobEditorMenu";
            this.MobEditorMenu.Size = new System.Drawing.Size(149, 22);
            this.MobEditorMenu.Text = "Mob Editor";
            this.MobEditorMenu.Click += new System.EventHandler(this.ClickIcon);
            // 
            // SectorEditorMenu
            // 
            this.SectorEditorMenu.Image = ((System.Drawing.Image)(resources.GetObject("SectorEditorMenu.Image")));
            this.SectorEditorMenu.Name = "SectorEditorMenu";
            this.SectorEditorMenu.Size = new System.Drawing.Size(149, 22);
            this.SectorEditorMenu.Text = "Sector Editor";
            this.SectorEditorMenu.Click += new System.EventHandler(this.ClickIcon);
            // 
            // StationEditorMenu
            // 
            this.StationEditorMenu.Image = ((System.Drawing.Image)(resources.GetObject("StationEditorMenu.Image")));
            this.StationEditorMenu.Name = "StationEditorMenu";
            this.StationEditorMenu.Size = new System.Drawing.Size(149, 22);
            this.StationEditorMenu.Text = "Station Editor";
            this.StationEditorMenu.Click += new System.EventHandler(this.ClickIcon);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(146, 6);
            // 
            // LaunchN7Menu
            // 
            this.LaunchN7Menu.Image = ((System.Drawing.Image)(resources.GetObject("LaunchN7Menu.Image")));
            this.LaunchN7Menu.Name = "LaunchN7Menu";
            this.LaunchN7Menu.Size = new System.Drawing.Size(149, 22);
            this.LaunchN7Menu.Text = "Launch Net7";
            this.LaunchN7Menu.Click += new System.EventHandler(this.LaunchN7_Click);
            // 
            // Update
            // 
            this.Update.Name = "Update";
            this.Update.Size = new System.Drawing.Size(99, 22);
            this.Update.Text = "Update";
            this.Update.Click += new System.EventHandler(this.ClickIcon);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(96, 6);
            // 
            // Minimize
            // 
            this.Minimize.Name = "Minimize";
            this.Minimize.Size = new System.Drawing.Size(99, 22);
            this.Minimize.Text = "Minimize";
            this.Minimize.Click += new System.EventHandler(this.Minimize_Click);
            // 
            // Maximize
            // 
            this.Maximize.Name = "Maximize";
            this.Maximize.Size = new System.Drawing.Size(99, 22);
            this.Maximize.Text = "Maximize";
            this.Maximize.Click += new System.EventHandler(this.Maximize_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(96, 6);
            // 
            // Quit
            // 
            this.Quit.Name = "Quit";
            this.Quit.Size = new System.Drawing.Size(99, 22);
            this.Quit.Text = "Quit";
            this.Quit.Click += new System.EventHandler(this.Quit_Click);
            // 
            // notifyIcon1
            // 
            this.notifyIcon1.ContextMenuStrip = this.contextMenuStrip1;
            this.notifyIcon1.Icon = ((System.Drawing.Icon)(resources.GetObject("notifyIcon1.Icon")));
            this.notifyIcon1.Text = "Net7 Tools Launch Pad";
            this.notifyIcon1.Visible = true;
            // 
            // ToolsLauncher
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.DarkGray;
            this.ClientSize = new System.Drawing.Size(250, 345);
            this.Controls.Add(this.ToolsPatch);
            this.Controls.Add(this.LaunchN7);
            this.Controls.Add(this.EffectEdit);
            this.Controls.Add(this.SectorEdit);
            this.Controls.Add(this.ItemEdit);
            this.Controls.Add(this.MissionEdit);
            this.Controls.Add(this.StationEdit);
            this.Controls.Add(this.MobEdit);
            this.Controls.Add(this.menuStrip1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "ToolsLauncher";
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
            this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
            this.Text = "Net7 Tools Launch Pad";
            this.Load += new System.EventHandler(this.ToolsLauncher_Load);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.ToolsLauncher_FormClosing);
            this.Resize += new System.EventHandler(this.ToolsLauncher_Resize);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.contextMenuStrip1.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button MobEdit;
        private System.Windows.Forms.Button StationEdit;
        private System.Windows.Forms.Button MissionEdit;
        private System.Windows.Forms.Button ItemEdit;
        private System.Windows.Forms.Button SectorEdit;
        private System.Windows.Forms.Button EffectEdit;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fTPToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadFTPToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem messengerToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem settingsToolStripMenuItem;
        private System.Windows.Forms.Button LaunchN7;
        private System.Windows.Forms.Button ToolsPatch;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
        private System.Windows.Forms.ToolStripMenuItem Launch;
        private System.Windows.Forms.ToolStripMenuItem EffectEditorMenu;
        private System.Windows.Forms.ToolStripMenuItem ItemEditorMenu;
        private System.Windows.Forms.ToolStripMenuItem MissionEditorMenu;
        private System.Windows.Forms.ToolStripMenuItem MobEditorMenu;
        private System.Windows.Forms.ToolStripMenuItem SectorEditorMenu;
        private System.Windows.Forms.ToolStripMenuItem StationEditorMenu;
        private System.Windows.Forms.ToolStripMenuItem Update;
        private System.Windows.Forms.ToolStripMenuItem Maximize;
        private System.Windows.Forms.ToolStripMenuItem Quit;
        private System.Windows.Forms.NotifyIcon notifyIcon1;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem LaunchN7Menu;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripMenuItem Minimize;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
    }
}

