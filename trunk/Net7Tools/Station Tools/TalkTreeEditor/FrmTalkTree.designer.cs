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
            this.npcText = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.reply1Txt = new System.Windows.Forms.TextBox();
            this.reply1Btn = new System.Windows.Forms.Button();
            this.reply2Btn = new System.Windows.Forms.Button();
            this.reply2Txt = new System.Windows.Forms.TextBox();
            this.reply3Btn = new System.Windows.Forms.Button();
            this.reply3Txt = new System.Windows.Forms.TextBox();
            this.okBtn = new System.Windows.Forms.Button();
            this.cancelBtn = new System.Windows.Forms.Button();
            this.reply0Btn = new System.Windows.Forms.Button();
            this.reply0Txt = new System.Windows.Forms.TextBox();
            this.talkTree = new System.Windows.Forms.TreeView();
            this.reply0Cbo = new System.Windows.Forms.ComboBox();
            this.reply1Cbo = new System.Windows.Forms.ComboBox();
            this.reply2Cbo = new System.Windows.Forms.ComboBox();
            this.reply3Cbo = new System.Windows.Forms.ComboBox();
            this.replyId0Txt = new System.Windows.Forms.TextBox();
            this.replyId1Txt = new System.Windows.Forms.TextBox();
            this.replyId2Txt = new System.Windows.Forms.TextBox();
            this.replyId3Txt = new System.Windows.Forms.TextBox();
            this.addNode = new System.Windows.Forms.Button();
            this.removeNode = new System.Windows.Forms.Button();
            this.viewXml = new System.Windows.Forms.Button();
            this.pasteXml = new System.Windows.Forms.Button();
            this.replyPrevious = new System.Windows.Forms.Button();
            this.xmlGroup = new System.Windows.Forms.GroupBox();
            this.copyXml = new System.Windows.Forms.Button();
            this.xmlGroup.SuspendLayout();
            this.SuspendLayout();
            // 
            // npcText
            // 
            this.npcText.Location = new System.Drawing.Point(516, 12);
            this.npcText.Multiline = true;
            this.npcText.Name = "npcText";
            this.npcText.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.npcText.Size = new System.Drawing.Size(393, 153);
            this.npcText.TabIndex = 1;
            this.npcText.TextChanged += new System.EventHandler(this.onNpcTextChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.ForeColor = System.Drawing.Color.DarkRed;
            this.label2.Location = new System.Drawing.Point(513, 191);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(73, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "Your Reply:";
            // 
            // reply1Txt
            // 
            this.reply1Txt.Location = new System.Drawing.Point(613, 234);
            this.reply1Txt.Name = "reply1Txt";
            this.reply1Txt.Size = new System.Drawing.Size(253, 20);
            this.reply1Txt.TabIndex = 9;
            this.reply1Txt.TextChanged += new System.EventHandler(this.onTextChanged);
            // 
            // reply1Btn
            // 
            this.reply1Btn.Location = new System.Drawing.Point(868, 233);
            this.reply1Btn.Name = "reply1Btn";
            this.reply1Btn.Size = new System.Drawing.Size(41, 23);
            this.reply1Btn.TabIndex = 10;
            this.reply1Btn.Text = "Goto";
            this.reply1Btn.UseVisualStyleBackColor = true;
            this.reply1Btn.Click += new System.EventHandler(this.onGotoNode);
            // 
            // reply2Btn
            // 
            this.reply2Btn.Location = new System.Drawing.Point(868, 260);
            this.reply2Btn.Name = "reply2Btn";
            this.reply2Btn.Size = new System.Drawing.Size(41, 23);
            this.reply2Btn.TabIndex = 14;
            this.reply2Btn.Text = "Goto";
            this.reply2Btn.UseVisualStyleBackColor = true;
            this.reply2Btn.Click += new System.EventHandler(this.onGotoNode);
            // 
            // reply2Txt
            // 
            this.reply2Txt.Location = new System.Drawing.Point(613, 261);
            this.reply2Txt.Name = "reply2Txt";
            this.reply2Txt.Size = new System.Drawing.Size(253, 20);
            this.reply2Txt.TabIndex = 13;
            this.reply2Txt.TextChanged += new System.EventHandler(this.onTextChanged);
            // 
            // reply3Btn
            // 
            this.reply3Btn.Location = new System.Drawing.Point(868, 286);
            this.reply3Btn.Name = "reply3Btn";
            this.reply3Btn.Size = new System.Drawing.Size(41, 23);
            this.reply3Btn.TabIndex = 18;
            this.reply3Btn.Text = "Goto";
            this.reply3Btn.UseVisualStyleBackColor = true;
            this.reply3Btn.Click += new System.EventHandler(this.onGotoNode);
            // 
            // reply3Txt
            // 
            this.reply3Txt.Location = new System.Drawing.Point(613, 287);
            this.reply3Txt.Name = "reply3Txt";
            this.reply3Txt.Size = new System.Drawing.Size(253, 20);
            this.reply3Txt.TabIndex = 17;
            this.reply3Txt.TextChanged += new System.EventHandler(this.onTextChanged);
            // 
            // okBtn
            // 
            this.okBtn.Location = new System.Drawing.Point(754, 349);
            this.okBtn.Name = "okBtn";
            this.okBtn.Size = new System.Drawing.Size(75, 23);
            this.okBtn.TabIndex = 23;
            this.okBtn.Text = "Ok";
            this.okBtn.UseVisualStyleBackColor = true;
            this.okBtn.Click += new System.EventHandler(this.okBtn_Click);
            // 
            // cancelBtn
            // 
            this.cancelBtn.Location = new System.Drawing.Point(835, 349);
            this.cancelBtn.Name = "cancelBtn";
            this.cancelBtn.Size = new System.Drawing.Size(75, 23);
            this.cancelBtn.TabIndex = 24;
            this.cancelBtn.Text = "Cancel";
            this.cancelBtn.UseVisualStyleBackColor = true;
            this.cancelBtn.Click += new System.EventHandler(this.cancelBtn_Click);
            // 
            // reply0Btn
            // 
            this.reply0Btn.Location = new System.Drawing.Point(868, 207);
            this.reply0Btn.Name = "reply0Btn";
            this.reply0Btn.Size = new System.Drawing.Size(41, 21);
            this.reply0Btn.TabIndex = 6;
            this.reply0Btn.Text = "Goto";
            this.reply0Btn.UseVisualStyleBackColor = true;
            this.reply0Btn.Click += new System.EventHandler(this.onGotoNode);
            // 
            // reply0Txt
            // 
            this.reply0Txt.Location = new System.Drawing.Point(613, 207);
            this.reply0Txt.Name = "reply0Txt";
            this.reply0Txt.Size = new System.Drawing.Size(253, 20);
            this.reply0Txt.TabIndex = 5;
            this.reply0Txt.TextChanged += new System.EventHandler(this.onTextChanged);
            // 
            // talkTree
            // 
            this.talkTree.FullRowSelect = true;
            this.talkTree.HideSelection = false;
            this.talkTree.Location = new System.Drawing.Point(12, 12);
            this.talkTree.Name = "talkTree";
            this.talkTree.Size = new System.Drawing.Size(486, 334);
            this.talkTree.TabIndex = 0;
            this.talkTree.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.onTreeSelection);
            // 
            // reply0Cbo
            // 
            this.reply0Cbo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.reply0Cbo.FormattingEnabled = true;
            this.reply0Cbo.Location = new System.Drawing.Point(516, 207);
            this.reply0Cbo.Name = "reply0Cbo";
            this.reply0Cbo.Size = new System.Drawing.Size(59, 21);
            this.reply0Cbo.TabIndex = 3;
            // 
            // reply1Cbo
            // 
            this.reply1Cbo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.reply1Cbo.FormattingEnabled = true;
            this.reply1Cbo.Location = new System.Drawing.Point(516, 234);
            this.reply1Cbo.Name = "reply1Cbo";
            this.reply1Cbo.Size = new System.Drawing.Size(59, 21);
            this.reply1Cbo.TabIndex = 7;
            // 
            // reply2Cbo
            // 
            this.reply2Cbo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.reply2Cbo.FormattingEnabled = true;
            this.reply2Cbo.Location = new System.Drawing.Point(516, 261);
            this.reply2Cbo.Name = "reply2Cbo";
            this.reply2Cbo.Size = new System.Drawing.Size(59, 21);
            this.reply2Cbo.TabIndex = 11;
            // 
            // reply3Cbo
            // 
            this.reply3Cbo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.reply3Cbo.FormattingEnabled = true;
            this.reply3Cbo.Location = new System.Drawing.Point(516, 287);
            this.reply3Cbo.Name = "reply3Cbo";
            this.reply3Cbo.Size = new System.Drawing.Size(59, 21);
            this.reply3Cbo.TabIndex = 15;
            // 
            // replyId0Txt
            // 
            this.replyId0Txt.Location = new System.Drawing.Point(577, 207);
            this.replyId0Txt.Name = "replyId0Txt";
            this.replyId0Txt.Size = new System.Drawing.Size(34, 20);
            this.replyId0Txt.TabIndex = 4;
            this.replyId0Txt.TextChanged += new System.EventHandler(this.onIdChange);
            // 
            // replyId1Txt
            // 
            this.replyId1Txt.Location = new System.Drawing.Point(577, 234);
            this.replyId1Txt.Name = "replyId1Txt";
            this.replyId1Txt.Size = new System.Drawing.Size(34, 20);
            this.replyId1Txt.TabIndex = 8;
            this.replyId1Txt.TextChanged += new System.EventHandler(this.onIdChange);
            // 
            // replyId2Txt
            // 
            this.replyId2Txt.Location = new System.Drawing.Point(577, 261);
            this.replyId2Txt.Name = "replyId2Txt";
            this.replyId2Txt.Size = new System.Drawing.Size(34, 20);
            this.replyId2Txt.TabIndex = 12;
            this.replyId2Txt.TextChanged += new System.EventHandler(this.onIdChange);
            // 
            // replyId3Txt
            // 
            this.replyId3Txt.Location = new System.Drawing.Point(577, 287);
            this.replyId3Txt.Name = "replyId3Txt";
            this.replyId3Txt.Size = new System.Drawing.Size(34, 20);
            this.replyId3Txt.TabIndex = 16;
            this.replyId3Txt.TextChanged += new System.EventHandler(this.onIdChange);
            // 
            // addNode
            // 
            this.addNode.Location = new System.Drawing.Point(12, 352);
            this.addNode.Name = "addNode";
            this.addNode.Size = new System.Drawing.Size(75, 23);
            this.addNode.TabIndex = 20;
            this.addNode.Text = "Add";
            this.addNode.UseVisualStyleBackColor = true;
            this.addNode.Click += new System.EventHandler(this.onAddNode);
            // 
            // removeNode
            // 
            this.removeNode.Location = new System.Drawing.Point(93, 352);
            this.removeNode.Name = "removeNode";
            this.removeNode.Size = new System.Drawing.Size(75, 23);
            this.removeNode.TabIndex = 21;
            this.removeNode.Text = "Remove";
            this.removeNode.UseVisualStyleBackColor = true;
            this.removeNode.Click += new System.EventHandler(this.onRemoveNode);
            // 
            // viewXml
            // 
            this.viewXml.Location = new System.Drawing.Point(113, 19);
            this.viewXml.Name = "viewXml";
            this.viewXml.Size = new System.Drawing.Size(73, 23);
            this.viewXml.TabIndex = 2;
            this.viewXml.Text = "View/Edit";
            this.viewXml.UseVisualStyleBackColor = true;
            this.viewXml.Click += new System.EventHandler(this.onViewXml);
            // 
            // pasteXml
            // 
            this.pasteXml.Location = new System.Drawing.Point(5, 19);
            this.pasteXml.Name = "pasteXml";
            this.pasteXml.Size = new System.Drawing.Size(48, 23);
            this.pasteXml.TabIndex = 0;
            this.pasteXml.Text = "Paste";
            this.pasteXml.UseVisualStyleBackColor = true;
            this.pasteXml.Click += new System.EventHandler(this.onPasteXml);
            // 
            // replyPrevious
            // 
            this.replyPrevious.Enabled = false;
            this.replyPrevious.Location = new System.Drawing.Point(869, 315);
            this.replyPrevious.Name = "replyPrevious";
            this.replyPrevious.Size = new System.Drawing.Size(41, 23);
            this.replyPrevious.TabIndex = 19;
            this.replyPrevious.Text = "Back";
            this.replyPrevious.UseVisualStyleBackColor = true;
            this.replyPrevious.Click += new System.EventHandler(this.onPreviousNode);
            // 
            // xmlGroup
            // 
            this.xmlGroup.Controls.Add(this.copyXml);
            this.xmlGroup.Controls.Add(this.pasteXml);
            this.xmlGroup.Controls.Add(this.viewXml);
            this.xmlGroup.Location = new System.Drawing.Point(504, 324);
            this.xmlGroup.Name = "xmlGroup";
            this.xmlGroup.Size = new System.Drawing.Size(194, 48);
            this.xmlGroup.TabIndex = 22;
            this.xmlGroup.TabStop = false;
            this.xmlGroup.Text = "XML";
            // 
            // copyXml
            // 
            this.copyXml.Location = new System.Drawing.Point(59, 19);
            this.copyXml.Name = "copyXml";
            this.copyXml.Size = new System.Drawing.Size(48, 23);
            this.copyXml.TabIndex = 1;
            this.copyXml.Text = "Copy";
            this.copyXml.UseVisualStyleBackColor = true;
            this.copyXml.Click += new System.EventHandler(this.onCopyXml);
            // 
            // FrmTalkTree
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(921, 387);
            this.Controls.Add(this.xmlGroup);
            this.Controls.Add(this.replyPrevious);
            this.Controls.Add(this.removeNode);
            this.Controls.Add(this.addNode);
            this.Controls.Add(this.replyId3Txt);
            this.Controls.Add(this.replyId2Txt);
            this.Controls.Add(this.replyId1Txt);
            this.Controls.Add(this.replyId0Txt);
            this.Controls.Add(this.reply3Cbo);
            this.Controls.Add(this.reply2Cbo);
            this.Controls.Add(this.reply1Cbo);
            this.Controls.Add(this.reply0Cbo);
            this.Controls.Add(this.talkTree);
            this.Controls.Add(this.reply0Btn);
            this.Controls.Add(this.reply0Txt);
            this.Controls.Add(this.cancelBtn);
            this.Controls.Add(this.okBtn);
            this.Controls.Add(this.reply3Btn);
            this.Controls.Add(this.reply3Txt);
            this.Controls.Add(this.reply2Btn);
            this.Controls.Add(this.reply2Txt);
            this.Controls.Add(this.reply1Btn);
            this.Controls.Add(this.reply1Txt);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.npcText);
            this.Name = "FrmTalkTree";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Talk Tree";
            this.xmlGroup.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox npcText;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox reply1Txt;
        private System.Windows.Forms.Button reply1Btn;
        private System.Windows.Forms.Button reply2Btn;
        private System.Windows.Forms.TextBox reply2Txt;
        private System.Windows.Forms.Button reply3Btn;
        private System.Windows.Forms.TextBox reply3Txt;
        private System.Windows.Forms.Button okBtn;
        private System.Windows.Forms.Button cancelBtn;
        private System.Windows.Forms.Button reply0Btn;
        private System.Windows.Forms.TextBox reply0Txt;
        private System.Windows.Forms.TreeView talkTree;
        private System.Windows.Forms.ComboBox reply0Cbo;
        private System.Windows.Forms.ComboBox reply1Cbo;
        private System.Windows.Forms.ComboBox reply2Cbo;
        private System.Windows.Forms.ComboBox reply3Cbo;
        private System.Windows.Forms.TextBox replyId0Txt;
        private System.Windows.Forms.TextBox replyId1Txt;
        private System.Windows.Forms.TextBox replyId2Txt;
        private System.Windows.Forms.TextBox replyId3Txt;
        private System.Windows.Forms.Button addNode;
        private System.Windows.Forms.Button removeNode;
        private System.Windows.Forms.Button viewXml;
        private System.Windows.Forms.Button pasteXml;
        private System.Windows.Forms.Button replyPrevious;
        private System.Windows.Forms.GroupBox xmlGroup;
        private System.Windows.Forms.Button copyXml;
    }
}

