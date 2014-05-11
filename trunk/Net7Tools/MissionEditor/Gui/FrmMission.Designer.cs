namespace MissionEditor.Gui
{
    partial class FrmMission
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FrmMission));
            this.guiTabControl = new System.Windows.Forms.TabControl();
            this.guiTabPageMission = new System.Windows.Forms.TabPage();
            this.guiTabPageStages = new System.Windows.Forms.TabPage();
            this.toolbar = new System.Windows.Forms.ToolStrip();
            this.guiAddBtn = new System.Windows.Forms.ToolStripButton();
            this.guiDeleteBtn = new System.Windows.Forms.ToolStripButton();
            this.guiSaveBtn = new System.Windows.Forms.ToolStripButton();
            this.guiCancelBtn = new System.Windows.Forms.ToolStripButton();
            this.guiEditBtn = new System.Windows.Forms.ToolStripButton();
            this.guiReportBtn = new System.Windows.Forms.ToolStripButton();
            this.guiSearchBtn = new System.Windows.Forms.ToolStripButton();
            this.guiSearchLbl = new System.Windows.Forms.ToolStripLabel();
            this.guiSearchTxt = new System.Windows.Forms.ToolStripTextBox();
            this.guiStatusLbl = new System.Windows.Forms.Label();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menuEdit = new System.Windows.Forms.ToolStripMenuItem();
            this.menuAdd = new System.Windows.Forms.ToolStripMenuItem();
            this.menuDelete = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.menuSave = new System.Windows.Forms.ToolStripMenuItem();
            this.menuCancel = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator5 = new System.Windows.Forms.ToolStripSeparator();
            this.menuSearch = new System.Windows.Forms.ToolStripMenuItem();
            this.guiTabControl.SuspendLayout();
            this.toolbar.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // guiTabControl
            // 
            this.guiTabControl.Controls.Add(this.guiTabPageMission);
            this.guiTabControl.Controls.Add(this.guiTabPageStages);
            this.guiTabControl.Location = new System.Drawing.Point(0, 66);
            this.guiTabControl.Name = "guiTabControl";
            this.guiTabControl.SelectedIndex = 0;
            this.guiTabControl.Size = new System.Drawing.Size(550, 546);
            this.guiTabControl.TabIndex = 1;
            // 
            // guiTabPageMission
            // 
            this.guiTabPageMission.Location = new System.Drawing.Point(4, 22);
            this.guiTabPageMission.Name = "guiTabPageMission";
            this.guiTabPageMission.Padding = new System.Windows.Forms.Padding(3);
            this.guiTabPageMission.Size = new System.Drawing.Size(542, 520);
            this.guiTabPageMission.TabIndex = 0;
            this.guiTabPageMission.Text = "Mission";
            this.guiTabPageMission.UseVisualStyleBackColor = true;
            // 
            // guiTabPageStages
            // 
            this.guiTabPageStages.Location = new System.Drawing.Point(4, 22);
            this.guiTabPageStages.Name = "guiTabPageStages";
            this.guiTabPageStages.Padding = new System.Windows.Forms.Padding(3);
            this.guiTabPageStages.Size = new System.Drawing.Size(542, 520);
            this.guiTabPageStages.TabIndex = 1;
            this.guiTabPageStages.Text = "Stages";
            this.guiTabPageStages.UseVisualStyleBackColor = true;
            // 
            // toolbar
            // 
            this.toolbar.ImageScalingSize = new System.Drawing.Size(32, 32);
            this.toolbar.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.guiAddBtn,
            this.guiDeleteBtn,
            this.guiSaveBtn,
            this.guiCancelBtn,
            this.guiEditBtn,
            this.guiReportBtn,
            this.guiSearchBtn,
            this.guiSearchLbl,
            this.guiSearchTxt});
            this.toolbar.Location = new System.Drawing.Point(0, 24);
            this.toolbar.Name = "toolbar";
            this.toolbar.Size = new System.Drawing.Size(549, 39);
            this.toolbar.TabIndex = 0;
            this.toolbar.Text = "toolStrip";
            // 
            // guiAddBtn
            // 
            this.guiAddBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.guiAddBtn.Image = ((System.Drawing.Image)(resources.GetObject("guiAddBtn.Image")));
            this.guiAddBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.guiAddBtn.Margin = new System.Windows.Forms.Padding(0, 1, 10, 2);
            this.guiAddBtn.Name = "guiAddBtn";
            this.guiAddBtn.Size = new System.Drawing.Size(36, 36);
            this.guiAddBtn.Text = "&New";
            this.guiAddBtn.ToolTipText = "Create a new record";
            this.guiAddBtn.Click += new System.EventHandler(this.onRecordAdd);
            // 
            // guiDeleteBtn
            // 
            this.guiDeleteBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.guiDeleteBtn.Enabled = false;
            this.guiDeleteBtn.Image = ((System.Drawing.Image)(resources.GetObject("guiDeleteBtn.Image")));
            this.guiDeleteBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.guiDeleteBtn.Margin = new System.Windows.Forms.Padding(0, 1, 10, 2);
            this.guiDeleteBtn.Name = "guiDeleteBtn";
            this.guiDeleteBtn.Size = new System.Drawing.Size(36, 36);
            this.guiDeleteBtn.Text = "&Delete";
            this.guiDeleteBtn.ToolTipText = "Delete the current record";
            this.guiDeleteBtn.Click += new System.EventHandler(this.onRecordDelete);
            // 
            // guiSaveBtn
            // 
            this.guiSaveBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.guiSaveBtn.Enabled = false;
            this.guiSaveBtn.Image = ((System.Drawing.Image)(resources.GetObject("guiSaveBtn.Image")));
            this.guiSaveBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.guiSaveBtn.Margin = new System.Windows.Forms.Padding(0, 1, 10, 2);
            this.guiSaveBtn.Name = "guiSaveBtn";
            this.guiSaveBtn.Size = new System.Drawing.Size(36, 36);
            this.guiSaveBtn.Text = "&Save";
            this.guiSaveBtn.ToolTipText = "Save the modifications";
            this.guiSaveBtn.Click += new System.EventHandler(this.onRecordSave);
            // 
            // guiCancelBtn
            // 
            this.guiCancelBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.guiCancelBtn.Enabled = false;
            this.guiCancelBtn.Image = ((System.Drawing.Image)(resources.GetObject("guiCancelBtn.Image")));
            this.guiCancelBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.guiCancelBtn.Margin = new System.Windows.Forms.Padding(0, 1, 10, 2);
            this.guiCancelBtn.Name = "guiCancelBtn";
            this.guiCancelBtn.Size = new System.Drawing.Size(36, 36);
            this.guiCancelBtn.Text = "&Cancel";
            this.guiCancelBtn.ToolTipText = "Cancel the modifications";
            this.guiCancelBtn.Click += new System.EventHandler(this.onRecordUndo);
            // 
            // guiEditBtn
            // 
            this.guiEditBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.guiEditBtn.Image = ((System.Drawing.Image)(resources.GetObject("guiEditBtn.Image")));
            this.guiEditBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.guiEditBtn.Margin = new System.Windows.Forms.Padding(0, 1, 10, 2);
            this.guiEditBtn.Name = "guiEditBtn";
            this.guiEditBtn.Size = new System.Drawing.Size(36, 36);
            this.guiEditBtn.Text = "toolStripButton1";
            this.guiEditBtn.ToolTipText = "Edit the XML data";
            this.guiEditBtn.Click += new System.EventHandler(this.onRecordEdit);
            // 
            // guiReportBtn
            // 
            this.guiReportBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.guiReportBtn.Image = ((System.Drawing.Image)(resources.GetObject("guiReportBtn.Image")));
            this.guiReportBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.guiReportBtn.Margin = new System.Windows.Forms.Padding(0, 1, 10, 2);
            this.guiReportBtn.Name = "guiReportBtn";
            this.guiReportBtn.Size = new System.Drawing.Size(36, 36);
            this.guiReportBtn.Text = "toolStripButton1";
            this.guiReportBtn.ToolTipText = "Report the data";
            this.guiReportBtn.Click += new System.EventHandler(this.onRecordReport);
            // 
            // guiSearchBtn
            // 
            this.guiSearchBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.guiSearchBtn.Image = ((System.Drawing.Image)(resources.GetObject("guiSearchBtn.Image")));
            this.guiSearchBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.guiSearchBtn.Margin = new System.Windows.Forms.Padding(0, 1, 10, 2);
            this.guiSearchBtn.Name = "guiSearchBtn";
            this.guiSearchBtn.Size = new System.Drawing.Size(36, 36);
            this.guiSearchBtn.Text = "btnSearch";
            this.guiSearchBtn.ToolTipText = "Search for a mission";
            this.guiSearchBtn.Click += new System.EventHandler(this.onRecordSearch);
            // 
            // guiSearchLbl
            // 
            this.guiSearchLbl.Name = "guiSearchLbl";
            this.guiSearchLbl.Size = new System.Drawing.Size(75, 36);
            this.guiSearchLbl.Text = "Search by ID:";
            // 
            // guiSearchTxt
            // 
            this.guiSearchTxt.Margin = new System.Windows.Forms.Padding(1, 0, 0, 0);
            this.guiSearchTxt.Name = "guiSearchTxt";
            this.guiSearchTxt.Size = new System.Drawing.Size(75, 39);
            this.guiSearchTxt.ToolTipText = "Search for an mission via its ID";
            this.guiSearchTxt.KeyDown += new System.Windows.Forms.KeyEventHandler(this.onRecordSearch);
            // 
            // guiStatusLbl
            // 
            this.guiStatusLbl.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.guiStatusLbl.AutoSize = true;
            this.guiStatusLbl.Location = new System.Drawing.Point(6, 618);
            this.guiStatusLbl.Name = "guiStatusLbl";
            this.guiStatusLbl.Size = new System.Drawing.Size(0, 13);
            this.guiStatusLbl.TabIndex = 2;
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.menuEdit});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(549, 24);
            this.menuStrip1.TabIndex = 3;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.F4)));
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.onExit);
            // 
            // menuEdit
            // 
            this.menuEdit.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menuAdd,
            this.menuDelete,
            this.toolStripSeparator1,
            this.menuSave,
            this.menuCancel,
            this.toolStripSeparator5,
            this.menuSearch});
            this.menuEdit.Name = "menuEdit";
            this.menuEdit.Size = new System.Drawing.Size(39, 20);
            this.menuEdit.Text = "Edit";
            // 
            // menuAdd
            // 
            this.menuAdd.Name = "menuAdd";
            this.menuAdd.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.N)));
            this.menuAdd.Size = new System.Drawing.Size(152, 22);
            this.menuAdd.Text = "Add";
            this.menuAdd.Click += new System.EventHandler(this.onRecordAdd);
            // 
            // menuDelete
            // 
            this.menuDelete.Name = "menuDelete";
            this.menuDelete.Size = new System.Drawing.Size(152, 22);
            this.menuDelete.Text = "Delete";
            this.menuDelete.Click += new System.EventHandler(this.onRecordDelete);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(149, 6);
            // 
            // menuSave
            // 
            this.menuSave.Name = "menuSave";
            this.menuSave.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
            this.menuSave.Size = new System.Drawing.Size(152, 22);
            this.menuSave.Text = "Save";
            this.menuSave.Click += new System.EventHandler(this.onRecordSave);
            // 
            // menuCancel
            // 
            this.menuCancel.Name = "menuCancel";
            this.menuCancel.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Z)));
            this.menuCancel.Size = new System.Drawing.Size(152, 22);
            this.menuCancel.Text = "Cancel";
            this.menuCancel.Click += new System.EventHandler(this.onRecordUndo);
            // 
            // toolStripSeparator5
            // 
            this.toolStripSeparator5.Name = "toolStripSeparator5";
            this.toolStripSeparator5.Size = new System.Drawing.Size(149, 6);
            // 
            // menuSearch
            // 
            this.menuSearch.Name = "menuSearch";
            this.menuSearch.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.F)));
            this.menuSearch.Size = new System.Drawing.Size(152, 22);
            this.menuSearch.Text = "Search";
            this.menuSearch.Click += new System.EventHandler(this.onRecordSearch);
            // 
            // FrmMission
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.ClientSize = new System.Drawing.Size(549, 634);
            this.Controls.Add(this.guiStatusLbl);
            this.Controls.Add(this.toolbar);
            this.Controls.Add(this.menuStrip1);
            this.Controls.Add(this.guiTabControl);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "FrmMission";
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Mission Editor";
            this.guiTabControl.ResumeLayout(false);
            this.toolbar.ResumeLayout(false);
            this.toolbar.PerformLayout();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TabControl guiTabControl;
        private System.Windows.Forms.TabPage guiTabPageMission;
        private System.Windows.Forms.TabPage guiTabPageStages;
        private System.Windows.Forms.ToolStrip toolbar;
        private System.Windows.Forms.ToolStripButton guiAddBtn;
        private System.Windows.Forms.ToolStripButton guiDeleteBtn;
        private System.Windows.Forms.ToolStripButton guiSaveBtn;
        private System.Windows.Forms.ToolStripButton guiCancelBtn;
        private System.Windows.Forms.ToolStripButton guiSearchBtn;
        private System.Windows.Forms.ToolStripLabel guiSearchLbl;
        private System.Windows.Forms.ToolStripTextBox guiSearchTxt;
        private System.Windows.Forms.Label guiStatusLbl;
        private System.Windows.Forms.ToolStripButton guiEditBtn;
        private System.Windows.Forms.ToolStripButton guiReportBtn;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem menuEdit;
        private System.Windows.Forms.ToolStripMenuItem menuAdd;
        private System.Windows.Forms.ToolStripMenuItem menuDelete;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem menuSave;
        private System.Windows.Forms.ToolStripMenuItem menuCancel;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator5;
        private System.Windows.Forms.ToolStripMenuItem menuSearch;

    }
}

