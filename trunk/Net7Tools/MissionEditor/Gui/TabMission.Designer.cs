namespace MissionEditor.Gui
{
    partial class TabMission
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(TabMission));
            this.guiSummaryTxt = new System.Windows.Forms.TextBox();
            this.guiSummaryLbl = new System.Windows.Forms.Label();
            this.guiNameTxt = new System.Windows.Forms.TextBox();
            this.guiNameLbl = new System.Windows.Forms.Label();
            this.guiForfeitableChk = new System.Windows.Forms.CheckBox();
            this.guiTimeTxt = new System.Windows.Forms.TextBox();
            this.guiTimeLbl = new System.Windows.Forms.Label();
            this.guiIdTxt = new System.Windows.Forms.TextBox();
            this.guiIdLbl = new System.Windows.Forms.Label();
            this.guiConditionsTbl = new System.Windows.Forms.ListView();
            this.guiColumnHeaderType = new System.Windows.Forms.ColumnHeader();
            this.guiColumnHeaderValue = new System.Windows.Forms.ColumnHeader();
            this.guiConditionsLbl = new System.Windows.Forms.Label();
            this.guiConditionsAddBtn = new System.Windows.Forms.Button();
            this.guiConditionsRemoveBtn = new System.Windows.Forms.Button();
            this.guiStagesRemoveBtn = new System.Windows.Forms.Button();
            this.guiStagesAddBtn = new System.Windows.Forms.Button();
            this.guiStagesLbl = new System.Windows.Forms.Label();
            this.guiStagesTbl = new System.Windows.Forms.ListView();
            this.guiColumnStageId = new System.Windows.Forms.ColumnHeader();
            this.guiColumnStageDescription = new System.Windows.Forms.ColumnHeader();
            this.guiConditionsEditBtn = new System.Windows.Forms.Button();
            this.guiTypeCbo = new System.Windows.Forms.ComboBox();
            this.guiTypeLbl = new System.Windows.Forms.Label();
            this.guiKeyLbl = new System.Windows.Forms.Label();
            this.guiKeyTxt = new System.Windows.Forms.TextBox();
            this.guiKeyBtn = new System.Windows.Forms.Button();
            this.guiStageDownBtn = new System.Windows.Forms.Button();
            this.guiStageUpBtn = new System.Windows.Forms.Button();
            this.guiConditionUpBtn = new System.Windows.Forms.Button();
            this.guiConditionDownBtn = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // guiSummaryTxt
            // 
            this.guiSummaryTxt.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.guiSummaryTxt.Location = new System.Drawing.Point(74, 66);
            this.guiSummaryTxt.Multiline = true;
            this.guiSummaryTxt.Name = "guiSummaryTxt";
            this.guiSummaryTxt.Size = new System.Drawing.Size(461, 45);
            this.guiSummaryTxt.TabIndex = 10;
            this.guiSummaryTxt.TextChanged += new System.EventHandler(this.onChanged);
            // 
            // guiSummaryLbl
            // 
            this.guiSummaryLbl.AutoSize = true;
            this.guiSummaryLbl.Location = new System.Drawing.Point(15, 69);
            this.guiSummaryLbl.Name = "guiSummaryLbl";
            this.guiSummaryLbl.Size = new System.Drawing.Size(53, 13);
            this.guiSummaryLbl.TabIndex = 9;
            this.guiSummaryLbl.Text = "Summary:";
            // 
            // guiNameTxt
            // 
            this.guiNameTxt.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.guiNameTxt.Location = new System.Drawing.Point(74, 40);
            this.guiNameTxt.Name = "guiNameTxt";
            this.guiNameTxt.Size = new System.Drawing.Size(356, 20);
            this.guiNameTxt.TabIndex = 8;
            this.guiNameTxt.TextChanged += new System.EventHandler(this.onChanged);
            // 
            // guiNameLbl
            // 
            this.guiNameLbl.AutoSize = true;
            this.guiNameLbl.Location = new System.Drawing.Point(30, 43);
            this.guiNameLbl.Name = "guiNameLbl";
            this.guiNameLbl.Size = new System.Drawing.Size(38, 13);
            this.guiNameLbl.TabIndex = 7;
            this.guiNameLbl.Text = "Name:";
            // 
            // guiForfeitableChk
            // 
            this.guiForfeitableChk.AutoSize = true;
            this.guiForfeitableChk.Location = new System.Drawing.Point(74, 142);
            this.guiForfeitableChk.Name = "guiForfeitableChk";
            this.guiForfeitableChk.Size = new System.Drawing.Size(75, 17);
            this.guiForfeitableChk.TabIndex = 13;
            this.guiForfeitableChk.Text = "Forfeitable";
            this.guiForfeitableChk.UseVisualStyleBackColor = true;
            this.guiForfeitableChk.CheckedChanged += new System.EventHandler(this.onChanged);
            // 
            // guiTimeTxt
            // 
            this.guiTimeTxt.Location = new System.Drawing.Point(74, 117);
            this.guiTimeTxt.Name = "guiTimeTxt";
            this.guiTimeTxt.Size = new System.Drawing.Size(100, 20);
            this.guiTimeTxt.TabIndex = 12;
            this.guiTimeTxt.TextChanged += new System.EventHandler(this.onChanged);
            this.guiTimeTxt.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.onTimeKeyPress);
            // 
            // guiTimeLbl
            // 
            this.guiTimeLbl.AutoSize = true;
            this.guiTimeLbl.Location = new System.Drawing.Point(35, 120);
            this.guiTimeLbl.Name = "guiTimeLbl";
            this.guiTimeLbl.Size = new System.Drawing.Size(33, 13);
            this.guiTimeLbl.TabIndex = 11;
            this.guiTimeLbl.Text = "Time:";
            // 
            // guiIdTxt
            // 
            this.guiIdTxt.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.guiIdTxt.Enabled = false;
            this.guiIdTxt.Location = new System.Drawing.Point(74, 13);
            this.guiIdTxt.Name = "guiIdTxt";
            this.guiIdTxt.Size = new System.Drawing.Size(55, 20);
            this.guiIdTxt.TabIndex = 1;
            // 
            // guiIdLbl
            // 
            this.guiIdLbl.AutoSize = true;
            this.guiIdLbl.Location = new System.Drawing.Point(47, 16);
            this.guiIdLbl.Name = "guiIdLbl";
            this.guiIdLbl.Size = new System.Drawing.Size(21, 13);
            this.guiIdLbl.TabIndex = 0;
            this.guiIdLbl.Text = "ID:";
            // 
            // guiConditionsTbl
            // 
            this.guiConditionsTbl.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.guiConditionsTbl.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.guiColumnHeaderType,
            this.guiColumnHeaderValue});
            this.guiConditionsTbl.FullRowSelect = true;
            this.guiConditionsTbl.HideSelection = false;
            this.guiConditionsTbl.Location = new System.Drawing.Point(74, 165);
            this.guiConditionsTbl.Name = "guiConditionsTbl";
            this.guiConditionsTbl.Size = new System.Drawing.Size(380, 113);
            this.guiConditionsTbl.TabIndex = 15;
            this.guiConditionsTbl.UseCompatibleStateImageBehavior = false;
            this.guiConditionsTbl.View = System.Windows.Forms.View.Details;
            // 
            // guiColumnHeaderType
            // 
            this.guiColumnHeaderType.Text = "Type";
            this.guiColumnHeaderType.Width = 100;
            // 
            // guiColumnHeaderValue
            // 
            this.guiColumnHeaderValue.Text = "Value";
            this.guiColumnHeaderValue.Width = 250;
            // 
            // guiConditionsLbl
            // 
            this.guiConditionsLbl.AutoSize = true;
            this.guiConditionsLbl.Location = new System.Drawing.Point(9, 165);
            this.guiConditionsLbl.Name = "guiConditionsLbl";
            this.guiConditionsLbl.Size = new System.Drawing.Size(59, 13);
            this.guiConditionsLbl.TabIndex = 14;
            this.guiConditionsLbl.Text = "Conditions:";
            // 
            // guiConditionsAddBtn
            // 
            this.guiConditionsAddBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.guiConditionsAddBtn.Location = new System.Drawing.Point(460, 165);
            this.guiConditionsAddBtn.Name = "guiConditionsAddBtn";
            this.guiConditionsAddBtn.Size = new System.Drawing.Size(75, 23);
            this.guiConditionsAddBtn.TabIndex = 16;
            this.guiConditionsAddBtn.Text = "Add";
            this.guiConditionsAddBtn.UseVisualStyleBackColor = true;
            this.guiConditionsAddBtn.Click += new System.EventHandler(this.onConditionAdd);
            // 
            // guiConditionsRemoveBtn
            // 
            this.guiConditionsRemoveBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.guiConditionsRemoveBtn.Enabled = false;
            this.guiConditionsRemoveBtn.Location = new System.Drawing.Point(460, 223);
            this.guiConditionsRemoveBtn.Name = "guiConditionsRemoveBtn";
            this.guiConditionsRemoveBtn.Size = new System.Drawing.Size(75, 23);
            this.guiConditionsRemoveBtn.TabIndex = 18;
            this.guiConditionsRemoveBtn.Text = "Remove";
            this.guiConditionsRemoveBtn.UseVisualStyleBackColor = true;
            this.guiConditionsRemoveBtn.Click += new System.EventHandler(this.onConditionRemove);
            // 
            // guiStagesRemoveBtn
            // 
            this.guiStagesRemoveBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.guiStagesRemoveBtn.Enabled = false;
            this.guiStagesRemoveBtn.Location = new System.Drawing.Point(458, 330);
            this.guiStagesRemoveBtn.Name = "guiStagesRemoveBtn";
            this.guiStagesRemoveBtn.Size = new System.Drawing.Size(75, 23);
            this.guiStagesRemoveBtn.TabIndex = 23;
            this.guiStagesRemoveBtn.Text = "Remove";
            this.guiStagesRemoveBtn.UseVisualStyleBackColor = true;
            this.guiStagesRemoveBtn.Click += new System.EventHandler(this.onStageRemove);
            // 
            // guiStagesAddBtn
            // 
            this.guiStagesAddBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.guiStagesAddBtn.Location = new System.Drawing.Point(458, 301);
            this.guiStagesAddBtn.Name = "guiStagesAddBtn";
            this.guiStagesAddBtn.Size = new System.Drawing.Size(75, 23);
            this.guiStagesAddBtn.TabIndex = 22;
            this.guiStagesAddBtn.Text = "Add";
            this.guiStagesAddBtn.UseVisualStyleBackColor = true;
            this.guiStagesAddBtn.Click += new System.EventHandler(this.onStageAdd);
            // 
            // guiStagesLbl
            // 
            this.guiStagesLbl.AutoSize = true;
            this.guiStagesLbl.Location = new System.Drawing.Point(25, 306);
            this.guiStagesLbl.Name = "guiStagesLbl";
            this.guiStagesLbl.Size = new System.Drawing.Size(43, 13);
            this.guiStagesLbl.TabIndex = 21;
            this.guiStagesLbl.Text = "Stages:";
            // 
            // guiStagesTbl
            // 
            this.guiStagesTbl.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.guiStagesTbl.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.guiColumnStageId,
            this.guiColumnStageDescription});
            this.guiStagesTbl.FullRowSelect = true;
            this.guiStagesTbl.HideSelection = false;
            this.guiStagesTbl.Location = new System.Drawing.Point(74, 301);
            this.guiStagesTbl.MultiSelect = false;
            this.guiStagesTbl.Name = "guiStagesTbl";
            this.guiStagesTbl.Size = new System.Drawing.Size(380, 209);
            this.guiStagesTbl.TabIndex = 20;
            this.guiStagesTbl.UseCompatibleStateImageBehavior = false;
            this.guiStagesTbl.View = System.Windows.Forms.View.Details;
            this.guiStagesTbl.SelectedIndexChanged += new System.EventHandler(this.onStageSelected);
            // 
            // guiColumnStageId
            // 
            this.guiColumnStageId.Text = "Id";
            this.guiColumnStageId.Width = 30;
            // 
            // guiColumnStageDescription
            // 
            this.guiColumnStageDescription.Text = "Value";
            this.guiColumnStageDescription.Width = 320;
            // 
            // guiConditionsEditBtn
            // 
            this.guiConditionsEditBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.guiConditionsEditBtn.Enabled = false;
            this.guiConditionsEditBtn.Location = new System.Drawing.Point(460, 194);
            this.guiConditionsEditBtn.Name = "guiConditionsEditBtn";
            this.guiConditionsEditBtn.Size = new System.Drawing.Size(75, 23);
            this.guiConditionsEditBtn.TabIndex = 17;
            this.guiConditionsEditBtn.Text = "Edit";
            this.guiConditionsEditBtn.UseVisualStyleBackColor = true;
            this.guiConditionsEditBtn.Click += new System.EventHandler(this.onConditionEdit);
            // 
            // guiTypeCbo
            // 
            this.guiTypeCbo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.guiTypeCbo.FormattingEnabled = true;
            this.guiTypeCbo.Location = new System.Drawing.Point(193, 13);
            this.guiTypeCbo.Name = "guiTypeCbo";
            this.guiTypeCbo.Size = new System.Drawing.Size(121, 21);
            this.guiTypeCbo.TabIndex = 3;
            this.guiTypeCbo.SelectedIndexChanged += new System.EventHandler(this.onChanged);
            // 
            // guiTypeLbl
            // 
            this.guiTypeLbl.AutoSize = true;
            this.guiTypeLbl.Location = new System.Drawing.Point(153, 16);
            this.guiTypeLbl.Name = "guiTypeLbl";
            this.guiTypeLbl.Size = new System.Drawing.Size(34, 13);
            this.guiTypeLbl.TabIndex = 2;
            this.guiTypeLbl.Text = "Type:";
            // 
            // guiKeyLbl
            // 
            this.guiKeyLbl.AutoSize = true;
            this.guiKeyLbl.Location = new System.Drawing.Point(320, 16);
            this.guiKeyLbl.Name = "guiKeyLbl";
            this.guiKeyLbl.Size = new System.Drawing.Size(28, 13);
            this.guiKeyLbl.TabIndex = 4;
            this.guiKeyLbl.Text = "Key:";
            // 
            // guiKeyTxt
            // 
            this.guiKeyTxt.Location = new System.Drawing.Point(354, 13);
            this.guiKeyTxt.Name = "guiKeyTxt";
            this.guiKeyTxt.Size = new System.Drawing.Size(100, 20);
            this.guiKeyTxt.TabIndex = 5;
            this.guiKeyTxt.TextChanged += new System.EventHandler(this.onChanged);
            // 
            // guiKeyBtn
            // 
            this.guiKeyBtn.Location = new System.Drawing.Point(460, 11);
            this.guiKeyBtn.Name = "guiKeyBtn";
            this.guiKeyBtn.Size = new System.Drawing.Size(75, 23);
            this.guiKeyBtn.TabIndex = 6;
            this.guiKeyBtn.Text = "Search";
            this.guiKeyBtn.UseVisualStyleBackColor = true;
            this.guiKeyBtn.Click += new System.EventHandler(this.onKeySearch);
            // 
            // guiStageDownBtn
            // 
            this.guiStageDownBtn.Image = ((System.Drawing.Image)(resources.GetObject("guiStageDownBtn.Image")));
            this.guiStageDownBtn.Location = new System.Drawing.Point(494, 359);
            this.guiStageDownBtn.Margin = new System.Windows.Forms.Padding(1);
            this.guiStageDownBtn.Name = "guiStageDownBtn";
            this.guiStageDownBtn.Size = new System.Drawing.Size(39, 35);
            this.guiStageDownBtn.TabIndex = 25;
            this.guiStageDownBtn.UseVisualStyleBackColor = true;
            // 
            // guiStageUpBtn
            // 
            this.guiStageUpBtn.Image = ((System.Drawing.Image)(resources.GetObject("guiStageUpBtn.Image")));
            this.guiStageUpBtn.Location = new System.Drawing.Point(456, 359);
            this.guiStageUpBtn.Margin = new System.Windows.Forms.Padding(1);
            this.guiStageUpBtn.Name = "guiStageUpBtn";
            this.guiStageUpBtn.Size = new System.Drawing.Size(39, 35);
            this.guiStageUpBtn.TabIndex = 24;
            this.guiStageUpBtn.UseVisualStyleBackColor = true;
            // 
            // guiConditionUpBtn
            // 
            this.guiConditionUpBtn.Image = ((System.Drawing.Image)(resources.GetObject("guiConditionUpBtn.Image")));
            this.guiConditionUpBtn.Location = new System.Drawing.Point(458, 250);
            this.guiConditionUpBtn.Margin = new System.Windows.Forms.Padding(1);
            this.guiConditionUpBtn.Name = "guiConditionUpBtn";
            this.guiConditionUpBtn.Size = new System.Drawing.Size(39, 35);
            this.guiConditionUpBtn.TabIndex = 19;
            this.guiConditionUpBtn.UseVisualStyleBackColor = true;
            this.guiConditionUpBtn.Click += new System.EventHandler(this.onSimpleChanged);
            // 
            // guiConditionDownBtn
            // 
            this.guiConditionDownBtn.Image = ((System.Drawing.Image)(resources.GetObject("guiConditionDownBtn.Image")));
            this.guiConditionDownBtn.Location = new System.Drawing.Point(496, 250);
            this.guiConditionDownBtn.Margin = new System.Windows.Forms.Padding(1);
            this.guiConditionDownBtn.Name = "guiConditionDownBtn";
            this.guiConditionDownBtn.Size = new System.Drawing.Size(39, 35);
            this.guiConditionDownBtn.TabIndex = 20;
            this.guiConditionDownBtn.UseVisualStyleBackColor = true;
            this.guiConditionDownBtn.Click += new System.EventHandler(this.onSimpleChanged);
            // 
            // TabMission
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.BackColor = System.Drawing.Color.AntiqueWhite;
            this.Controls.Add(this.guiConditionUpBtn);
            this.Controls.Add(this.guiConditionDownBtn);
            this.Controls.Add(this.guiStageUpBtn);
            this.Controls.Add(this.guiStageDownBtn);
            this.Controls.Add(this.guiKeyBtn);
            this.Controls.Add(this.guiKeyTxt);
            this.Controls.Add(this.guiKeyLbl);
            this.Controls.Add(this.guiTypeLbl);
            this.Controls.Add(this.guiTypeCbo);
            this.Controls.Add(this.guiConditionsEditBtn);
            this.Controls.Add(this.guiStagesRemoveBtn);
            this.Controls.Add(this.guiStagesAddBtn);
            this.Controls.Add(this.guiStagesLbl);
            this.Controls.Add(this.guiStagesTbl);
            this.Controls.Add(this.guiConditionsRemoveBtn);
            this.Controls.Add(this.guiConditionsAddBtn);
            this.Controls.Add(this.guiConditionsLbl);
            this.Controls.Add(this.guiConditionsTbl);
            this.Controls.Add(this.guiSummaryTxt);
            this.Controls.Add(this.guiSummaryLbl);
            this.Controls.Add(this.guiNameTxt);
            this.Controls.Add(this.guiNameLbl);
            this.Controls.Add(this.guiForfeitableChk);
            this.Controls.Add(this.guiTimeTxt);
            this.Controls.Add(this.guiTimeLbl);
            this.Controls.Add(this.guiIdTxt);
            this.Controls.Add(this.guiIdLbl);
            this.Name = "TabMission";
            this.Size = new System.Drawing.Size(550, 525);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox guiSummaryTxt;
        private System.Windows.Forms.Label guiSummaryLbl;
        private System.Windows.Forms.TextBox guiNameTxt;
        private System.Windows.Forms.Label guiNameLbl;
        private System.Windows.Forms.CheckBox guiForfeitableChk;
        private System.Windows.Forms.TextBox guiTimeTxt;
        private System.Windows.Forms.Label guiTimeLbl;
        private System.Windows.Forms.TextBox guiIdTxt;
        private System.Windows.Forms.Label guiIdLbl;
        private System.Windows.Forms.ListView guiConditionsTbl;
        private System.Windows.Forms.ColumnHeader guiColumnHeaderType;
        private System.Windows.Forms.ColumnHeader guiColumnHeaderValue;
        private System.Windows.Forms.Label guiConditionsLbl;
        private System.Windows.Forms.Button guiConditionsAddBtn;
        private System.Windows.Forms.Button guiConditionsRemoveBtn;
        private System.Windows.Forms.Button guiStagesRemoveBtn;
        private System.Windows.Forms.Button guiStagesAddBtn;
        private System.Windows.Forms.Label guiStagesLbl;
        private System.Windows.Forms.ListView guiStagesTbl;
        private System.Windows.Forms.ColumnHeader guiColumnStageId;
        private System.Windows.Forms.ColumnHeader guiColumnStageDescription;
        private System.Windows.Forms.Button guiConditionsEditBtn;
        private System.Windows.Forms.ComboBox guiTypeCbo;
        private System.Windows.Forms.Label guiTypeLbl;
        private System.Windows.Forms.Label guiKeyLbl;
        private System.Windows.Forms.TextBox guiKeyTxt;
        private System.Windows.Forms.Button guiKeyBtn;
        private System.Windows.Forms.Button guiStageDownBtn;
        private System.Windows.Forms.Button guiStageUpBtn;
        private System.Windows.Forms.Button guiConditionUpBtn;
        private System.Windows.Forms.Button guiConditionDownBtn;
    }
}
