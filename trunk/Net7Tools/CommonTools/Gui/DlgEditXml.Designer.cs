namespace CommonTools.Gui
{
    partial class DlgEditXml
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
            this.guiOkBtn = new System.Windows.Forms.Button();
            this.guiCancelBtn = new System.Windows.Forms.Button();
            this.guiXmlTxt = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // guiOkBtn
            // 
            this.guiOkBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.guiOkBtn.Enabled = false;
            this.guiOkBtn.Location = new System.Drawing.Point(460, 573);
            this.guiOkBtn.Name = "guiOkBtn";
            this.guiOkBtn.Size = new System.Drawing.Size(75, 23);
            this.guiOkBtn.TabIndex = 1;
            this.guiOkBtn.Text = "Ok";
            this.guiOkBtn.UseVisualStyleBackColor = true;
            this.guiOkBtn.Click += new System.EventHandler(this.onOk);
            // 
            // guiCancelBtn
            // 
            this.guiCancelBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.guiCancelBtn.Location = new System.Drawing.Point(541, 573);
            this.guiCancelBtn.Name = "guiCancelBtn";
            this.guiCancelBtn.Size = new System.Drawing.Size(75, 23);
            this.guiCancelBtn.TabIndex = 2;
            this.guiCancelBtn.Text = "Cancel";
            this.guiCancelBtn.UseVisualStyleBackColor = true;
            this.guiCancelBtn.Click += new System.EventHandler(this.onCancel);
            // 
            // guiXmlTxt
            // 
            this.guiXmlTxt.AcceptsReturn = true;
            this.guiXmlTxt.AcceptsTab = true;
            this.guiXmlTxt.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.guiXmlTxt.Location = new System.Drawing.Point(11, 12);
            this.guiXmlTxt.Multiline = true;
            this.guiXmlTxt.Name = "guiXmlTxt";
            this.guiXmlTxt.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.guiXmlTxt.Size = new System.Drawing.Size(604, 549);
            this.guiXmlTxt.TabIndex = 0;
            this.guiXmlTxt.WordWrap = false;
            this.guiXmlTxt.KeyDown += new System.Windows.Forms.KeyEventHandler(this.onKeyDown);
            // 
            // DlgEditXml
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(628, 608);
            this.Controls.Add(this.guiXmlTxt);
            this.Controls.Add(this.guiCancelBtn);
            this.Controls.Add(this.guiOkBtn);
            this.Name = "DlgEditXml";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Edit XML";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button guiOkBtn;
        private System.Windows.Forms.Button guiCancelBtn;
        private System.Windows.Forms.TextBox guiXmlTxt;
    }
}