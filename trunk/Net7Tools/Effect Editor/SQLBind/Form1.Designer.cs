namespace SQLBind
{
    partial class Form1
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
            this.EffectType = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.EffectName = new System.Windows.Forms.TextBox();
            this.EffectDesc = new System.Windows.Forms.TextBox();
            this.EffectToolTip = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.TargetTypeGrp = new System.Windows.Forms.GroupBox();
            this.VisualEffect = new System.Windows.Forms.TextBox();
            this.label18 = new System.Windows.Forms.Label();
            this.TSelf = new System.Windows.Forms.RadioButton();
            this.EffectBuff = new System.Windows.Forms.ComboBox();
            this.label12 = new System.Windows.Forms.Label();
            this.RequireT = new System.Windows.Forms.CheckBox();
            this.TGroupM = new System.Windows.Forms.RadioButton();
            this.TEnemy = new System.Windows.Forms.RadioButton();
            this.TFriend = new System.Windows.Forms.RadioButton();
            this.button1 = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.VarType3 = new System.Windows.Forms.ComboBox();
            this.VarStat3 = new System.Windows.Forms.ComboBox();
            this.label7 = new System.Windows.Forms.Label();
            this.VarType2 = new System.Windows.Forms.ComboBox();
            this.VarStat2 = new System.Windows.Forms.ComboBox();
            this.label6 = new System.Windows.Forms.Label();
            this.VarType1 = new System.Windows.Forms.ComboBox();
            this.VarStat1 = new System.Windows.Forms.ComboBox();
            this.label5 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label16 = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.ConstStat1 = new System.Windows.Forms.ComboBox();
            this.ConstValue2 = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.ConstType2 = new System.Windows.Forms.ComboBox();
            this.ConstType1 = new System.Windows.Forms.ComboBox();
            this.ConstStat2 = new System.Windows.Forms.ComboBox();
            this.ConstValue1 = new System.Windows.Forms.TextBox();
            this.label11 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.Save = new System.Windows.Forms.Button();
            this.label13 = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.NewEffect = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.TargetTypeGrp.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // EffectType
            // 
            this.EffectType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.EffectType.FormattingEnabled = true;
            this.EffectType.Items.AddRange(new object[] {
            "Equipable (0)",
            "Activatable (1)"});
            this.EffectType.Location = new System.Drawing.Point(85, 40);
            this.EffectType.Name = "EffectType";
            this.EffectType.Size = new System.Drawing.Size(123, 21);
            this.EffectType.TabIndex = 0;
            this.EffectType.SelectedIndexChanged += new System.EventHandler(this.EffectType_SelectedIndexChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(14, 43);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(65, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Effect Type:";
            // 
            // EffectName
            // 
            this.EffectName.Location = new System.Drawing.Point(85, 67);
            this.EffectName.Name = "EffectName";
            this.EffectName.Size = new System.Drawing.Size(185, 20);
            this.EffectName.TabIndex = 2;
            // 
            // EffectDesc
            // 
            this.EffectDesc.Location = new System.Drawing.Point(85, 93);
            this.EffectDesc.Name = "EffectDesc";
            this.EffectDesc.Size = new System.Drawing.Size(185, 20);
            this.EffectDesc.TabIndex = 3;
            // 
            // EffectToolTip
            // 
            this.EffectToolTip.Location = new System.Drawing.Point(85, 119);
            this.EffectToolTip.Name = "EffectToolTip";
            this.EffectToolTip.Size = new System.Drawing.Size(573, 20);
            this.EffectToolTip.TabIndex = 4;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(41, 70);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(38, 13);
            this.label2.TabIndex = 5;
            this.label2.Text = "Name:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(16, 96);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(63, 13);
            this.label3.TabIndex = 6;
            this.label3.Text = "Description:";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(37, 122);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(42, 13);
            this.label4.TabIndex = 7;
            this.label4.Text = "Tooltip:";
            // 
            // TargetTypeGrp
            // 
            this.TargetTypeGrp.Controls.Add(this.VisualEffect);
            this.TargetTypeGrp.Controls.Add(this.label18);
            this.TargetTypeGrp.Controls.Add(this.TSelf);
            this.TargetTypeGrp.Controls.Add(this.EffectBuff);
            this.TargetTypeGrp.Controls.Add(this.label12);
            this.TargetTypeGrp.Controls.Add(this.RequireT);
            this.TargetTypeGrp.Controls.Add(this.TGroupM);
            this.TargetTypeGrp.Controls.Add(this.TEnemy);
            this.TargetTypeGrp.Controls.Add(this.TFriend);
            this.TargetTypeGrp.Location = new System.Drawing.Point(455, 145);
            this.TargetTypeGrp.Name = "TargetTypeGrp";
            this.TargetTypeGrp.Size = new System.Drawing.Size(273, 200);
            this.TargetTypeGrp.TabIndex = 8;
            this.TargetTypeGrp.TabStop = false;
            this.TargetTypeGrp.Text = "Activatable";
            // 
            // VisualEffect
            // 
            this.VisualEffect.Location = new System.Drawing.Point(80, 165);
            this.VisualEffect.Name = "VisualEffect";
            this.VisualEffect.Size = new System.Drawing.Size(171, 20);
            this.VisualEffect.TabIndex = 33;
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Location = new System.Drawing.Point(8, 168);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(66, 13);
            this.label18.TabIndex = 32;
            this.label18.Text = "Visual Effect";
            // 
            // TSelf
            // 
            this.TSelf.AutoSize = true;
            this.TSelf.Location = new System.Drawing.Point(20, 92);
            this.TSelf.Name = "TSelf";
            this.TSelf.Size = new System.Drawing.Size(77, 17);
            this.TSelf.TabIndex = 30;
            this.TSelf.Text = "Target Self";
            this.TSelf.UseVisualStyleBackColor = true;
            // 
            // EffectBuff
            // 
            this.EffectBuff.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.EffectBuff.FormattingEnabled = true;
            this.EffectBuff.Location = new System.Drawing.Point(80, 138);
            this.EffectBuff.Name = "EffectBuff";
            this.EffectBuff.Size = new System.Drawing.Size(171, 21);
            this.EffectBuff.TabIndex = 29;
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(45, 141);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(29, 13);
            this.label12.TabIndex = 28;
            this.label12.Text = "Buff:";
            // 
            // RequireT
            // 
            this.RequireT.AutoSize = true;
            this.RequireT.Location = new System.Drawing.Point(18, 115);
            this.RequireT.Name = "RequireT";
            this.RequireT.Size = new System.Drawing.Size(97, 17);
            this.RequireT.TabIndex = 9;
            this.RequireT.Text = "Require Target";
            this.RequireT.UseVisualStyleBackColor = true;
            // 
            // TGroupM
            // 
            this.TGroupM.AutoSize = true;
            this.TGroupM.Location = new System.Drawing.Point(20, 69);
            this.TGroupM.Name = "TGroupM";
            this.TGroupM.Size = new System.Drawing.Size(129, 17);
            this.TGroupM.TabIndex = 9;
            this.TGroupM.Text = "Target Group Member";
            this.TGroupM.UseVisualStyleBackColor = true;
            // 
            // TEnemy
            // 
            this.TEnemy.AutoSize = true;
            this.TEnemy.Location = new System.Drawing.Point(20, 46);
            this.TEnemy.Name = "TEnemy";
            this.TEnemy.Size = new System.Drawing.Size(91, 17);
            this.TEnemy.TabIndex = 9;
            this.TEnemy.TabStop = true;
            this.TEnemy.Text = "Target Enemy";
            this.TEnemy.UseVisualStyleBackColor = true;
            // 
            // TFriend
            // 
            this.TFriend.AutoSize = true;
            this.TFriend.Checked = true;
            this.TFriend.Location = new System.Drawing.Point(20, 23);
            this.TFriend.Name = "TFriend";
            this.TFriend.Size = new System.Drawing.Size(95, 17);
            this.TFriend.TabIndex = 0;
            this.TFriend.TabStop = true;
            this.TFriend.Text = "Target Friendly";
            this.TFriend.UseVisualStyleBackColor = true;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(85, 12);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(61, 22);
            this.button1.TabIndex = 23;
            this.button1.Text = "Search";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.VarType3);
            this.groupBox2.Controls.Add(this.VarStat3);
            this.groupBox2.Controls.Add(this.label7);
            this.groupBox2.Controls.Add(this.VarType2);
            this.groupBox2.Controls.Add(this.VarStat2);
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.Controls.Add(this.VarType1);
            this.groupBox2.Controls.Add(this.VarStat1);
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Location = new System.Drawing.Point(17, 145);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(422, 122);
            this.groupBox2.TabIndex = 21;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Varable Types";
            // 
            // VarType3
            // 
            this.VarType3.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.VarType3.FormattingEnabled = true;
            this.VarType3.Location = new System.Drawing.Point(295, 82);
            this.VarType3.Name = "VarType3";
            this.VarType3.Size = new System.Drawing.Size(121, 21);
            this.VarType3.TabIndex = 8;
            // 
            // VarStat3
            // 
            this.VarStat3.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.VarStat3.FormattingEnabled = true;
            this.VarStat3.Location = new System.Drawing.Point(48, 82);
            this.VarStat3.Name = "VarStat3";
            this.VarStat3.Size = new System.Drawing.Size(241, 21);
            this.VarStat3.TabIndex = 7;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(10, 85);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(32, 13);
            this.label7.TabIndex = 6;
            this.label7.Text = "Var3:";
            // 
            // VarType2
            // 
            this.VarType2.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.VarType2.FormattingEnabled = true;
            this.VarType2.Location = new System.Drawing.Point(295, 55);
            this.VarType2.Name = "VarType2";
            this.VarType2.Size = new System.Drawing.Size(121, 21);
            this.VarType2.TabIndex = 5;
            // 
            // VarStat2
            // 
            this.VarStat2.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.VarStat2.FormattingEnabled = true;
            this.VarStat2.Location = new System.Drawing.Point(48, 55);
            this.VarStat2.Name = "VarStat2";
            this.VarStat2.Size = new System.Drawing.Size(241, 21);
            this.VarStat2.TabIndex = 4;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(10, 58);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(32, 13);
            this.label6.TabIndex = 3;
            this.label6.Text = "Var2:";
            // 
            // VarType1
            // 
            this.VarType1.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.VarType1.FormattingEnabled = true;
            this.VarType1.Location = new System.Drawing.Point(295, 28);
            this.VarType1.Name = "VarType1";
            this.VarType1.Size = new System.Drawing.Size(121, 21);
            this.VarType1.TabIndex = 2;
            // 
            // VarStat1
            // 
            this.VarStat1.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.VarStat1.FormattingEnabled = true;
            this.VarStat1.Location = new System.Drawing.Point(48, 28);
            this.VarStat1.Name = "VarStat1";
            this.VarStat1.Size = new System.Drawing.Size(241, 21);
            this.VarStat1.TabIndex = 1;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(10, 31);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(32, 13);
            this.label5.TabIndex = 0;
            this.label5.Text = "Var1:";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label16);
            this.groupBox1.Controls.Add(this.label15);
            this.groupBox1.Controls.Add(this.label10);
            this.groupBox1.Controls.Add(this.ConstStat1);
            this.groupBox1.Controls.Add(this.ConstValue2);
            this.groupBox1.Controls.Add(this.label8);
            this.groupBox1.Controls.Add(this.ConstType2);
            this.groupBox1.Controls.Add(this.ConstType1);
            this.groupBox1.Controls.Add(this.ConstStat2);
            this.groupBox1.Controls.Add(this.ConstValue1);
            this.groupBox1.Controls.Add(this.label11);
            this.groupBox1.Controls.Add(this.label9);
            this.groupBox1.Location = new System.Drawing.Point(17, 279);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(422, 129);
            this.groupBox1.TabIndex = 24;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Constants";
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(304, 102);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(84, 13);
            this.label16.TabIndex = 30;
            this.label16.Text = "(No signs or %\'s)";
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(304, 49);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(84, 13);
            this.label15.TabIndex = 29;
            this.label15.Text = "(No signs or %\'s)";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(8, 102);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(37, 13);
            this.label10.TabIndex = 43;
            this.label10.Text = "Value:";
            // 
            // ConstStat1
            // 
            this.ConstStat1.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.ConstStat1.FormattingEnabled = true;
            this.ConstStat1.Location = new System.Drawing.Point(57, 19);
            this.ConstStat1.Name = "ConstStat1";
            this.ConstStat1.Size = new System.Drawing.Size(241, 21);
            this.ConstStat1.TabIndex = 35;
            // 
            // ConstValue2
            // 
            this.ConstValue2.Location = new System.Drawing.Point(57, 99);
            this.ConstValue2.Name = "ConstValue2";
            this.ConstValue2.Size = new System.Drawing.Size(241, 20);
            this.ConstValue2.TabIndex = 42;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(8, 22);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(43, 13);
            this.label8.TabIndex = 34;
            this.label8.Text = "Const1:";
            // 
            // ConstType2
            // 
            this.ConstType2.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.ConstType2.FormattingEnabled = true;
            this.ConstType2.Location = new System.Drawing.Point(304, 72);
            this.ConstType2.Name = "ConstType2";
            this.ConstType2.Size = new System.Drawing.Size(112, 21);
            this.ConstType2.TabIndex = 41;
            // 
            // ConstType1
            // 
            this.ConstType1.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.ConstType1.FormattingEnabled = true;
            this.ConstType1.Location = new System.Drawing.Point(304, 19);
            this.ConstType1.Name = "ConstType1";
            this.ConstType1.Size = new System.Drawing.Size(112, 21);
            this.ConstType1.TabIndex = 36;
            // 
            // ConstStat2
            // 
            this.ConstStat2.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.ConstStat2.FormattingEnabled = true;
            this.ConstStat2.Location = new System.Drawing.Point(57, 72);
            this.ConstStat2.Name = "ConstStat2";
            this.ConstStat2.Size = new System.Drawing.Size(241, 21);
            this.ConstStat2.TabIndex = 40;
            // 
            // ConstValue1
            // 
            this.ConstValue1.Location = new System.Drawing.Point(57, 46);
            this.ConstValue1.Name = "ConstValue1";
            this.ConstValue1.Size = new System.Drawing.Size(241, 20);
            this.ConstValue1.TabIndex = 37;
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(8, 75);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(43, 13);
            this.label11.TabIndex = 39;
            this.label11.Text = "Const2:";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(8, 49);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(37, 13);
            this.label9.TabIndex = 38;
            this.label9.Text = "Value:";
            // 
            // Save
            // 
            this.Save.Location = new System.Drawing.Point(12, 413);
            this.Save.Name = "Save";
            this.Save.Size = new System.Drawing.Size(115, 36);
            this.Save.TabIndex = 25;
            this.Save.Text = "Save/Update";
            this.Save.UseVisualStyleBackColor = true;
            this.Save.Click += new System.EventHandler(this.Save_Click);
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(276, 70);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(123, 13);
            this.label13.TabIndex = 26;
            this.label13.Text = "(Not Displayed on Client)";
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(276, 96);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(163, 13);
            this.label14.TabIndex = 27;
            this.label14.Text = "(Name that is displayed on Client)";
            // 
            // NewEffect
            // 
            this.NewEffect.Location = new System.Drawing.Point(133, 414);
            this.NewEffect.Name = "NewEffect";
            this.NewEffect.Size = new System.Drawing.Size(115, 36);
            this.NewEffect.TabIndex = 28;
            this.NewEffect.Text = "New";
            this.NewEffect.UseVisualStyleBackColor = true;
            this.NewEffect.Click += new System.EventHandler(this.NewEffect_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(553, 403);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(119, 46);
            this.button2.TabIndex = 29;
            this.button2.Text = "Edit Items";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(740, 461);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.NewEffect);
            this.Controls.Add(this.label14);
            this.Controls.Add(this.label13);
            this.Controls.Add(this.Save);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.TargetTypeGrp);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.EffectToolTip);
            this.Controls.Add(this.EffectDesc);
            this.Controls.Add(this.EffectName);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.EffectType);
            this.Name = "Form1";
            this.Text = "Effect Editor";
            this.TargetTypeGrp.ResumeLayout(false);
            this.TargetTypeGrp.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox EffectType;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox EffectName;
        private System.Windows.Forms.TextBox EffectDesc;
        private System.Windows.Forms.TextBox EffectToolTip;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.GroupBox TargetTypeGrp;
        private System.Windows.Forms.RadioButton TGroupM;
        private System.Windows.Forms.RadioButton TEnemy;
        private System.Windows.Forms.RadioButton TFriend;
        private System.Windows.Forms.CheckBox RequireT;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.ComboBox VarType3;
        private System.Windows.Forms.ComboBox VarStat3;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.ComboBox VarType2;
        private System.Windows.Forms.ComboBox VarStat2;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.ComboBox VarType1;
        private System.Windows.Forms.ComboBox VarStat1;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.ComboBox ConstStat1;
        private System.Windows.Forms.TextBox ConstValue2;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.ComboBox ConstType2;
        private System.Windows.Forms.ComboBox ConstType1;
        private System.Windows.Forms.ComboBox ConstStat2;
        private System.Windows.Forms.TextBox ConstValue1;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Button Save;
        private System.Windows.Forms.ComboBox EffectBuff;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.Button NewEffect;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.RadioButton TSelf;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.TextBox VisualEffect;

    }
}

