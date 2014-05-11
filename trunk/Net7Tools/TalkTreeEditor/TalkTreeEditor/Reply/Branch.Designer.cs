namespace TalkTreeEditor.Reply
{
    partial class Branch
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
            this.guiGotoTxt = new System.Windows.Forms.TextBox();
            this.reply0Btn = new System.Windows.Forms.Button();
            this.guiReplyTxt = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // guiGotoTxt
            // 
            this.guiGotoTxt.Location = new System.Drawing.Point(0, 0);
            this.guiGotoTxt.Name = "guiGotoTxt";
            this.guiGotoTxt.Size = new System.Drawing.Size(34, 20);
            this.guiGotoTxt.TabIndex = 0;
            this.guiGotoTxt.TextChanged += new System.EventHandler(this.onTextChanged);
            // 
            // reply0Btn
            // 
            this.reply0Btn.Location = new System.Drawing.Point(291, 0);
            this.reply0Btn.Name = "reply0Btn";
            this.reply0Btn.Size = new System.Drawing.Size(41, 21);
            this.reply0Btn.TabIndex = 2;
            this.reply0Btn.Text = "Goto";
            this.reply0Btn.UseVisualStyleBackColor = true;
            this.reply0Btn.Click += new System.EventHandler(this.onGoto);
            // 
            // guiReplyTxt
            // 
            this.guiReplyTxt.Location = new System.Drawing.Point(36, 0);
            this.guiReplyTxt.Name = "guiReplyTxt";
            this.guiReplyTxt.Size = new System.Drawing.Size(253, 20);
            this.guiReplyTxt.TabIndex = 1;
            this.guiReplyTxt.TextChanged += new System.EventHandler(this.onTextChanged);
            // 
            // Branch
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Control;
            this.Controls.Add(this.guiGotoTxt);
            this.Controls.Add(this.reply0Btn);
            this.Controls.Add(this.guiReplyTxt);
            this.Name = "Branch";
            this.Size = new System.Drawing.Size(333, 21);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox guiGotoTxt;
        private System.Windows.Forms.Button reply0Btn;
        private System.Windows.Forms.TextBox guiReplyTxt;
    }
}
