namespace ToolsLauncher
{
    partial class IRCMessenger
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
            System.Windows.Forms.TreeNode treeNode1 = new System.Windows.Forms.TreeNode("Online");
            this.UserList = new System.Windows.Forms.TreeView();
            this.SuspendLayout();
            // 
            // UserList
            // 
            this.UserList.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.UserList.Location = new System.Drawing.Point(-1, 0);
            this.UserList.Name = "UserList";
            treeNode1.Name = "Online";
            treeNode1.Text = "Online";
            this.UserList.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
            treeNode1});
            this.UserList.ShowLines = false;
            this.UserList.Size = new System.Drawing.Size(203, 291);
            this.UserList.TabIndex = 0;
            this.UserList.NodeMouseDoubleClick += new System.Windows.Forms.TreeNodeMouseClickEventHandler(this.UserList_NodeMouseDoubleClick);
            // 
            // IRCMessenger
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(202, 345);
            this.Controls.Add(this.UserList);
            this.Name = "IRCMessenger";
            this.Text = "IRCMessenger";
            this.Load += new System.EventHandler(this.IRCMessenger_Load);
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.IRCMessenger_FormClosed);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TreeView UserList;
    }
}