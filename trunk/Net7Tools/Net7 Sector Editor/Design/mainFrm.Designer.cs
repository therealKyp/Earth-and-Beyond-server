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
            TD.SandDock.DockingRules dockingRules5 = new TD.SandDock.DockingRules();
            TD.SandDock.DockingRules dockingRules6 = new TD.SandDock.DockingRules();
            TD.SandDock.DockingRules dockingRules4 = new TD.SandDock.DockingRules();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(mainFrm));
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newSectorToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveSectorToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.forumsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.sandDockManager1 = new TD.SandDock.SandDockManager();
            this.sectorDock = new TD.SandDock.DockableWindow();
            this.treeView1 = new System.Windows.Forms.TreeView();
            this.PropertiesDock = new TD.SandDock.DockableWindow();
            this.propertyGrid1 = new System.Windows.Forms.PropertyGrid();
            this.consoleDock = new TD.SandDock.DockableWindow();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.tableDock = new TD.SandDock.DockableWindow();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.toolboxDock = new TD.SandDock.DockableWindow();
            this.dockContainer2 = new TD.SandDock.DockContainer();
            this.dockContainer3 = new TD.SandDock.DockContainer();
            this.sectorOptionsDock = new TD.SandDock.DockableWindow();
            this.dockContainer1 = new TD.SandDock.DockContainer();
            this.toolStripButton1 = new System.Windows.Forms.ToolStripButton();
            this.toolStripButton2 = new System.Windows.Forms.ToolStripButton();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.SectorTab = new TD.SandDock.TabbedDocument();
            this.pCanvas1 = new UMD.HCIL.Piccolo.PCanvas();
            this.documentContainer1 = new TD.SandDock.DocumentContainer();
            this.menuStrip1.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.sectorDock.SuspendLayout();
            this.PropertiesDock.SuspendLayout();
            this.consoleDock.SuspendLayout();
            this.tableDock.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.dockContainer2.SuspendLayout();
            this.dockContainer3.SuspendLayout();
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
            this.helpToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1016, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            this.menuStrip1.ItemClicked += new System.Windows.Forms.ToolStripItemClickedEventHandler(this.menuStrip1_ItemClicked);
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
            this.newSectorToolStripMenuItem.Name = "newSectorToolStripMenuItem";
            this.newSectorToolStripMenuItem.Size = new System.Drawing.Size(136, 22);
            this.newSectorToolStripMenuItem.Text = "New Sector";
            // 
            // saveSectorToolStripMenuItem
            // 
            this.saveSectorToolStripMenuItem.Name = "saveSectorToolStripMenuItem";
            this.saveSectorToolStripMenuItem.Size = new System.Drawing.Size(136, 22);
            this.saveSectorToolStripMenuItem.Text = "Save Sector";
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(133, 6);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(136, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem,
            this.toolStripSeparator2,
            this.forumsToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(41, 20);
            this.helpToolStripMenuItem.Text = "Help";
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(111, 22);
            this.aboutToolStripMenuItem.Text = "About";
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(108, 6);
            // 
            // forumsToolStripMenuItem
            // 
            this.forumsToolStripMenuItem.Name = "forumsToolStripMenuItem";
            this.forumsToolStripMenuItem.Size = new System.Drawing.Size(111, 22);
            this.forumsToolStripMenuItem.Text = "Forums";
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
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(41, 17);
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
            this.sectorDock.AllowCollapse = false;
            this.sectorDock.BorderStyle = TD.SandDock.Rendering.BorderStyle.RaisedThin;
            this.sectorDock.Controls.Add(this.treeView1);
            dockingRules5.AllowDockBottom = true;
            dockingRules5.AllowDockLeft = true;
            dockingRules5.AllowDockRight = true;
            dockingRules5.AllowDockTop = true;
            dockingRules5.AllowFloat = false;
            dockingRules5.AllowTab = false;
            this.sectorDock.DockingRules = dockingRules5;
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
            this.PropertiesDock.AllowCollapse = false;
            this.PropertiesDock.BorderStyle = TD.SandDock.Rendering.BorderStyle.RaisedThin;
            this.PropertiesDock.Controls.Add(this.propertyGrid1);
            dockingRules6.AllowDockBottom = true;
            dockingRules6.AllowDockLeft = true;
            dockingRules6.AllowDockRight = true;
            dockingRules6.AllowDockTop = true;
            dockingRules6.AllowFloat = false;
            dockingRules6.AllowTab = false;
            this.PropertiesDock.DockingRules = dockingRules6;
            this.PropertiesDock.Guid = new System.Guid("04d72d9b-c92e-437a-ae5c-625399e065c7");
            this.PropertiesDock.Location = new System.Drawing.Point(0, 359);
            this.PropertiesDock.Name = "PropertiesDock";
            this.PropertiesDock.Size = new System.Drawing.Size(244, 281);
            this.PropertiesDock.TabIndex = 1;
            this.PropertiesDock.Text = "Properties";
            // 
            // propertyGrid1
            // 
            this.propertyGrid1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.propertyGrid1.Location = new System.Drawing.Point(1, 1);
            this.propertyGrid1.Name = "propertyGrid1";
            this.propertyGrid1.Size = new System.Drawing.Size(242, 279);
            this.propertyGrid1.TabIndex = 0;
            // 
            // consoleDock
            // 
            this.consoleDock.AllowClose = false;
            this.consoleDock.BorderStyle = TD.SandDock.Rendering.BorderStyle.RaisedThin;
            this.consoleDock.Controls.Add(this.textBox1);
            this.consoleDock.Guid = new System.Guid("402768ce-57b1-42b8-913a-0d0a8f3bb3b1");
            this.consoleDock.Location = new System.Drawing.Point(0, 0);
            this.consoleDock.Name = "consoleDock";
            this.consoleDock.Size = new System.Drawing.Size(768, 135);
            this.consoleDock.TabIndex = 2;
            this.consoleDock.Text = "Console";
            // 
            // textBox1
            // 
            this.textBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBox1.Location = new System.Drawing.Point(1, 1);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBox1.Size = new System.Drawing.Size(766, 133);
            this.textBox1.TabIndex = 0;
            // 
            // tableDock
            // 
            this.tableDock.BorderStyle = TD.SandDock.Rendering.BorderStyle.RaisedThin;
            this.tableDock.Controls.Add(this.dataGridView1);
            dockingRules4.AllowDockBottom = true;
            dockingRules4.AllowDockLeft = true;
            dockingRules4.AllowDockRight = true;
            dockingRules4.AllowDockTop = true;
            dockingRules4.AllowFloat = false;
            dockingRules4.AllowTab = false;
            this.tableDock.DockingRules = dockingRules4;
            this.tableDock.Guid = new System.Guid("6149e346-cd5b-4941-8400-3e1548ea5f74");
            this.tableDock.Location = new System.Drawing.Point(0, 29);
            this.tableDock.Name = "tableDock";
            this.tableDock.Size = new System.Drawing.Size(768, 135);
            this.tableDock.TabIndex = 3;
            this.tableDock.Text = "Sector Table";
            // 
            // dataGridView1
            // 
            this.dataGridView1.AllowUserToOrderColumns = true;
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dataGridView1.Location = new System.Drawing.Point(1, 1);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.Size = new System.Drawing.Size(766, 133);
            this.dataGridView1.TabIndex = 0;
            // 
            // toolboxDock
            // 
            this.toolboxDock.AllowClose = false;
            this.toolboxDock.BorderStyle = TD.SandDock.Rendering.BorderStyle.RaisedThin;
            this.toolboxDock.Collapsed = true;
            this.toolboxDock.Guid = new System.Guid("b2270f15-5913-41e1-bbf4-3fa1b89e11d2");
            this.toolboxDock.Location = new System.Drawing.Point(4, 25);
            this.toolboxDock.Name = "toolboxDock";
            this.toolboxDock.Size = new System.Drawing.Size(200, 428);
            this.toolboxDock.TabIndex = 4;
            this.toolboxDock.Text = "ToolBox";
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
            this.dockContainer3.ContentSize = 183;
            this.dockContainer3.Controls.Add(this.consoleDock);
            this.dockContainer3.Controls.Add(this.tableDock);
            this.dockContainer3.Controls.Add(this.sectorOptionsDock);
            this.dockContainer3.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.dockContainer3.LayoutSystem = new TD.SandDock.SplitLayoutSystem(new System.Drawing.SizeF(250F, 400F), System.Windows.Forms.Orientation.Vertical, new TD.SandDock.LayoutSystemBase[] {
            ((TD.SandDock.LayoutSystemBase)(new TD.SandDock.ControlLayoutSystem(new System.Drawing.SizeF(250F, 400F), new TD.SandDock.DockControl[] {
                        ((TD.SandDock.DockControl)(this.consoleDock)),
                        ((TD.SandDock.DockControl)(this.tableDock)),
                        ((TD.SandDock.DockControl)(this.sectorOptionsDock))}, this.tableDock)))});
            this.dockContainer3.Location = new System.Drawing.Point(248, 525);
            this.dockContainer3.Manager = this.sandDockManager1;
            this.dockContainer3.Name = "dockContainer3";
            this.dockContainer3.Size = new System.Drawing.Size(768, 187);
            this.dockContainer3.TabIndex = 5;
            // 
            // sectorOptionsDock
            // 
            this.sectorOptionsDock.BackColor = System.Drawing.Color.White;
            this.sectorOptionsDock.Guid = new System.Guid("4b4734ea-f7f1-4bc0-9469-5a8dc25849d9");
            this.sectorOptionsDock.Location = new System.Drawing.Point(0, 0);
            this.sectorOptionsDock.Name = "sectorOptionsDock";
            this.sectorOptionsDock.Size = new System.Drawing.Size(768, 138);
            this.sectorOptionsDock.TabIndex = 5;
            this.sectorOptionsDock.Text = "Sector Options";
            // 
            // dockContainer1
            // 
            this.dockContainer1.ContentSize = 200;
            this.dockContainer1.Controls.Add(this.toolboxDock);
            this.dockContainer1.Dock = System.Windows.Forms.DockStyle.Right;
            this.dockContainer1.LayoutSystem = new TD.SandDock.SplitLayoutSystem(new System.Drawing.SizeF(250F, 400F), System.Windows.Forms.Orientation.Horizontal, new TD.SandDock.LayoutSystemBase[] {
            ((TD.SandDock.LayoutSystemBase)(new TD.SandDock.ControlLayoutSystem(new System.Drawing.SizeF(250F, 400F), new TD.SandDock.DockControl[] {
                        ((TD.SandDock.DockControl)(this.toolboxDock))}, this.toolboxDock)))});
            this.dockContainer1.Location = new System.Drawing.Point(812, 49);
            this.dockContainer1.Manager = this.sandDockManager1;
            this.dockContainer1.Name = "dockContainer1";
            this.dockContainer1.Size = new System.Drawing.Size(204, 476);
            this.dockContainer1.TabIndex = 7;
            // 
            // toolStripButton1
            // 
            this.toolStripButton1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButton1.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton1.Image")));
            this.toolStripButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton1.Name = "toolStripButton1";
            this.toolStripButton1.Size = new System.Drawing.Size(23, 22);
            this.toolStripButton1.Text = "toolStripButton1";
            // 
            // toolStripButton2
            // 
            this.toolStripButton2.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButton2.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton2.Image")));
            this.toolStripButton2.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton2.Name = "toolStripButton2";
            this.toolStripButton2.Size = new System.Drawing.Size(23, 22);
            this.toolStripButton2.Text = "toolStripButton2";
            // 
            // toolStrip1
            // 
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripButton1,
            this.toolStripButton2});
            this.toolStrip1.Location = new System.Drawing.Point(0, 24);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.RenderMode = System.Windows.Forms.ToolStripRenderMode.Professional;
            this.toolStrip1.Size = new System.Drawing.Size(1016, 25);
            this.toolStrip1.TabIndex = 1;
            this.toolStrip1.Text = "toolStrip1";
            this.toolStrip1.ItemClicked += new System.Windows.Forms.ToolStripItemClickedEventHandler(this.toolStrip1_ItemClicked);
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
            this.SectorTab.Size = new System.Drawing.Size(554, 438);
            this.SectorTab.TabIndex = 0;
            this.SectorTab.Text = "SectorName";
            // 
            // pCanvas1
            // 
            this.pCanvas1.AllowDrop = true;
            this.pCanvas1.BackColor = System.Drawing.Color.White;
            this.pCanvas1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pCanvas1.GridFitText = false;
            this.pCanvas1.Location = new System.Drawing.Point(0, 0);
            this.pCanvas1.Name = "pCanvas1";
            this.pCanvas1.RegionManagement = true;
            this.pCanvas1.Size = new System.Drawing.Size(554, 438);
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
            this.documentContainer1.Size = new System.Drawing.Size(564, 476);
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
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "mainFrm";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.sectorDock.ResumeLayout(false);
            this.PropertiesDock.ResumeLayout(false);
            this.consoleDock.ResumeLayout(false);
            this.consoleDock.PerformLayout();
            this.tableDock.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.dockContainer2.ResumeLayout(false);
            this.dockContainer3.ResumeLayout(false);
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
        private TD.SandDock.DockableWindow consoleDock;
        private TD.SandDock.DockableWindow tableDock;
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
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripMenuItem forumsToolStripMenuItem;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton toolStripButton1;
        private System.Windows.Forms.ToolStripButton toolStripButton2;
        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.TextBox textBox1;
        private TD.SandDock.DockableWindow sectorOptionsDock;
        private System.Windows.Forms.TreeView treeView1;
        private System.Windows.Forms.PropertyGrid propertyGrid1;
        private TD.SandDock.DocumentContainer documentContainer1;
        private TD.SandDock.TabbedDocument SectorTab;
        private UMD.HCIL.Piccolo.PCanvas pCanvas1;
    }
}

