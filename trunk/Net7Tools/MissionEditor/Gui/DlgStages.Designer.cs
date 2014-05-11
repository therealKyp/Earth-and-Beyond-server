namespace MissionEditor.Gui
{
    partial class DlgStages
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
            this.guiDescriptionLbl = new System.Windows.Forms.Label();
            this.guiDescriptionTxt = new System.Windows.Forms.TextBox();
            this.guiCancelBtn = new System.Windows.Forms.Button();
            this.guiOkBtn = new System.Windows.Forms.Button();
            this.guiIdLbl = new System.Windows.Forms.Label();
            this.guiIdTxt = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // guiDescriptionLbl
            // 
            this.guiDescriptionLbl.AutoSize = true;
            this.guiDescriptionLbl.Location = new System.Drawing.Point(3, 35);
            this.guiDescriptionLbl.Name = "guiDescriptionLbl";
            this.guiDescriptionLbl.Size = new System.Drawing.Size(63, 13);
            this.guiDescriptionLbl.TabIndex = 2;
            this.guiDescriptionLbl.Text = "Description:";
            // 
            // guiDescriptionTxt
            // 
            this.guiDescriptionTxt.Location = new System.Drawing.Point(72, 32);
            this.guiDescriptionTxt.Multiline = true;
            this.guiDescriptionTxt.Name = "guiDescriptionTxt";
            this.guiDescriptionTxt.Size = new System.Drawing.Size(433, 78);
            this.guiDescriptionTxt.TabIndex = 3;
            // 
            // guiCancelBtn
            // 
            this.guiCancelBtn.Location = new System.Drawing.Point(430, 116);
            this.guiCancelBtn.Name = "guiCancelBtn";
            this.guiCancelBtn.Size = new System.Drawing.Size(75, 23);
            this.guiCancelBtn.TabIndex = 5;
            this.guiCancelBtn.Text = "Cancel";
            this.guiCancelBtn.UseVisualStyleBackColor = true;
            this.guiCancelBtn.Click += new System.EventHandler(this.onCancel);
            // 
            // guiOkBtn
            // 
            this.guiOkBtn.Location = new System.Drawing.Point(349, 116);
            this.guiOkBtn.Name = "guiOkBtn";
            this.guiOkBtn.Size = new System.Drawing.Size(75, 23);
            this.guiOkBtn.TabIndex = 4;
            this.guiOkBtn.Text = "Ok";
            this.guiOkBtn.UseVisualStyleBackColor = true;
            this.guiOkBtn.Click += new System.EventHandler(this.onOk);
            // 
            // guiIdLbl
            // 
            this.guiIdLbl.AutoSize = true;
            this.guiIdLbl.Location = new System.Drawing.Point(47, 9);
            this.guiIdLbl.Name = "guiIdLbl";
            this.guiIdLbl.Size = new System.Drawing.Size(19, 13);
            this.guiIdLbl.TabIndex = 0;
            this.guiIdLbl.Text = "Id:";
            // 
            // guiIdTxt
            // 
            this.guiIdTxt.Enabled = false;
            this.guiIdTxt.Location = new System.Drawing.Point(72, 6);
            this.guiIdTxt.Name = "guiIdTxt";
            this.guiIdTxt.Size = new System.Drawing.Size(46, 20);
            this.guiIdTxt.TabIndex = 1;
            // 
            // DlgStages
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(517, 151);
            this.Controls.Add(this.guiIdTxt);
            this.Controls.Add(this.guiIdLbl);
            this.Controls.Add(this.guiCancelBtn);
            this.Controls.Add(this.guiOkBtn);
            this.Controls.Add(this.guiDescriptionTxt);
            this.Controls.Add(this.guiDescriptionLbl);
            this.Name = "DlgStages";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "DlgStages";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label guiDescriptionLbl;
        private System.Windows.Forms.TextBox guiDescriptionTxt;
        private System.Windows.Forms.Button guiCancelBtn;
        private System.Windows.Forms.Button guiOkBtn;
        private System.Windows.Forms.Label guiIdLbl;
        private System.Windows.Forms.TextBox guiIdTxt;
    }
}