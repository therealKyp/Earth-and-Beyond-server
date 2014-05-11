namespace MissionEditor.Gui
{
    partial class DlgReport
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
            this.guiWebBrowser = new System.Windows.Forms.WebBrowser();
            this.SuspendLayout();
            // 
            // guiWebBrowser
            // 
            this.guiWebBrowser.AllowNavigation = false;
            this.guiWebBrowser.AllowWebBrowserDrop = false;
            this.guiWebBrowser.Dock = System.Windows.Forms.DockStyle.Fill;
            this.guiWebBrowser.Location = new System.Drawing.Point(0, 0);
            this.guiWebBrowser.MinimumSize = new System.Drawing.Size(20, 20);
            this.guiWebBrowser.Name = "guiWebBrowser";
            this.guiWebBrowser.Size = new System.Drawing.Size(541, 492);
            this.guiWebBrowser.TabIndex = 0;
            // 
            // DlgReport
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(541, 492);
            this.Controls.Add(this.guiWebBrowser);
            this.Name = "DlgReport";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "DlgReport";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.WebBrowser guiWebBrowser;
    }
}