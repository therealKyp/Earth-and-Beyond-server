namespace TalkTreeEditor
{
    partial class EditTalkTree
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
            this.TalkTreeData = new System.Windows.Forms.TextBox();
            this.SaveTalkTree = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // TalkTreeData
            // 
            this.TalkTreeData.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.TalkTreeData.Location = new System.Drawing.Point(3, 15);
            this.TalkTreeData.Multiline = true;
            this.TalkTreeData.Name = "TalkTreeData";
            this.TalkTreeData.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.TalkTreeData.Size = new System.Drawing.Size(821, 361);
            this.TalkTreeData.TabIndex = 0;
            this.TalkTreeData.WordWrap = false;
            // 
            // SaveTalkTree
            // 
            this.SaveTalkTree.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.SaveTalkTree.Location = new System.Drawing.Point(12, 382);
            this.SaveTalkTree.Name = "SaveTalkTree";
            this.SaveTalkTree.Size = new System.Drawing.Size(104, 32);
            this.SaveTalkTree.TabIndex = 1;
            this.SaveTalkTree.Text = "Save";
            this.SaveTalkTree.UseVisualStyleBackColor = true;
            this.SaveTalkTree.Click += new System.EventHandler(this.SaveTalkTree_Click);
            // 
            // EditTalkTree
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(827, 426);
            this.Controls.Add(this.SaveTalkTree);
            this.Controls.Add(this.TalkTreeData);
            this.Name = "EditTalkTree";
            this.Text = "EditTalkTree";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox TalkTreeData;
        private System.Windows.Forms.Button SaveTalkTree;
    }
}