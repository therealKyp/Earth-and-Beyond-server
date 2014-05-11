namespace TalkTreeEditor.Reply
{
    partial class Trade
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
            this.guiQuantityTxt = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // guiQuantityTxt
            // 
            this.guiQuantityTxt.Location = new System.Drawing.Point(3, 2);
            this.guiQuantityTxt.Name = "guiQuantityTxt";
            this.guiQuantityTxt.Size = new System.Drawing.Size(35, 20);
            this.guiQuantityTxt.TabIndex = 13;
            this.guiQuantityTxt.TextChanged += new System.EventHandler(this.onTextChanged);
            // 
            // Trade
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.guiQuantityTxt);
            this.Name = "Trade";
            this.Size = new System.Drawing.Size(330, 25);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox guiQuantityTxt;
    }
}
