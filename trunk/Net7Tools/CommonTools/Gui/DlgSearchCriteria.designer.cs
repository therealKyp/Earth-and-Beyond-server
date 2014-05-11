namespace CommonTools.Gui
{
    partial class DlgSearchCriteria
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
            this.guiFieldCbo = new System.Windows.Forms.ComboBox();
            this.guiFieldLbl = new System.Windows.Forms.Label();
            this.guiPatternTipBtn = new System.Windows.Forms.Button();
            this.guiPatternTxt = new System.Windows.Forms.TextBox();
            this.guiPatternLbl = new System.Windows.Forms.Label();
            this.guiComparisonCbo = new System.Windows.Forms.ComboBox();
            this.guiComparisonTxt = new System.Windows.Forms.TextBox();
            this.guiComparisonLbl = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // guiOkBtn
            // 
            this.guiOkBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.guiOkBtn.Location = new System.Drawing.Point(302, 94);
            this.guiOkBtn.Name = "guiOkBtn";
            this.guiOkBtn.Size = new System.Drawing.Size(75, 23);
            this.guiOkBtn.TabIndex = 9;
            this.guiOkBtn.Text = "Ok";
            this.guiOkBtn.UseVisualStyleBackColor = true;
            this.guiOkBtn.Click += new System.EventHandler(this.onOk);
            this.guiOkBtn.KeyDown += new System.Windows.Forms.KeyEventHandler(this.onKeyDown);
            // 
            // guiCancelBtn
            // 
            this.guiCancelBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.guiCancelBtn.Location = new System.Drawing.Point(382, 94);
            this.guiCancelBtn.Name = "guiCancelBtn";
            this.guiCancelBtn.Size = new System.Drawing.Size(75, 23);
            this.guiCancelBtn.TabIndex = 10;
            this.guiCancelBtn.Text = "Cancel";
            this.guiCancelBtn.UseVisualStyleBackColor = true;
            this.guiCancelBtn.Click += new System.EventHandler(this.onCancel);
            this.guiCancelBtn.KeyDown += new System.Windows.Forms.KeyEventHandler(this.onKeyDown);
            // 
            // guiFieldCbo
            // 
            this.guiFieldCbo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.guiFieldCbo.FormattingEnabled = true;
            this.guiFieldCbo.Location = new System.Drawing.Point(79, 12);
            this.guiFieldCbo.MaxDropDownItems = 20;
            this.guiFieldCbo.Name = "guiFieldCbo";
            this.guiFieldCbo.Size = new System.Drawing.Size(378, 21);
            this.guiFieldCbo.TabIndex = 1;
            this.guiFieldCbo.SelectionChangeCommitted += new System.EventHandler(this.onFieldSelected);
            this.guiFieldCbo.KeyDown += new System.Windows.Forms.KeyEventHandler(this.onKeyDown);
            // 
            // guiFieldLbl
            // 
            this.guiFieldLbl.AutoSize = true;
            this.guiFieldLbl.Location = new System.Drawing.Point(41, 15);
            this.guiFieldLbl.Name = "guiFieldLbl";
            this.guiFieldLbl.Size = new System.Drawing.Size(32, 13);
            this.guiFieldLbl.TabIndex = 0;
            this.guiFieldLbl.Text = "Field:";
            this.guiFieldLbl.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // guiPatternTipBtn
            // 
            this.guiPatternTipBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.guiPatternTipBtn.BackColor = System.Drawing.Color.LightSteelBlue;
            this.guiPatternTipBtn.Location = new System.Drawing.Point(438, 34);
            this.guiPatternTipBtn.Name = "guiPatternTipBtn";
            this.guiPatternTipBtn.Size = new System.Drawing.Size(19, 23);
            this.guiPatternTipBtn.TabIndex = 4;
            this.guiPatternTipBtn.Text = "?";
            this.guiPatternTipBtn.UseVisualStyleBackColor = false;
            this.guiPatternTipBtn.Click += new System.EventHandler(this.onPatternTip_Click);
            this.guiPatternTipBtn.KeyDown += new System.Windows.Forms.KeyEventHandler(this.onKeyDown);
            // 
            // guiPatternTxt
            // 
            this.guiPatternTxt.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.guiPatternTxt.Location = new System.Drawing.Point(79, 36);
            this.guiPatternTxt.Name = "guiPatternTxt";
            this.guiPatternTxt.Size = new System.Drawing.Size(353, 20);
            this.guiPatternTxt.TabIndex = 3;
            this.guiPatternTxt.KeyDown += new System.Windows.Forms.KeyEventHandler(this.onKeyDown);
            // 
            // guiPatternLbl
            // 
            this.guiPatternLbl.AutoSize = true;
            this.guiPatternLbl.Location = new System.Drawing.Point(29, 39);
            this.guiPatternLbl.Name = "guiPatternLbl";
            this.guiPatternLbl.Size = new System.Drawing.Size(44, 13);
            this.guiPatternLbl.TabIndex = 2;
            this.guiPatternLbl.Text = "Pattern:";
            this.guiPatternLbl.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // guiComparisonCbo
            // 
            this.guiComparisonCbo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.guiComparisonCbo.Font = new System.Drawing.Font("Courier New", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.guiComparisonCbo.FormattingEnabled = true;
            this.guiComparisonCbo.Location = new System.Drawing.Point(79, 62);
            this.guiComparisonCbo.Name = "guiComparisonCbo";
            this.guiComparisonCbo.Size = new System.Drawing.Size(297, 22);
            this.guiComparisonCbo.TabIndex = 6;
            this.guiComparisonCbo.KeyDown += new System.Windows.Forms.KeyEventHandler(this.onKeyDown);
            // 
            // guiComparisonTxt
            // 
            this.guiComparisonTxt.Location = new System.Drawing.Point(382, 62);
            this.guiComparisonTxt.Name = "guiComparisonTxt";
            this.guiComparisonTxt.Size = new System.Drawing.Size(75, 20);
            this.guiComparisonTxt.TabIndex = 7;
            this.guiComparisonTxt.KeyDown += new System.Windows.Forms.KeyEventHandler(this.onKeyDown);
            // 
            // guiComparisonLbl
            // 
            this.guiComparisonLbl.AutoSize = true;
            this.guiComparisonLbl.Location = new System.Drawing.Point(8, 65);
            this.guiComparisonLbl.Name = "guiComparisonLbl";
            this.guiComparisonLbl.Size = new System.Drawing.Size(65, 13);
            this.guiComparisonLbl.TabIndex = 5;
            this.guiComparisonLbl.Text = "Comparison:";
            this.guiComparisonLbl.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // FrmSearchCriteria
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(469, 129);
            this.Controls.Add(this.guiComparisonLbl);
            this.Controls.Add(this.guiComparisonTxt);
            this.Controls.Add(this.guiComparisonCbo);
            this.Controls.Add(this.guiPatternTipBtn);
            this.Controls.Add(this.guiPatternTxt);
            this.Controls.Add(this.guiPatternLbl);
            this.Controls.Add(this.guiFieldLbl);
            this.Controls.Add(this.guiFieldCbo);
            this.Controls.Add(this.guiCancelBtn);
            this.Controls.Add(this.guiOkBtn);
            this.Name = "FrmSearchCriteria";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Search Criteria";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button guiOkBtn;
        private System.Windows.Forms.Button guiCancelBtn;
        private System.Windows.Forms.ComboBox guiFieldCbo;
        private System.Windows.Forms.Label guiFieldLbl;
        private System.Windows.Forms.Button guiPatternTipBtn;
        private System.Windows.Forms.TextBox guiPatternTxt;
        private System.Windows.Forms.Label guiPatternLbl;
        private System.Windows.Forms.ComboBox guiComparisonCbo;
        private System.Windows.Forms.TextBox guiComparisonTxt;
        private System.Windows.Forms.Label guiComparisonLbl;
    }
}