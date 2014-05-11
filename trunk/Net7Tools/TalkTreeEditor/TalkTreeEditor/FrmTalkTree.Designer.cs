namespace TalkTreeEditor
{
    partial class FrmTalkTree
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FrmTalkTree));
            this.guiNpcText = new System.Windows.Forms.TextBox();
            this.guiReplyLbl = new System.Windows.Forms.Label();
            this.guiOkBtn = new System.Windows.Forms.Button();
            this.guiCancelBtn = new System.Windows.Forms.Button();
            this.guiTalkTree = new System.Windows.Forms.TreeView();
            this.guiReply0Cbo = new System.Windows.Forms.ComboBox();
            this.guiReply1Cbo = new System.Windows.Forms.ComboBox();
            this.guiReply2Cbo = new System.Windows.Forms.ComboBox();
            this.guiReply3Cbo = new System.Windows.Forms.ComboBox();
            this.guiAddNode = new System.Windows.Forms.Button();
            this.guiRemoveNode = new System.Windows.Forms.Button();
            this.guiEditXml = new System.Windows.Forms.Button();
            this.guiReplyPreviousBtn = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // guiNpcText
            // 
            this.guiNpcText.Location = new System.Drawing.Point(516, 12);
            this.guiNpcText.Multiline = true;
            this.guiNpcText.Name = "guiNpcText";
            this.guiNpcText.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.guiNpcText.Size = new System.Drawing.Size(393, 153);
            this.guiNpcText.TabIndex = 1;
            this.guiNpcText.TextChanged += new System.EventHandler(this.onNpcTextChanged);
            // 
            // guiReplyLbl
            // 
            this.guiReplyLbl.AutoSize = true;
            this.guiReplyLbl.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.guiReplyLbl.ForeColor = System.Drawing.Color.DarkRed;
            this.guiReplyLbl.Location = new System.Drawing.Point(513, 191);
            this.guiReplyLbl.Name = "guiReplyLbl";
            this.guiReplyLbl.Size = new System.Drawing.Size(73, 13);
            this.guiReplyLbl.TabIndex = 2;
            this.guiReplyLbl.Text = "Your Reply:";
            // 
            // guiOkBtn
            // 
            this.guiOkBtn.Location = new System.Drawing.Point(754, 349);
            this.guiOkBtn.Name = "guiOkBtn";
            this.guiOkBtn.Size = new System.Drawing.Size(75, 23);
            this.guiOkBtn.TabIndex = 23;
            this.guiOkBtn.Text = "Ok";
            this.guiOkBtn.UseVisualStyleBackColor = true;
            this.guiOkBtn.Click += new System.EventHandler(this.onOk);
            // 
            // guiCancelBtn
            // 
            this.guiCancelBtn.Location = new System.Drawing.Point(835, 349);
            this.guiCancelBtn.Name = "guiCancelBtn";
            this.guiCancelBtn.Size = new System.Drawing.Size(75, 23);
            this.guiCancelBtn.TabIndex = 24;
            this.guiCancelBtn.Text = "Cancel";
            this.guiCancelBtn.UseVisualStyleBackColor = true;
            this.guiCancelBtn.Click += new System.EventHandler(this.onCancel);
            // 
            // guiTalkTree
            // 
            this.guiTalkTree.FullRowSelect = true;
            this.guiTalkTree.HideSelection = false;
            this.guiTalkTree.Location = new System.Drawing.Point(12, 12);
            this.guiTalkTree.Name = "guiTalkTree";
            this.guiTalkTree.Size = new System.Drawing.Size(486, 334);
            this.guiTalkTree.TabIndex = 0;
            this.guiTalkTree.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.onTreeSelection);
            // 
            // guiReply0Cbo
            // 
            this.guiReply0Cbo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.guiReply0Cbo.FormattingEnabled = true;
            this.guiReply0Cbo.Location = new System.Drawing.Point(516, 207);
            this.guiReply0Cbo.Name = "guiReply0Cbo";
            this.guiReply0Cbo.Size = new System.Drawing.Size(59, 21);
            this.guiReply0Cbo.TabIndex = 3;
            // 
            // guiReply1Cbo
            // 
            this.guiReply1Cbo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.guiReply1Cbo.FormattingEnabled = true;
            this.guiReply1Cbo.Location = new System.Drawing.Point(516, 234);
            this.guiReply1Cbo.Name = "guiReply1Cbo";
            this.guiReply1Cbo.Size = new System.Drawing.Size(59, 21);
            this.guiReply1Cbo.TabIndex = 7;
            // 
            // guiReply2Cbo
            // 
            this.guiReply2Cbo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.guiReply2Cbo.FormattingEnabled = true;
            this.guiReply2Cbo.Location = new System.Drawing.Point(516, 261);
            this.guiReply2Cbo.Name = "guiReply2Cbo";
            this.guiReply2Cbo.Size = new System.Drawing.Size(59, 21);
            this.guiReply2Cbo.TabIndex = 11;
            // 
            // guiReply3Cbo
            // 
            this.guiReply3Cbo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.guiReply3Cbo.FormattingEnabled = true;
            this.guiReply3Cbo.Location = new System.Drawing.Point(516, 287);
            this.guiReply3Cbo.Name = "guiReply3Cbo";
            this.guiReply3Cbo.Size = new System.Drawing.Size(59, 21);
            this.guiReply3Cbo.TabIndex = 15;
            // 
            // guiAddNode
            // 
            this.guiAddNode.Location = new System.Drawing.Point(12, 352);
            this.guiAddNode.Name = "guiAddNode";
            this.guiAddNode.Size = new System.Drawing.Size(75, 23);
            this.guiAddNode.TabIndex = 20;
            this.guiAddNode.Text = "Add";
            this.guiAddNode.UseVisualStyleBackColor = true;
            this.guiAddNode.Click += new System.EventHandler(this.onNodeAdd);
            // 
            // guiRemoveNode
            // 
            this.guiRemoveNode.Location = new System.Drawing.Point(93, 352);
            this.guiRemoveNode.Name = "guiRemoveNode";
            this.guiRemoveNode.Size = new System.Drawing.Size(75, 23);
            this.guiRemoveNode.TabIndex = 21;
            this.guiRemoveNode.Text = "Remove";
            this.guiRemoveNode.UseVisualStyleBackColor = true;
            this.guiRemoveNode.Click += new System.EventHandler(this.onNodeRemove);
            // 
            // guiEditXml
            // 
            this.guiEditXml.Location = new System.Drawing.Point(516, 349);
            this.guiEditXml.Name = "guiEditXml";
            this.guiEditXml.Size = new System.Drawing.Size(59, 23);
            this.guiEditXml.TabIndex = 0;
            this.guiEditXml.Text = "XML";
            this.guiEditXml.UseVisualStyleBackColor = true;
            this.guiEditXml.Click += new System.EventHandler(this.onEditXml);
            // 
            // guiReplyPreviousBtn
            // 
            this.guiReplyPreviousBtn.Enabled = false;
            this.guiReplyPreviousBtn.Location = new System.Drawing.Point(869, 315);
            this.guiReplyPreviousBtn.Name = "guiReplyPreviousBtn";
            this.guiReplyPreviousBtn.Size = new System.Drawing.Size(41, 23);
            this.guiReplyPreviousBtn.TabIndex = 19;
            this.guiReplyPreviousBtn.Text = "Back";
            this.guiReplyPreviousBtn.UseVisualStyleBackColor = true;
            this.guiReplyPreviousBtn.Click += new System.EventHandler(this.onNodePrevious);
            // 
            // FrmTalkTree
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(921, 387);
            this.Controls.Add(this.guiEditXml);
            this.Controls.Add(this.guiReplyPreviousBtn);
            this.Controls.Add(this.guiRemoveNode);
            this.Controls.Add(this.guiAddNode);
            this.Controls.Add(this.guiReply3Cbo);
            this.Controls.Add(this.guiReply2Cbo);
            this.Controls.Add(this.guiReply1Cbo);
            this.Controls.Add(this.guiReply0Cbo);
            this.Controls.Add(this.guiTalkTree);
            this.Controls.Add(this.guiCancelBtn);
            this.Controls.Add(this.guiOkBtn);
            this.Controls.Add(this.guiReplyLbl);
            this.Controls.Add(this.guiNpcText);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "FrmTalkTree";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Talk Tree";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox guiNpcText;
        private System.Windows.Forms.Label guiReplyLbl;
        private System.Windows.Forms.Button guiOkBtn;
        private System.Windows.Forms.Button guiCancelBtn;
        private System.Windows.Forms.TreeView guiTalkTree;
        private System.Windows.Forms.ComboBox guiReply0Cbo;
        private System.Windows.Forms.ComboBox guiReply1Cbo;
        private System.Windows.Forms.ComboBox guiReply2Cbo;
        private System.Windows.Forms.ComboBox guiReply3Cbo;
        private System.Windows.Forms.Button guiAddNode;
        private System.Windows.Forms.Button guiRemoveNode;
        private System.Windows.Forms.Button guiEditXml;
        private System.Windows.Forms.Button guiReplyPreviousBtn;
    }
}

