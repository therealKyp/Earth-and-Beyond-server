namespace Station_Tools
{
    partial class VenderTab
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
            this.Vender_NewItem = new System.Windows.Forms.Button();
            this.Vender_DelItem = new System.Windows.Forms.Button();
            this.Vender_EditItem = new System.Windows.Forms.Button();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.VBP = new System.Windows.Forms.CheckBox();
            this.VSP = new System.Windows.Forms.CheckBox();
            this.DNSell = new System.Windows.Forms.CheckBox();
            this.DNBuy = new System.Windows.Forms.CheckBox();
            this.IBBuy = new System.Windows.Forms.CheckBox();
            this.IBSell = new System.Windows.Forms.CheckBox();
            this.Vender_SaveItem = new System.Windows.Forms.Button();
            this.Vender_BrowseItems = new System.Windows.Forms.Button();
            this.Vender_QuanityUnlimited = new System.Windows.Forms.CheckBox();
            this.Vender_Quanity = new System.Windows.Forms.TextBox();
            this.label33 = new System.Windows.Forms.Label();
            this.Vender_BuyPrice = new System.Windows.Forms.TextBox();
            this.label32 = new System.Windows.Forms.Label();
            this.Vender_SellPrice = new System.Windows.Forms.TextBox();
            this.label31 = new System.Windows.Forms.Label();
            this.Vender_NewItemID = new System.Windows.Forms.TextBox();
            this.label30 = new System.Windows.Forms.Label();
            this.Vender_ItemLists = new System.Windows.Forms.DataGridView();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.Vender_BuyList = new System.Windows.Forms.CheckBox();
            this.Vender_BuyMult = new System.Windows.Forms.TextBox();
            this.label29 = new System.Windows.Forms.Label();
            this.Vender_SellMult = new System.Windows.Forms.TextBox();
            this.label28 = new System.Windows.Forms.Label();
            this.Vender_UpdateName = new System.Windows.Forms.Button();
            this.Vender_AddGroup = new System.Windows.Forms.Button();
            this.Vender_GroupName = new System.Windows.Forms.TextBox();
            this.label27 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.Vender_ReloadGroup = new System.Windows.Forms.Button();
            this.Vender_LoadGroup = new System.Windows.Forms.Button();
            this.Vender_DeleteGroup = new System.Windows.Forms.Button();
            this.label26 = new System.Windows.Forms.Label();
            this.Vender_SelectGroup = new System.Windows.Forms.ComboBox();
            this.MultAdd = new System.Windows.Forms.Button();
            this.IDNum = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.ItemName = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.ItemID = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.SellCost = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.BuyPrice = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Quanity = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.groupBox5.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.Vender_ItemLists)).BeginInit();
            this.groupBox3.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // Vender_NewItem
            // 
            this.Vender_NewItem.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.Vender_NewItem.Location = new System.Drawing.Point(714, 455);
            this.Vender_NewItem.Name = "Vender_NewItem";
            this.Vender_NewItem.Size = new System.Drawing.Size(115, 44);
            this.Vender_NewItem.TabIndex = 12;
            this.Vender_NewItem.Text = "New Item";
            this.Vender_NewItem.UseVisualStyleBackColor = true;
            this.Vender_NewItem.Click += new System.EventHandler(this.Vender_NewItem_Click);
            // 
            // Vender_DelItem
            // 
            this.Vender_DelItem.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.Vender_DelItem.Location = new System.Drawing.Point(593, 455);
            this.Vender_DelItem.Name = "Vender_DelItem";
            this.Vender_DelItem.Size = new System.Drawing.Size(115, 44);
            this.Vender_DelItem.TabIndex = 11;
            this.Vender_DelItem.Text = "Delete Item";
            this.Vender_DelItem.UseVisualStyleBackColor = true;
            this.Vender_DelItem.Click += new System.EventHandler(this.Vender_DelItem_Click);
            // 
            // Vender_EditItem
            // 
            this.Vender_EditItem.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.Vender_EditItem.Location = new System.Drawing.Point(472, 455);
            this.Vender_EditItem.Name = "Vender_EditItem";
            this.Vender_EditItem.Size = new System.Drawing.Size(115, 44);
            this.Vender_EditItem.TabIndex = 10;
            this.Vender_EditItem.Text = "Edit Item";
            this.Vender_EditItem.UseVisualStyleBackColor = true;
            this.Vender_EditItem.Click += new System.EventHandler(this.Vender_EditItem_Click);
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.VBP);
            this.groupBox5.Controls.Add(this.VSP);
            this.groupBox5.Controls.Add(this.DNSell);
            this.groupBox5.Controls.Add(this.DNBuy);
            this.groupBox5.Controls.Add(this.IBBuy);
            this.groupBox5.Controls.Add(this.IBSell);
            this.groupBox5.Controls.Add(this.Vender_SaveItem);
            this.groupBox5.Controls.Add(this.Vender_BrowseItems);
            this.groupBox5.Controls.Add(this.Vender_QuanityUnlimited);
            this.groupBox5.Controls.Add(this.Vender_Quanity);
            this.groupBox5.Controls.Add(this.label33);
            this.groupBox5.Controls.Add(this.Vender_BuyPrice);
            this.groupBox5.Controls.Add(this.label32);
            this.groupBox5.Controls.Add(this.Vender_SellPrice);
            this.groupBox5.Controls.Add(this.label31);
            this.groupBox5.Controls.Add(this.Vender_NewItemID);
            this.groupBox5.Controls.Add(this.label30);
            this.groupBox5.Location = new System.Drawing.Point(18, 284);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(448, 215);
            this.groupBox5.TabIndex = 9;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Items";
            // 
            // VBP
            // 
            this.VBP.AutoSize = true;
            this.VBP.Checked = true;
            this.VBP.CheckState = System.Windows.Forms.CheckState.Checked;
            this.VBP.Location = new System.Drawing.Point(87, 74);
            this.VBP.Name = "VBP";
            this.VBP.Size = new System.Drawing.Size(15, 14);
            this.VBP.TabIndex = 22;
            this.VBP.UseVisualStyleBackColor = true;
            this.VBP.CheckedChanged += new System.EventHandler(this.VBP_CheckedChanged);
            // 
            // VSP
            // 
            this.VSP.AutoSize = true;
            this.VSP.Checked = true;
            this.VSP.CheckState = System.Windows.Forms.CheckState.Checked;
            this.VSP.Location = new System.Drawing.Point(87, 48);
            this.VSP.Name = "VSP";
            this.VSP.Size = new System.Drawing.Size(15, 14);
            this.VSP.TabIndex = 21;
            this.VSP.UseVisualStyleBackColor = true;
            this.VSP.CheckedChanged += new System.EventHandler(this.VSP_CheckedChanged);
            // 
            // DNSell
            // 
            this.DNSell.AutoSize = true;
            this.DNSell.Location = new System.Drawing.Point(356, 48);
            this.DNSell.Name = "DNSell";
            this.DNSell.Size = new System.Drawing.Size(80, 17);
            this.DNSell.TabIndex = 20;
            this.DNSell.Text = "Do Not Sell";
            this.DNSell.UseVisualStyleBackColor = true;
            this.DNSell.CheckedChanged += new System.EventHandler(this.DNSell_CheckedChanged);
            // 
            // DNBuy
            // 
            this.DNBuy.AutoSize = true;
            this.DNBuy.Location = new System.Drawing.Point(356, 74);
            this.DNBuy.Name = "DNBuy";
            this.DNBuy.Size = new System.Drawing.Size(81, 17);
            this.DNBuy.TabIndex = 19;
            this.DNBuy.Text = "Do Not Buy";
            this.DNBuy.UseVisualStyleBackColor = true;
            this.DNBuy.CheckedChanged += new System.EventHandler(this.DNBuy_CheckedChanged);
            // 
            // IBBuy
            // 
            this.IBBuy.AutoSize = true;
            this.IBBuy.Location = new System.Drawing.Point(253, 73);
            this.IBBuy.Name = "IBBuy";
            this.IBBuy.Size = new System.Drawing.Size(97, 17);
            this.IBBuy.TabIndex = 18;
            this.IBBuy.Text = "ItemBase Price";
            this.IBBuy.UseVisualStyleBackColor = true;
            this.IBBuy.CheckedChanged += new System.EventHandler(this.IBBuy_CheckedChanged);
            // 
            // IBSell
            // 
            this.IBSell.AutoSize = true;
            this.IBSell.Location = new System.Drawing.Point(253, 46);
            this.IBSell.Name = "IBSell";
            this.IBSell.Size = new System.Drawing.Size(97, 17);
            this.IBSell.TabIndex = 17;
            this.IBSell.Text = "ItemBase Price";
            this.IBSell.UseVisualStyleBackColor = true;
            this.IBSell.CheckedChanged += new System.EventHandler(this.IBSell_CheckedChanged);
            // 
            // Vender_SaveItem
            // 
            this.Vender_SaveItem.Location = new System.Drawing.Point(173, 154);
            this.Vender_SaveItem.Name = "Vender_SaveItem";
            this.Vender_SaveItem.Size = new System.Drawing.Size(134, 38);
            this.Vender_SaveItem.TabIndex = 16;
            this.Vender_SaveItem.Text = "Save/Update";
            this.Vender_SaveItem.UseVisualStyleBackColor = true;
            this.Vender_SaveItem.Click += new System.EventHandler(this.Vender_SaveItem_Click);
            // 
            // Vender_BrowseItems
            // 
            this.Vender_BrowseItems.Location = new System.Drawing.Point(184, 19);
            this.Vender_BrowseItems.Name = "Vender_BrowseItems";
            this.Vender_BrowseItems.Size = new System.Drawing.Size(75, 20);
            this.Vender_BrowseItems.TabIndex = 11;
            this.Vender_BrowseItems.Text = "Browse...";
            this.Vender_BrowseItems.UseVisualStyleBackColor = true;
            this.Vender_BrowseItems.Click += new System.EventHandler(this.Vender_BrowseItems_Click);
            // 
            // Vender_QuanityUnlimited
            // 
            this.Vender_QuanityUnlimited.AutoSize = true;
            this.Vender_QuanityUnlimited.Location = new System.Drawing.Point(139, 99);
            this.Vender_QuanityUnlimited.Name = "Vender_QuanityUnlimited";
            this.Vender_QuanityUnlimited.Size = new System.Drawing.Size(67, 17);
            this.Vender_QuanityUnlimited.TabIndex = 8;
            this.Vender_QuanityUnlimited.Text = "Unlmited";
            this.Vender_QuanityUnlimited.UseVisualStyleBackColor = true;
            this.Vender_QuanityUnlimited.CheckedChanged += new System.EventHandler(this.Vender_QuanityUnlimited_CheckedChanged);
            // 
            // Vender_Quanity
            // 
            this.Vender_Quanity.Location = new System.Drawing.Point(87, 97);
            this.Vender_Quanity.Name = "Vender_Quanity";
            this.Vender_Quanity.Size = new System.Drawing.Size(46, 20);
            this.Vender_Quanity.TabIndex = 7;
            this.Vender_Quanity.TextChanged += new System.EventHandler(this.Vender_ItemUpdate);
            // 
            // label33
            // 
            this.label33.AutoSize = true;
            this.label33.Location = new System.Drawing.Point(35, 100);
            this.label33.Name = "label33";
            this.label33.Size = new System.Drawing.Size(46, 13);
            this.label33.TabIndex = 6;
            this.label33.Text = "Quanity:";
            // 
            // Vender_BuyPrice
            // 
            this.Vender_BuyPrice.Location = new System.Drawing.Point(108, 71);
            this.Vender_BuyPrice.Name = "Vender_BuyPrice";
            this.Vender_BuyPrice.Size = new System.Drawing.Size(131, 20);
            this.Vender_BuyPrice.TabIndex = 5;
            this.Vender_BuyPrice.TextChanged += new System.EventHandler(this.Vender_ItemUpdate);
            // 
            // label32
            // 
            this.label32.AutoSize = true;
            this.label32.Location = new System.Drawing.Point(26, 74);
            this.label32.Name = "label32";
            this.label32.Size = new System.Drawing.Size(55, 13);
            this.label32.TabIndex = 4;
            this.label32.Text = "Buy Price:";
            // 
            // Vender_SellPrice
            // 
            this.Vender_SellPrice.Location = new System.Drawing.Point(108, 45);
            this.Vender_SellPrice.Name = "Vender_SellPrice";
            this.Vender_SellPrice.Size = new System.Drawing.Size(131, 20);
            this.Vender_SellPrice.TabIndex = 3;
            this.Vender_SellPrice.TextChanged += new System.EventHandler(this.Vender_ItemUpdate);
            // 
            // label31
            // 
            this.label31.AutoSize = true;
            this.label31.Location = new System.Drawing.Point(27, 48);
            this.label31.Name = "label31";
            this.label31.Size = new System.Drawing.Size(54, 13);
            this.label31.TabIndex = 2;
            this.label31.Text = "Sell Price:";
            // 
            // Vender_NewItemID
            // 
            this.Vender_NewItemID.Location = new System.Drawing.Point(87, 19);
            this.Vender_NewItemID.Name = "Vender_NewItemID";
            this.Vender_NewItemID.Size = new System.Drawing.Size(88, 20);
            this.Vender_NewItemID.TabIndex = 1;
            this.Vender_NewItemID.TextChanged += new System.EventHandler(this.Vender_ItemUpdate);
            // 
            // label30
            // 
            this.label30.AutoSize = true;
            this.label30.Location = new System.Drawing.Point(40, 22);
            this.label30.Name = "label30";
            this.label30.Size = new System.Drawing.Size(41, 13);
            this.label30.TabIndex = 0;
            this.label30.Text = "ItemID:";
            // 
            // Vender_ItemLists
            // 
            this.Vender_ItemLists.AllowUserToAddRows = false;
            this.Vender_ItemLists.AllowUserToDeleteRows = false;
            this.Vender_ItemLists.AllowUserToOrderColumns = true;
            this.Vender_ItemLists.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.Vender_ItemLists.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.Vender_ItemLists.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.IDNum,
            this.ItemName,
            this.ItemID,
            this.SellCost,
            this.BuyPrice,
            this.Quanity});
            this.Vender_ItemLists.EditMode = System.Windows.Forms.DataGridViewEditMode.EditProgrammatically;
            this.Vender_ItemLists.Location = new System.Drawing.Point(472, 12);
            this.Vender_ItemLists.Name = "Vender_ItemLists";
            this.Vender_ItemLists.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.Vender_ItemLists.Size = new System.Drawing.Size(659, 437);
            this.Vender_ItemLists.TabIndex = 7;
            this.Vender_ItemLists.CellDoubleClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.Vender_EditItem_Click);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.Vender_BuyList);
            this.groupBox3.Controls.Add(this.Vender_BuyMult);
            this.groupBox3.Controls.Add(this.label29);
            this.groupBox3.Controls.Add(this.Vender_SellMult);
            this.groupBox3.Controls.Add(this.label28);
            this.groupBox3.Controls.Add(this.Vender_UpdateName);
            this.groupBox3.Controls.Add(this.Vender_AddGroup);
            this.groupBox3.Controls.Add(this.Vender_GroupName);
            this.groupBox3.Controls.Add(this.label27);
            this.groupBox3.Location = new System.Drawing.Point(18, 130);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(448, 148);
            this.groupBox3.TabIndex = 6;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Group";
            // 
            // Vender_BuyList
            // 
            this.Vender_BuyList.AutoSize = true;
            this.Vender_BuyList.Location = new System.Drawing.Point(182, 78);
            this.Vender_BuyList.Name = "Vender_BuyList";
            this.Vender_BuyList.Size = new System.Drawing.Size(87, 17);
            this.Vender_BuyList.TabIndex = 11;
            this.Vender_BuyList.Text = "Only Buy List";
            this.Vender_BuyList.UseVisualStyleBackColor = true;
            // 
            // Vender_BuyMult
            // 
            this.Vender_BuyMult.Location = new System.Drawing.Point(90, 76);
            this.Vender_BuyMult.Name = "Vender_BuyMult";
            this.Vender_BuyMult.Size = new System.Drawing.Size(88, 20);
            this.Vender_BuyMult.TabIndex = 10;
            // 
            // label29
            // 
            this.label29.AutoSize = true;
            this.label29.Location = new System.Drawing.Point(9, 79);
            this.label29.Name = "label29";
            this.label29.Size = new System.Drawing.Size(75, 13);
            this.label29.TabIndex = 9;
            this.label29.Text = "Buy Multiplyer:";
            // 
            // Vender_SellMult
            // 
            this.Vender_SellMult.Location = new System.Drawing.Point(90, 50);
            this.Vender_SellMult.Name = "Vender_SellMult";
            this.Vender_SellMult.Size = new System.Drawing.Size(88, 20);
            this.Vender_SellMult.TabIndex = 8;
            // 
            // label28
            // 
            this.label28.AutoSize = true;
            this.label28.Location = new System.Drawing.Point(10, 53);
            this.label28.Name = "label28";
            this.label28.Size = new System.Drawing.Size(74, 13);
            this.label28.TabIndex = 7;
            this.label28.Text = "Sell Multiplyer:";
            // 
            // Vender_UpdateName
            // 
            this.Vender_UpdateName.Location = new System.Drawing.Point(338, 51);
            this.Vender_UpdateName.Name = "Vender_UpdateName";
            this.Vender_UpdateName.Size = new System.Drawing.Size(96, 23);
            this.Vender_UpdateName.TabIndex = 6;
            this.Vender_UpdateName.Text = "Update Group";
            this.Vender_UpdateName.UseVisualStyleBackColor = true;
            this.Vender_UpdateName.Click += new System.EventHandler(this.Vender_UpdateName_Click);
            // 
            // Vender_AddGroup
            // 
            this.Vender_AddGroup.Location = new System.Drawing.Point(338, 22);
            this.Vender_AddGroup.Name = "Vender_AddGroup";
            this.Vender_AddGroup.Size = new System.Drawing.Size(96, 23);
            this.Vender_AddGroup.TabIndex = 5;
            this.Vender_AddGroup.Text = "Create Group";
            this.Vender_AddGroup.UseVisualStyleBackColor = true;
            this.Vender_AddGroup.Click += new System.EventHandler(this.Vender_AddGroup_Click);
            // 
            // Vender_GroupName
            // 
            this.Vender_GroupName.Location = new System.Drawing.Point(90, 24);
            this.Vender_GroupName.Name = "Vender_GroupName";
            this.Vender_GroupName.Size = new System.Drawing.Size(242, 20);
            this.Vender_GroupName.TabIndex = 1;
            // 
            // label27
            // 
            this.label27.AutoSize = true;
            this.label27.Location = new System.Drawing.Point(14, 27);
            this.label27.Name = "label27";
            this.label27.Size = new System.Drawing.Size(70, 13);
            this.label27.TabIndex = 0;
            this.label27.Text = "Group Name:";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.Vender_ReloadGroup);
            this.groupBox2.Controls.Add(this.Vender_LoadGroup);
            this.groupBox2.Controls.Add(this.Vender_DeleteGroup);
            this.groupBox2.Controls.Add(this.label26);
            this.groupBox2.Controls.Add(this.Vender_SelectGroup);
            this.groupBox2.Location = new System.Drawing.Point(18, 12);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(448, 112);
            this.groupBox2.TabIndex = 5;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Vender Groups";
            // 
            // Vender_ReloadGroup
            // 
            this.Vender_ReloadGroup.Location = new System.Drawing.Point(338, 77);
            this.Vender_ReloadGroup.Name = "Vender_ReloadGroup";
            this.Vender_ReloadGroup.Size = new System.Drawing.Size(96, 24);
            this.Vender_ReloadGroup.TabIndex = 4;
            this.Vender_ReloadGroup.Text = "Reload Groups";
            this.Vender_ReloadGroup.UseVisualStyleBackColor = true;
            this.Vender_ReloadGroup.Click += new System.EventHandler(this.Vender_ReloadGroup_Click);
            // 
            // Vender_LoadGroup
            // 
            this.Vender_LoadGroup.Location = new System.Drawing.Point(338, 17);
            this.Vender_LoadGroup.Name = "Vender_LoadGroup";
            this.Vender_LoadGroup.Size = new System.Drawing.Size(96, 24);
            this.Vender_LoadGroup.TabIndex = 3;
            this.Vender_LoadGroup.Text = "Load Group";
            this.Vender_LoadGroup.UseVisualStyleBackColor = true;
            this.Vender_LoadGroup.Click += new System.EventHandler(this.Vender_LoadGroup_Click);
            // 
            // Vender_DeleteGroup
            // 
            this.Vender_DeleteGroup.Location = new System.Drawing.Point(338, 47);
            this.Vender_DeleteGroup.Name = "Vender_DeleteGroup";
            this.Vender_DeleteGroup.Size = new System.Drawing.Size(96, 24);
            this.Vender_DeleteGroup.TabIndex = 2;
            this.Vender_DeleteGroup.Text = "Delete Group";
            this.Vender_DeleteGroup.UseVisualStyleBackColor = true;
            this.Vender_DeleteGroup.Click += new System.EventHandler(this.Vender_DeleteGroup_Click);
            // 
            // label26
            // 
            this.label26.AutoSize = true;
            this.label26.Location = new System.Drawing.Point(8, 23);
            this.label26.Name = "label26";
            this.label26.Size = new System.Drawing.Size(76, 13);
            this.label26.TabIndex = 1;
            this.label26.Text = "Vender Group:";
            // 
            // Vender_SelectGroup
            // 
            this.Vender_SelectGroup.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.Vender_SelectGroup.FormattingEnabled = true;
            this.Vender_SelectGroup.Location = new System.Drawing.Point(90, 20);
            this.Vender_SelectGroup.Name = "Vender_SelectGroup";
            this.Vender_SelectGroup.Size = new System.Drawing.Size(242, 21);
            this.Vender_SelectGroup.TabIndex = 0;
            // 
            // MultAdd
            // 
            this.MultAdd.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.MultAdd.Location = new System.Drawing.Point(835, 455);
            this.MultAdd.Name = "MultAdd";
            this.MultAdd.Size = new System.Drawing.Size(115, 44);
            this.MultAdd.TabIndex = 13;
            this.MultAdd.Text = "Multi-Item Add";
            this.MultAdd.UseVisualStyleBackColor = true;
            this.MultAdd.Click += new System.EventHandler(this.MultAdd_Click);
            // 
            // IDNum
            // 
            this.IDNum.DataPropertyName = "ID";
            this.IDNum.HeaderText = "ID";
            this.IDNum.Name = "IDNum";
            this.IDNum.Visible = false;
            this.IDNum.Width = 50;
            // 
            // ItemName
            // 
            this.ItemName.DataPropertyName = "ItemName";
            this.ItemName.HeaderText = "Item Name";
            this.ItemName.Name = "ItemName";
            this.ItemName.ToolTipText = "Item Name";
            this.ItemName.Width = 200;
            // 
            // ItemID
            // 
            this.ItemID.DataPropertyName = "ItemID";
            this.ItemID.HeaderText = "Item ID";
            this.ItemID.Name = "ItemID";
            this.ItemID.ToolTipText = "Item ID Number";
            // 
            // SellCost
            // 
            this.SellCost.DataPropertyName = "SellPrice";
            this.SellCost.HeaderText = "Sell Price";
            this.SellCost.Name = "SellCost";
            this.SellCost.ToolTipText = "Sell Price of Item";
            // 
            // BuyPrice
            // 
            this.BuyPrice.DataPropertyName = "BuyPrice";
            this.BuyPrice.HeaderText = "Buy Price";
            this.BuyPrice.Name = "BuyPrice";
            this.BuyPrice.ToolTipText = "Buy Price of Item";
            // 
            // Quanity
            // 
            this.Quanity.DataPropertyName = "Qty";
            this.Quanity.HeaderText = "Qty";
            this.Quanity.Name = "Quanity";
            this.Quanity.ToolTipText = "Quanity to Sell per Day";
            // 
            // VenderTab
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.MultAdd);
            this.Controls.Add(this.Vender_NewItem);
            this.Controls.Add(this.Vender_DelItem);
            this.Controls.Add(this.Vender_EditItem);
            this.Controls.Add(this.groupBox5);
            this.Controls.Add(this.Vender_ItemLists);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Name = "VenderTab";
            this.Size = new System.Drawing.Size(1146, 619);
            this.Load += new System.EventHandler(this.VenderTab_Load);
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.Vender_ItemLists)).EndInit();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button Vender_NewItem;
        private System.Windows.Forms.Button Vender_DelItem;
        private System.Windows.Forms.Button Vender_EditItem;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.CheckBox VBP;
        private System.Windows.Forms.CheckBox VSP;
        private System.Windows.Forms.CheckBox DNSell;
        private System.Windows.Forms.CheckBox DNBuy;
        private System.Windows.Forms.CheckBox IBBuy;
        private System.Windows.Forms.CheckBox IBSell;
        private System.Windows.Forms.Button Vender_SaveItem;
        private System.Windows.Forms.Button Vender_BrowseItems;
        private System.Windows.Forms.CheckBox Vender_QuanityUnlimited;
        private System.Windows.Forms.TextBox Vender_Quanity;
        private System.Windows.Forms.Label label33;
        private System.Windows.Forms.TextBox Vender_BuyPrice;
        private System.Windows.Forms.Label label32;
        private System.Windows.Forms.TextBox Vender_SellPrice;
        private System.Windows.Forms.Label label31;
        private System.Windows.Forms.TextBox Vender_NewItemID;
        private System.Windows.Forms.Label label30;
        private System.Windows.Forms.DataGridView Vender_ItemLists;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.CheckBox Vender_BuyList;
        private System.Windows.Forms.TextBox Vender_BuyMult;
        private System.Windows.Forms.Label label29;
        private System.Windows.Forms.TextBox Vender_SellMult;
        private System.Windows.Forms.Label label28;
        private System.Windows.Forms.Button Vender_UpdateName;
        private System.Windows.Forms.Button Vender_AddGroup;
        private System.Windows.Forms.TextBox Vender_GroupName;
        private System.Windows.Forms.Label label27;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button Vender_ReloadGroup;
        private System.Windows.Forms.Button Vender_LoadGroup;
        private System.Windows.Forms.Button Vender_DeleteGroup;
        private System.Windows.Forms.Label label26;
        private System.Windows.Forms.ComboBox Vender_SelectGroup;
        private System.Windows.Forms.Button MultAdd;
        private System.Windows.Forms.DataGridViewTextBoxColumn IDNum;
        private System.Windows.Forms.DataGridViewTextBoxColumn ItemName;
        private System.Windows.Forms.DataGridViewTextBoxColumn ItemID;
        private System.Windows.Forms.DataGridViewTextBoxColumn SellCost;
        private System.Windows.Forms.DataGridViewTextBoxColumn BuyPrice;
        private System.Windows.Forms.DataGridViewTextBoxColumn Quanity;
    }
}
