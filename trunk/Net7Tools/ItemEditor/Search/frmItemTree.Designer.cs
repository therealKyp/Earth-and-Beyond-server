namespace Net7_Tools
{
    partial class FrmItemTree : System.Windows.Forms.Form
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FrmItemTree));
            this.treeItems = new System.Windows.Forms.TreeView();
            this.btnOk = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            this.frmItemTreeTabControl = new System.Windows.Forms.TabControl();
            this.frmItemTreeTabBrowse = new System.Windows.Forms.TabPage();
            this.frmItemTreeTabSearch = new System.Windows.Forms.TabPage();
            this.searchSearchCriteriaLst = new System.Windows.Forms.ListBox();
            this.searchRemoveSearchCriteriaBtn = new System.Windows.Forms.Button();
            this.searchEditSearchCriteriaBtn = new System.Windows.Forms.Button();
            this.searchAddSearchCriteriaBtn = new System.Windows.Forms.Button();
            this.searchResultCount = new System.Windows.Forms.Label();
            this.searchSearchBtn = new System.Windows.Forms.Button();
            this.searchResultLbl = new System.Windows.Forms.Label();
            this.searchResultList = new System.Windows.Forms.ListView();
            this.searchResultListId = new System.Windows.Forms.ColumnHeader();
            this.searchResultListName = new System.Windows.Forms.ColumnHeader();
            this.searchResultListCategory = new System.Windows.Forms.ColumnHeader();
            this.searchResultListSubCategory = new System.Windows.Forms.ColumnHeader();
            this.searchResultListLevel = new System.Windows.Forms.ColumnHeader();
            this.searchResultListStatus = new System.Windows.Forms.ColumnHeader();
            this.clipboardBtn = new System.Windows.Forms.Button();
            this.guiTableCbo = new System.Windows.Forms.ComboBox();
            this.guiTableLbl = new System.Windows.Forms.Label();
            this.frmItemTreeTabControl.SuspendLayout();
            this.frmItemTreeTabBrowse.SuspendLayout();
            this.frmItemTreeTabSearch.SuspendLayout();
            this.SuspendLayout();
            // 
            // treeItems
            // 
            this.treeItems.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.treeItems.Location = new System.Drawing.Point(0, 0);
            this.treeItems.Name = "treeItems";
            this.treeItems.Size = new System.Drawing.Size(575, 438);
            this.treeItems.TabIndex = 0;
            this.treeItems.BeforeExpand += new System.Windows.Forms.TreeViewCancelEventHandler(this.beforeNodeExpand);
            this.treeItems.DoubleClick += new System.EventHandler(this.btnOk_Click);
            // 
            // btnOk
            // 
            this.btnOk.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnOk.Location = new System.Drawing.Point(451, 467);
            this.btnOk.Name = "btnOk";
            this.btnOk.Size = new System.Drawing.Size(75, 23);
            this.btnOk.TabIndex = 1;
            this.btnOk.Text = "Ok";
            this.btnOk.UseVisualStyleBackColor = true;
            this.btnOk.Click += new System.EventHandler(this.btnOk_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnCancel.Location = new System.Drawing.Point(532, 467);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(75, 23);
            this.btnCancel.TabIndex = 2;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // frmItemTreeTabControl
            // 
            this.frmItemTreeTabControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.frmItemTreeTabControl.Controls.Add(this.frmItemTreeTabBrowse);
            this.frmItemTreeTabControl.Controls.Add(this.frmItemTreeTabSearch);
            this.frmItemTreeTabControl.Location = new System.Drawing.Point(1, 1);
            this.frmItemTreeTabControl.Name = "frmItemTreeTabControl";
            this.frmItemTreeTabControl.SelectedIndex = 0;
            this.frmItemTreeTabControl.Size = new System.Drawing.Size(618, 460);
            this.frmItemTreeTabControl.TabIndex = 0;
            // 
            // frmItemTreeTabBrowse
            // 
            this.frmItemTreeTabBrowse.Controls.Add(this.treeItems);
            this.frmItemTreeTabBrowse.Location = new System.Drawing.Point(4, 22);
            this.frmItemTreeTabBrowse.Name = "frmItemTreeTabBrowse";
            this.frmItemTreeTabBrowse.Padding = new System.Windows.Forms.Padding(3);
            this.frmItemTreeTabBrowse.Size = new System.Drawing.Size(610, 434);
            this.frmItemTreeTabBrowse.TabIndex = 0;
            this.frmItemTreeTabBrowse.Text = "Browse";
            this.frmItemTreeTabBrowse.UseVisualStyleBackColor = true;
            // 
            // frmItemTreeTabSearch
            // 
            this.frmItemTreeTabSearch.Controls.Add(this.guiTableLbl);
            this.frmItemTreeTabSearch.Controls.Add(this.guiTableCbo);
            this.frmItemTreeTabSearch.Controls.Add(this.searchSearchCriteriaLst);
            this.frmItemTreeTabSearch.Controls.Add(this.searchRemoveSearchCriteriaBtn);
            this.frmItemTreeTabSearch.Controls.Add(this.searchEditSearchCriteriaBtn);
            this.frmItemTreeTabSearch.Controls.Add(this.searchAddSearchCriteriaBtn);
            this.frmItemTreeTabSearch.Controls.Add(this.searchResultCount);
            this.frmItemTreeTabSearch.Controls.Add(this.searchSearchBtn);
            this.frmItemTreeTabSearch.Controls.Add(this.searchResultLbl);
            this.frmItemTreeTabSearch.Controls.Add(this.searchResultList);
            this.frmItemTreeTabSearch.Location = new System.Drawing.Point(4, 22);
            this.frmItemTreeTabSearch.Name = "frmItemTreeTabSearch";
            this.frmItemTreeTabSearch.Padding = new System.Windows.Forms.Padding(3);
            this.frmItemTreeTabSearch.Size = new System.Drawing.Size(610, 434);
            this.frmItemTreeTabSearch.TabIndex = 1;
            this.frmItemTreeTabSearch.Text = "Search";
            this.frmItemTreeTabSearch.UseVisualStyleBackColor = true;
            // 
            // searchSearchCriteriaLst
            // 
            this.searchSearchCriteriaLst.FormattingEnabled = true;
            this.searchSearchCriteriaLst.Location = new System.Drawing.Point(7, 6);
            this.searchSearchCriteriaLst.Name = "searchSearchCriteriaLst";
            this.searchSearchCriteriaLst.Size = new System.Drawing.Size(444, 95);
            this.searchSearchCriteriaLst.TabIndex = 21;
            // 
            // searchRemoveSearchCriteriaBtn
            // 
            this.searchRemoveSearchCriteriaBtn.Location = new System.Drawing.Point(364, 107);
            this.searchRemoveSearchCriteriaBtn.Name = "searchRemoveSearchCriteriaBtn";
            this.searchRemoveSearchCriteriaBtn.Size = new System.Drawing.Size(87, 22);
            this.searchRemoveSearchCriteriaBtn.TabIndex = 20;
            this.searchRemoveSearchCriteriaBtn.Text = "&Remove";
            this.searchRemoveSearchCriteriaBtn.UseVisualStyleBackColor = true;
            this.searchRemoveSearchCriteriaBtn.Click += new System.EventHandler(this.onRemoveSearchCriteria_Click);
            // 
            // searchEditSearchCriteriaBtn
            // 
            this.searchEditSearchCriteriaBtn.Location = new System.Drawing.Point(271, 107);
            this.searchEditSearchCriteriaBtn.Name = "searchEditSearchCriteriaBtn";
            this.searchEditSearchCriteriaBtn.Size = new System.Drawing.Size(87, 22);
            this.searchEditSearchCriteriaBtn.TabIndex = 19;
            this.searchEditSearchCriteriaBtn.Text = "&Edit";
            this.searchEditSearchCriteriaBtn.UseVisualStyleBackColor = true;
            this.searchEditSearchCriteriaBtn.Click += new System.EventHandler(this.onEditSearchCriteria_Click);
            // 
            // searchAddSearchCriteriaBtn
            // 
            this.searchAddSearchCriteriaBtn.Location = new System.Drawing.Point(178, 107);
            this.searchAddSearchCriteriaBtn.Name = "searchAddSearchCriteriaBtn";
            this.searchAddSearchCriteriaBtn.Size = new System.Drawing.Size(87, 22);
            this.searchAddSearchCriteriaBtn.TabIndex = 18;
            this.searchAddSearchCriteriaBtn.Text = "&Add";
            this.searchAddSearchCriteriaBtn.UseVisualStyleBackColor = true;
            this.searchAddSearchCriteriaBtn.Click += new System.EventHandler(this.onAddSearchCriteria_Click);
            // 
            // searchResultCount
            // 
            this.searchResultCount.AutoSize = true;
            this.searchResultCount.Location = new System.Drawing.Point(555, 107);
            this.searchResultCount.Name = "searchResultCount";
            this.searchResultCount.Size = new System.Drawing.Size(47, 13);
            this.searchResultCount.TabIndex = 16;
            this.searchResultCount.Text = "<Count>";
            // 
            // searchSearchBtn
            // 
            this.searchSearchBtn.Location = new System.Drawing.Point(477, 6);
            this.searchSearchBtn.Name = "searchSearchBtn";
            this.searchSearchBtn.Size = new System.Drawing.Size(125, 95);
            this.searchSearchBtn.TabIndex = 14;
            this.searchSearchBtn.Text = "&Search";
            this.searchSearchBtn.UseVisualStyleBackColor = true;
            this.searchSearchBtn.Click += new System.EventHandler(this.btnSearch_Click);
            // 
            // searchResultLbl
            // 
            this.searchResultLbl.AutoSize = true;
            this.searchResultLbl.Location = new System.Drawing.Point(479, 107);
            this.searchResultLbl.Name = "searchResultLbl";
            this.searchResultLbl.Size = new System.Drawing.Size(72, 13);
            this.searchResultLbl.TabIndex = 15;
            this.searchResultLbl.Text = "Search result:";
            // 
            // searchResultList
            // 
            this.searchResultList.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.searchResultList.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.searchResultListId,
            this.searchResultListName,
            this.searchResultListCategory,
            this.searchResultListSubCategory,
            this.searchResultListLevel,
            this.searchResultListStatus});
            this.searchResultList.FullRowSelect = true;
            this.searchResultList.Location = new System.Drawing.Point(3, 135);
            this.searchResultList.MultiSelect = false;
            this.searchResultList.Name = "searchResultList";
            this.searchResultList.Size = new System.Drawing.Size(599, 293);
            this.searchResultList.TabIndex = 17;
            this.searchResultList.UseCompatibleStateImageBehavior = false;
            this.searchResultList.View = System.Windows.Forms.View.Details;
            this.searchResultList.DoubleClick += new System.EventHandler(this.btnOk_Click);
            this.searchResultList.ColumnClick += new System.Windows.Forms.ColumnClickEventHandler(this.onColumnClick);
            // 
            // searchResultListId
            // 
            this.searchResultListId.Text = "ID";
            this.searchResultListId.Width = 57;
            // 
            // searchResultListName
            // 
            this.searchResultListName.Text = "Name";
            this.searchResultListName.Width = 200;
            // 
            // searchResultListCategory
            // 
            this.searchResultListCategory.Text = "Category";
            this.searchResultListCategory.Width = 120;
            // 
            // searchResultListSubCategory
            // 
            this.searchResultListSubCategory.Text = "SubCategory";
            this.searchResultListSubCategory.Width = 124;
            // 
            // searchResultListLevel
            // 
            this.searchResultListLevel.Text = "Level";
            this.searchResultListLevel.Width = 38;
            // 
            // searchResultListStatus
            // 
            this.searchResultListStatus.Text = "Status";
            this.searchResultListStatus.Width = 38;
            // 
            // clipboardBtn
            // 
            this.clipboardBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.clipboardBtn.Location = new System.Drawing.Point(8, 467);
            this.clipboardBtn.Name = "clipboardBtn";
            this.clipboardBtn.Size = new System.Drawing.Size(75, 23);
            this.clipboardBtn.TabIndex = 4;
            this.clipboardBtn.Text = "Clipboard";
            this.clipboardBtn.UseVisualStyleBackColor = true;
            this.clipboardBtn.Click += new System.EventHandler(this.onClipboard);
            // 
            // guiTableCbo
            // 
            this.guiTableCbo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.guiTableCbo.FormattingEnabled = true;
            this.guiTableCbo.Location = new System.Drawing.Point(41, 107);
            this.guiTableCbo.Name = "guiTableCbo";
            this.guiTableCbo.Size = new System.Drawing.Size(131, 21);
            this.guiTableCbo.TabIndex = 22;
            this.guiTableCbo.SelectedIndexChanged += new System.EventHandler(this.onTableChanged);
            // 
            // guiTableLbl
            // 
            this.guiTableLbl.AutoSize = true;
            this.guiTableLbl.Location = new System.Drawing.Point(3, 110);
            this.guiTableLbl.Name = "guiTableLbl";
            this.guiTableLbl.Size = new System.Drawing.Size(37, 13);
            this.guiTableLbl.TabIndex = 23;
            this.guiTableLbl.Text = "Table:";
            // 
            // frmItemTree
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(619, 502);
            this.Controls.Add(this.clipboardBtn);
            this.Controls.Add(this.frmItemTreeTabControl);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnOk);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "frmItemTree";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Item Hierarchy";
            this.frmItemTreeTabControl.ResumeLayout(false);
            this.frmItemTreeTabBrowse.ResumeLayout(false);
            this.frmItemTreeTabSearch.ResumeLayout(false);
            this.frmItemTreeTabSearch.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TreeView treeItems;
        private System.Windows.Forms.Button btnOk;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.TabControl frmItemTreeTabControl;
        private System.Windows.Forms.TabPage frmItemTreeTabBrowse;
        private System.Windows.Forms.TabPage frmItemTreeTabSearch;
        private System.Windows.Forms.Label searchResultLbl;
        private System.Windows.Forms.ListView searchResultList;
        private System.Windows.Forms.Button searchSearchBtn;
        private System.Windows.Forms.ColumnHeader searchResultListId;
        private System.Windows.Forms.ColumnHeader searchResultListName;
        private System.Windows.Forms.ColumnHeader searchResultListCategory;
        private System.Windows.Forms.ColumnHeader searchResultListSubCategory;
        private System.Windows.Forms.ColumnHeader searchResultListLevel;
        private System.Windows.Forms.Label searchResultCount;
        private System.Windows.Forms.ColumnHeader searchResultListStatus;
        private System.Windows.Forms.Button searchRemoveSearchCriteriaBtn;
        private System.Windows.Forms.Button searchEditSearchCriteriaBtn;
        private System.Windows.Forms.Button searchAddSearchCriteriaBtn;
        private System.Windows.Forms.ListBox searchSearchCriteriaLst;
        private System.Windows.Forms.Button clipboardBtn;
        private System.Windows.Forms.Label guiTableLbl;
        private System.Windows.Forms.ComboBox guiTableCbo;
    }
}