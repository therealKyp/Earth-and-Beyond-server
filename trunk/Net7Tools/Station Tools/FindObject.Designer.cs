namespace Station_Tools
{
    partial class FindObject
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
            this.StationList = new System.Windows.Forms.DataGridView();
            this.Ok = new System.Windows.Forms.Button();
            this.Cancel = new System.Windows.Forms.Button();
            this.Column4 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Column3 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Column2 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Column1 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            ((System.ComponentModel.ISupportInitialize)(this.StationList)).BeginInit();
            this.SuspendLayout();
            // 
            // StationList
            // 
            this.StationList.AllowUserToAddRows = false;
            this.StationList.AllowUserToDeleteRows = false;
            this.StationList.AllowUserToOrderColumns = true;
            this.StationList.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.StationList.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.Column4,
            this.Column3,
            this.Column2,
            this.Column1});
            this.StationList.Location = new System.Drawing.Point(12, 12);
            this.StationList.MultiSelect = false;
            this.StationList.Name = "StationList";
            this.StationList.ReadOnly = true;
            this.StationList.Size = new System.Drawing.Size(474, 174);
            this.StationList.TabIndex = 0;
            // 
            // Ok
            // 
            this.Ok.Location = new System.Drawing.Point(12, 203);
            this.Ok.Name = "Ok";
            this.Ok.Size = new System.Drawing.Size(90, 31);
            this.Ok.TabIndex = 1;
            this.Ok.Text = "Ok";
            this.Ok.UseVisualStyleBackColor = true;
            this.Ok.Click += new System.EventHandler(this.Ok_Click);
            // 
            // Cancel
            // 
            this.Cancel.Location = new System.Drawing.Point(396, 203);
            this.Cancel.Name = "Cancel";
            this.Cancel.Size = new System.Drawing.Size(90, 31);
            this.Cancel.TabIndex = 2;
            this.Cancel.Text = "Cancel";
            this.Cancel.UseVisualStyleBackColor = true;
            this.Cancel.Click += new System.EventHandler(this.Cancel_Click);
            // 
            // Column4
            // 
            this.Column4.FillWeight = 300F;
            this.Column4.HeaderText = "SectorID";
            this.Column4.Name = "Column4";
            this.Column4.Visible = false;
            // 
            // Column3
            // 
            this.Column3.HeaderText = "StationID";
            this.Column3.Name = "Column3";
            this.Column3.Visible = false;
            // 
            // Column2
            // 
            this.Column2.HeaderText = "SectorName";
            this.Column2.Name = "Column2";
            this.Column2.Width = 150;
            // 
            // Column1
            // 
            this.Column1.HeaderText = "Station Name";
            this.Column1.Name = "Column1";
            this.Column1.Width = 250;
            // 
            // FindObject
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(498, 244);
            this.Controls.Add(this.Cancel);
            this.Controls.Add(this.Ok);
            this.Controls.Add(this.StationList);
            this.Name = "FindObject";
            this.Text = "FindObject";
            this.Load += new System.EventHandler(this.FindObject_Load);
            ((System.ComponentModel.ISupportInitialize)(this.StationList)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.DataGridView StationList;
        private System.Windows.Forms.Button Ok;
        private System.Windows.Forms.Button Cancel;
        private System.Windows.Forms.DataGridViewTextBoxColumn Column4;
        private System.Windows.Forms.DataGridViewTextBoxColumn Column3;
        private System.Windows.Forms.DataGridViewTextBoxColumn Column2;
        private System.Windows.Forms.DataGridViewTextBoxColumn Column1;
    }
}