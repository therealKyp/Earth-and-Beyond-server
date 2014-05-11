namespace CommonTools.Gui
{
    partial class DlgSearch
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
            this.guiSearchCriteriaTbl = new System.Windows.Forms.ListBox();
            this.guiRemoveSearchCriteriaBtn = new System.Windows.Forms.Button();
            this.guiEditSearchCriteriaBtn = new System.Windows.Forms.Button();
            this.guiAddSearchCriteriaBtn = new System.Windows.Forms.Button();
            this.guiSearchBtn = new System.Windows.Forms.Button();
            this.guiResultLbl = new System.Windows.Forms.Label();
            this.guiResultTbl = new System.Windows.Forms.ListView();
            this.searchResultListId = new System.Windows.Forms.ColumnHeader();
            this.searchResultListName = new System.Windows.Forms.ColumnHeader();
            this.searchResultListCategory = new System.Windows.Forms.ColumnHeader();
            this.searchResultListSubCategory = new System.Windows.Forms.ColumnHeader();
            this.searchResultListLevel = new System.Windows.Forms.ColumnHeader();
            this.searchResultListStatus = new System.Windows.Forms.ColumnHeader();
            this.guiCancelBtn = new System.Windows.Forms.Button();
            this.guiOkBtn = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // guiSearchCriteriaTbl
            // 
            this.guiSearchCriteriaTbl.FormattingEnabled = true;
            this.guiSearchCriteriaTbl.Location = new System.Drawing.Point(12, 12);
            this.guiSearchCriteriaTbl.Name = "guiSearchCriteriaTbl";
            this.guiSearchCriteriaTbl.Size = new System.Drawing.Size(444, 95);
            this.guiSearchCriteriaTbl.TabIndex = 28;
            // 
            // guiRemoveSearchCriteriaBtn
            // 
            this.guiRemoveSearchCriteriaBtn.Location = new System.Drawing.Point(276, 113);
            this.guiRemoveSearchCriteriaBtn.Name = "guiRemoveSearchCriteriaBtn";
            this.guiRemoveSearchCriteriaBtn.Size = new System.Drawing.Size(87, 22);
            this.guiRemoveSearchCriteriaBtn.TabIndex = 27;
            this.guiRemoveSearchCriteriaBtn.Text = "&Remove";
            this.guiRemoveSearchCriteriaBtn.UseVisualStyleBackColor = true;
            this.guiRemoveSearchCriteriaBtn.Click += new System.EventHandler(this.onRemoveClick);
            // 
            // guiEditSearchCriteriaBtn
            // 
            this.guiEditSearchCriteriaBtn.Location = new System.Drawing.Point(183, 113);
            this.guiEditSearchCriteriaBtn.Name = "guiEditSearchCriteriaBtn";
            this.guiEditSearchCriteriaBtn.Size = new System.Drawing.Size(87, 22);
            this.guiEditSearchCriteriaBtn.TabIndex = 26;
            this.guiEditSearchCriteriaBtn.Text = "&Edit";
            this.guiEditSearchCriteriaBtn.UseVisualStyleBackColor = true;
            this.guiEditSearchCriteriaBtn.Click += new System.EventHandler(this.onEditClick);
            // 
            // guiAddSearchCriteriaBtn
            // 
            this.guiAddSearchCriteriaBtn.Location = new System.Drawing.Point(90, 113);
            this.guiAddSearchCriteriaBtn.Name = "guiAddSearchCriteriaBtn";
            this.guiAddSearchCriteriaBtn.Size = new System.Drawing.Size(87, 22);
            this.guiAddSearchCriteriaBtn.TabIndex = 25;
            this.guiAddSearchCriteriaBtn.Text = "&Add";
            this.guiAddSearchCriteriaBtn.UseVisualStyleBackColor = true;
            this.guiAddSearchCriteriaBtn.Click += new System.EventHandler(this.onAddClick);
            // 
            // guiSearchBtn
            // 
            this.guiSearchBtn.Location = new System.Drawing.Point(482, 12);
            this.guiSearchBtn.Name = "guiSearchBtn";
            this.guiSearchBtn.Size = new System.Drawing.Size(125, 95);
            this.guiSearchBtn.TabIndex = 22;
            this.guiSearchBtn.Text = "&Search";
            this.guiSearchBtn.UseVisualStyleBackColor = true;
            this.guiSearchBtn.Click += new System.EventHandler(this.onSearchClick);
            // 
            // guiResultLbl
            // 
            this.guiResultLbl.AutoSize = true;
            this.guiResultLbl.Location = new System.Drawing.Point(484, 113);
            this.guiResultLbl.Name = "guiResultLbl";
            this.guiResultLbl.Size = new System.Drawing.Size(72, 13);
            this.guiResultLbl.TabIndex = 23;
            this.guiResultLbl.Text = "Search result:";
            // 
            // guiResultTbl
            // 
            this.guiResultTbl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.guiResultTbl.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.searchResultListId,
            this.searchResultListName,
            this.searchResultListCategory,
            this.searchResultListSubCategory,
            this.searchResultListLevel,
            this.searchResultListStatus});
            this.guiResultTbl.FullRowSelect = true;
            this.guiResultTbl.Location = new System.Drawing.Point(8, 141);
            this.guiResultTbl.MultiSelect = false;
            this.guiResultTbl.Name = "guiResultTbl";
            this.guiResultTbl.Size = new System.Drawing.Size(599, 302);
            this.guiResultTbl.TabIndex = 24;
            this.guiResultTbl.UseCompatibleStateImageBehavior = false;
            this.guiResultTbl.View = System.Windows.Forms.View.Details;
            this.guiResultTbl.DoubleClick += new System.EventHandler(this.onOkClick);
            this.guiResultTbl.ColumnClick += new System.Windows.Forms.ColumnClickEventHandler(this.onResultColumnClick);
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
            // guiCancelBtn
            // 
            this.guiCancelBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.guiCancelBtn.Location = new System.Drawing.Point(532, 449);
            this.guiCancelBtn.Name = "guiCancelBtn";
            this.guiCancelBtn.Size = new System.Drawing.Size(75, 23);
            this.guiCancelBtn.TabIndex = 30;
            this.guiCancelBtn.Text = "Cancel";
            this.guiCancelBtn.UseVisualStyleBackColor = true;
            this.guiCancelBtn.Click += new System.EventHandler(this.onCancelClick);
            // 
            // guiOkBtn
            // 
            this.guiOkBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.guiOkBtn.Location = new System.Drawing.Point(451, 449);
            this.guiOkBtn.Name = "guiOkBtn";
            this.guiOkBtn.Size = new System.Drawing.Size(75, 23);
            this.guiOkBtn.TabIndex = 29;
            this.guiOkBtn.Text = "Ok";
            this.guiOkBtn.UseVisualStyleBackColor = true;
            this.guiOkBtn.Click += new System.EventHandler(this.onOkClick);
            // 
            // DlgSearch
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(616, 484);
            this.Controls.Add(this.guiCancelBtn);
            this.Controls.Add(this.guiOkBtn);
            this.Controls.Add(this.guiSearchCriteriaTbl);
            this.Controls.Add(this.guiRemoveSearchCriteriaBtn);
            this.Controls.Add(this.guiEditSearchCriteriaBtn);
            this.Controls.Add(this.guiAddSearchCriteriaBtn);
            this.Controls.Add(this.guiSearchBtn);
            this.Controls.Add(this.guiResultLbl);
            this.Controls.Add(this.guiResultTbl);
            this.Name = "DlgSearch";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "DlgSearch";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListBox guiSearchCriteriaTbl;
        private System.Windows.Forms.Button guiRemoveSearchCriteriaBtn;
        private System.Windows.Forms.Button guiEditSearchCriteriaBtn;
        private System.Windows.Forms.Button guiAddSearchCriteriaBtn;
        private System.Windows.Forms.Button guiSearchBtn;
        private System.Windows.Forms.Label guiResultLbl;
        private System.Windows.Forms.ListView guiResultTbl;
        private System.Windows.Forms.ColumnHeader searchResultListId;
        private System.Windows.Forms.ColumnHeader searchResultListName;
        private System.Windows.Forms.ColumnHeader searchResultListCategory;
        private System.Windows.Forms.ColumnHeader searchResultListSubCategory;
        private System.Windows.Forms.ColumnHeader searchResultListLevel;
        private System.Windows.Forms.ColumnHeader searchResultListStatus;
        private System.Windows.Forms.Button guiCancelBtn;
        private System.Windows.Forms.Button guiOkBtn;
    }
}