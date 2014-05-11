namespace MissionEditor.Gui
{
    partial class DlgCompletions
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
            this.guiValueTxt = new System.Windows.Forms.TextBox();
            this.guiAmountLbl = new System.Windows.Forms.Label();
            this.guiValueLbl = new System.Windows.Forms.Label();
            this.guiValueDescriptionTxt = new System.Windows.Forms.TextBox();
            this.guiValueSearchBtn = new System.Windows.Forms.Button();
            this.guiAmountTxt = new System.Windows.Forms.TextBox();
            this.guiDataLbl = new System.Windows.Forms.Label();
            this.guiDataDescriptionTxt = new System.Windows.Forms.TextBox();
            this.guiDataSearchBtn = new System.Windows.Forms.Button();
            this.guiDataTxt = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // guiTypeLbl
            // 
            this.guiTypeLbl.AutoSize = true;
            this.guiTypeLbl.Location = new System.Drawing.Point(23, 13);
            this.guiTypeLbl.Name = "guiTypeLbl";
            this.guiTypeLbl.Size = new System.Drawing.Size(34, 13);
            this.guiTypeLbl.TabIndex = 0;
            this.guiTypeLbl.Text = "Type:";
            // 
            // guiTypeCbo
            // 
            this.guiTypeCbo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.guiTypeCbo.FormattingEnabled = true;
            this.guiTypeCbo.Location = new System.Drawing.Point(63, 10);
            this.guiTypeCbo.Name = "guiTypeCbo";
            this.guiTypeCbo.Size = new System.Drawing.Size(338, 21);
            this.guiTypeCbo.TabIndex = 1;
            this.guiTypeCbo.SelectedIndexChanged += new System.EventHandler(this.onTypeSelected);
            // 
            // guiOkBtn
            // 
            this.guiOkBtn.Location = new System.Drawing.Point(245, 115);
            this.guiOkBtn.Name = "guiOkBtn";
            this.guiOkBtn.Size = new System.Drawing.Size(75, 23);
            this.guiOkBtn.TabIndex = 12;
            this.guiOkBtn.Text = "Ok";
            this.guiOkBtn.UseVisualStyleBackColor = true;
            this.guiOkBtn.Click += new System.EventHandler(this.onOk);
            // 
            // guiCancelBtn
            // 
            this.guiCancelBtn.Location = new System.Drawing.Point(326, 115);
            this.guiCancelBtn.Name = "guiCancelBtn";
            this.guiCancelBtn.Size = new System.Drawing.Size(75, 23);
            this.guiCancelBtn.TabIndex = 13;
            this.guiCancelBtn.Text = "Cancel";
            this.guiCancelBtn.UseVisualStyleBackColor = true;
            this.guiCancelBtn.Click += new System.EventHandler(this.onCancel);
            // 
            // guiValueTxt
            // 
            this.guiValueTxt.Location = new System.Drawing.Point(63, 40);
            this.guiValueTxt.Name = "guiValueTxt";
            this.guiValueTxt.Size = new System.Drawing.Size(43, 20);
            this.guiValueTxt.TabIndex = 3;
            // 
            // guiAmountLbl
            // 
            this.guiAmountLbl.AutoSize = true;
            this.guiAmountLbl.Location = new System.Drawing.Point(11, 96);
            this.guiAmountLbl.Name = "guiAmountLbl";
            this.guiAmountLbl.Size = new System.Drawing.Size(46, 13);
            this.guiAmountLbl.TabIndex = 10;
            this.guiAmountLbl.Text = "Amount:";
            // 
            // guiValueLbl
            // 
            this.guiValueLbl.AutoSize = true;
            this.guiValueLbl.Location = new System.Drawing.Point(23, 43);
            this.guiValueLbl.Name = "guiValueLbl";
            this.guiValueLbl.Size = new System.Drawing.Size(37, 13);
            this.guiValueLbl.TabIndex = 2;
            this.guiValueLbl.Text = "Value:";
            // 
            // guiValueDescriptionTxt
            // 
            this.guiValueDescriptionTxt.Location = new System.Drawing.Point(111, 40);
            this.guiValueDescriptionTxt.Name = "guiValueDescriptionTxt";
            this.guiValueDescriptionTxt.ReadOnly = true;
            this.guiValueDescriptionTxt.Size = new System.Drawing.Size(231, 20);
            this.guiValueDescriptionTxt.TabIndex = 4;
            this.guiValueDescriptionTxt.TabStop = false;
            // 
            // guiValueSearchBtn
            // 
            this.guiValueSearchBtn.Location = new System.Drawing.Point(348, 35);
            this.guiValueSearchBtn.Name = "guiValueSearchBtn";
            this.guiValueSearchBtn.Size = new System.Drawing.Size(52, 23);
            this.guiValueSearchBtn.TabIndex = 5;
            this.guiValueSearchBtn.Text = "Search";
            this.guiValueSearchBtn.UseVisualStyleBackColor = true;
            // 
            // guiAmountTxt
            // 
            this.guiAmountTxt.Location = new System.Drawing.Point(63, 93);
            this.guiAmountTxt.Name = "guiAmountTxt";
            this.guiAmountTxt.Size = new System.Drawing.Size(88, 20);
            this.guiAmountTxt.TabIndex = 11;
            // 
            // guiDataLbl
            // 
            this.guiDataLbl.AutoSize = true;
            this.guiDataLbl.Location = new System.Drawing.Point(23, 69);
            this.guiDataLbl.Name = "guiDataLbl";
            this.guiDataLbl.Size = new System.Drawing.Size(33, 13);
            this.guiDataLbl.TabIndex = 6;
            this.guiDataLbl.Text = "Data:";
            // 
            // guiDataDescriptionTxt
            // 
            this.guiDataDescriptionTxt.Location = new System.Drawing.Point(112, 66);
            this.guiDataDescriptionTxt.Name = "guiDataDescriptionTxt";
            this.guiDataDescriptionTxt.ReadOnly = true;
            this.guiDataDescriptionTxt.Size = new System.Drawing.Size(231, 20);
            this.guiDataDescriptionTxt.TabIndex = 8;
            this.guiDataDescriptionTxt.TabStop = false;
            // 
            // guiDataSearchBtn
            // 
            this.guiDataSearchBtn.Location = new System.Drawing.Point(348, 64);
            this.guiDataSearchBtn.Name = "guiDataSearchBtn";
            this.guiDataSearchBtn.Size = new System.Drawing.Size(52, 23);
            this.guiDataSearchBtn.TabIndex = 9;
            this.guiDataSearchBtn.Text = "Search";
            this.guiDataSearchBtn.UseVisualStyleBackColor = true;
            // 
            // guiDataTxt
            // 
            this.guiDataTxt.Location = new System.Drawing.Point(63, 66);
            this.guiDataTxt.Name = "guiDataTxt";
            this.guiDataTxt.Size = new System.Drawing.Size(43, 20);
            this.guiDataTxt.TabIndex = 7;
            // 
            // DlgCompletions
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(413, 150);
            this.Controls.Add(this.guiDataLbl);
            this.Controls.Add(this.guiDataDescriptionTxt);
            this.Controls.Add(this.guiDataSearchBtn);
            this.Controls.Add(this.guiDataTxt);
            this.Controls.Add(this.guiAmountTxt);
            this.Controls.Add(this.guiAmountLbl);
            this.Controls.Add(this.guiValueLbl);
            this.Controls.Add(this.guiValueDescriptionTxt);
            this.Controls.Add(this.guiValueSearchBtn);
            this.Controls.Add(this.guiValueTxt);
            this.Controls.Add(this.guiCancelBtn);
            this.Controls.Add(this.guiOkBtn);
            this.Controls.Add(this.guiTypeCbo);
            this.Controls.Add(this.guiTypeLbl);
            this.Name = "DlgCompletions";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Completions";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label guiTypeLbl;
        private System.Windows.Forms.ComboBox guiTypeCbo;
        private System.Windows.Forms.Button guiOkBtn;
        private System.Windows.Forms.Button guiCancelBtn;
        private System.Windows.Forms.TextBox guiValueTxt;
        private System.Windows.Forms.Label guiAmountLbl;
        private System.Windows.Forms.Label guiValueLbl;
        private System.Windows.Forms.TextBox guiValueDescriptionTxt;
        private System.Windows.Forms.Button guiValueSearchBtn;
        private System.Windows.Forms.TextBox guiAmountTxt;
        private System.Windows.Forms.Label guiDataLbl;
        private System.Windows.Forms.TextBox guiDataDescriptionTxt;
        private System.Windows.Forms.Button guiDataSearchBtn;
        private System.Windows.Forms.TextBox guiDataTxt;
    }
}