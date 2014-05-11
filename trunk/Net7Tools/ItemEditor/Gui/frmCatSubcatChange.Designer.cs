namespace Net7_Tools.Gui
{
    partial class frmCatSubcatChange
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
            System.Windows.Forms.Label baseSubCategoryLabel;
            System.Windows.Forms.Label baseCategoryLabel;
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frmCatSubcatChange));
            this.cboSubCategory = new System.Windows.Forms.ComboBox();
            this.cboCategory = new System.Windows.Forms.ComboBox();
            this.manufacturingCancel = new System.Windows.Forms.Button();
            this.manufacturingOk = new System.Windows.Forms.Button();
            baseSubCategoryLabel = new System.Windows.Forms.Label();
            baseCategoryLabel = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // baseSubCategoryLabel
            // 
            baseSubCategoryLabel.AutoSize = true;
            baseSubCategoryLabel.Location = new System.Drawing.Point(11, 42);
            baseSubCategoryLabel.Name = "baseSubCategoryLabel";
            baseSubCategoryLabel.Size = new System.Drawing.Size(71, 13);
            baseSubCategoryLabel.TabIndex = 2;
            baseSubCategoryLabel.Text = "SubCategory:";
            // 
            // baseCategoryLabel
            // 
            baseCategoryLabel.AutoSize = true;
            baseCategoryLabel.Location = new System.Drawing.Point(28, 15);
            baseCategoryLabel.Name = "baseCategoryLabel";
            baseCategoryLabel.Size = new System.Drawing.Size(52, 13);
            baseCategoryLabel.TabIndex = 0;
            baseCategoryLabel.Text = "Category:";
            // 
            // cboSubCategory
            // 
            this.cboSubCategory.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboSubCategory.FormattingEnabled = true;
            this.cboSubCategory.Location = new System.Drawing.Point(86, 39);
            this.cboSubCategory.Name = "cboSubCategory";
            this.cboSubCategory.Size = new System.Drawing.Size(331, 21);
            this.cboSubCategory.TabIndex = 3;
            // 
            // cboCategory
            // 
            this.cboCategory.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboCategory.FormattingEnabled = true;
            this.cboCategory.Location = new System.Drawing.Point(86, 12);
            this.cboCategory.Name = "cboCategory";
            this.cboCategory.Size = new System.Drawing.Size(331, 21);
            this.cboCategory.TabIndex = 1;
            // 
            // manufacturingCancel
            // 
            this.manufacturingCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.manufacturingCancel.Location = new System.Drawing.Point(342, 83);
            this.manufacturingCancel.Name = "manufacturingCancel";
            this.manufacturingCancel.Size = new System.Drawing.Size(75, 23);
            this.manufacturingCancel.TabIndex = 5;
            this.manufacturingCancel.Text = "Cancel";
            this.manufacturingCancel.UseVisualStyleBackColor = true;
            this.manufacturingCancel.Click += new System.EventHandler(this.OnCancel);
            // 
            // manufacturingOk
            // 
            this.manufacturingOk.Location = new System.Drawing.Point(261, 83);
            this.manufacturingOk.Name = "manufacturingOk";
            this.manufacturingOk.Size = new System.Drawing.Size(75, 23);
            this.manufacturingOk.TabIndex = 4;
            this.manufacturingOk.Text = "Ok";
            this.manufacturingOk.UseVisualStyleBackColor = true;
            this.manufacturingOk.Click += new System.EventHandler(this.onOk);
            // 
            // frmCatSubcatChange
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(426, 119);
            this.Controls.Add(this.manufacturingCancel);
            this.Controls.Add(this.manufacturingOk);
            this.Controls.Add(this.cboSubCategory);
            this.Controls.Add(baseSubCategoryLabel);
            this.Controls.Add(baseCategoryLabel);
            this.Controls.Add(this.cboCategory);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "frmCatSubcatChange";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "frmCatSubcatChange";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox cboSubCategory;
        private System.Windows.Forms.ComboBox cboCategory;
        public System.Windows.Forms.Button manufacturingCancel;
        public System.Windows.Forms.Button manufacturingOk;
    }
}