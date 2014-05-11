namespace DataImport
{
    partial class DataImport
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
            this.guiTableCbo = new System.Windows.Forms.ComboBox();
            this.guiTableLbl = new System.Windows.Forms.Label();
            this.guiFileTxt = new System.Windows.Forms.TextBox();
            this.guiFileBtn = new System.Windows.Forms.Button();
            this.guiFileLbl = new System.Windows.Forms.Label();
            this.guiImportBtn = new System.Windows.Forms.Button();
            this.guiCloseBtn = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // guiTableCbo
            // 
            this.guiTableCbo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.guiTableCbo.FormattingEnabled = true;
            this.guiTableCbo.Location = new System.Drawing.Point(51, 12);
            this.guiTableCbo.Name = "guiTableCbo";
            this.guiTableCbo.Size = new System.Drawing.Size(342, 21);
            this.guiTableCbo.TabIndex = 0;
            // 
            // guiTableLbl
            // 
            this.guiTableLbl.AutoSize = true;
            this.guiTableLbl.Location = new System.Drawing.Point(8, 15);
            this.guiTableLbl.Name = "guiTableLbl";
            this.guiTableLbl.Size = new System.Drawing.Size(37, 13);
            this.guiTableLbl.TabIndex = 1;
            this.guiTableLbl.Text = "Table:";
            // 
            // guiFileTxt
            // 
            this.guiFileTxt.Location = new System.Drawing.Point(51, 39);
            this.guiFileTxt.Name = "guiFileTxt";
            this.guiFileTxt.Size = new System.Drawing.Size(342, 20);
            this.guiFileTxt.TabIndex = 2;
            // 
            // guiFileBtn
            // 
            this.guiFileBtn.Location = new System.Drawing.Point(399, 37);
            this.guiFileBtn.Name = "guiFileBtn";
            this.guiFileBtn.Size = new System.Drawing.Size(75, 23);
            this.guiFileBtn.TabIndex = 3;
            this.guiFileBtn.Text = "Browse";
            this.guiFileBtn.UseVisualStyleBackColor = true;
            this.guiFileBtn.Click += new System.EventHandler(this.onBrowse);
            // 
            // guiFileLbl
            // 
            this.guiFileLbl.AutoSize = true;
            this.guiFileLbl.Location = new System.Drawing.Point(19, 42);
            this.guiFileLbl.Name = "guiFileLbl";
            this.guiFileLbl.Size = new System.Drawing.Size(26, 13);
            this.guiFileLbl.TabIndex = 4;
            this.guiFileLbl.Text = "File:";
            // 
            // guiImportBtn
            // 
            this.guiImportBtn.Location = new System.Drawing.Point(318, 248);
            this.guiImportBtn.Name = "guiImportBtn";
            this.guiImportBtn.Size = new System.Drawing.Size(75, 23);
            this.guiImportBtn.TabIndex = 5;
            this.guiImportBtn.Text = "Import";
            this.guiImportBtn.UseVisualStyleBackColor = true;
            this.guiImportBtn.Click += new System.EventHandler(this.onImport);
            // 
            // guiCloseBtn
            // 
            this.guiCloseBtn.Location = new System.Drawing.Point(399, 248);
            this.guiCloseBtn.Name = "guiCloseBtn";
            this.guiCloseBtn.Size = new System.Drawing.Size(75, 23);
            this.guiCloseBtn.TabIndex = 6;
            this.guiCloseBtn.Text = "Close";
            this.guiCloseBtn.UseVisualStyleBackColor = true;
            this.guiCloseBtn.Click += new System.EventHandler(this.onClose);
            // 
            // DataImport
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(486, 283);
            this.Controls.Add(this.guiCloseBtn);
            this.Controls.Add(this.guiImportBtn);
            this.Controls.Add(this.guiFileLbl);
            this.Controls.Add(this.guiFileBtn);
            this.Controls.Add(this.guiFileTxt);
            this.Controls.Add(this.guiTableLbl);
            this.Controls.Add(this.guiTableCbo);
            this.Name = "DataImport";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Data Import";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox guiTableCbo;
        private System.Windows.Forms.Label guiTableLbl;
        private System.Windows.Forms.TextBox guiFileTxt;
        private System.Windows.Forms.Button guiFileBtn;
        private System.Windows.Forms.Label guiFileLbl;
        private System.Windows.Forms.Button guiImportBtn;
        private System.Windows.Forms.Button guiCloseBtn;
    }
}

