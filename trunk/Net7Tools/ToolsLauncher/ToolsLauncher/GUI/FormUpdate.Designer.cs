namespace ToolsLauncher
{
    partial class FormUpdate
    {
        /// <summary>
        /// Erforderliche Designervariable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Verwendete Ressourcen bereinigen.
        /// </summary>
        /// <param name="disposing">True, wenn verwaltete Ressourcen gelöscht werden sollen; andernfalls False.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Vom Windows Form-Designer generierter Code

        /// <summary>
        /// Erforderliche Methode für die Designerunterstützung.
        /// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
        /// </summary>
        private void InitializeComponent()
        {
            this.c_Button_Update = new System.Windows.Forms.Button();
            this.c_Button_Cancel = new System.Windows.Forms.Button();
            this.c_Button_MoreInfo = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.c_ProgressBar2 = new System.Windows.Forms.ProgressBar();
            this.c_ProgressBar1 = new System.Windows.Forms.ProgressBar();
            this.c_LabelProgress2 = new System.Windows.Forms.Label();
            this.c_LabelProgress1 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label4 = new System.Windows.Forms.Label();
            this.c_Label_NewVersion = new System.Windows.Forms.Label();
            this.c_Label_CurrentVersion = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.c_Label_Application = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.c_PictureBox = new System.Windows.Forms.PictureBox();
            this.c_Button_Skip = new System.Windows.Forms.Button();
            this.c_TextBox_Changelog = new System.Windows.Forms.TextBox();
            this.c_CheckBox_WordWrap = new System.Windows.Forms.CheckBox();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.c_PictureBox)).BeginInit();
            this.SuspendLayout();
            // 
            // c_Button_Update
            // 
            this.c_Button_Update.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.c_Button_Update.Location = new System.Drawing.Point(379, 257);
            this.c_Button_Update.Name = "c_Button_Update";
            this.c_Button_Update.Size = new System.Drawing.Size(75, 23);
            this.c_Button_Update.TabIndex = 0;
            this.c_Button_Update.Text = "&Update";
            this.c_Button_Update.UseVisualStyleBackColor = true;
            this.c_Button_Update.Click += new System.EventHandler(this.c_Button_Update_Click);
            // 
            // c_Button_Cancel
            // 
            this.c_Button_Cancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.c_Button_Cancel.Location = new System.Drawing.Point(541, 257);
            this.c_Button_Cancel.Name = "c_Button_Cancel";
            this.c_Button_Cancel.Size = new System.Drawing.Size(75, 23);
            this.c_Button_Cancel.TabIndex = 1;
            this.c_Button_Cancel.Text = "&Cancel";
            this.c_Button_Cancel.UseVisualStyleBackColor = true;
            this.c_Button_Cancel.Click += new System.EventHandler(this.c_Button_Cancel_Click);
            // 
            // c_Button_MoreInfo
            // 
            this.c_Button_MoreInfo.Enabled = false;
            this.c_Button_MoreInfo.Location = new System.Drawing.Point(66, 257);
            this.c_Button_MoreInfo.Name = "c_Button_MoreInfo";
            this.c_Button_MoreInfo.Size = new System.Drawing.Size(75, 23);
            this.c_Button_MoreInfo.TabIndex = 2;
            this.c_Button_MoreInfo.Text = "Details >>";
            this.c_Button_MoreInfo.UseVisualStyleBackColor = true;
            this.c_Button_MoreInfo.Click += new System.EventHandler(this.c_Button_MoreInfo_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.c_ProgressBar2);
            this.groupBox1.Controls.Add(this.c_ProgressBar1);
            this.groupBox1.Controls.Add(this.c_LabelProgress2);
            this.groupBox1.Controls.Add(this.c_LabelProgress1);
            this.groupBox1.Location = new System.Drawing.Point(66, 136);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(550, 115);
            this.groupBox1.TabIndex = 4;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Progress";
            // 
            // c_ProgressBar2
            // 
            this.c_ProgressBar2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.c_ProgressBar2.Enabled = false;
            this.c_ProgressBar2.Location = new System.Drawing.Point(9, 80);
            this.c_ProgressBar2.Name = "c_ProgressBar2";
            this.c_ProgressBar2.Size = new System.Drawing.Size(535, 23);
            this.c_ProgressBar2.TabIndex = 3;
            this.c_ProgressBar2.Visible = false;
            // 
            // c_ProgressBar1
            // 
            this.c_ProgressBar1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.c_ProgressBar1.Enabled = false;
            this.c_ProgressBar1.Location = new System.Drawing.Point(9, 35);
            this.c_ProgressBar1.Name = "c_ProgressBar1";
            this.c_ProgressBar1.Size = new System.Drawing.Size(535, 23);
            this.c_ProgressBar1.TabIndex = 2;
            this.c_ProgressBar1.Visible = false;
            // 
            // c_LabelProgress2
            // 
            this.c_LabelProgress2.AutoSize = true;
            this.c_LabelProgress2.Location = new System.Drawing.Point(6, 64);
            this.c_LabelProgress2.Margin = new System.Windows.Forms.Padding(3, 3, 3, 0);
            this.c_LabelProgress2.Name = "c_LabelProgress2";
            this.c_LabelProgress2.Size = new System.Drawing.Size(60, 13);
            this.c_LabelProgress2.TabIndex = 1;
            this.c_LabelProgress2.Text = "[Progress2]";
            // 
            // c_LabelProgress1
            // 
            this.c_LabelProgress1.AutoSize = true;
            this.c_LabelProgress1.Location = new System.Drawing.Point(6, 19);
            this.c_LabelProgress1.Margin = new System.Windows.Forms.Padding(3, 3, 3, 0);
            this.c_LabelProgress1.Name = "c_LabelProgress1";
            this.c_LabelProgress1.Size = new System.Drawing.Size(60, 13);
            this.c_LabelProgress1.TabIndex = 0;
            this.c_LabelProgress1.Text = "[Progress1]";
            // 
            // groupBox2
            // 
            this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.c_Label_NewVersion);
            this.groupBox2.Controls.Add(this.c_Label_CurrentVersion);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.c_Label_Application);
            this.groupBox2.Controls.Add(this.label1);
            this.groupBox2.Location = new System.Drawing.Point(66, 12);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(550, 118);
            this.groupBox2.TabIndex = 5;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Info";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 82);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(67, 13);
            this.label4.TabIndex = 4;
            this.label4.Text = "New Version";
            // 
            // c_Label_NewVersion
            // 
            this.c_Label_NewVersion.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.c_Label_NewVersion.AutoEllipsis = true;
            this.c_Label_NewVersion.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.c_Label_NewVersion.Location = new System.Drawing.Point(91, 77);
            this.c_Label_NewVersion.Margin = new System.Windows.Forms.Padding(3);
            this.c_Label_NewVersion.Name = "c_Label_NewVersion";
            this.c_Label_NewVersion.Size = new System.Drawing.Size(453, 23);
            this.c_Label_NewVersion.TabIndex = 3;
            this.c_Label_NewVersion.Text = "label5";
            this.c_Label_NewVersion.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // c_Label_CurrentVersion
            // 
            this.c_Label_CurrentVersion.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.c_Label_CurrentVersion.AutoEllipsis = true;
            this.c_Label_CurrentVersion.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.c_Label_CurrentVersion.Location = new System.Drawing.Point(91, 48);
            this.c_Label_CurrentVersion.Margin = new System.Windows.Forms.Padding(3);
            this.c_Label_CurrentVersion.Name = "c_Label_CurrentVersion";
            this.c_Label_CurrentVersion.Size = new System.Drawing.Size(453, 23);
            this.c_Label_CurrentVersion.TabIndex = 2;
            this.c_Label_CurrentVersion.Text = "label2";
            this.c_Label_CurrentVersion.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 53);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(79, 13);
            this.label3.TabIndex = 3;
            this.label3.Text = "Current Version";
            // 
            // c_Label_Application
            // 
            this.c_Label_Application.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.c_Label_Application.AutoEllipsis = true;
            this.c_Label_Application.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.c_Label_Application.Location = new System.Drawing.Point(91, 19);
            this.c_Label_Application.Margin = new System.Windows.Forms.Padding(3);
            this.c_Label_Application.Name = "c_Label_Application";
            this.c_Label_Application.Size = new System.Drawing.Size(453, 23);
            this.c_Label_Application.TabIndex = 1;
            this.c_Label_Application.Text = "label2";
            this.c_Label_Application.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 24);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(59, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Application";
            // 
            // c_PictureBox
            // 
            this.c_PictureBox.Location = new System.Drawing.Point(12, 12);
            this.c_PictureBox.Name = "c_PictureBox";
            this.c_PictureBox.Size = new System.Drawing.Size(48, 48);
            this.c_PictureBox.TabIndex = 3;
            this.c_PictureBox.TabStop = false;
            // 
            // c_Button_Skip
            // 
            this.c_Button_Skip.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.c_Button_Skip.Location = new System.Drawing.Point(460, 257);
            this.c_Button_Skip.Name = "c_Button_Skip";
            this.c_Button_Skip.Size = new System.Drawing.Size(75, 23);
            this.c_Button_Skip.TabIndex = 6;
            this.c_Button_Skip.Text = "&Skip";
            this.c_Button_Skip.UseVisualStyleBackColor = true;
            this.c_Button_Skip.Click += new System.EventHandler(this.c_Button_Skip_Click);
            // 
            // c_TextBox_Changelog
            // 
            this.c_TextBox_Changelog.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.c_TextBox_Changelog.Font = new System.Drawing.Font("Courier New", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_TextBox_Changelog.Location = new System.Drawing.Point(66, 286);
            this.c_TextBox_Changelog.Multiline = true;
            this.c_TextBox_Changelog.Name = "c_TextBox_Changelog";
            this.c_TextBox_Changelog.ReadOnly = true;
            this.c_TextBox_Changelog.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.c_TextBox_Changelog.Size = new System.Drawing.Size(550, 170);
            this.c_TextBox_Changelog.TabIndex = 8;
            // 
            // c_CheckBox_WordWrap
            // 
            this.c_CheckBox_WordWrap.AutoSize = true;
            this.c_CheckBox_WordWrap.Checked = true;
            this.c_CheckBox_WordWrap.CheckState = System.Windows.Forms.CheckState.Checked;
            this.c_CheckBox_WordWrap.Location = new System.Drawing.Point(147, 261);
            this.c_CheckBox_WordWrap.Name = "c_CheckBox_WordWrap";
            this.c_CheckBox_WordWrap.Size = new System.Drawing.Size(81, 17);
            this.c_CheckBox_WordWrap.TabIndex = 9;
            this.c_CheckBox_WordWrap.Text = "Word-Wrap";
            this.c_CheckBox_WordWrap.UseVisualStyleBackColor = true;
            // 
            // FormUpdate
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(628, 468);
            this.Controls.Add(this.c_CheckBox_WordWrap);
            this.Controls.Add(this.c_TextBox_Changelog);
            this.Controls.Add(this.c_Button_Skip);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.c_PictureBox);
            this.Controls.Add(this.c_Button_MoreInfo);
            this.Controls.Add(this.c_Button_Cancel);
            this.Controls.Add(this.c_Button_Update);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.MinimumSize = new System.Drawing.Size(634, 319);
            this.Name = "FormUpdate";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Update available";
            this.Load += new System.EventHandler(this.FormUpdate_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.c_PictureBox)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button c_Button_Update;
        private System.Windows.Forms.Button c_Button_Cancel;
        private System.Windows.Forms.Button c_Button_MoreInfo;
        private System.Windows.Forms.PictureBox c_PictureBox;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label c_Label_Application;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label c_Label_NewVersion;
        private System.Windows.Forms.Label c_Label_CurrentVersion;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ProgressBar c_ProgressBar2;
        private System.Windows.Forms.ProgressBar c_ProgressBar1;
        private System.Windows.Forms.Label c_LabelProgress2;
        private System.Windows.Forms.Label c_LabelProgress1;
        private System.Windows.Forms.Button c_Button_Skip;
        private System.Windows.Forms.TextBox c_TextBox_Changelog;
        private System.Windows.Forms.CheckBox c_CheckBox_WordWrap;
    }
}