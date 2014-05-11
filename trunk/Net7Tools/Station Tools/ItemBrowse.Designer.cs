namespace Station_Tools
{
    partial class ItemBrowse
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
            this.ItemList = new System.Windows.Forms.DataGridView();
            this.Column6 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Column2 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Column1 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Column5 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Column3 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Column4 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.ItemDescription = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.ItemNameQuery = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.ItemLevelQuery = new System.Windows.Forms.ComboBox();
            this.label4 = new System.Windows.Forms.Label();
            this.ItemTypeQuery = new System.Windows.Forms.ComboBox();
            this.Search = new System.Windows.Forms.Button();
            this.AddSelected = new System.Windows.Forms.Button();
            this.CloseWindow = new System.Windows.Forms.Button();
            this.RowCount = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.ItemList)).BeginInit();
            this.SuspendLayout();
            // 
            // ItemList
            // 
            this.ItemList.AllowUserToAddRows = false;
            this.ItemList.AllowUserToDeleteRows = false;
            this.ItemList.AllowUserToOrderColumns = true;
            this.ItemList.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.ItemList.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.ItemList.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.Column6,
            this.Column2,
            this.Column1,
            this.Column5,
            this.Column3,
            this.Column4});
            this.ItemList.Location = new System.Drawing.Point(197, 12);
            this.ItemList.MultiSelect = false;
            this.ItemList.Name = "ItemList";
            this.ItemList.ReadOnly = true;
            this.ItemList.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.ItemList.Size = new System.Drawing.Size(402, 213);
            this.ItemList.TabIndex = 0;
            this.ItemList.SelectionChanged += new System.EventHandler(this.ItemList_SelectionChanged);
            // 
            // Column6
            // 
            this.Column6.HeaderText = "ID";
            this.Column6.Name = "Column6";
            this.Column6.ReadOnly = true;
            this.Column6.Visible = false;
            // 
            // Column2
            // 
            this.Column2.HeaderText = "Item ID";
            this.Column2.Name = "Column2";
            this.Column2.ReadOnly = true;
            this.Column2.Width = 70;
            // 
            // Column1
            // 
            this.Column1.HeaderText = "Item Name";
            this.Column1.Name = "Column1";
            this.Column1.ReadOnly = true;
            this.Column1.Width = 200;
            // 
            // Column5
            // 
            this.Column5.HeaderText = "ItemType";
            this.Column5.Name = "Column5";
            this.Column5.ReadOnly = true;
            this.Column5.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            // 
            // Column3
            // 
            this.Column3.HeaderText = "Item Level";
            this.Column3.Name = "Column3";
            this.Column3.ReadOnly = true;
            // 
            // Column4
            // 
            this.Column4.HeaderText = "Price";
            this.Column4.Name = "Column4";
            this.Column4.ReadOnly = true;
            // 
            // ItemDescription
            // 
            this.ItemDescription.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.ItemDescription.Location = new System.Drawing.Point(197, 244);
            this.ItemDescription.Multiline = true;
            this.ItemDescription.Name = "ItemDescription";
            this.ItemDescription.Size = new System.Drawing.Size(401, 80);
            this.ItemDescription.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(194, 228);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(63, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Description:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 14);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(61, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Item Name:";
            // 
            // ItemNameQuery
            // 
            this.ItemNameQuery.Location = new System.Drawing.Point(15, 30);
            this.ItemNameQuery.Name = "ItemNameQuery";
            this.ItemNameQuery.Size = new System.Drawing.Size(176, 20);
            this.ItemNameQuery.TabIndex = 4;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 53);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(56, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "Item Level";
            // 
            // ItemLevelQuery
            // 
            this.ItemLevelQuery.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.ItemLevelQuery.FormattingEnabled = true;
            this.ItemLevelQuery.Items.AddRange(new object[] {
            "Not Selected",
            "1",
            "2",
            "3",
            "4",
            "5",
            "6",
            "7",
            "8",
            "9"});
            this.ItemLevelQuery.Location = new System.Drawing.Point(15, 69);
            this.ItemLevelQuery.Name = "ItemLevelQuery";
            this.ItemLevelQuery.Size = new System.Drawing.Size(71, 21);
            this.ItemLevelQuery.TabIndex = 6;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(12, 93);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(57, 13);
            this.label4.TabIndex = 7;
            this.label4.Text = "Item Type:";
            // 
            // ItemTypeQuery
            // 
            this.ItemTypeQuery.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.ItemTypeQuery.FormattingEnabled = true;
            this.ItemTypeQuery.Items.AddRange(new object[] {
            "Not Selected",
            "System",
            "Weapon",
            "Shields",
            "Sensor",
            "Ejector",
            "Turret",
            "Engine",
            "Reactor",
            "Controler",
            "Robot",
            "Ammo",
            "Devices",
            "System",
            "Compontents",
            "Beam Weapon",
            "Missile Launcher",
            "Projectile Weapon",
            "Countermesure",
            "Over Rides"});
            this.ItemTypeQuery.Location = new System.Drawing.Point(15, 109);
            this.ItemTypeQuery.Name = "ItemTypeQuery";
            this.ItemTypeQuery.Size = new System.Drawing.Size(176, 21);
            this.ItemTypeQuery.TabIndex = 8;
            // 
            // Search
            // 
            this.Search.Location = new System.Drawing.Point(15, 136);
            this.Search.Name = "Search";
            this.Search.Size = new System.Drawing.Size(80, 26);
            this.Search.TabIndex = 9;
            this.Search.Text = "Search";
            this.Search.UseVisualStyleBackColor = true;
            this.Search.Click += new System.EventHandler(this.Search_Click);
            // 
            // AddSelected
            // 
            this.AddSelected.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.AddSelected.Location = new System.Drawing.Point(12, 239);
            this.AddSelected.Name = "AddSelected";
            this.AddSelected.Size = new System.Drawing.Size(116, 41);
            this.AddSelected.TabIndex = 10;
            this.AddSelected.Text = "Add Selected Item";
            this.AddSelected.UseVisualStyleBackColor = true;
            this.AddSelected.Click += new System.EventHandler(this.AddSelected_Click);
            // 
            // CloseWindow
            // 
            this.CloseWindow.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.CloseWindow.Location = new System.Drawing.Point(12, 286);
            this.CloseWindow.Name = "CloseWindow";
            this.CloseWindow.Size = new System.Drawing.Size(116, 41);
            this.CloseWindow.TabIndex = 11;
            this.CloseWindow.Text = "Close";
            this.CloseWindow.UseVisualStyleBackColor = true;
            this.CloseWindow.Click += new System.EventHandler(this.CloseWindow_Click);
            // 
            // RowCount
            // 
            this.RowCount.AutoSize = true;
            this.RowCount.Location = new System.Drawing.Point(12, 165);
            this.RowCount.Name = "RowCount";
            this.RowCount.Size = new System.Drawing.Size(100, 13);
            this.RowCount.TabIndex = 12;
            this.RowCount.Text = "Number of Results: ";
            // 
            // ItemBrowse
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(611, 339);
            this.Controls.Add(this.RowCount);
            this.Controls.Add(this.CloseWindow);
            this.Controls.Add(this.AddSelected);
            this.Controls.Add(this.Search);
            this.Controls.Add(this.ItemTypeQuery);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.ItemLevelQuery);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.ItemNameQuery);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.ItemDescription);
            this.Controls.Add(this.ItemList);
            this.MinimumSize = new System.Drawing.Size(619, 366);
            this.Name = "ItemBrowse";
            this.Text = "ItemBrowse";
            ((System.ComponentModel.ISupportInitialize)(this.ItemList)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.DataGridView ItemList;
        private System.Windows.Forms.TextBox ItemDescription;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox ItemNameQuery;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ComboBox ItemLevelQuery;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.ComboBox ItemTypeQuery;
        private System.Windows.Forms.Button Search;
        private System.Windows.Forms.Button AddSelected;
        private System.Windows.Forms.Button CloseWindow;
        private System.Windows.Forms.DataGridViewTextBoxColumn Column6;
        private System.Windows.Forms.DataGridViewTextBoxColumn Column2;
        private System.Windows.Forms.DataGridViewTextBoxColumn Column1;
        private System.Windows.Forms.DataGridViewTextBoxColumn Column5;
        private System.Windows.Forms.DataGridViewTextBoxColumn Column3;
        private System.Windows.Forms.DataGridViewTextBoxColumn Column4;
        private System.Windows.Forms.Label RowCount;
    }
}