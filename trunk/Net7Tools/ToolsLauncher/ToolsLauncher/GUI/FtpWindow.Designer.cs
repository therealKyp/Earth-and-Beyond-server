namespace ToolsLauncher
{
    partial class FtpWindow
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
            this.FtpBrowser = new System.Windows.Forms.WebBrowser();
            this.SuspendLayout();
            // 
            // FtpBrowser
            // 
            this.FtpBrowser.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.FtpBrowser.Location = new System.Drawing.Point(2, 1);
            this.FtpBrowser.MinimumSize = new System.Drawing.Size(20, 20);
            this.FtpBrowser.Name = "FtpBrowser";
            this.FtpBrowser.Size = new System.Drawing.Size(542, 260);
            this.FtpBrowser.TabIndex = 0;
            // 
            // FtpWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(544, 260);
            this.Controls.Add(this.FtpBrowser);
            this.Name = "FtpWindow";
            this.Text = "FtpWindow";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.FtpWindow_FormClosed);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.WebBrowser FtpBrowser;
    }
}