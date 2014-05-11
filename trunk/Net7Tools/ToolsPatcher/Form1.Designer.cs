namespace ToolsPatcher
{
    partial class Form1
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.VerLabel = new System.Windows.Forms.Label();
            this.ClientVer = new System.Windows.Forms.Label();
            this.FileProgress = new System.Windows.Forms.ProgressBar();
            this.CFileName = new System.Windows.Forms.Label();
            this.ProgressValue = new System.Windows.Forms.Label();
            this.PatchNotes = new System.Windows.Forms.WebBrowser();
            this.TotalProgressValue = new System.Windows.Forms.Label();
            this.TotalProgress = new System.Windows.Forms.ProgressBar();
            this.label2 = new System.Windows.Forms.Label();
            this.StartUpdate = new System.Windows.Forms.Timer(this.components);
            this.SuspendLayout();
            // 
            // VerLabel
            // 
            this.VerLabel.AutoSize = true;
            this.VerLabel.Location = new System.Drawing.Point(12, 9);
            this.VerLabel.Name = "VerLabel";
            this.VerLabel.Size = new System.Drawing.Size(82, 13);
            this.VerLabel.TabIndex = 1;
            this.VerLabel.Text = "Server Version: ";
            // 
            // ClientVer
            // 
            this.ClientVer.AutoSize = true;
            this.ClientVer.Location = new System.Drawing.Point(12, 22);
            this.ClientVer.Name = "ClientVer";
            this.ClientVer.Size = new System.Drawing.Size(77, 13);
            this.ClientVer.TabIndex = 2;
            this.ClientVer.Text = "Client Version: ";
            // 
            // FileProgress
            // 
            this.FileProgress.Location = new System.Drawing.Point(152, 25);
            this.FileProgress.MarqueeAnimationSpeed = 1;
            this.FileProgress.Name = "FileProgress";
            this.FileProgress.Size = new System.Drawing.Size(268, 24);
            this.FileProgress.Style = System.Windows.Forms.ProgressBarStyle.Continuous;
            this.FileProgress.TabIndex = 3;
            // 
            // CFileName
            // 
            this.CFileName.AutoSize = true;
            this.CFileName.Location = new System.Drawing.Point(149, 9);
            this.CFileName.Name = "CFileName";
            this.CFileName.Size = new System.Drawing.Size(91, 13);
            this.CFileName.TabIndex = 4;
            this.CFileName.Text = "File Downloading:";
            // 
            // ProgressValue
            // 
            this.ProgressValue.AutoSize = true;
            this.ProgressValue.Location = new System.Drawing.Point(276, 31);
            this.ProgressValue.Name = "ProgressValue";
            this.ProgressValue.Size = new System.Drawing.Size(21, 13);
            this.ProgressValue.TabIndex = 5;
            this.ProgressValue.Text = "0%";
            // 
            // PatchNotes
            // 
            this.PatchNotes.Location = new System.Drawing.Point(12, 98);
            this.PatchNotes.MinimumSize = new System.Drawing.Size(20, 20);
            this.PatchNotes.Name = "PatchNotes";
            this.PatchNotes.Size = new System.Drawing.Size(549, 252);
            this.PatchNotes.TabIndex = 6;
            this.PatchNotes.Url = new System.Uri("http://toolspatch.net-7.org/PatchNotes.html", System.UriKind.Absolute);
            // 
            // TotalProgressValue
            // 
            this.TotalProgressValue.AutoSize = true;
            this.TotalProgressValue.Location = new System.Drawing.Point(276, 74);
            this.TotalProgressValue.Name = "TotalProgressValue";
            this.TotalProgressValue.Size = new System.Drawing.Size(21, 13);
            this.TotalProgressValue.TabIndex = 8;
            this.TotalProgressValue.Text = "0%";
            // 
            // TotalProgress
            // 
            this.TotalProgress.Location = new System.Drawing.Point(152, 68);
            this.TotalProgress.MarqueeAnimationSpeed = 1;
            this.TotalProgress.Name = "TotalProgress";
            this.TotalProgress.Size = new System.Drawing.Size(268, 24);
            this.TotalProgress.Style = System.Windows.Forms.ProgressBarStyle.Continuous;
            this.TotalProgress.TabIndex = 7;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(149, 52);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(78, 13);
            this.label2.TabIndex = 9;
            this.label2.Text = "Total Progress:";
            // 
            // StartUpdate
            // 
            this.StartUpdate.Enabled = true;
            this.StartUpdate.Interval = 500;
            this.StartUpdate.Tick += new System.EventHandler(this.StartUpdate_Tick);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(573, 363);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.TotalProgressValue);
            this.Controls.Add(this.TotalProgress);
            this.Controls.Add(this.PatchNotes);
            this.Controls.Add(this.ProgressValue);
            this.Controls.Add(this.CFileName);
            this.Controls.Add(this.FileProgress);
            this.Controls.Add(this.ClientVer);
            this.Controls.Add(this.VerLabel);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Form1";
            this.Text = "E&B Tools Patcher";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label VerLabel;
        private System.Windows.Forms.Label ClientVer;
        private System.Windows.Forms.ProgressBar FileProgress;
        private System.Windows.Forms.Label CFileName;
        private System.Windows.Forms.Label ProgressValue;
        private System.Windows.Forms.WebBrowser PatchNotes;
        private System.Windows.Forms.Label TotalProgressValue;
        private System.Windows.Forms.ProgressBar TotalProgress;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Timer StartUpdate;

    }
}

