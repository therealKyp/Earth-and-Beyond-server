namespace TalkTreeEditor.Reply
{
    partial class Flag
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.guiValueTxt = new System.Windows.Forms.TextBox();
            this.guiTypeCbo = new System.Windows.Forms.ComboBox();
            this.guiValueCbo = new System.Windows.Forms.ComboBox();
            this.SuspendLayout();
            // 
            // guiValueTxt
            // 
            this.guiValueTxt.Location = new System.Drawing.Point(130, 0);
            this.guiValueTxt.Name = "guiValueTxt";
            this.guiValueTxt.Size = new System.Drawing.Size(203, 20);
            this.guiValueTxt.TabIndex = 2;
            this.guiValueTxt.TextChanged += new System.EventHandler(this.onValueSelected);
            // 
            // guiTypeCbo
            // 
            this.guiTypeCbo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.guiTypeCbo.FormattingEnabled = true;
            this.guiTypeCbo.Location = new System.Drawing.Point(2, 0);
            this.guiTypeCbo.Name = "guiTypeCbo";
            this.guiTypeCbo.Size = new System.Drawing.Size(122, 21);
            this.guiTypeCbo.TabIndex = 0;
            this.guiTypeCbo.SelectedIndexChanged += new System.EventHandler(this.onTypeSelected);
            // 
            // guiValueCbo
            // 
            this.guiValueCbo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.guiValueCbo.FormattingEnabled = true;
            this.guiValueCbo.Location = new System.Drawing.Point(130, 0);
            this.guiValueCbo.Name = "guiValueCbo";
            this.guiValueCbo.Size = new System.Drawing.Size(203, 21);
            this.guiValueCbo.TabIndex = 1;
            this.guiValueCbo.SelectedIndexChanged += new System.EventHandler(this.onValueSelected);
            // 
            // Flag
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Control;
            this.Controls.Add(this.guiValueCbo);
            this.Controls.Add(this.guiTypeCbo);
            this.Controls.Add(this.guiValueTxt);
            this.Name = "Flag";
            this.Size = new System.Drawing.Size(333, 21);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox guiValueTxt;
        private System.Windows.Forms.ComboBox guiTypeCbo;
        private System.Windows.Forms.ComboBox guiValueCbo;
    }
}
