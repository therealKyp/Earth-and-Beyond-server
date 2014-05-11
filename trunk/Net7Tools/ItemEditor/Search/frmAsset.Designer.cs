namespace Net7_Tools.Search
{
    partial class FrmAsset
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FrmAsset));
            this.assetCategoryLbl = new System.Windows.Forms.Label();
            this.assetCategoryCbo = new System.Windows.Forms.ComboBox();
            this.assetSubCategoryCbo = new System.Windows.Forms.ComboBox();
            this.assetSubCategoryLbl = new System.Windows.Forms.Label();
            this.assetItems = new System.Windows.Forms.ListView();
            this.assetImages = new System.Windows.Forms.ImageList(this.components);
            this.assetOk = new System.Windows.Forms.Button();
            this.assetCancel = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // assetCategoryLbl
            // 
            this.assetCategoryLbl.AutoSize = true;
            this.assetCategoryLbl.Location = new System.Drawing.Point(21, 15);
            this.assetCategoryLbl.Name = "assetCategoryLbl";
            this.assetCategoryLbl.Size = new System.Drawing.Size(52, 13);
            this.assetCategoryLbl.TabIndex = 1;
            this.assetCategoryLbl.Text = "Category:";
            // 
            // assetCategoryCbo
            // 
            this.assetCategoryCbo.DropDownHeight = 340;
            this.assetCategoryCbo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.assetCategoryCbo.FormattingEnabled = true;
            this.assetCategoryCbo.IntegralHeight = false;
            this.assetCategoryCbo.Location = new System.Drawing.Point(79, 12);
            this.assetCategoryCbo.Name = "assetCategoryCbo";
            this.assetCategoryCbo.Size = new System.Drawing.Size(225, 21);
            this.assetCategoryCbo.TabIndex = 2;
            // 
            // assetSubCategoryCbo
            // 
            this.assetSubCategoryCbo.DropDownHeight = 340;
            this.assetSubCategoryCbo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.assetSubCategoryCbo.FormattingEnabled = true;
            this.assetSubCategoryCbo.IntegralHeight = false;
            this.assetSubCategoryCbo.Location = new System.Drawing.Point(507, 12);
            this.assetSubCategoryCbo.Name = "assetSubCategoryCbo";
            this.assetSubCategoryCbo.Size = new System.Drawing.Size(225, 21);
            this.assetSubCategoryCbo.TabIndex = 4;
            // 
            // assetSubCategoryLbl
            // 
            this.assetSubCategoryLbl.AutoSize = true;
            this.assetSubCategoryLbl.Location = new System.Drawing.Point(427, 15);
            this.assetSubCategoryLbl.Name = "assetSubCategoryLbl";
            this.assetSubCategoryLbl.Size = new System.Drawing.Size(74, 13);
            this.assetSubCategoryLbl.TabIndex = 3;
            this.assetSubCategoryLbl.Text = "Sub-Category:";
            // 
            // assetItems
            // 
            this.assetItems.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.assetItems.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.assetItems.LargeImageList = this.assetImages;
            this.assetItems.Location = new System.Drawing.Point(24, 52);
            this.assetItems.MultiSelect = false;
            this.assetItems.Name = "assetItems";
            this.assetItems.ShowItemToolTips = true;
            this.assetItems.Size = new System.Drawing.Size(708, 329);
            this.assetItems.TabIndex = 0;
            this.assetItems.UseCompatibleStateImageBehavior = false;
            this.assetItems.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.onDoubleClick);
            // 
            // assetImages
            // 
            this.assetImages.ColorDepth = System.Windows.Forms.ColorDepth.Depth32Bit;
            this.assetImages.ImageSize = new System.Drawing.Size(128, 128);
            this.assetImages.TransparentColor = System.Drawing.Color.White;
            // 
            // assetOk
            // 
            this.assetOk.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.assetOk.Location = new System.Drawing.Point(576, 387);
            this.assetOk.Name = "assetOk";
            this.assetOk.Size = new System.Drawing.Size(75, 23);
            this.assetOk.TabIndex = 5;
            this.assetOk.Text = "Ok";
            this.assetOk.UseVisualStyleBackColor = true;
            this.assetOk.Click += new System.EventHandler(this.onOk);
            // 
            // assetCancel
            // 
            this.assetCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.assetCancel.Location = new System.Drawing.Point(657, 387);
            this.assetCancel.Name = "assetCancel";
            this.assetCancel.Size = new System.Drawing.Size(75, 23);
            this.assetCancel.TabIndex = 6;
            this.assetCancel.Text = "Cancel";
            this.assetCancel.UseVisualStyleBackColor = true;
            this.assetCancel.Click += new System.EventHandler(this.onCancel);
            // 
            // frmAsset
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(744, 414);
            this.Controls.Add(this.assetCancel);
            this.Controls.Add(this.assetOk);
            this.Controls.Add(this.assetItems);
            this.Controls.Add(this.assetSubCategoryCbo);
            this.Controls.Add(this.assetSubCategoryLbl);
            this.Controls.Add(this.assetCategoryCbo);
            this.Controls.Add(this.assetCategoryLbl);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "frmAsset";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "frmAsset";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label assetCategoryLbl;
        private System.Windows.Forms.ComboBox assetCategoryCbo;
        private System.Windows.Forms.ComboBox assetSubCategoryCbo;
        private System.Windows.Forms.Label assetSubCategoryLbl;
        private System.Windows.Forms.ListView assetItems;
        private System.Windows.Forms.ImageList assetImages;
        private System.Windows.Forms.Button assetOk;
        private System.Windows.Forms.Button assetCancel;
    }
}