namespace N7
{
    partial class mainFrm
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
            TD.SandDock.DockingRules dockingRules2 = new TD.SandDock.DockingRules();
            TD.SandDock.DockingRules dockingRules1 = new TD.SandDock.DockingRules();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(mainFrm));
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newSectorToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.systemToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.sectorToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.sectorObjectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveSectorToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.settingsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.sandDockManager1 = new TD.SandDock.SandDockManager();
            this.sectorDock = new TD.SandDock.DockableWindow();
            this.treeView1 = new System.Windows.Forms.TreeView();
            this.PropertiesDock = new TD.SandDock.DockableWindow();
            this.toolboxDock = new TD.SandDock.DockableWindow();
            this.toolStrip2 = new System.Windows.Forms.ToolStrip();
            this.toolStripButton3 = new System.Windows.Forms.ToolStripButton();
            this.toolStripButton4 = new System.Windows.Forms.ToolStripButton();
            this.deleteSelected = new System.Windows.Forms.ToolStripButton();
            this.toolStripButton1 = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.dockContainer2 = new TD.SandDock.DockContainer();
            this.dockContainer3 = new TD.SandDock.DockContainer();
            this.sectorOptionsDock = new TD.SandDock.DockableWindow();
            this.dockableWindow1 = new TD.SandDock.DockableWindow();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.id = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.name = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.base_asset_id = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.type = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.dockContainer1 = new TD.SandDock.DockContainer();
            this.newToolbarClick = new System.Windows.Forms.ToolStripButton();
            this.save = new System.Windows.Forms.ToolStripButton();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.SectorTab = new TD.SandDock.TabbedDocument();
            this.pCanvas1 = new UMD.HCIL.Piccolo.PCanvas();
            this.documentContainer1 = new TD.SandDock.DocumentContainer();
            this.menuStrip1.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.sectorDock.SuspendLayout();
            this.toolboxDock.SuspendLayout();
            this.toolStrip2.SuspendLayout();
            this.dockContainer2.SuspendLayout();
            this.dockContainer3.SuspendLayout();
            this.dockableWindow1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.dockContainer1.SuspendLayout();
            this.toolStrip1.SuspendLayout();
            this.SectorTab.SuspendLayout();
            this.documentContainer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.editToolStripMenuItem,
            this.helpToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1016, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newSectorToolStripMenuItem,
            this.saveSectorToolStripMenuItem,
            this.toolStripSeparator1,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(35, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // newSectorToolStripMenuItem
            // 
            this.newSectorToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.systemToolStripMenuItem,
            this.sectorToolStripMenuItem,
            this.sectorObjectToolStripMenuItem});
            this.newSectorToolStripMenuItem.Image = global::N7.Properties.Resources.document_new;
            this.newSectorToolStripMenuItem.Name = "newSectorToolStripMenuItem";
            this.newSectorToolStripMenuItem.Size = new System.Drawing.Size(109, 22);
            this.newSectorToolStripMenuItem.Text = "New";
            // 
            // systemToolStripMenuItem
            // 
            this.systemToolStripMenuItem.Name = "systemToolStripMenuItem";
            this.systemToolStripMenuItem.Size = new System.Drawing.Size(151, 22);
            this.systemToolStripMenuItem.Text = "System";
            this.systemToolStripMenuItem.Click += new System.EventHandler(this.systemToolStripMenuItem_Click);
            // 
            // sectorToolStripMenuItem
            // 
            this.sectorToolStripMenuItem.Name = "sectorToolStripMenuItem";
            this.sectorToolStripMenuItem.Size = new System.Drawing.Size(151, 22);
            this.sectorToolStripMenuItem.Text = "Sector";
            this.sectorToolStripMenuItem.Click += new System.EventHandler(this.sectorToolStripMenuItem_Click);
            // 
            // sectorObjectToolStripMenuItem
            // 
            this.sectorObjectToolStripMenuItem.Name = "sectorObjectToolStripMenuItem";
            this.sectorObjectToolStripMenuItem.Size = new System.Drawing.Size(151, 22);
            this.sectorObjectToolStripMenuItem.Text = "Sector Object";
            this.sectorObjectToolStripMenuItem.Click += new System.EventHandler(this.sectorObjectToolStripMenuItem_Click);
            // 
            // saveSectorToolStripMenuItem
            // 
            this.saveSectorToolStripMenuItem.Image = global::N7.Properties.Resources.media_floppy_smaller;
            this.saveSectorToolStripMenuItem.Name = "saveSectorToolStripMenuItem";
            this.saveSectorToolStripMenuItem.Size = new System.Drawing.Size(109, 22);
            this.saveSectorToolStripMenuItem.Text = "Save";
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(106, 6);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(109, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // editToolStripMenuItem
            // 
            this.editToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.settingsToolStripMenuItem});
            this.editToolStripMenuItem.Name = "editToolStripMenuItem";
            this.editToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.editToolStripMenuItem.Text = "Edit";
            // 
            // settingsToolStripMenuItem
            // 
            this.settingsToolStripMenuItem.Name = "settingsToolStripMenuItem";
            this.settingsToolStripMenuItem.Size = new System.Drawing.Size(124, 22);
            this.settingsToolStripMenuItem.Text = "Settings";
            this.settingsToolStripMenuItem.Click += new System.EventHandler(this.settingsToolStripMenuItem_Click);
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(40, 20);
            this.helpToolStripMenuItem.Text = "Help";
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(114, 22);
            this.aboutToolStripMenuItem.Text = "About";
            this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1});
            this.statusStrip1.Location = new System.Drawing.Point(0, 712);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(1016, 22);
            this.statusStrip1.TabIndex = 2;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.BackColor = System.Drawing.SystemColors.Control;
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(42, 17);
            this.toolStripStatusLabel1.Text = "Ready.";
            // 
            // sandDockManager1
            // 
            this.sandDockManager1.AllowMiddleButtonClosure = false;
            this.sandDockManager1.BorderStyle = TD.SandDock.Rendering.BorderStyle.RaisedThin;
            this.sandDockManager1.DockSystemContainer = this;
            this.sandDockManager1.EnableEmptyEnvironment = true;
            this.sandDockManager1.OwnerForm = this;
            this.sandDockManager1.Renderer = new TD.SandDock.Rendering.Office2003Renderer();
            // 
            // sectorDock
            // 
            this.sectorDock.AllowClose = false;
            this.sectorDock.BorderStyle = TD.SandDock.Rendering.BorderStyle.RaisedThin;
            this.sectorDock.Controls.Add(this.treeView1);
            dockingRules2.AllowDockBottom = true;
            dockingRules2.AllowDockLeft = true;
            dockingRules2.AllowDockRight = true;
            dockingRules2.AllowDockTop = true;
            dockingRules2.AllowFloat = false;
            dockingRules2.AllowTab = false;
            this.sectorDock.DockingRules = dockingRules2;
            this.sectorDock.Guid = new System.Guid("692b9042-35f3-4b24-b4ac-253564a3d5b1");
            this.sectorDock.Location = new System.Drawing.Point(0, 25);
            this.sectorDock.Name = "sectorDock";
            this.sectorDock.Size = new System.Drawing.Size(244, 282);
            this.sectorDock.TabIndex = 0;
            this.sectorDock.Text = "Universe";
            // 
            // treeView1
            // 
            this.treeView1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.treeView1.Location = new System.Drawing.Point(1, 1);
            this.treeView1.Name = "treeView1";
            this.treeView1.Size = new System.Drawing.Size(242, 280);
            this.treeView1.TabIndex = 0;
            this.treeView1.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.treeView1_AfterSelect);
            // 
            // PropertiesDock
            // 
            this.PropertiesDock.AllowClose = false;
            this.PropertiesDock.BorderStyle = TD.SandDock.Rendering.BorderStyle.RaisedThin;
            this.PropertiesDock.Guid = new System.Guid("04d72d9b-c92e-437a-ae5c-625399e065c7");
            this.PropertiesDock.Location = new System.Drawing.Point(0, 359);
            this.PropertiesDock.Name = "PropertiesDock";
            this.PropertiesDock.Size = new System.Drawing.Size(244, 281);
            this.PropertiesDock.TabIndex = 1;
            this.PropertiesDock.Text = "Properties";
            // 
            // toolboxDock
            // 
            this.toolboxDock.AllowClose = false;
            this.toolboxDock.BorderStyle = TD.SandDock.Rendering.BorderStyle.RaisedThin;
            this.toolboxDock.Collapsed = true;
            this.toolboxDock.Controls.Add(this.toolStrip2);
            this.toolboxDock.Guid = new System.Guid("b2270f15-5913-41e1-bbf4-3fa1b89e11d2");
            this.toolboxDock.Location = new System.Drawing.Point(4, 25);
            this.toolboxDock.Name = "toolboxDock";
            this.toolboxDock.Size = new System.Drawing.Size(114, 447);
            this.toolboxDock.TabIndex = 4;
            this.toolboxDock.Text = "ToolBox";
            // 
            // toolStrip2
            // 
            this.toolStrip2.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.toolStrip2.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripButton3,
            this.toolStripButton4,
            this.deleteSelected,
            this.toolStripButton1,
            this.toolStripSeparator2});
            this.toolStrip2.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.Table;
            this.toolStrip2.Location = new System.Drawing.Point(1, 1);
            this.toolStrip2.Name = "toolStrip2";
            this.toolStrip2.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
            this.toolStrip2.Size = new System.Drawing.Size(112, 99);
            this.toolStrip2.TabIndex = 0;
            this.toolStrip2.Text = "toolStrip2";
            // 
            // toolStripButton3
            // 
            this.toolStripButton3.Image = global::N7.Properties.Resources.wb_arrow;
            this.toolStripButton3.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton3.Name = "toolStripButton3";
            this.toolStripButton3.Size = new System.Drawing.Size(53, 20);
            this.toolStripButton3.Text = "Move";
            this.toolStripButton3.Click += new System.EventHandler(this.toolStripButton3_Click);
            // 
            // toolStripButton4
            // 
            this.toolStripButton4.Image = global::N7.Properties.Resources.view_fullscreen;
            this.toolStripButton4.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton4.Name = "toolStripButton4";
            this.toolStripButton4.Size = new System.Drawing.Size(45, 20);
            this.toolStripButton4.Text = "Pan";
            this.toolStripButton4.Click += new System.EventHandler(this.toolStripButton4_Click);
            // 
            // deleteSelected
            // 
            this.deleteSelected.Image = global::N7.Properties.Resources.process_stop;
            this.deleteSelected.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.deleteSelected.Name = "deleteSelected";
            this.deleteSelected.Size = new System.Drawing.Size(102, 20);
            this.deleteSelected.Text = "Delete Selected";
            this.deleteSelected.Click += new System.EventHandler(this.deleteSelected_Click);
            // 
            // toolStripButton1
            // 
            this.toolStripButton1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton1.Name = "toolStripButton1";
            this.toolStripButton1.Size = new System.Drawing.Size(23, 4);
            this.toolStripButton1.Text = "toolStripButton1";
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 23);
            // 
            // dockContainer2
            // 
            this.dockContainer2.ContentSize = 244;
            this.dockContainer2.Controls.Add(this.sectorDock);
            this.dockContainer2.Controls.Add(this.PropertiesDock);
            this.dockContainer2.Dock = System.Windows.Forms.DockStyle.Left;
            this.dockContainer2.LayoutSystem = new TD.SandDock.SplitLayoutSystem(new System.Drawing.SizeF(250F, 400F), System.Windows.Forms.Orientation.Horizontal, new TD.SandDock.LayoutSystemBase[] {
            ((TD.SandDock.LayoutSystemBase)(new TD.SandDock.ControlLayoutSystem(new System.Drawing.SizeF(250F, 400F), new TD.SandDock.DockControl[] {
                        ((TD.SandDock.DockControl)(this.sectorDock))}, this.sectorDock))),
            ((TD.SandDock.LayoutSystemBase)(new TD.SandDock.ControlLayoutSystem(new System.Drawing.SizeF(250F, 400F), new TD.SandDock.DockControl[] {
                        ((TD.SandDock.DockControl)(this.PropertiesDock))}, this.PropertiesDock)))});
            this.dockContainer2.Location = new System.Drawing.Point(0, 49);
            this.dockContainer2.Manager = this.sandDockManager1;
            this.dockContainer2.Name = "dockContainer2";
            this.dockContainer2.Size = new System.Drawing.Size(248, 663);
            this.dockContainer2.TabIndex = 4;
            // 
            // dockContainer3
            // 
            this.dockContainer3.ContentSize = 164;
            this.dockContainer3.Controls.Add(this.sectorOptionsDock);
            this.dockContainer3.Controls.Add(this.dockableWindow1);
            this.dockContainer3.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.dockContainer3.LayoutSystem = new TD.SandDock.SplitLayoutSystem(new System.Drawing.SizeF(250F, 400F), System.Windows.Forms.Orientation.Vertical, new TD.SandDock.LayoutSystemBase[] {
            ((TD.SandDock.LayoutSystemBase)(new TD.SandDock.ControlLayoutSystem(new System.Drawing.SizeF(250F, 400F), new TD.SandDock.DockControl[] {
                        ((TD.SandDock.DockControl)(this.sectorOptionsDock)),
                        ((TD.SandDock.DockControl)(this.dockableWindow1))}, this.sectorOptionsDock)))});
            this.dockContainer3.Location = new System.Drawing.Point(248, 544);
            this.dockContainer3.Manager = this.sandDockManager1;
            this.dockContainer3.Name = "dockContainer3";
            this.dockContainer3.Size = new System.Drawing.Size(768, 168);
            this.dockContainer3.TabIndex = 5;
            // 
            // sectorOptionsDock
            // 
            this.sectorOptionsDock.AllowClose = false;
            this.sectorOptionsDock.AutoScroll = true;
            dockingRules1.AllowDockBottom = true;
            dockingRules1.AllowDockLeft = true;
            dockingRules1.AllowDockRight = true;
            dockingRules1.AllowDockTop = true;
            dockingRules1.AllowFloat = true;
            dockingRules1.AllowTab = true;
            this.sectorOptionsDock.DockingRules = dockingRules1;
            this.sectorOptionsDock.Guid = new System.Guid("4b4734ea-f7f1-4bc0-9469-5a8dc25849d9");
            this.sectorOptionsDock.Location = new System.Drawing.Point(0, 29);
            this.sectorOptionsDock.Name = "sectorOptionsDock";
            this.sectorOptionsDock.Size = new System.Drawing.Size(768, 116);
            this.sectorOptionsDock.TabIndex = 5;
            this.sectorOptionsDock.Text = "Sector Visualization Options";
            // 
            // dockableWindow1
            // 
            this.dockableWindow1.Controls.Add(this.dataGridView1);
            this.dockableWindow1.Guid = new System.Guid("da5ea1f3-6c21-4fd0-8452-ee732e407aa8");
            this.dockableWindow1.Location = new System.Drawing.Point(0, 29);
            this.dockableWindow1.Name = "dockableWindow1";
            this.dockableWindow1.Size = new System.Drawing.Size(768, 116);
            this.dockableWindow1.TabIndex = 5;
            this.dockableWindow1.Text = "Sector Object Table";
            // 
            // dataGridView1
            // 
            this.dataGridView1.AllowUserToAddRows = false;
            this.dataGridView1.AllowUserToDeleteRows = false;
            this.dataGridView1.AllowUserToOrderColumns = true;
            this.dataGridView1.AllowUserToResizeRows = false;
            dataGridViewCellStyle1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.dataGridView1.AlternatingRowsDefaultCellStyle = dataGridViewCellStyle1;
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.id,
            this.name,
            this.base_asset_id,
            this.type});
            this.dataGridView1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dataGridView1.Location = new System.Drawing.Point(0, 0);
            this.dataGridView1.MultiSelect = false;
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.ReadOnly = true;
            this.dataGridView1.RowHeadersVisible = false;
            this.dataGridView1.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dataGridView1.Size = new System.Drawing.Size(768, 116);
            this.dataGridView1.TabIndex = 0;
            this.dataGridView1.RowEnter += new System.Windows.Forms.DataGridViewCellEventHandler(this.datagridview1_rowEnter);
            // 
            // id
            // 
            this.id.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.ColumnHeader;
            this.id.HeaderText = "Object ID";
            this.id.Name = "id";
            this.id.ReadOnly = true;
            this.id.Width = 77;
            // 
            // name
            // 
            this.name.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.name.HeaderText = "Name";
            this.name.Name = "name";
            this.name.ReadOnly = true;
            // 
            // base_asset_id
            // 
            this.base_asset_id.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.ColumnHeader;
            this.base_asset_id.HeaderText = "Asset ID";
            this.base_asset_id.Name = "base_asset_id";
            this.base_asset_id.ReadOnly = true;
            this.base_asset_id.Width = 72;
            // 
            // type
            // 
            this.type.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.ColumnHeader;
            this.type.HeaderText = "Type";
            this.type.Name = "type";
            this.type.ReadOnly = true;
            this.type.Width = 56;
            // 
            // dockContainer1
            // 
            this.dockContainer1.ContentSize = 114;
            this.dockContainer1.Controls.Add(this.toolboxDock);
            this.dockContainer1.Dock = System.Windows.Forms.DockStyle.Right;
            this.dockContainer1.LayoutSystem = new TD.SandDock.SplitLayoutSystem(new System.Drawing.SizeF(250F, 400F), System.Windows.Forms.Orientation.Horizontal, new TD.SandDock.LayoutSystemBase[] {
            ((TD.SandDock.LayoutSystemBase)(new TD.SandDock.ControlLayoutSystem(new System.Drawing.SizeF(250F, 400F), new TD.SandDock.DockControl[] {
                        ((TD.SandDock.DockControl)(this.toolboxDock))}, this.toolboxDock)))});
            this.dockContainer1.Location = new System.Drawing.Point(898, 49);
            this.dockContainer1.Manager = this.sandDockManager1;
            this.dockContainer1.Name = "dockContainer1";
            this.dockContainer1.Size = new System.Drawing.Size(118, 495);
            this.dockContainer1.TabIndex = 7;
            // 
            // newToolbarClick
            // 
            this.newToolbarClick.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.newToolbarClick.Image = global::N7.Properties.Resources.document_new;
            this.newToolbarClick.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.newToolbarClick.Name = "newToolbarClick";
            this.newToolbarClick.Size = new System.Drawing.Size(23, 22);
            this.newToolbarClick.Text = "New";
            this.newToolbarClick.Click += new System.EventHandler(this.newToolbarClick_Click);
            // 
            // save
            // 
            this.save.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.save.Image = global::N7.Properties.Resources.media_floppy_smaller;
            this.save.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.save.Name = "save";
            this.save.Size = new System.Drawing.Size(23, 22);
            this.save.Text = "Save";
            this.save.Click += new System.EventHandler(this.save_Click);
            // 
            // toolStrip1
            // 
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newToolbarClick,
            this.save});
            this.toolStrip1.Location = new System.Drawing.Point(0, 24);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.RenderMode = System.Windows.Forms.ToolStripRenderMode.Professional;
            this.toolStrip1.Size = new System.Drawing.Size(1016, 25);
            this.toolStrip1.TabIndex = 1;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // SectorTab
            // 
            this.SectorTab.AllowClose = false;
            this.SectorTab.AllowCollapse = false;
            this.SectorTab.Controls.Add(this.pCanvas1);
            this.SectorTab.FloatingSize = new System.Drawing.Size(550, 400);
            this.SectorTab.Guid = new System.Guid("3cdd5ebc-6aa5-43ab-a7a2-a26142602396");
            this.SectorTab.Location = new System.Drawing.Point(5, 33);
            this.SectorTab.Name = "SectorTab";
            this.SectorTab.Size = new System.Drawing.Size(640, 457);
            this.SectorTab.TabIndex = 0;
            this.SectorTab.Text = "SectorName";
            // 
            // pCanvas1
            // 
            this.pCanvas1.AllowDrop = true;
            this.pCanvas1.BackColor = System.Drawing.Color.Black;
            this.pCanvas1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pCanvas1.GridFitText = false;
            this.pCanvas1.Location = new System.Drawing.Point(0, 0);
            this.pCanvas1.Name = "pCanvas1";
            this.pCanvas1.RegionManagement = true;
            this.pCanvas1.Size = new System.Drawing.Size(640, 457);
            this.pCanvas1.TabIndex = 0;
            this.pCanvas1.Text = "pCanvas1";
            // 
            // documentContainer1
            // 
            this.documentContainer1.ContentSize = 200;
            this.documentContainer1.Controls.Add(this.SectorTab);
            this.documentContainer1.LayoutSystem = new TD.SandDock.SplitLayoutSystem(new System.Drawing.SizeF(250F, 400F), System.Windows.Forms.Orientation.Horizontal, new TD.SandDock.LayoutSystemBase[] {
            ((TD.SandDock.LayoutSystemBase)(new TD.SandDock.DocumentLayoutSystem(new System.Drawing.SizeF(550F, 400F), new TD.SandDock.DockControl[] {
                        ((TD.SandDock.DockControl)(this.SectorTab))}, this.SectorTab)))});
            this.documentContainer1.Location = new System.Drawing.Point(248, 49);
            this.documentContainer1.Manager = this.sandDockManager1;
            this.documentContainer1.Name = "documentContainer1";
            this.documentContainer1.Size = new System.Drawing.Size(650, 495);
            this.documentContainer1.TabIndex = 8;
            // 
            // mainFrm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.AppWorkspace;
            this.ClientSize = new System.Drawing.Size(1016, 734);
            this.Controls.Add(this.documentContainer1);
            this.Controls.Add(this.dockContainer1);
            this.Controls.Add(this.dockContainer3);
            this.Controls.Add(this.dockContainer2);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.toolStrip1);
            this.Controls.Add(this.menuStrip1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "mainFrm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "N7 Sector Editor";
            this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            this.Load += new System.EventHandler(this.mainFrm_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.sectorDock.ResumeLayout(false);
            this.toolboxDock.ResumeLayout(false);
            this.toolboxDock.PerformLayout();
            this.toolStrip2.ResumeLayout(false);
            this.toolStrip2.PerformLayout();
            this.dockContainer2.ResumeLayout(false);
            this.dockContainer3.ResumeLayout(false);
            this.dockableWindow1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.dockContainer1.ResumeLayout(false);
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.SectorTab.ResumeLayout(false);
            this.documentContainer1.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private TD.SandDock.SandDockManager sandDockManager1;
        private TD.SandDock.DockableWindow sectorDock;
        private TD.SandDock.DockableWindow toolboxDock;
        private TD.SandDock.DockContainer dockContainer2;
        private TD.SandDock.DockableWindow PropertiesDock;
        private TD.SandDock.DockContainer dockContainer1;
        private TD.SandDock.DockContainer dockContainer3;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newSectorToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveSectorToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton newToolbarClick;
        private System.Windows.Forms.ToolStripButton save;
        private TD.SandDock.DockableWindow sectorOptionsDock;
        private System.Windows.Forms.TreeView treeView1;
        private System.Windows.Forms.PropertyGrid propertyGrid1;
        private TD.SandDock.DocumentContainer documentContainer1;
        private TD.SandDock.TabbedDocument SectorTab;
        private UMD.HCIL.Piccolo.PCanvas pCanvas1;
        private System.Windows.Forms.ToolStrip toolStrip2;
        private System.Windows.Forms.ToolStripButton toolStripButton3;
        private System.Windows.Forms.ToolStripButton toolStripButton4;
        private N7.GUI.OptionsGui optionsGui1;
        private System.Windows.Forms.ToolStripButton deleteSelected;
        private System.Windows.Forms.ToolStripMenuItem systemToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem sectorToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem sectorObjectToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem editToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem settingsToolStripMenuItem;
        private System.Windows.Forms.ToolStripButton toolStripButton1;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private TD.SandDock.DockableWindow dockableWindow1;
        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.DataGridViewTextBoxColumn id;
        private System.Windows.Forms.DataGridViewTextBoxColumn name;
        private System.Windows.Forms.DataGridViewTextBoxColumn base_asset_id;
        private System.Windows.Forms.DataGridViewTextBoxColumn type;
    }
}

