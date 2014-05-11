namespace LaunchNet7
{
    partial class AdvancedSettings
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
            this.ForceUpdate = new System.Windows.Forms.Button();
            this.AuthLog = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // ForceUpdate
            // 
            this.ForceUpdate.Location = new System.Drawing.Point(12, 12);
            this.ForceUpdate.Name = "ForceUpdate";
            this.ForceUpdate.Size = new System.Drawing.Size(117, 35);
            this.ForceUpdate.TabIndex = 0;
            this.ForceUpdate.Text = "Force Update";
            this.ForceUpdate.UseVisualStyleBackColor = true;
            this.ForceUpdate.Click += new System.EventHandler(this.ForceUpdate_Click);
            // 
            // AuthLog
            // 
            this.AuthLog.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.AuthLog.Location = new System.Drawing.Point(12, 69);
            this.AuthLog.Multiline = true;
            this.AuthLog.Name = "AuthLog";
            this.AuthLog.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.AuthLog.Size = new System.Drawing.Size(468, 135);
            this.AuthLog.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 53);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(53, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Auth Log:";
            // 
            // AdvancedSettings
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(494, 215);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.AuthLog);
            this.Controls.Add(this.ForceUpdate);
            this.Name = "AdvancedSettings";
            this.Text = "Advanced Settings";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button ForceUpdate;
        private System.Windows.Forms.TextBox AuthLog;
        private System.Windows.Forms.Label label1;
    }
}