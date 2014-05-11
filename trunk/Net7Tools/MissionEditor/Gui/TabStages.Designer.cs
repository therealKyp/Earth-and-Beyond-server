namespace MissionEditor.Gui
{
    partial class TabStages
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(TabStages));
            this.guiDescriptionLbl = new System.Windows.Forms.Label();
            this.guiDescriptionTxt = new System.Windows.Forms.TextBox();
            this.guiCompletionsRemoveBtn = new System.Windows.Forms.Button();
            this.guiCompletionsAddBtn = new System.Windows.Forms.Button();
            this.guiCompletionsLbl = new System.Windows.Forms.Label();
            this.guiCompletionsTbl = new System.Windows.Forms.ListView();
            this.guiColumnHeaderType = new System.Windows.Forms.ColumnHeader();
            this.guiColumnHeaderValue = new System.Windows.Forms.ColumnHeader();
            this.guiRewardsRemoveBtn = new System.Windows.Forms.Button();
            this.guiRewardsAddBtn = new System.Windows.Forms.Button();
            this.guiRewardsLbl = new System.Windows.Forms.Label();
            this.guiRewardsTbl = new System.Windows.Forms.ListView();
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
            this.guiTalkTreeBtn = new System.Windows.Forms.Button();
            this.guiRewardsEditBtn = new System.Windows.Forms.Button();
            this.guiCompletionsEditBtn = new System.Windows.Forms.Button();
            this.guiStagesLbl = new System.Windows.Forms.Label();
            this.guiStagesCbo = new System.Windows.Forms.ComboBox();
            this.guiStagesAddBtn = new System.Windows.Forms.Button();
            this.guiCompletionUpBtn = new System.Windows.Forms.Button();
            this.guiCompletionDownBtn = new System.Windows.Forms.Button();
            this.guiRewardUpBtn = new System.Windows.Forms.Button();
            this.guiRewardDownBtn = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // guiDescriptionLbl
            // 
            this.guiDescriptionLbl.AutoSize = true;
            this.guiDescriptionLbl.Location = new System.Drawing.Point(18, 46);
            this.guiDescriptionLbl.Name = "guiDescriptionLbl";
            this.guiDescriptionLbl.Size = new System.Drawing.Size(63, 13);
            this.guiDescriptionLbl.TabIndex = 3;
            this.guiDescriptionLbl.Text = "Description:";
            // 
            // guiDescriptionTxt
            // 
            this.guiDescriptionTxt.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.guiDescriptionTxt.Location = new System.Drawing.Point(87, 43);
            this.guiDescriptionTxt.Multiline = true;
            this.guiDescriptionTxt.Name = "guiDescriptionTxt";
            this.guiDescriptionTxt.Size = new System.Drawing.Size(437, 49);
            this.guiDescriptionTxt.TabIndex = 4;
            this.guiDescriptionTxt.TextChanged += new System.EventHandler(this.onChanged);
            this.guiDescriptionTxt.Leave += new System.EventHandler(this.onLeaveDescription);
            // 
            // guiCompletionsRemoveBtn
            // 
            this.guiCompletionsRemoveBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.guiCompletionsRemoveBtn.Enabled = false;
            this.guiCompletionsRemoveBtn.Location = new System.Drawing.Point(449, 156);
            this.guiCompletionsRemoveBtn.Name = "guiCompletionsRemoveBtn";
            this.guiCompletionsRemoveBtn.Size = new System.Drawing.Size(75, 23);
            this.guiCompletionsRemoveBtn.TabIndex = 9;
            this.guiCompletionsRemoveBtn.Text = "Remove";
            this.guiCompletionsRemoveBtn.UseVisualStyleBackColor = true;
            this.guiCompletionsRemoveBtn.Click += new System.EventHandler(this.onCompletionRemove);
            // 
            // guiCompletionsAddBtn
            // 
            this.guiCompletionsAddBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.guiCompletionsAddBtn.Location = new System.Drawing.Point(449, 98);
            this.guiCompletionsAddBtn.Name = "guiCompletionsAddBtn";
            this.guiCompletionsAddBtn.Size = new System.Drawing.Size(75, 23);
            this.guiCompletionsAddBtn.TabIndex = 7;
            this.guiCompletionsAddBtn.Text = "Add";
            this.guiCompletionsAddBtn.UseVisualStyleBackColor = true;
            this.guiCompletionsAddBtn.Click += new System.EventHandler(this.onCompletionAdd);
            // 
            // guiCompletionsLbl
            // 
            this.guiCompletionsLbl.AutoSize = true;
            this.guiCompletionsLbl.Location = new System.Drawing.Point(14, 98);
            this.guiCompletionsLbl.Name = "guiCompletionsLbl";
            this.guiCompletionsLbl.Size = new System.Drawing.Size(67, 13);
            this.guiCompletionsLbl.TabIndex = 5;
            this.guiCompletionsLbl.Text = "Completions:";
            // 
            // guiCompletionsTbl
            // 
            this.guiCompletionsTbl.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.guiCompletionsTbl.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.guiColumnHeaderType,
            this.guiColumnHeaderValue});
            this.guiCompletionsTbl.FullRowSelect = true;
            this.guiCompletionsTbl.HideSelection = false;
            this.guiCompletionsTbl.Location = new System.Drawing.Point(87, 98);
            this.guiCompletionsTbl.Name = "guiCompletionsTbl";
            this.guiCompletionsTbl.Size = new System.Drawing.Size(356, 189);
            this.guiCompletionsTbl.TabIndex = 6;
            this.guiCompletionsTbl.UseCompatibleStateImageBehavior = false;
            this.guiCompletionsTbl.View = System.Windows.Forms.View.Details;
            // 
            // guiColumnHeaderType
            // 
            this.guiColumnHeaderType.Text = "Type";
            this.guiColumnHeaderType.Width = 130;
            // 
            // guiColumnHeaderValue
            // 
            this.guiColumnHeaderValue.Text = "Value";
            this.guiColumnHeaderValue.Width = 200;
            // 
            // guiRewardsRemoveBtn
            // 
            this.guiRewardsRemoveBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.guiRewardsRemoveBtn.Enabled = false;
            this.guiRewardsRemoveBtn.Location = new System.Drawing.Point(449, 351);
            this.guiRewardsRemoveBtn.Name = "guiRewardsRemoveBtn";
            this.guiRewardsRemoveBtn.Size = new System.Drawing.Size(75, 23);
            this.guiRewardsRemoveBtn.TabIndex = 14;
            this.guiRewardsRemoveBtn.Text = "Remove";
            this.guiRewardsRemoveBtn.UseVisualStyleBackColor = true;
            this.guiRewardsRemoveBtn.Click += new System.EventHandler(this.onRewardsRemove);
            // 
            // guiRewardsAddBtn
            // 
            this.guiRewardsAddBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.guiRewardsAddBtn.Location = new System.Drawing.Point(449, 293);
            this.guiRewardsAddBtn.Name = "guiRewardsAddBtn";
            this.guiRewardsAddBtn.Size = new System.Drawing.Size(75, 23);
            this.guiRewardsAddBtn.TabIndex = 12;
            this.guiRewardsAddBtn.Text = "Add";
            this.guiRewardsAddBtn.UseVisualStyleBackColor = true;
            this.guiRewardsAddBtn.Click += new System.EventHandler(this.onRewardsAdd);
            // 
            // guiRewardsLbl
            // 
            this.guiRewardsLbl.AutoSize = true;
            this.guiRewardsLbl.Location = new System.Drawing.Point(29, 293);
            this.guiRewardsLbl.Name = "guiRewardsLbl";
            this.guiRewardsLbl.Size = new System.Drawing.Size(52, 13);
            this.guiRewardsLbl.TabIndex = 10;
            this.guiRewardsLbl.Text = "Rewards:";
            // 
            // guiRewardsTbl
            // 
            this.guiRewardsTbl.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.guiRewardsTbl.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});
            this.guiRewardsTbl.FullRowSelect = true;
            this.guiRewardsTbl.HideSelection = false;
            this.guiRewardsTbl.Location = new System.Drawing.Point(87, 293);
            this.guiRewardsTbl.Name = "guiRewardsTbl";
            this.guiRewardsTbl.Size = new System.Drawing.Size(356, 158);
            this.guiRewardsTbl.TabIndex = 11;
            this.guiRewardsTbl.UseCompatibleStateImageBehavior = false;
            this.guiRewardsTbl.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Type";
            this.columnHeader1.Width = 130;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Value";
            this.columnHeader2.Width = 200;
            // 
            // guiTalkTreeBtn
            // 
            this.guiTalkTreeBtn.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.guiTalkTreeBtn.Location = new System.Drawing.Point(87, 457);
            this.guiTalkTreeBtn.Name = "guiTalkTreeBtn";
            this.guiTalkTreeBtn.Size = new System.Drawing.Size(356, 53);
            this.guiTalkTreeBtn.TabIndex = 15;
            this.guiTalkTreeBtn.Text = "Talk Tree";
            this.guiTalkTreeBtn.UseVisualStyleBackColor = true;
            this.guiTalkTreeBtn.Click += new System.EventHandler(this.onTalkTree);
            // 
            // guiRewardsEditBtn
            // 
            this.guiRewardsEditBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.guiRewardsEditBtn.Enabled = false;
            this.guiRewardsEditBtn.Location = new System.Drawing.Point(449, 322);
            this.guiRewardsEditBtn.Name = "guiRewardsEditBtn";
            this.guiRewardsEditBtn.Size = new System.Drawing.Size(75, 23);
            this.guiRewardsEditBtn.TabIndex = 13;
            this.guiRewardsEditBtn.Text = "Edit";
            this.guiRewardsEditBtn.UseVisualStyleBackColor = true;
            this.guiRewardsEditBtn.Click += new System.EventHandler(this.onRewardEdit);
            // 
            // guiCompletionsEditBtn
            // 
            this.guiCompletionsEditBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.guiCompletionsEditBtn.Enabled = false;
            this.guiCompletionsEditBtn.Location = new System.Drawing.Point(449, 127);
            this.guiCompletionsEditBtn.Name = "guiCompletionsEditBtn";
            this.guiCompletionsEditBtn.Size = new System.Drawing.Size(75, 23);
            this.guiCompletionsEditBtn.TabIndex = 8;
            this.guiCompletionsEditBtn.Text = "Edit";
            this.guiCompletionsEditBtn.UseVisualStyleBackColor = true;
            this.guiCompletionsEditBtn.Click += new System.EventHandler(this.onCompletionEdit);
            // 
            // guiStagesLbl
            // 
            this.guiStagesLbl.AutoSize = true;
            this.guiStagesLbl.Location = new System.Drawing.Point(38, 13);
            this.guiStagesLbl.Name = "guiStagesLbl";
            this.guiStagesLbl.Size = new System.Drawing.Size(43, 13);
            this.guiStagesLbl.TabIndex = 0;
            this.guiStagesLbl.Text = "Stages:";
            // 
            // guiStagesCbo
            // 
            this.guiStagesCbo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.guiStagesCbo.FormattingEnabled = true;
            this.guiStagesCbo.Location = new System.Drawing.Point(87, 13);
            this.guiStagesCbo.Name = "guiStagesCbo";
            this.guiStagesCbo.Size = new System.Drawing.Size(356, 21);
            this.guiStagesCbo.TabIndex = 1;
            this.guiStagesCbo.SelectedIndexChanged += new System.EventHandler(this.onStageSelected);
            // 
            // guiStagesAddBtn
            // 
            this.guiStagesAddBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.guiStagesAddBtn.Location = new System.Drawing.Point(449, 11);
            this.guiStagesAddBtn.Name = "guiStagesAddBtn";
            this.guiStagesAddBtn.Size = new System.Drawing.Size(75, 23);
            this.guiStagesAddBtn.TabIndex = 2;
            this.guiStagesAddBtn.Text = "Add";
            this.guiStagesAddBtn.UseVisualStyleBackColor = true;
            this.guiStagesAddBtn.Click += new System.EventHandler(this.onAddStage);
            // 
            // guiCompletionUpBtn
            // 
            this.guiCompletionUpBtn.Image = ((System.Drawing.Image)(resources.GetObject("guiCompletionUpBtn.Image")));
            this.guiCompletionUpBtn.Location = new System.Drawing.Point(449, 183);
            this.guiCompletionUpBtn.Margin = new System.Windows.Forms.Padding(1);
            this.guiCompletionUpBtn.Name = "guiCompletionUpBtn";
            this.guiCompletionUpBtn.Size = new System.Drawing.Size(39, 35);
            this.guiCompletionUpBtn.TabIndex = 27;
            this.guiCompletionUpBtn.UseVisualStyleBackColor = true;
            this.guiCompletionUpBtn.Click += new System.EventHandler(this.onSimpleChanged);
            // 
            // guiCompletionDownBtn
            // 
            this.guiCompletionDownBtn.Image = ((System.Drawing.Image)(resources.GetObject("guiCompletionDownBtn.Image")));
            this.guiCompletionDownBtn.Location = new System.Drawing.Point(487, 183);
            this.guiCompletionDownBtn.Margin = new System.Windows.Forms.Padding(1);
            this.guiCompletionDownBtn.Name = "guiCompletionDownBtn";
            this.guiCompletionDownBtn.Size = new System.Drawing.Size(39, 35);
            this.guiCompletionDownBtn.TabIndex = 26;
            this.guiCompletionDownBtn.UseVisualStyleBackColor = true;
            this.guiCompletionDownBtn.Click += new System.EventHandler(this.onSimpleChanged);
            // 
            // guiRewardUpBtn
            // 
            this.guiRewardUpBtn.Image = ((System.Drawing.Image)(resources.GetObject("guiRewardUpBtn.Image")));
            this.guiRewardUpBtn.Location = new System.Drawing.Point(449, 378);
            this.guiRewardUpBtn.Margin = new System.Windows.Forms.Padding(1);
            this.guiRewardUpBtn.Name = "guiRewardUpBtn";
            this.guiRewardUpBtn.Size = new System.Drawing.Size(39, 35);
            this.guiRewardUpBtn.TabIndex = 29;
            this.guiRewardUpBtn.UseVisualStyleBackColor = true;
            this.guiRewardUpBtn.Click += new System.EventHandler(this.onSimpleChanged);
            // 
            // guiRewardDownBtn
            // 
            this.guiRewardDownBtn.Image = ((System.Drawing.Image)(resources.GetObject("guiRewardDownBtn.Image")));
            this.guiRewardDownBtn.Location = new System.Drawing.Point(487, 378);
            this.guiRewardDownBtn.Margin = new System.Windows.Forms.Padding(1);
            this.guiRewardDownBtn.Name = "guiRewardDownBtn";
            this.guiRewardDownBtn.Size = new System.Drawing.Size(39, 35);
            this.guiRewardDownBtn.TabIndex = 28;
            this.guiRewardDownBtn.UseVisualStyleBackColor = true;
            this.guiRewardDownBtn.Click += new System.EventHandler(this.onSimpleChanged);
            // 
            // TabStages
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.BackColor = System.Drawing.Color.PaleGreen;
            this.Controls.Add(this.guiRewardUpBtn);
            this.Controls.Add(this.guiRewardDownBtn);
            this.Controls.Add(this.guiCompletionUpBtn);
            this.Controls.Add(this.guiCompletionDownBtn);
            this.Controls.Add(this.guiStagesAddBtn);
            this.Controls.Add(this.guiStagesCbo);
            this.Controls.Add(this.guiStagesLbl);
            this.Controls.Add(this.guiCompletionsEditBtn);
            this.Controls.Add(this.guiRewardsEditBtn);
            this.Controls.Add(this.guiTalkTreeBtn);
            this.Controls.Add(this.guiRewardsRemoveBtn);
            this.Controls.Add(this.guiRewardsAddBtn);
            this.Controls.Add(this.guiRewardsLbl);
            this.Controls.Add(this.guiRewardsTbl);
            this.Controls.Add(this.guiCompletionsRemoveBtn);
            this.Controls.Add(this.guiCompletionsAddBtn);
            this.Controls.Add(this.guiCompletionsLbl);
            this.Controls.Add(this.guiCompletionsTbl);
            this.Controls.Add(this.guiDescriptionTxt);
            this.Controls.Add(this.guiDescriptionLbl);
            this.Name = "TabStages";
            this.Size = new System.Drawing.Size(550, 525);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label guiDescriptionLbl;
        private System.Windows.Forms.TextBox guiDescriptionTxt;
        private System.Windows.Forms.Button guiCompletionsRemoveBtn;
        private System.Windows.Forms.Button guiCompletionsAddBtn;
        private System.Windows.Forms.Label guiCompletionsLbl;
        private System.Windows.Forms.ListView guiCompletionsTbl;
        private System.Windows.Forms.ColumnHeader guiColumnHeaderType;
        private System.Windows.Forms.ColumnHeader guiColumnHeaderValue;
        private System.Windows.Forms.Button guiRewardsRemoveBtn;
        private System.Windows.Forms.Button guiRewardsAddBtn;
        private System.Windows.Forms.Label guiRewardsLbl;
        private System.Windows.Forms.ListView guiRewardsTbl;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.Button guiTalkTreeBtn;
        private System.Windows.Forms.Button guiRewardsEditBtn;
        private System.Windows.Forms.Button guiCompletionsEditBtn;
        private System.Windows.Forms.Label guiStagesLbl;
        private System.Windows.Forms.ComboBox guiStagesCbo;
        private System.Windows.Forms.Button guiStagesAddBtn;
        private System.Windows.Forms.Button guiCompletionUpBtn;
        private System.Windows.Forms.Button guiCompletionDownBtn;
        private System.Windows.Forms.Button guiRewardUpBtn;
        private System.Windows.Forms.Button guiRewardDownBtn;
    }
}
