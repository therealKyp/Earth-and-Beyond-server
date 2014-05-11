namespace Net7_Tools.Gui
{
    partial class FrmItemValidationReport
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
            System.Windows.Forms.ColumnHeader columnHeaderId;
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FrmItemValidationReport));
            this.closeBtn = new System.Windows.Forms.Button();
            this.report = new System.Windows.Forms.ListView();
            this.columnHeaderField = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderError = new System.Windows.Forms.ColumnHeader();
            this.progressBar = new System.Windows.Forms.ProgressBar();
            this.clipboardBtn = new System.Windows.Forms.Button();
            columnHeaderId = new System.Windows.Forms.ColumnHeader();
            this.SuspendLayout();
            // 
            // columnHeaderId
            // 
            columnHeaderId.Text = "ID";
            // 
            // closeBtn
            // 
            this.closeBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.closeBtn.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.closeBtn.Location = new System.Drawing.Point(610, 367);
            this.closeBtn.Name = "closeBtn";
            this.closeBtn.Size = new System.Drawing.Size(75, 23);
            this.closeBtn.TabIndex = 0;
            this.closeBtn.Text = "close";
            this.closeBtn.UseVisualStyleBackColor = true;
            this.closeBtn.Click += new System.EventHandler(this.onClose);
            // 
            // report
            // 
            this.report.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.report.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            columnHeaderId,
            this.columnHeaderField,
            this.columnHeaderError});
            this.report.FullRowSelect = true;
            this.report.GridLines = true;
            this.report.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.report.Location = new System.Drawing.Point(12, 12);
            this.report.Name = "report";
            this.report.Size = new System.Drawing.Size(673, 349);
            this.report.TabIndex = 1;
            this.report.UseCompatibleStateImageBehavior = false;
            this.report.View = System.Windows.Forms.View.Details;
            // 
            // columnHeaderField
            // 
            this.columnHeaderField.Text = "Field";
            this.columnHeaderField.Width = 175;
            // 
            // columnHeaderError
            // 
            this.columnHeaderError.Text = "Error";
            this.columnHeaderError.Width = 433;
            // 
            // progressBar
            // 
            this.progressBar.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.progressBar.Location = new System.Drawing.Point(12, 367);
            this.progressBar.Name = "progressBar";
            this.progressBar.Size = new System.Drawing.Size(501, 23);
            this.progressBar.Step = 1;
            this.progressBar.TabIndex = 2;
            // 
            // clipboardBtn
            // 
            this.clipboardBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.clipboardBtn.Location = new System.Drawing.Point(529, 367);
            this.clipboardBtn.Name = "clipboardBtn";
            this.clipboardBtn.Size = new System.Drawing.Size(75, 23);
            this.clipboardBtn.TabIndex = 3;
            this.clipboardBtn.Text = "Clipboard";
            this.clipboardBtn.UseVisualStyleBackColor = true;
            this.clipboardBtn.Click += new System.EventHandler(this.onClipboard);
            // 
            // FrmItemValidationReport
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CausesValidation = false;
            this.ClientSize = new System.Drawing.Size(697, 402);
            this.Controls.Add(this.clipboardBtn);
            this.Controls.Add(this.progressBar);
            this.Controls.Add(this.report);
            this.Controls.Add(this.closeBtn);
            this.DoubleBuffered = true;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "FrmItemValidationReport";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Item Validation Report";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button closeBtn;
        private System.Windows.Forms.ListView report;
        private System.Windows.Forms.ColumnHeader columnHeaderField;
        private System.Windows.Forms.ColumnHeader columnHeaderError;
        private System.Windows.Forms.ProgressBar progressBar;
        private System.Windows.Forms.Button clipboardBtn;
    }
}