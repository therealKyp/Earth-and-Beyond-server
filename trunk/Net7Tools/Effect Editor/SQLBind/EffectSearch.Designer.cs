namespace SQLBind
{
    partial class EffectSearch
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
            this.EffectList = new System.Windows.Forms.DataGridView();
            this.OkBt = new System.Windows.Forms.Button();
            this.CancelBt = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.EffectList)).BeginInit();
            this.SuspendLayout();
            // 
            // EffectList
            // 
            this.EffectList.AllowUserToAddRows = false;
            this.EffectList.AllowUserToDeleteRows = false;
            this.EffectList.AllowUserToOrderColumns = true;
            this.EffectList.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.EffectList.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.EffectList.EditMode = System.Windows.Forms.DataGridViewEditMode.EditProgrammatically;
            this.EffectList.Location = new System.Drawing.Point(160, 11);
            this.EffectList.MultiSelect = false;
            this.EffectList.Name = "EffectList";
            this.EffectList.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.EffectList.Size = new System.Drawing.Size(439, 254);
            this.EffectList.TabIndex = 0;
            this.EffectList.CellMouseClick += new System.Windows.Forms.DataGridViewCellMouseEventHandler(this.EffectList_CellMouseClick);
            // 
            // OkBt
            // 
            this.OkBt.Location = new System.Drawing.Point(12, 181);
            this.OkBt.Name = "OkBt";
            this.OkBt.Size = new System.Drawing.Size(101, 39);
            this.OkBt.TabIndex = 1;
            this.OkBt.Text = "Ok";
            this.OkBt.UseVisualStyleBackColor = true;
            this.OkBt.Click += new System.EventHandler(this.OkBt_Click);
            // 
            // CancelBt
            // 
            this.CancelBt.Location = new System.Drawing.Point(12, 226);
            this.CancelBt.Name = "CancelBt";
            this.CancelBt.Size = new System.Drawing.Size(101, 39);
            this.CancelBt.TabIndex = 2;
            this.CancelBt.Text = "Cancel";
            this.CancelBt.UseVisualStyleBackColor = true;
            this.CancelBt.Click += new System.EventHandler(this.CancelBt_Click);
            // 
            // EffectSearch
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(611, 277);
            this.Controls.Add(this.CancelBt);
            this.Controls.Add(this.OkBt);
            this.Controls.Add(this.EffectList);
            this.Name = "EffectSearch";
            this.Text = "EffectSearch";
            this.Load += new System.EventHandler(this.EffectSearch_Load);
            ((System.ComponentModel.ISupportInitialize)(this.EffectList)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.DataGridView EffectList;
        private System.Windows.Forms.Button OkBt;
        private System.Windows.Forms.Button CancelBt;
    }
}