namespace MissionEditor.Gui
{
    partial class DlgRewards
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
            this.guiTypeLbl = new System.Windows.Forms.Label();
            this.guiTypeCbo = new System.Windows.Forms.ComboBox();
            this.guiOkBtn = new System.Windows.Forms.Button();
            this.guiCancelBtn = new System.Windows.Forms.Button();
            this.guiAmountTxt = new System.Windows.Forms.TextBox();
            this.guiCodeLbl = new System.Windows.Forms.Label();
            this.guiCodeDescriptionTxt = new System.Windows.Forms.TextBox();
            this.guiCodeSearchBtn = new System.Windows.Forms.Button();
            this.guiCodeTxt = new System.Windows.Forms.TextBox();
            this.guiAmountLbl = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // guiTypeLbl
            // 
            this.guiTypeLbl.AutoSize = true;
            this.guiTypeLbl.Location = new System.Drawing.Point(20, 13);
            this.guiTypeLbl.Name = "guiTypeLbl";
            this.guiTypeLbl.Size = new System.Drawing.Size(34, 13);
            this.guiTypeLbl.TabIndex = 0;
            this.guiTypeLbl.Text = "Type:";
            // 
            // guiTypeCbo
            // 
            this.guiTypeCbo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.guiTypeCbo.FormattingEnabled = true;
            this.guiTypeCbo.Location = new System.Drawing.Point(60, 10);
            this.guiTypeCbo.Name = "guiTypeCbo";
            this.guiTypeCbo.Size = new System.Drawing.Size(341, 21);
            this.guiTypeCbo.TabIndex = 1;
            this.guiTypeCbo.SelectedIndexChanged += new System.EventHandler(this.onTypeSelected);
            // 
            // guiOkBtn
            // 
            this.guiOkBtn.Location = new System.Drawing.Point(245, 115);
            this.guiOkBtn.Name = "guiOkBtn";
            this.guiOkBtn.Size = new System.Drawing.Size(75, 23);
            this.guiOkBtn.TabIndex = 8;
            this.guiOkBtn.Text = "Ok";
            this.guiOkBtn.UseVisualStyleBackColor = true;
            this.guiOkBtn.Click += new System.EventHandler(this.onOk);
            // 
            // guiCancelBtn
            // 
            this.guiCancelBtn.Location = new System.Drawing.Point(326, 115);
            this.guiCancelBtn.Name = "guiCancelBtn";
            this.guiCancelBtn.Size = new System.Drawing.Size(75, 23);
            this.guiCancelBtn.TabIndex = 9;
            this.guiCancelBtn.Text = "Cancel";
            this.guiCancelBtn.UseVisualStyleBackColor = true;
            this.guiCancelBtn.Click += new System.EventHandler(this.onCancel);
            // 
            // guiAmountTxt
            // 
            this.guiAmountTxt.Location = new System.Drawing.Point(60, 37);
            this.guiAmountTxt.Name = "guiAmountTxt";
            this.guiAmountTxt.Size = new System.Drawing.Size(71, 20);
            this.guiAmountTxt.TabIndex = 3;
            // 
            // guiCodeLbl
            // 
            this.guiCodeLbl.AutoSize = true;
            this.guiCodeLbl.Location = new System.Drawing.Point(19, 68);
            this.guiCodeLbl.Name = "guiCodeLbl";
            this.guiCodeLbl.Size = new System.Drawing.Size(35, 13);
            this.guiCodeLbl.TabIndex = 4;
            this.guiCodeLbl.Text = "Code:";
            // 
            // guiCodeDescriptionTxt
            // 
            this.guiCodeDescriptionTxt.Location = new System.Drawing.Point(108, 65);
            this.guiCodeDescriptionTxt.Name = "guiCodeDescriptionTxt";
            this.guiCodeDescriptionTxt.ReadOnly = true;
            this.guiCodeDescriptionTxt.Size = new System.Drawing.Size(235, 20);
            this.guiCodeDescriptionTxt.TabIndex = 6;
            this.guiCodeDescriptionTxt.TabStop = false;
            // 
            // guiCodeSearchBtn
            // 
            this.guiCodeSearchBtn.Location = new System.Drawing.Point(349, 63);
            this.guiCodeSearchBtn.Name = "guiCodeSearchBtn";
            this.guiCodeSearchBtn.Size = new System.Drawing.Size(52, 23);
            this.guiCodeSearchBtn.TabIndex = 7;
            this.guiCodeSearchBtn.Text = "Search";
            this.guiCodeSearchBtn.UseVisualStyleBackColor = true;
            // 
            // guiCodeTxt
            // 
            this.guiCodeTxt.Location = new System.Drawing.Point(60, 65);
            this.guiCodeTxt.Name = "guiCodeTxt";
            this.guiCodeTxt.Size = new System.Drawing.Size(43, 20);
            this.guiCodeTxt.TabIndex = 5;
            // 
            // guiAmountLbl
            // 
            this.guiAmountLbl.AutoSize = true;
            this.guiAmountLbl.Location = new System.Drawing.Point(12, 40);
            this.guiAmountLbl.Name = "guiAmountLbl";
            this.guiAmountLbl.Size = new System.Drawing.Size(46, 13);
            this.guiAmountLbl.TabIndex = 2;
            this.guiAmountLbl.Text = "Amount:";
            // 
            // DlgRewards
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(413, 150);
            this.Controls.Add(this.guiCodeLbl);
            this.Controls.Add(this.guiCodeDescriptionTxt);
            this.Controls.Add(this.guiCodeSearchBtn);
            this.Controls.Add(this.guiCodeTxt);
            this.Controls.Add(this.guiAmountLbl);
            this.Controls.Add(this.guiAmountTxt);
            this.Controls.Add(this.guiCancelBtn);
            this.Controls.Add(this.guiOkBtn);
            this.Controls.Add(this.guiTypeCbo);
            this.Controls.Add(this.guiTypeLbl);
            this.Name = "DlgRewards";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Rewards";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label guiTypeLbl;
        private System.Windows.Forms.ComboBox guiTypeCbo;
        private System.Windows.Forms.Button guiOkBtn;
        private System.Windows.Forms.Button guiCancelBtn;
        private System.Windows.Forms.TextBox guiAmountTxt;
        private System.Windows.Forms.Label guiCodeLbl;
        private System.Windows.Forms.TextBox guiCodeDescriptionTxt;
        private System.Windows.Forms.Button guiCodeSearchBtn;
        private System.Windows.Forms.TextBox guiCodeTxt;
        private System.Windows.Forms.Label guiAmountLbl;
    }
}