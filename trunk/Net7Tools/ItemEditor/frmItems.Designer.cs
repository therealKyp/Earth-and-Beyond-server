namespace Net7_Tools
{
	partial class frmItems
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
            System.Windows.Forms.Label lblItemId;
            System.Windows.Forms.Label baseManufacturerLabel;
            System.Windows.Forms.Label basePriceLabel;
            System.Windows.Forms.Label baseStackSizeLabel;
            System.Windows.Forms.Label baseLevelLabel;
            System.Windows.Forms.Label baseSubCategoryLabel;
            System.Windows.Forms.Label baseCategoryLabel;
            System.Windows.Forms.Label baseDescriptionLabel;
            System.Windows.Forms.Label baseNameLabel;
            System.Windows.Forms.Label base3dAssetLabel;
            System.Windows.Forms.Label base2DAssessLabel;
            System.Windows.Forms.Label label13;
            System.Windows.Forms.Label label40;
            System.Windows.Forms.Label label42;
            System.Windows.Forms.Label label27;
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frmItems));
            this.txtId = new System.Windows.Forms.TextBox();
            this.tabsMain = new System.Windows.Forms.TabControl();
            this.tabBase = new System.Windows.Forms.TabPage();
            this.lblType = new System.Windows.Forms.Label();
            this.txtType = new System.Windows.Forms.TextBox();
            this.grpRestictions = new System.Windows.Forms.GroupBox();
            this.lblProfRestResult = new System.Windows.Forms.Label();
            this.chkExplorerRest = new System.Windows.Forms.CheckBox();
            this.lblRaceRestResult = new System.Windows.Forms.Label();
            this.chkProjenRest = new System.Windows.Forms.CheckBox();
            this.chkWarriorRest = new System.Windows.Forms.CheckBox();
            this.chkTerranRest = new System.Windows.Forms.CheckBox();
            this.chkTraderRest = new System.Windows.Forms.CheckBox();
            this.chkJenquaiRest = new System.Windows.Forms.CheckBox();
            this.grpFlags = new System.Windows.Forms.GroupBox();
            this.chkUnique = new System.Windows.Forms.CheckBox();
            this.chkNoDestroy = new System.Windows.Forms.CheckBox();
            this.chkNoManu = new System.Windows.Forms.CheckBox();
            this.chkNoTrade = new System.Windows.Forms.CheckBox();
            this.chkNoStore = new System.Windows.Forms.CheckBox();
            this.txt2DAsset = new System.Windows.Forms.TextBox();
            this.txt3DAsset = new System.Windows.Forms.TextBox();
            this.cboManufacturer = new System.Windows.Forms.ComboBox();
            this.txtPrice = new System.Windows.Forms.TextBox();
            this.txtStackSize = new System.Windows.Forms.TextBox();
            this.cboLevel = new System.Windows.Forms.ComboBox();
            this.cboSubCategory = new System.Windows.Forms.ComboBox();
            this.txtDescription = new System.Windows.Forms.TextBox();
            this.cboCategory = new System.Windows.Forms.ComboBox();
            this.txtName = new System.Windows.Forms.TextBox();
            this.tabBeam = new System.Windows.Forms.TabPage();
            this.txtBeamReload = new System.Windows.Forms.TextBox();
            this.label22 = new System.Windows.Forms.Label();
            this.txtBeamEnergy = new System.Windows.Forms.TextBox();
            this.label20 = new System.Windows.Forms.Label();
            this.txtBeamRange = new System.Windows.Forms.TextBox();
            this.label17 = new System.Windows.Forms.Label();
            this.txtBeamDamage200 = new System.Windows.Forms.TextBox();
            this.label16 = new System.Windows.Forms.Label();
            this.txtBeamDamage100 = new System.Windows.Forms.TextBox();
            this.label15 = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.txtBeamFireEffect = new System.Windows.Forms.TextBox();
            this.cboBeamDamageType = new System.Windows.Forms.ComboBox();
            this.tabProjectile = new System.Windows.Forms.TabPage();
            this.txtProjectileAmmoName = new System.Windows.Forms.TextBox();
            this.txtProjectileEnergy = new System.Windows.Forms.TextBox();
            this.label19 = new System.Windows.Forms.Label();
            this.txtProjectileRange = new System.Windows.Forms.TextBox();
            this.label21 = new System.Windows.Forms.Label();
            this.txtProjectileReload200 = new System.Windows.Forms.TextBox();
            this.label28 = new System.Windows.Forms.Label();
            this.txtProjectileReload100 = new System.Windows.Forms.TextBox();
            this.label29 = new System.Windows.Forms.Label();
            this.label30 = new System.Windows.Forms.Label();
            this.txtProjectileRoundsFired = new System.Windows.Forms.TextBox();
            this.tabMissile = new System.Windows.Forms.TabPage();
            this.txtMissleAmmoName = new System.Windows.Forms.TextBox();
            this.txtMissleEnergy = new System.Windows.Forms.TextBox();
            this.label18 = new System.Windows.Forms.Label();
            this.txtMissleReload200 = new System.Windows.Forms.TextBox();
            this.label24 = new System.Windows.Forms.Label();
            this.txtMissleReload100 = new System.Windows.Forms.TextBox();
            this.label25 = new System.Windows.Forms.Label();
            this.label26 = new System.Windows.Forms.Label();
            this.txtMissleRoundsFired = new System.Windows.Forms.TextBox();
            this.tabAmmo = new System.Windows.Forms.TabPage();
            this.txtAmmoVisual = new System.Windows.Forms.TextBox();
            this.label47 = new System.Windows.Forms.Label();
            this.txtAmmoRange = new System.Windows.Forms.TextBox();
            this.label46 = new System.Windows.Forms.Label();
            this.cboAmmoDamageType = new System.Windows.Forms.ComboBox();
            this.txtAmmoManouv = new System.Windows.Forms.TextBox();
            this.label23 = new System.Windows.Forms.Label();
            this.txtAmmoDamage200 = new System.Windows.Forms.TextBox();
            this.label41 = new System.Windows.Forms.Label();
            this.txtAmmoDamage100 = new System.Windows.Forms.TextBox();
            this.label43 = new System.Windows.Forms.Label();
            this.label44 = new System.Windows.Forms.Label();
            this.cmdAmmoLauncherRefresh = new System.Windows.Forms.Button();
            this.cboAmmoLauncher = new System.Windows.Forms.ComboBox();
            this.label55 = new System.Windows.Forms.Label();
            this.tabDevice = new System.Windows.Forms.TabPage();
            this.label35 = new System.Windows.Forms.Label();
            this.txtDeviceCooldown = new System.Windows.Forms.TextBox();
            this.label51 = new System.Windows.Forms.Label();
            this.txtDeviceRange = new System.Windows.Forms.TextBox();
            this.label48 = new System.Windows.Forms.Label();
            this.txtDeviceEnergy200 = new System.Windows.Forms.TextBox();
            this.label49 = new System.Windows.Forms.Label();
            this.txtDeviceEnergy100 = new System.Windows.Forms.TextBox();
            this.label50 = new System.Windows.Forms.Label();
            this.tabRS = new System.Windows.Forms.TabPage();
            this.txtRSCapacity = new System.Windows.Forms.TextBox();
            this.label32 = new System.Windows.Forms.Label();
            this.txtRSRecharge200 = new System.Windows.Forms.TextBox();
            this.label33 = new System.Windows.Forms.Label();
            this.txtRSRecharge100 = new System.Windows.Forms.TextBox();
            this.label34 = new System.Windows.Forms.Label();
            this.tabEngine = new System.Windows.Forms.TabPage();
            this.txtEngineFreeWarp200 = new System.Windows.Forms.TextBox();
            this.label52 = new System.Windows.Forms.Label();
            this.txtEngineFreeWarp100 = new System.Windows.Forms.TextBox();
            this.label53 = new System.Windows.Forms.Label();
            this.txtEngineSignature200 = new System.Windows.Forms.TextBox();
            this.label31 = new System.Windows.Forms.Label();
            this.txtEngineSignature100 = new System.Windows.Forms.TextBox();
            this.label36 = new System.Windows.Forms.Label();
            this.txtEngineThrust200 = new System.Windows.Forms.TextBox();
            this.label37 = new System.Windows.Forms.Label();
            this.txtEngineWarpCharge = new System.Windows.Forms.TextBox();
            this.label38 = new System.Windows.Forms.Label();
            this.txtEngineWarpSpeed = new System.Windows.Forms.TextBox();
            this.label39 = new System.Windows.Forms.Label();
            this.label45 = new System.Windows.Forms.Label();
            this.txtEngineThrust100 = new System.Windows.Forms.TextBox();
            this.tabRefine = new System.Windows.Forms.TabPage();
            this.tabManufacture = new System.Windows.Forms.TabPage();
            this.tabEffects = new System.Windows.Forms.TabPage();
            this.tabAddReq = new System.Windows.Forms.TabPage();
            this.button1 = new System.Windows.Forms.Button();
            this.cmdEditManu = new System.Windows.Forms.Button();
            this.cmdEditEffects = new System.Windows.Forms.Button();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.menuFile = new System.Windows.Forms.ToolStripMenuItem();
            this.menuExit = new System.Windows.Forms.ToolStripMenuItem();
            this.menuEdit = new System.Windows.Forms.ToolStripMenuItem();
            this.menuAdd = new System.Windows.Forms.ToolStripMenuItem();
            this.menuDelete = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.menuSave = new System.Windows.Forms.ToolStripMenuItem();
            this.menuCancel = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator5 = new System.Windows.Forms.ToolStripSeparator();
            this.menuSearch = new System.Windows.Forms.ToolStripMenuItem();
            this.menuDatabase = new System.Windows.Forms.ToolStripMenuItem();
            this.itemsToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.menuManufacturers = new System.Windows.Forms.ToolStripMenuItem();
            this.menuCategories = new System.Windows.Forms.ToolStripMenuItem();
            this.menuSubCategories = new System.Windows.Forms.ToolStripMenuItem();
            this.menuItems = new System.Windows.Forms.ToolStripMenuItem();
            this.menuFactions = new System.Windows.Forms.ToolStripMenuItem();
            this.menuMissions = new System.Windows.Forms.ToolStripMenuItem();
            this.toolbar = new System.Windows.Forms.ToolStrip();
            this.btnNew = new System.Windows.Forms.ToolStripButton();
            this.btnDelete = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator = new System.Windows.Forms.ToolStripSeparator();
            this.btnSave = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.btnCancel = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.btnSearch = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.lblSearch = new System.Windows.Forms.ToolStripLabel();
            this.flnSearch = new System.Windows.Forms.ToolStripTextBox();
            lblItemId = new System.Windows.Forms.Label();
            baseManufacturerLabel = new System.Windows.Forms.Label();
            basePriceLabel = new System.Windows.Forms.Label();
            baseStackSizeLabel = new System.Windows.Forms.Label();
            baseLevelLabel = new System.Windows.Forms.Label();
            baseSubCategoryLabel = new System.Windows.Forms.Label();
            baseCategoryLabel = new System.Windows.Forms.Label();
            baseDescriptionLabel = new System.Windows.Forms.Label();
            baseNameLabel = new System.Windows.Forms.Label();
            base3dAssetLabel = new System.Windows.Forms.Label();
            base2DAssessLabel = new System.Windows.Forms.Label();
            label13 = new System.Windows.Forms.Label();
            label40 = new System.Windows.Forms.Label();
            label42 = new System.Windows.Forms.Label();
            label27 = new System.Windows.Forms.Label();
            this.tabsMain.SuspendLayout();
            this.tabBase.SuspendLayout();
            this.grpRestictions.SuspendLayout();
            this.grpFlags.SuspendLayout();
            this.tabBeam.SuspendLayout();
            this.tabProjectile.SuspendLayout();
            this.tabMissile.SuspendLayout();
            this.tabAmmo.SuspendLayout();
            this.tabDevice.SuspendLayout();
            this.tabRS.SuspendLayout();
            this.tabEngine.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.toolbar.SuspendLayout();
            this.SuspendLayout();
            // 
            // lblItemId
            // 
            lblItemId.AutoSize = true;
            lblItemId.Location = new System.Drawing.Point(91, 55);
            lblItemId.Name = "lblItemId";
            lblItemId.Size = new System.Drawing.Size(44, 13);
            lblItemId.TabIndex = 0;
            lblItemId.Text = "Item ID:";
            lblItemId.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // baseManufacturerLabel
            // 
            baseManufacturerLabel.AutoSize = true;
            baseManufacturerLabel.Location = new System.Drawing.Point(63, 312);
            baseManufacturerLabel.Name = "baseManufacturerLabel";
            baseManufacturerLabel.Size = new System.Drawing.Size(73, 13);
            baseManufacturerLabel.TabIndex = 8;
            baseManufacturerLabel.Text = "Manufacturer:";
            baseManufacturerLabel.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // basePriceLabel
            // 
            basePriceLabel.AutoSize = true;
            basePriceLabel.Location = new System.Drawing.Point(598, 104);
            basePriceLabel.Name = "basePriceLabel";
            basePriceLabel.Size = new System.Drawing.Size(34, 13);
            basePriceLabel.TabIndex = 15;
            basePriceLabel.Text = "Price:";
            // 
            // baseStackSizeLabel
            // 
            baseStackSizeLabel.AutoSize = true;
            baseStackSizeLabel.Location = new System.Drawing.Point(571, 81);
            baseStackSizeLabel.Name = "baseStackSizeLabel";
            baseStackSizeLabel.Size = new System.Drawing.Size(61, 13);
            baseStackSizeLabel.TabIndex = 13;
            baseStackSizeLabel.Text = "Stack Size:";
            // 
            // baseLevelLabel
            // 
            baseLevelLabel.AutoSize = true;
            baseLevelLabel.Location = new System.Drawing.Point(596, 54);
            baseLevelLabel.Name = "baseLevelLabel";
            baseLevelLabel.Size = new System.Drawing.Size(36, 13);
            baseLevelLabel.TabIndex = 11;
            baseLevelLabel.Text = "Level:";
            // 
            // baseSubCategoryLabel
            // 
            baseSubCategoryLabel.AutoSize = true;
            baseSubCategoryLabel.Location = new System.Drawing.Point(65, 134);
            baseSubCategoryLabel.Name = "baseSubCategoryLabel";
            baseSubCategoryLabel.Size = new System.Drawing.Size(71, 13);
            baseSubCategoryLabel.TabIndex = 4;
            baseSubCategoryLabel.Text = "SubCategory:";
            // 
            // baseCategoryLabel
            // 
            baseCategoryLabel.AutoSize = true;
            baseCategoryLabel.Location = new System.Drawing.Point(82, 107);
            baseCategoryLabel.Name = "baseCategoryLabel";
            baseCategoryLabel.Size = new System.Drawing.Size(52, 13);
            baseCategoryLabel.TabIndex = 2;
            baseCategoryLabel.Text = "Category:";
            // 
            // baseDescriptionLabel
            // 
            baseDescriptionLabel.AutoSize = true;
            baseDescriptionLabel.Location = new System.Drawing.Point(73, 158);
            baseDescriptionLabel.Name = "baseDescriptionLabel";
            baseDescriptionLabel.Size = new System.Drawing.Size(63, 13);
            baseDescriptionLabel.TabIndex = 6;
            baseDescriptionLabel.Text = "Description:";
            baseDescriptionLabel.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // baseNameLabel
            // 
            baseNameLabel.AutoSize = true;
            baseNameLabel.Location = new System.Drawing.Point(96, 81);
            baseNameLabel.Name = "baseNameLabel";
            baseNameLabel.Size = new System.Drawing.Size(38, 13);
            baseNameLabel.TabIndex = 0;
            baseNameLabel.Text = "Name:";
            // 
            // base3dAssetLabel
            // 
            base3dAssetLabel.AutoSize = true;
            base3dAssetLabel.Location = new System.Drawing.Point(579, 161);
            base3dAssetLabel.Name = "base3dAssetLabel";
            base3dAssetLabel.Size = new System.Drawing.Size(53, 13);
            base3dAssetLabel.TabIndex = 19;
            base3dAssetLabel.Text = "3D Asset:";
            // 
            // base2DAssessLabel
            // 
            base2DAssessLabel.AutoSize = true;
            base2DAssessLabel.Location = new System.Drawing.Point(579, 134);
            base2DAssessLabel.Name = "base2DAssessLabel";
            base2DAssessLabel.Size = new System.Drawing.Size(53, 13);
            base2DAssessLabel.TabIndex = 17;
            base2DAssessLabel.Text = "2D Asset:";
            // 
            // label13
            // 
            label13.AutoSize = true;
            label13.Location = new System.Drawing.Point(48, 106);
            label13.Name = "label13";
            label13.Size = new System.Drawing.Size(77, 13);
            label13.TabIndex = 8;
            label13.Text = "Damage Type:";
            // 
            // label40
            // 
            label40.AutoSize = true;
            label40.Location = new System.Drawing.Point(21, 25);
            label40.Name = "label40";
            label40.Size = new System.Drawing.Size(111, 18);
            label40.TabIndex = 8;
            label40.Text = "Damage Type:";
            // 
            // label42
            // 
            label42.AutoSize = true;
            label42.Location = new System.Drawing.Point(48, 124);
            label42.Name = "label42";
            label42.Size = new System.Drawing.Size(70, 13);
            label42.TabIndex = 26;
            label42.Text = "Ammo Name:";
            // 
            // label27
            // 
            label27.AutoSize = true;
            label27.Location = new System.Drawing.Point(48, 124);
            label27.Name = "label27";
            label27.Size = new System.Drawing.Size(70, 13);
            label27.TabIndex = 41;
            label27.Text = "Ammo Name:";
            // 
            // txtId
            // 
            this.txtId.Location = new System.Drawing.Point(140, 52);
            this.txtId.Name = "txtId";
            this.txtId.Size = new System.Drawing.Size(74, 20);
            this.txtId.TabIndex = 1;
            // 
            // tabsMain
            // 
            this.tabsMain.Controls.Add(this.tabBase);
            this.tabsMain.Controls.Add(this.tabBeam);
            this.tabsMain.Controls.Add(this.tabProjectile);
            this.tabsMain.Controls.Add(this.tabMissile);
            this.tabsMain.Controls.Add(this.tabAmmo);
            this.tabsMain.Controls.Add(this.tabDevice);
            this.tabsMain.Controls.Add(this.tabRS);
            this.tabsMain.Controls.Add(this.tabEngine);
            this.tabsMain.Controls.Add(this.tabRefine);
            this.tabsMain.Controls.Add(this.tabManufacture);
            this.tabsMain.Controls.Add(this.tabEffects);
            this.tabsMain.Controls.Add(this.tabAddReq);
            this.tabsMain.Location = new System.Drawing.Point(12, 66);
            this.tabsMain.Name = "tabsMain";
            this.tabsMain.SelectedIndex = 0;
            this.tabsMain.Size = new System.Drawing.Size(794, 438);
            this.tabsMain.TabIndex = 1;
            this.tabsMain.Selecting += new System.Windows.Forms.TabControlCancelEventHandler(this.selectingTabPage);
            // 
            // tabBase
            // 
            this.tabBase.Controls.Add(this.lblType);
            this.tabBase.Controls.Add(this.txtType);
            this.tabBase.Controls.Add(this.grpRestictions);
            this.tabBase.Controls.Add(this.grpFlags);
            this.tabBase.Controls.Add(this.txtId);
            this.tabBase.Controls.Add(lblItemId);
            this.tabBase.Controls.Add(this.txt2DAsset);
            this.tabBase.Controls.Add(base2DAssessLabel);
            this.tabBase.Controls.Add(this.txt3DAsset);
            this.tabBase.Controls.Add(base3dAssetLabel);
            this.tabBase.Controls.Add(this.cboManufacturer);
            this.tabBase.Controls.Add(baseManufacturerLabel);
            this.tabBase.Controls.Add(this.txtPrice);
            this.tabBase.Controls.Add(basePriceLabel);
            this.tabBase.Controls.Add(this.txtStackSize);
            this.tabBase.Controls.Add(baseStackSizeLabel);
            this.tabBase.Controls.Add(this.cboLevel);
            this.tabBase.Controls.Add(baseLevelLabel);
            this.tabBase.Controls.Add(this.cboSubCategory);
            this.tabBase.Controls.Add(baseSubCategoryLabel);
            this.tabBase.Controls.Add(this.txtDescription);
            this.tabBase.Controls.Add(baseCategoryLabel);
            this.tabBase.Controls.Add(baseDescriptionLabel);
            this.tabBase.Controls.Add(this.cboCategory);
            this.tabBase.Controls.Add(this.txtName);
            this.tabBase.Controls.Add(baseNameLabel);
            this.tabBase.Location = new System.Drawing.Point(4, 22);
            this.tabBase.Name = "tabBase";
            this.tabBase.Padding = new System.Windows.Forms.Padding(3);
            this.tabBase.Size = new System.Drawing.Size(786, 412);
            this.tabBase.TabIndex = 0;
            this.tabBase.Text = "Base";
            this.tabBase.UseVisualStyleBackColor = true;
            // 
            // lblType
            // 
            this.lblType.AutoSize = true;
            this.lblType.Location = new System.Drawing.Point(600, 187);
            this.lblType.Name = "lblType";
            this.lblType.Size = new System.Drawing.Size(34, 13);
            this.lblType.TabIndex = 12;
            this.lblType.Text = "Type:";
            // 
            // txtType
            // 
            this.txtType.Location = new System.Drawing.Point(638, 184);
            this.txtType.Name = "txtType";
            this.txtType.Size = new System.Drawing.Size(136, 20);
            this.txtType.TabIndex = 31;
            // 
            // grpRestictions
            // 
            this.grpRestictions.Controls.Add(this.lblProfRestResult);
            this.grpRestictions.Controls.Add(this.chkExplorerRest);
            this.grpRestictions.Controls.Add(this.lblRaceRestResult);
            this.grpRestictions.Controls.Add(this.chkProjenRest);
            this.grpRestictions.Controls.Add(this.chkWarriorRest);
            this.grpRestictions.Controls.Add(this.chkTerranRest);
            this.grpRestictions.Controls.Add(this.chkTraderRest);
            this.grpRestictions.Controls.Add(this.chkJenquaiRest);
            this.grpRestictions.Location = new System.Drawing.Point(66, 337);
            this.grpRestictions.Name = "grpRestictions";
            this.grpRestictions.Size = new System.Drawing.Size(708, 61);
            this.grpRestictions.TabIndex = 30;
            this.grpRestictions.TabStop = false;
            this.grpRestictions.Text = "Restrictions";
            // 
            // lblProfRestResult
            // 
            this.lblProfRestResult.Location = new System.Drawing.Point(424, 33);
            this.lblProfRestResult.Name = "lblProfRestResult";
            this.lblProfRestResult.Size = new System.Drawing.Size(270, 20);
            this.lblProfRestResult.TabIndex = 8;
            this.lblProfRestResult.Text = "Result: No profession restriction";
            this.lblProfRestResult.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // chkExplorerRest
            // 
            this.chkExplorerRest.AutoSize = true;
            this.chkExplorerRest.Enabled = false;
            this.chkExplorerRest.Location = new System.Drawing.Point(251, 36);
            this.chkExplorerRest.Name = "chkExplorerRest";
            this.chkExplorerRest.Size = new System.Drawing.Size(64, 17);
            this.chkExplorerRest.TabIndex = 6;
            this.chkExplorerRest.Text = "Explorer";
            this.chkExplorerRest.UseVisualStyleBackColor = true;
            // 
            // lblRaceRestResult
            // 
            this.lblRaceRestResult.Location = new System.Drawing.Point(424, 12);
            this.lblRaceRestResult.Name = "lblRaceRestResult";
            this.lblRaceRestResult.Size = new System.Drawing.Size(274, 17);
            this.lblRaceRestResult.TabIndex = 7;
            this.lblRaceRestResult.Text = "Result: No race restriction";
            this.lblRaceRestResult.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // chkProjenRest
            // 
            this.chkProjenRest.AutoSize = true;
            this.chkProjenRest.Enabled = false;
            this.chkProjenRest.Location = new System.Drawing.Point(251, 13);
            this.chkProjenRest.Name = "chkProjenRest";
            this.chkProjenRest.Size = new System.Drawing.Size(56, 17);
            this.chkProjenRest.TabIndex = 2;
            this.chkProjenRest.Text = "Projen";
            this.chkProjenRest.UseVisualStyleBackColor = true;
            // 
            // chkWarriorRest
            // 
            this.chkWarriorRest.AutoSize = true;
            this.chkWarriorRest.Enabled = false;
            this.chkWarriorRest.Location = new System.Drawing.Point(74, 35);
            this.chkWarriorRest.Name = "chkWarriorRest";
            this.chkWarriorRest.Size = new System.Drawing.Size(60, 17);
            this.chkWarriorRest.TabIndex = 3;
            this.chkWarriorRest.Text = "Warrior";
            this.chkWarriorRest.UseVisualStyleBackColor = true;
            // 
            // chkTerranRest
            // 
            this.chkTerranRest.AutoSize = true;
            this.chkTerranRest.Enabled = false;
            this.chkTerranRest.Location = new System.Drawing.Point(74, 13);
            this.chkTerranRest.Name = "chkTerranRest";
            this.chkTerranRest.Size = new System.Drawing.Size(57, 17);
            this.chkTerranRest.TabIndex = 0;
            this.chkTerranRest.Text = "Terran";
            this.chkTerranRest.UseVisualStyleBackColor = true;
            // 
            // chkTraderRest
            // 
            this.chkTraderRest.AutoSize = true;
            this.chkTraderRest.Enabled = false;
            this.chkTraderRest.Location = new System.Drawing.Point(159, 35);
            this.chkTraderRest.Name = "chkTraderRest";
            this.chkTraderRest.Size = new System.Drawing.Size(57, 17);
            this.chkTraderRest.TabIndex = 4;
            this.chkTraderRest.Text = "Trader";
            this.chkTraderRest.UseVisualStyleBackColor = true;
            // 
            // chkJenquaiRest
            // 
            this.chkJenquaiRest.AutoSize = true;
            this.chkJenquaiRest.Enabled = false;
            this.chkJenquaiRest.Location = new System.Drawing.Point(159, 13);
            this.chkJenquaiRest.Name = "chkJenquaiRest";
            this.chkJenquaiRest.Size = new System.Drawing.Size(63, 17);
            this.chkJenquaiRest.TabIndex = 1;
            this.chkJenquaiRest.Text = "Jenquai";
            this.chkJenquaiRest.UseVisualStyleBackColor = true;
            // 
            // grpFlags
            // 
            this.grpFlags.Controls.Add(this.chkUnique);
            this.grpFlags.Controls.Add(this.chkNoDestroy);
            this.grpFlags.Controls.Add(this.chkNoManu);
            this.grpFlags.Controls.Add(this.chkNoTrade);
            this.grpFlags.Controls.Add(this.chkNoStore);
            this.grpFlags.Location = new System.Drawing.Point(493, 222);
            this.grpFlags.Name = "grpFlags";
            this.grpFlags.Size = new System.Drawing.Size(282, 109);
            this.grpFlags.TabIndex = 21;
            this.grpFlags.TabStop = false;
            // 
            // chkUnique
            // 
            this.chkUnique.AutoSize = true;
            this.chkUnique.Location = new System.Drawing.Point(103, 76);
            this.chkUnique.Name = "chkUnique";
            this.chkUnique.Size = new System.Drawing.Size(60, 17);
            this.chkUnique.TabIndex = 4;
            this.chkUnique.Text = "Unique";
            this.chkUnique.UseVisualStyleBackColor = true;
            // 
            // chkNoDestroy
            // 
            this.chkNoDestroy.AutoSize = true;
            this.chkNoDestroy.Location = new System.Drawing.Point(13, 48);
            this.chkNoDestroy.Name = "chkNoDestroy";
            this.chkNoDestroy.Size = new System.Drawing.Size(85, 17);
            this.chkNoDestroy.TabIndex = 2;
            this.chkNoDestroy.Text = "Non-Destroy";
            this.chkNoDestroy.UseVisualStyleBackColor = true;
            // 
            // chkNoManu
            // 
            this.chkNoManu.AutoSize = true;
            this.chkNoManu.Location = new System.Drawing.Point(147, 48);
            this.chkNoManu.Name = "chkNoManu";
            this.chkNoManu.Size = new System.Drawing.Size(76, 17);
            this.chkNoManu.TabIndex = 3;
            this.chkNoManu.Text = "Non-Manu";
            this.chkNoManu.UseVisualStyleBackColor = true;
            // 
            // chkNoTrade
            // 
            this.chkNoTrade.AutoSize = true;
            this.chkNoTrade.Location = new System.Drawing.Point(13, 20);
            this.chkNoTrade.Name = "chkNoTrade";
            this.chkNoTrade.Size = new System.Drawing.Size(91, 17);
            this.chkNoTrade.TabIndex = 0;
            this.chkNoTrade.Text = "Non-Tradable";
            this.chkNoTrade.UseVisualStyleBackColor = true;
            // 
            // chkNoStore
            // 
            this.chkNoStore.AutoSize = true;
            this.chkNoStore.Location = new System.Drawing.Point(147, 20);
            this.chkNoStore.Name = "chkNoStore";
            this.chkNoStore.Size = new System.Drawing.Size(88, 17);
            this.chkNoStore.TabIndex = 1;
            this.chkNoStore.Text = "Non-Storable";
            this.chkNoStore.UseVisualStyleBackColor = true;
            // 
            // txt2DAsset
            // 
            this.txt2DAsset.Location = new System.Drawing.Point(638, 131);
            this.txt2DAsset.Name = "txt2DAsset";
            this.txt2DAsset.Size = new System.Drawing.Size(136, 20);
            this.txt2DAsset.TabIndex = 18;
            // 
            // txt3DAsset
            // 
            this.txt3DAsset.Location = new System.Drawing.Point(638, 158);
            this.txt3DAsset.Name = "txt3DAsset";
            this.txt3DAsset.Size = new System.Drawing.Size(136, 20);
            this.txt3DAsset.TabIndex = 20;
            // 
            // cboManufacturer
            // 
            this.cboManufacturer.FormattingEnabled = true;
            this.cboManufacturer.Location = new System.Drawing.Point(140, 309);
            this.cboManufacturer.Name = "cboManufacturer";
            this.cboManufacturer.Size = new System.Drawing.Size(331, 21);
            this.cboManufacturer.TabIndex = 9;
            // 
            // txtPrice
            // 
            this.txtPrice.Location = new System.Drawing.Point(638, 104);
            this.txtPrice.Name = "txtPrice";
            this.txtPrice.Size = new System.Drawing.Size(136, 20);
            this.txtPrice.TabIndex = 16;
            // 
            // txtStackSize
            // 
            this.txtStackSize.Location = new System.Drawing.Point(638, 78);
            this.txtStackSize.Name = "txtStackSize";
            this.txtStackSize.Size = new System.Drawing.Size(136, 20);
            this.txtStackSize.TabIndex = 14;
            // 
            // cboLevel
            // 
            this.cboLevel.DropDownHeight = 165;
            this.cboLevel.FormattingEnabled = true;
            this.cboLevel.IntegralHeight = false;
            this.cboLevel.Location = new System.Drawing.Point(638, 51);
            this.cboLevel.Name = "cboLevel";
            this.cboLevel.Size = new System.Drawing.Size(136, 21);
            this.cboLevel.TabIndex = 12;
            // 
            // cboSubCategory
            // 
            this.cboSubCategory.FormattingEnabled = true;
            this.cboSubCategory.Location = new System.Drawing.Point(140, 131);
            this.cboSubCategory.Name = "cboSubCategory";
            this.cboSubCategory.Size = new System.Drawing.Size(331, 21);
            this.cboSubCategory.TabIndex = 5;
            this.cboSubCategory.SelectionChangeCommitted += new System.EventHandler(this.cboSubCategory_SelectionChangeCommitted);
            // 
            // txtDescription
            // 
            this.txtDescription.Location = new System.Drawing.Point(140, 158);
            this.txtDescription.Multiline = true;
            this.txtDescription.Name = "txtDescription";
            this.txtDescription.Size = new System.Drawing.Size(333, 141);
            this.txtDescription.TabIndex = 7;
            // 
            // cboCategory
            // 
            this.cboCategory.FormattingEnabled = true;
            this.cboCategory.Location = new System.Drawing.Point(140, 104);
            this.cboCategory.Name = "cboCategory";
            this.cboCategory.Size = new System.Drawing.Size(331, 21);
            this.cboCategory.TabIndex = 3;
            this.cboCategory.SelectionChangeCommitted += new System.EventHandler(this.cboItemCategory_SelectionChangeCommitted);
            // 
            // txtName
            // 
            this.txtName.Location = new System.Drawing.Point(140, 78);
            this.txtName.Name = "txtName";
            this.txtName.Size = new System.Drawing.Size(331, 20);
            this.txtName.TabIndex = 1;
            // 
            // tabBeam
            // 
            this.tabBeam.Controls.Add(this.txtBeamReload);
            this.tabBeam.Controls.Add(this.label22);
            this.tabBeam.Controls.Add(this.txtBeamEnergy);
            this.tabBeam.Controls.Add(this.label20);
            this.tabBeam.Controls.Add(this.txtBeamRange);
            this.tabBeam.Controls.Add(this.label17);
            this.tabBeam.Controls.Add(this.txtBeamDamage200);
            this.tabBeam.Controls.Add(this.label16);
            this.tabBeam.Controls.Add(this.txtBeamDamage100);
            this.tabBeam.Controls.Add(this.label15);
            this.tabBeam.Controls.Add(this.label14);
            this.tabBeam.Controls.Add(this.txtBeamFireEffect);
            this.tabBeam.Controls.Add(this.cboBeamDamageType);
            this.tabBeam.Controls.Add(label13);
            this.tabBeam.Location = new System.Drawing.Point(4, 22);
            this.tabBeam.Name = "tabBeam";
            this.tabBeam.Padding = new System.Windows.Forms.Padding(3);
            this.tabBeam.Size = new System.Drawing.Size(786, 412);
            this.tabBeam.TabIndex = 1;
            this.tabBeam.Text = "Beam";
            this.tabBeam.UseVisualStyleBackColor = true;
            // 
            // txtBeamReload
            // 
            this.txtBeamReload.Location = new System.Drawing.Point(588, 246);
            this.txtBeamReload.Name = "txtBeamReload";
            this.txtBeamReload.Size = new System.Drawing.Size(151, 20);
            this.txtBeamReload.TabIndex = 25;
            // 
            // label22
            // 
            this.label22.AutoSize = true;
            this.label22.Location = new System.Drawing.Point(466, 249);
            this.label22.Name = "label22";
            this.label22.Size = new System.Drawing.Size(44, 13);
            this.label22.TabIndex = 24;
            this.label22.Text = "Reload:";
            // 
            // txtBeamEnergy
            // 
            this.txtBeamEnergy.Location = new System.Drawing.Point(588, 209);
            this.txtBeamEnergy.Name = "txtBeamEnergy";
            this.txtBeamEnergy.Size = new System.Drawing.Size(151, 20);
            this.txtBeamEnergy.TabIndex = 21;
            // 
            // label20
            // 
            this.label20.AutoSize = true;
            this.label20.Location = new System.Drawing.Point(466, 212);
            this.label20.Name = "label20";
            this.label20.Size = new System.Drawing.Size(43, 13);
            this.label20.TabIndex = 20;
            this.label20.Text = "Energy:";
            // 
            // txtBeamRange
            // 
            this.txtBeamRange.Location = new System.Drawing.Point(588, 172);
            this.txtBeamRange.Name = "txtBeamRange";
            this.txtBeamRange.Size = new System.Drawing.Size(151, 20);
            this.txtBeamRange.TabIndex = 17;
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(465, 175);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(42, 13);
            this.label17.TabIndex = 16;
            this.label17.Text = "Range:";
            // 
            // txtBeamDamage200
            // 
            this.txtBeamDamage200.Location = new System.Drawing.Point(219, 209);
            this.txtBeamDamage200.Name = "txtBeamDamage200";
            this.txtBeamDamage200.Size = new System.Drawing.Size(151, 20);
            this.txtBeamDamage200.TabIndex = 15;
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(48, 212);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(93, 13);
            this.label16.TabIndex = 14;
            this.label16.Text = "Damage @ 200%:";
            // 
            // txtBeamDamage100
            // 
            this.txtBeamDamage100.Location = new System.Drawing.Point(219, 172);
            this.txtBeamDamage100.Name = "txtBeamDamage100";
            this.txtBeamDamage100.Size = new System.Drawing.Size(151, 20);
            this.txtBeamDamage100.TabIndex = 13;
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(48, 175);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(93, 13);
            this.label15.TabIndex = 12;
            this.label15.Text = "Damage @ 100%:";
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(465, 106);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(69, 13);
            this.label14.TabIndex = 11;
            this.label14.Text = "Visual Effect:";
            // 
            // txtBeamFireEffect
            // 
            this.txtBeamFireEffect.Location = new System.Drawing.Point(588, 103);
            this.txtBeamFireEffect.Name = "txtBeamFireEffect";
            this.txtBeamFireEffect.Size = new System.Drawing.Size(151, 20);
            this.txtBeamFireEffect.TabIndex = 10;
            // 
            // cboBeamDamageType
            // 
            this.cboBeamDamageType.FormattingEnabled = true;
            this.cboBeamDamageType.Location = new System.Drawing.Point(219, 103);
            this.cboBeamDamageType.Name = "cboBeamDamageType";
            this.cboBeamDamageType.Size = new System.Drawing.Size(151, 21);
            this.cboBeamDamageType.TabIndex = 9;
            // 
            // tabProjectile
            // 
            this.tabProjectile.Controls.Add(this.txtProjectileAmmoName);
            this.tabProjectile.Controls.Add(this.txtProjectileEnergy);
            this.tabProjectile.Controls.Add(this.label19);
            this.tabProjectile.Controls.Add(this.txtProjectileRange);
            this.tabProjectile.Controls.Add(this.label21);
            this.tabProjectile.Controls.Add(this.txtProjectileReload200);
            this.tabProjectile.Controls.Add(this.label28);
            this.tabProjectile.Controls.Add(this.txtProjectileReload100);
            this.tabProjectile.Controls.Add(this.label29);
            this.tabProjectile.Controls.Add(this.label30);
            this.tabProjectile.Controls.Add(this.txtProjectileRoundsFired);
            this.tabProjectile.Controls.Add(label42);
            this.tabProjectile.Location = new System.Drawing.Point(4, 22);
            this.tabProjectile.Name = "tabProjectile";
            this.tabProjectile.Size = new System.Drawing.Size(786, 412);
            this.tabProjectile.TabIndex = 11;
            this.tabProjectile.Text = "Projectile Launcher";
            this.tabProjectile.UseVisualStyleBackColor = true;
            // 
            // txtProjectileAmmoName
            // 
            this.txtProjectileAmmoName.Location = new System.Drawing.Point(219, 121);
            this.txtProjectileAmmoName.Name = "txtProjectileAmmoName";
            this.txtProjectileAmmoName.Size = new System.Drawing.Size(151, 20);
            this.txtProjectileAmmoName.TabIndex = 40;
            // 
            // txtProjectileEnergy
            // 
            this.txtProjectileEnergy.Location = new System.Drawing.Point(588, 227);
            this.txtProjectileEnergy.Name = "txtProjectileEnergy";
            this.txtProjectileEnergy.Size = new System.Drawing.Size(151, 20);
            this.txtProjectileEnergy.TabIndex = 37;
            // 
            // label19
            // 
            this.label19.AutoSize = true;
            this.label19.Location = new System.Drawing.Point(466, 230);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(43, 13);
            this.label19.TabIndex = 36;
            this.label19.Text = "Energy:";
            // 
            // txtProjectileRange
            // 
            this.txtProjectileRange.Location = new System.Drawing.Point(588, 190);
            this.txtProjectileRange.Name = "txtProjectileRange";
            this.txtProjectileRange.Size = new System.Drawing.Size(151, 20);
            this.txtProjectileRange.TabIndex = 35;
            // 
            // label21
            // 
            this.label21.AutoSize = true;
            this.label21.Location = new System.Drawing.Point(465, 193);
            this.label21.Name = "label21";
            this.label21.Size = new System.Drawing.Size(42, 13);
            this.label21.TabIndex = 34;
            this.label21.Text = "Range:";
            // 
            // txtProjectileReload200
            // 
            this.txtProjectileReload200.Location = new System.Drawing.Point(219, 227);
            this.txtProjectileReload200.Name = "txtProjectileReload200";
            this.txtProjectileReload200.Size = new System.Drawing.Size(151, 20);
            this.txtProjectileReload200.TabIndex = 33;
            // 
            // label28
            // 
            this.label28.AutoSize = true;
            this.label28.Location = new System.Drawing.Point(48, 230);
            this.label28.Name = "label28";
            this.label28.Size = new System.Drawing.Size(87, 13);
            this.label28.TabIndex = 32;
            this.label28.Text = "Reload @ 200%:";
            // 
            // txtProjectileReload100
            // 
            this.txtProjectileReload100.Location = new System.Drawing.Point(219, 190);
            this.txtProjectileReload100.Name = "txtProjectileReload100";
            this.txtProjectileReload100.Size = new System.Drawing.Size(151, 20);
            this.txtProjectileReload100.TabIndex = 31;
            // 
            // label29
            // 
            this.label29.AutoSize = true;
            this.label29.Location = new System.Drawing.Point(48, 193);
            this.label29.Name = "label29";
            this.label29.Size = new System.Drawing.Size(87, 13);
            this.label29.TabIndex = 30;
            this.label29.Text = "Reload @ 100%:";
            // 
            // label30
            // 
            this.label30.AutoSize = true;
            this.label30.Location = new System.Drawing.Point(465, 124);
            this.label30.Name = "label30";
            this.label30.Size = new System.Drawing.Size(73, 13);
            this.label30.TabIndex = 29;
            this.label30.Text = "Rounds Fired:";
            // 
            // txtProjectileRoundsFired
            // 
            this.txtProjectileRoundsFired.Location = new System.Drawing.Point(588, 121);
            this.txtProjectileRoundsFired.Name = "txtProjectileRoundsFired";
            this.txtProjectileRoundsFired.Size = new System.Drawing.Size(151, 20);
            this.txtProjectileRoundsFired.TabIndex = 28;
            // 
            // tabMissile
            // 
            this.tabMissile.Controls.Add(this.txtMissleAmmoName);
            this.tabMissile.Controls.Add(this.txtMissleEnergy);
            this.tabMissile.Controls.Add(this.label18);
            this.tabMissile.Controls.Add(this.txtMissleReload200);
            this.tabMissile.Controls.Add(this.label24);
            this.tabMissile.Controls.Add(this.txtMissleReload100);
            this.tabMissile.Controls.Add(this.label25);
            this.tabMissile.Controls.Add(this.label26);
            this.tabMissile.Controls.Add(this.txtMissleRoundsFired);
            this.tabMissile.Controls.Add(label27);
            this.tabMissile.Location = new System.Drawing.Point(4, 22);
            this.tabMissile.Name = "tabMissile";
            this.tabMissile.Size = new System.Drawing.Size(786, 412);
            this.tabMissile.TabIndex = 2;
            this.tabMissile.Text = "Missle Launcher";
            this.tabMissile.UseVisualStyleBackColor = true;
            // 
            // txtMissleAmmoName
            // 
            this.txtMissleAmmoName.Location = new System.Drawing.Point(219, 121);
            this.txtMissleAmmoName.Name = "txtMissleAmmoName";
            this.txtMissleAmmoName.Size = new System.Drawing.Size(151, 20);
            this.txtMissleAmmoName.TabIndex = 52;
            // 
            // txtMissleEnergy
            // 
            this.txtMissleEnergy.Location = new System.Drawing.Point(588, 190);
            this.txtMissleEnergy.Name = "txtMissleEnergy";
            this.txtMissleEnergy.Size = new System.Drawing.Size(151, 20);
            this.txtMissleEnergy.TabIndex = 51;
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Location = new System.Drawing.Point(465, 193);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(43, 13);
            this.label18.TabIndex = 50;
            this.label18.Text = "Energy:";
            // 
            // txtMissleReload200
            // 
            this.txtMissleReload200.Location = new System.Drawing.Point(219, 227);
            this.txtMissleReload200.Name = "txtMissleReload200";
            this.txtMissleReload200.Size = new System.Drawing.Size(151, 20);
            this.txtMissleReload200.TabIndex = 47;
            // 
            // label24
            // 
            this.label24.AutoSize = true;
            this.label24.Location = new System.Drawing.Point(48, 230);
            this.label24.Name = "label24";
            this.label24.Size = new System.Drawing.Size(87, 13);
            this.label24.TabIndex = 46;
            this.label24.Text = "Reload @ 200%:";
            // 
            // txtMissleReload100
            // 
            this.txtMissleReload100.Location = new System.Drawing.Point(219, 190);
            this.txtMissleReload100.Name = "txtMissleReload100";
            this.txtMissleReload100.Size = new System.Drawing.Size(151, 20);
            this.txtMissleReload100.TabIndex = 45;
            // 
            // label25
            // 
            this.label25.AutoSize = true;
            this.label25.Location = new System.Drawing.Point(48, 193);
            this.label25.Name = "label25";
            this.label25.Size = new System.Drawing.Size(87, 13);
            this.label25.TabIndex = 44;
            this.label25.Text = "Reload @ 100%:";
            // 
            // label26
            // 
            this.label26.AutoSize = true;
            this.label26.Location = new System.Drawing.Point(465, 124);
            this.label26.Name = "label26";
            this.label26.Size = new System.Drawing.Size(73, 13);
            this.label26.TabIndex = 43;
            this.label26.Text = "Rounds Fired:";
            // 
            // txtMissleRoundsFired
            // 
            this.txtMissleRoundsFired.Location = new System.Drawing.Point(588, 121);
            this.txtMissleRoundsFired.Name = "txtMissleRoundsFired";
            this.txtMissleRoundsFired.Size = new System.Drawing.Size(151, 20);
            this.txtMissleRoundsFired.TabIndex = 42;
            // 
            // tabAmmo
            // 
            this.tabAmmo.Controls.Add(this.txtAmmoVisual);
            this.tabAmmo.Controls.Add(this.label47);
            this.tabAmmo.Controls.Add(this.txtAmmoRange);
            this.tabAmmo.Controls.Add(this.label46);
            this.tabAmmo.Controls.Add(this.cboAmmoDamageType);
            this.tabAmmo.Controls.Add(this.txtAmmoManouv);
            this.tabAmmo.Controls.Add(this.label23);
            this.tabAmmo.Controls.Add(this.txtAmmoDamage200);
            this.tabAmmo.Controls.Add(this.label41);
            this.tabAmmo.Controls.Add(this.txtAmmoDamage100);
            this.tabAmmo.Controls.Add(this.label43);
            this.tabAmmo.Controls.Add(this.label44);
            this.tabAmmo.Controls.Add(this.cmdAmmoLauncherRefresh);
            this.tabAmmo.Controls.Add(this.cboAmmoLauncher);
            this.tabAmmo.Controls.Add(this.label55);
            this.tabAmmo.Location = new System.Drawing.Point(4, 22);
            this.tabAmmo.Name = "tabAmmo";
            this.tabAmmo.Size = new System.Drawing.Size(786, 412);
            this.tabAmmo.TabIndex = 3;
            this.tabAmmo.Text = "Ammo";
            this.tabAmmo.UseVisualStyleBackColor = true;
            // 
            // txtAmmoVisual
            // 
            this.txtAmmoVisual.Location = new System.Drawing.Point(588, 152);
            this.txtAmmoVisual.Name = "txtAmmoVisual";
            this.txtAmmoVisual.Size = new System.Drawing.Size(151, 20);
            this.txtAmmoVisual.TabIndex = 82;
            // 
            // label47
            // 
            this.label47.AutoSize = true;
            this.label47.Location = new System.Drawing.Point(465, 155);
            this.label47.Name = "label47";
            this.label47.Size = new System.Drawing.Size(69, 13);
            this.label47.TabIndex = 81;
            this.label47.Text = "Visual Effect:";
            // 
            // txtAmmoRange
            // 
            this.txtAmmoRange.Location = new System.Drawing.Point(588, 254);
            this.txtAmmoRange.Name = "txtAmmoRange";
            this.txtAmmoRange.Size = new System.Drawing.Size(151, 20);
            this.txtAmmoRange.TabIndex = 80;
            // 
            // label46
            // 
            this.label46.AutoSize = true;
            this.label46.Location = new System.Drawing.Point(465, 257);
            this.label46.Name = "label46";
            this.label46.Size = new System.Drawing.Size(60, 13);
            this.label46.TabIndex = 79;
            this.label46.Text = "Range (M):";
            // 
            // cboAmmoDamageType
            // 
            this.cboAmmoDamageType.FormattingEnabled = true;
            this.cboAmmoDamageType.Location = new System.Drawing.Point(219, 152);
            this.cboAmmoDamageType.Name = "cboAmmoDamageType";
            this.cboAmmoDamageType.Size = new System.Drawing.Size(151, 21);
            this.cboAmmoDamageType.TabIndex = 78;
            // 
            // txtAmmoManouv
            // 
            this.txtAmmoManouv.Location = new System.Drawing.Point(588, 217);
            this.txtAmmoManouv.Name = "txtAmmoManouv";
            this.txtAmmoManouv.Size = new System.Drawing.Size(151, 20);
            this.txtAmmoManouv.TabIndex = 76;
            // 
            // label23
            // 
            this.label23.AutoSize = true;
            this.label23.Location = new System.Drawing.Point(465, 220);
            this.label23.Name = "label23";
            this.label23.Size = new System.Drawing.Size(84, 13);
            this.label23.TabIndex = 75;
            this.label23.Text = "Manouverability:";
            // 
            // txtAmmoDamage200
            // 
            this.txtAmmoDamage200.Location = new System.Drawing.Point(219, 254);
            this.txtAmmoDamage200.Name = "txtAmmoDamage200";
            this.txtAmmoDamage200.Size = new System.Drawing.Size(151, 20);
            this.txtAmmoDamage200.TabIndex = 74;
            // 
            // label41
            // 
            this.label41.AutoSize = true;
            this.label41.Location = new System.Drawing.Point(48, 257);
            this.label41.Name = "label41";
            this.label41.Size = new System.Drawing.Size(93, 13);
            this.label41.TabIndex = 73;
            this.label41.Text = "Damage @ 200%:";
            // 
            // txtAmmoDamage100
            // 
            this.txtAmmoDamage100.Location = new System.Drawing.Point(219, 217);
            this.txtAmmoDamage100.Name = "txtAmmoDamage100";
            this.txtAmmoDamage100.Size = new System.Drawing.Size(151, 20);
            this.txtAmmoDamage100.TabIndex = 72;
            // 
            // label43
            // 
            this.label43.AutoSize = true;
            this.label43.Location = new System.Drawing.Point(48, 220);
            this.label43.Name = "label43";
            this.label43.Size = new System.Drawing.Size(93, 13);
            this.label43.TabIndex = 71;
            this.label43.Text = "Damage @ 100%:";
            // 
            // label44
            // 
            this.label44.AutoSize = true;
            this.label44.Location = new System.Drawing.Point(48, 155);
            this.label44.Name = "label44";
            this.label44.Size = new System.Drawing.Size(77, 13);
            this.label44.TabIndex = 70;
            this.label44.Text = "Damage Type:";
            // 
            // cmdAmmoLauncherRefresh
            // 
            this.cmdAmmoLauncherRefresh.BackgroundImage = global::Net7_Tools.Properties.Resources.Refresh;
            this.cmdAmmoLauncherRefresh.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.cmdAmmoLauncherRefresh.Location = new System.Drawing.Point(707, 95);
            this.cmdAmmoLauncherRefresh.Name = "cmdAmmoLauncherRefresh";
            this.cmdAmmoLauncherRefresh.Size = new System.Drawing.Size(32, 27);
            this.cmdAmmoLauncherRefresh.TabIndex = 67;
            this.cmdAmmoLauncherRefresh.UseVisualStyleBackColor = true;
            // 
            // cboAmmoLauncher
            // 
            this.cboAmmoLauncher.DropDownHeight = 354;
            this.cboAmmoLauncher.FormattingEnabled = true;
            this.cboAmmoLauncher.IntegralHeight = false;
            this.cboAmmoLauncher.Location = new System.Drawing.Point(219, 96);
            this.cboAmmoLauncher.Name = "cboAmmoLauncher";
            this.cboAmmoLauncher.Size = new System.Drawing.Size(482, 21);
            this.cboAmmoLauncher.TabIndex = 66;
            // 
            // label55
            // 
            this.label55.AutoSize = true;
            this.label55.Location = new System.Drawing.Point(48, 99);
            this.label55.Name = "label55";
            this.label55.Size = new System.Drawing.Size(83, 13);
            this.label55.TabIndex = 65;
            this.label55.Text = "Launcher Used:";
            // 
            // tabDevice
            // 
            this.tabDevice.Controls.Add(this.label35);
            this.tabDevice.Controls.Add(this.txtDeviceCooldown);
            this.tabDevice.Controls.Add(this.label51);
            this.tabDevice.Controls.Add(this.txtDeviceRange);
            this.tabDevice.Controls.Add(this.label48);
            this.tabDevice.Controls.Add(this.txtDeviceEnergy200);
            this.tabDevice.Controls.Add(this.label49);
            this.tabDevice.Controls.Add(this.txtDeviceEnergy100);
            this.tabDevice.Controls.Add(this.label50);
            this.tabDevice.Location = new System.Drawing.Point(4, 22);
            this.tabDevice.Name = "tabDevice";
            this.tabDevice.Size = new System.Drawing.Size(786, 412);
            this.tabDevice.TabIndex = 4;
            this.tabDevice.Text = "Device";
            this.tabDevice.UseVisualStyleBackColor = true;
            // 
            // label35
            // 
            this.label35.AutoSize = true;
            this.label35.Location = new System.Drawing.Point(210, 228);
            this.label35.Name = "label35";
            this.label35.Size = new System.Drawing.Size(257, 13);
            this.label35.TabIndex = 64;
            this.label35.Text = "Note: These values overwrite those in the effects tab";
            // 
            // txtDeviceCooldown
            // 
            this.txtDeviceCooldown.Location = new System.Drawing.Point(588, 165);
            this.txtDeviceCooldown.Name = "txtDeviceCooldown";
            this.txtDeviceCooldown.Size = new System.Drawing.Size(151, 20);
            this.txtDeviceCooldown.TabIndex = 63;
            // 
            // label51
            // 
            this.label51.AutoSize = true;
            this.label51.Location = new System.Drawing.Point(465, 168);
            this.label51.Name = "label51";
            this.label51.Size = new System.Drawing.Size(57, 13);
            this.label51.TabIndex = 62;
            this.label51.Text = "Cooldown:";
            // 
            // txtDeviceRange
            // 
            this.txtDeviceRange.Location = new System.Drawing.Point(588, 128);
            this.txtDeviceRange.Name = "txtDeviceRange";
            this.txtDeviceRange.Size = new System.Drawing.Size(151, 20);
            this.txtDeviceRange.TabIndex = 61;
            // 
            // label48
            // 
            this.label48.AutoSize = true;
            this.label48.Location = new System.Drawing.Point(465, 131);
            this.label48.Name = "label48";
            this.label48.Size = new System.Drawing.Size(42, 13);
            this.label48.TabIndex = 60;
            this.label48.Text = "Range:";
            // 
            // txtDeviceEnergy200
            // 
            this.txtDeviceEnergy200.Location = new System.Drawing.Point(219, 165);
            this.txtDeviceEnergy200.Name = "txtDeviceEnergy200";
            this.txtDeviceEnergy200.Size = new System.Drawing.Size(151, 20);
            this.txtDeviceEnergy200.TabIndex = 59;
            // 
            // label49
            // 
            this.label49.AutoSize = true;
            this.label49.Location = new System.Drawing.Point(48, 168);
            this.label49.Name = "label49";
            this.label49.Size = new System.Drawing.Size(86, 13);
            this.label49.TabIndex = 58;
            this.label49.Text = "Energy @ 200%:";
            // 
            // txtDeviceEnergy100
            // 
            this.txtDeviceEnergy100.Location = new System.Drawing.Point(219, 128);
            this.txtDeviceEnergy100.Name = "txtDeviceEnergy100";
            this.txtDeviceEnergy100.Size = new System.Drawing.Size(151, 20);
            this.txtDeviceEnergy100.TabIndex = 57;
            // 
            // label50
            // 
            this.label50.AutoSize = true;
            this.label50.Location = new System.Drawing.Point(48, 131);
            this.label50.Name = "label50";
            this.label50.Size = new System.Drawing.Size(86, 13);
            this.label50.TabIndex = 56;
            this.label50.Text = "Energy @ 100%:";
            // 
            // tabRS
            // 
            this.tabRS.Controls.Add(this.txtRSCapacity);
            this.tabRS.Controls.Add(this.label32);
            this.tabRS.Controls.Add(this.txtRSRecharge200);
            this.tabRS.Controls.Add(this.label33);
            this.tabRS.Controls.Add(this.txtRSRecharge100);
            this.tabRS.Controls.Add(this.label34);
            this.tabRS.Location = new System.Drawing.Point(4, 22);
            this.tabRS.Name = "tabRS";
            this.tabRS.Size = new System.Drawing.Size(786, 412);
            this.tabRS.TabIndex = 5;
            this.tabRS.Text = "Reactor / Shield";
            this.tabRS.UseVisualStyleBackColor = true;
            // 
            // txtRSCapacity
            // 
            this.txtRSCapacity.Location = new System.Drawing.Point(588, 156);
            this.txtRSCapacity.Name = "txtRSCapacity";
            this.txtRSCapacity.Size = new System.Drawing.Size(151, 20);
            this.txtRSCapacity.TabIndex = 69;
            // 
            // label32
            // 
            this.label32.AutoSize = true;
            this.label32.Location = new System.Drawing.Point(465, 159);
            this.label32.Name = "label32";
            this.label32.Size = new System.Drawing.Size(51, 13);
            this.label32.TabIndex = 68;
            this.label32.Text = "Capacity:";
            // 
            // txtRSRecharge200
            // 
            this.txtRSRecharge200.Location = new System.Drawing.Point(219, 193);
            this.txtRSRecharge200.Name = "txtRSRecharge200";
            this.txtRSRecharge200.Size = new System.Drawing.Size(151, 20);
            this.txtRSRecharge200.TabIndex = 67;
            // 
            // label33
            // 
            this.label33.AutoSize = true;
            this.label33.Location = new System.Drawing.Point(48, 196);
            this.label33.Name = "label33";
            this.label33.Size = new System.Drawing.Size(100, 13);
            this.label33.TabIndex = 66;
            this.label33.Text = "Recharge @ 200%:";
            // 
            // txtRSRecharge100
            // 
            this.txtRSRecharge100.Location = new System.Drawing.Point(219, 156);
            this.txtRSRecharge100.Name = "txtRSRecharge100";
            this.txtRSRecharge100.Size = new System.Drawing.Size(151, 20);
            this.txtRSRecharge100.TabIndex = 65;
            // 
            // label34
            // 
            this.label34.AutoSize = true;
            this.label34.Location = new System.Drawing.Point(48, 159);
            this.label34.Name = "label34";
            this.label34.Size = new System.Drawing.Size(100, 13);
            this.label34.TabIndex = 64;
            this.label34.Text = "Recharge @ 100%:";
            // 
            // tabEngine
            // 
            this.tabEngine.Controls.Add(this.txtEngineFreeWarp200);
            this.tabEngine.Controls.Add(this.label52);
            this.tabEngine.Controls.Add(this.txtEngineFreeWarp100);
            this.tabEngine.Controls.Add(this.label53);
            this.tabEngine.Controls.Add(this.txtEngineSignature200);
            this.tabEngine.Controls.Add(this.label31);
            this.tabEngine.Controls.Add(this.txtEngineSignature100);
            this.tabEngine.Controls.Add(this.label36);
            this.tabEngine.Controls.Add(this.txtEngineThrust200);
            this.tabEngine.Controls.Add(this.label37);
            this.tabEngine.Controls.Add(this.txtEngineWarpCharge);
            this.tabEngine.Controls.Add(this.label38);
            this.tabEngine.Controls.Add(this.txtEngineWarpSpeed);
            this.tabEngine.Controls.Add(this.label39);
            this.tabEngine.Controls.Add(this.label45);
            this.tabEngine.Controls.Add(this.txtEngineThrust100);
            this.tabEngine.Location = new System.Drawing.Point(4, 22);
            this.tabEngine.Name = "tabEngine";
            this.tabEngine.Size = new System.Drawing.Size(786, 412);
            this.tabEngine.TabIndex = 6;
            this.tabEngine.Text = "Engine";
            this.tabEngine.UseVisualStyleBackColor = true;
            // 
            // txtEngineFreeWarp200
            // 
            this.txtEngineFreeWarp200.Location = new System.Drawing.Point(219, 241);
            this.txtEngineFreeWarp200.Name = "txtEngineFreeWarp200";
            this.txtEngineFreeWarp200.Size = new System.Drawing.Size(151, 20);
            this.txtEngineFreeWarp200.TabIndex = 43;
            // 
            // label52
            // 
            this.label52.AutoSize = true;
            this.label52.Location = new System.Drawing.Point(48, 244);
            this.label52.Name = "label52";
            this.label52.Size = new System.Drawing.Size(100, 13);
            this.label52.TabIndex = 42;
            this.label52.Text = "Free warp @ 200%:";
            // 
            // txtEngineFreeWarp100
            // 
            this.txtEngineFreeWarp100.Location = new System.Drawing.Point(219, 204);
            this.txtEngineFreeWarp100.Name = "txtEngineFreeWarp100";
            this.txtEngineFreeWarp100.Size = new System.Drawing.Size(151, 20);
            this.txtEngineFreeWarp100.TabIndex = 41;
            // 
            // label53
            // 
            this.label53.AutoSize = true;
            this.label53.Location = new System.Drawing.Point(48, 204);
            this.label53.Name = "label53";
            this.label53.Size = new System.Drawing.Size(100, 13);
            this.label53.TabIndex = 40;
            this.label53.Text = "Free warp @ 100%:";
            // 
            // txtEngineSignature200
            // 
            this.txtEngineSignature200.Location = new System.Drawing.Point(588, 246);
            this.txtEngineSignature200.Name = "txtEngineSignature200";
            this.txtEngineSignature200.Size = new System.Drawing.Size(151, 20);
            this.txtEngineSignature200.TabIndex = 39;
            // 
            // label31
            // 
            this.label31.AutoSize = true;
            this.label31.Location = new System.Drawing.Point(433, 249);
            this.label31.Name = "label31";
            this.label31.Size = new System.Drawing.Size(98, 13);
            this.label31.TabIndex = 38;
            this.label31.Text = "Signature @ 200%:";
            // 
            // txtEngineSignature100
            // 
            this.txtEngineSignature100.Location = new System.Drawing.Point(588, 209);
            this.txtEngineSignature100.Name = "txtEngineSignature100";
            this.txtEngineSignature100.Size = new System.Drawing.Size(151, 20);
            this.txtEngineSignature100.TabIndex = 37;
            // 
            // label36
            // 
            this.label36.AutoSize = true;
            this.label36.Location = new System.Drawing.Point(433, 212);
            this.label36.Name = "label36";
            this.label36.Size = new System.Drawing.Size(98, 13);
            this.label36.TabIndex = 36;
            this.label36.Text = "Signature @ 100%:";
            // 
            // txtEngineThrust200
            // 
            this.txtEngineThrust200.Location = new System.Drawing.Point(588, 140);
            this.txtEngineThrust200.Name = "txtEngineThrust200";
            this.txtEngineThrust200.Size = new System.Drawing.Size(151, 20);
            this.txtEngineThrust200.TabIndex = 35;
            // 
            // label37
            // 
            this.label37.AutoSize = true;
            this.label37.Location = new System.Drawing.Point(433, 143);
            this.label37.Name = "label37";
            this.label37.Size = new System.Drawing.Size(83, 13);
            this.label37.TabIndex = 34;
            this.label37.Text = "Thrust @ 200%:";
            // 
            // txtEngineWarpCharge
            // 
            this.txtEngineWarpCharge.Location = new System.Drawing.Point(219, 140);
            this.txtEngineWarpCharge.Name = "txtEngineWarpCharge";
            this.txtEngineWarpCharge.Size = new System.Drawing.Size(151, 20);
            this.txtEngineWarpCharge.TabIndex = 33;
            // 
            // label38
            // 
            this.label38.AutoSize = true;
            this.label38.Location = new System.Drawing.Point(48, 143);
            this.label38.Name = "label38";
            this.label38.Size = new System.Drawing.Size(73, 13);
            this.label38.TabIndex = 32;
            this.label38.Text = "Warp Charge:";
            // 
            // txtEngineWarpSpeed
            // 
            this.txtEngineWarpSpeed.Location = new System.Drawing.Point(219, 103);
            this.txtEngineWarpSpeed.Name = "txtEngineWarpSpeed";
            this.txtEngineWarpSpeed.Size = new System.Drawing.Size(151, 20);
            this.txtEngineWarpSpeed.TabIndex = 31;
            // 
            // label39
            // 
            this.label39.AutoSize = true;
            this.label39.Location = new System.Drawing.Point(48, 106);
            this.label39.Name = "label39";
            this.label39.Size = new System.Drawing.Size(70, 13);
            this.label39.TabIndex = 30;
            this.label39.Text = "Warp Speed:";
            // 
            // label45
            // 
            this.label45.AutoSize = true;
            this.label45.Location = new System.Drawing.Point(433, 106);
            this.label45.Name = "label45";
            this.label45.Size = new System.Drawing.Size(83, 13);
            this.label45.TabIndex = 29;
            this.label45.Text = "Thrust @ 100%:";
            // 
            // txtEngineThrust100
            // 
            this.txtEngineThrust100.Location = new System.Drawing.Point(588, 103);
            this.txtEngineThrust100.Name = "txtEngineThrust100";
            this.txtEngineThrust100.Size = new System.Drawing.Size(151, 20);
            this.txtEngineThrust100.TabIndex = 28;
            // 
            // tabRefine
            // 
            this.tabRefine.Location = new System.Drawing.Point(4, 22);
            this.tabRefine.Name = "tabRefine";
            this.tabRefine.Size = new System.Drawing.Size(786, 412);
            this.tabRefine.TabIndex = 7;
            this.tabRefine.Text = "Refining";
            this.tabRefine.UseVisualStyleBackColor = true;
            // 
            // tabManufacture
            // 
            this.tabManufacture.Location = new System.Drawing.Point(4, 22);
            this.tabManufacture.Name = "tabManufacture";
            this.tabManufacture.Size = new System.Drawing.Size(786, 412);
            this.tabManufacture.TabIndex = 8;
            this.tabManufacture.Text = "Manufacturing";
            this.tabManufacture.UseVisualStyleBackColor = true;
            // 
            // tabEffects
            // 
            this.tabEffects.Location = new System.Drawing.Point(4, 22);
            this.tabEffects.Name = "tabEffects";
            this.tabEffects.Size = new System.Drawing.Size(786, 412);
            this.tabEffects.TabIndex = 9;
            this.tabEffects.Text = "Effects";
            this.tabEffects.UseVisualStyleBackColor = true;
            // 
            // tabAddReq
            // 
            this.tabAddReq.Location = new System.Drawing.Point(4, 22);
            this.tabAddReq.Name = "tabAddReq";
            this.tabAddReq.Size = new System.Drawing.Size(786, 412);
            this.tabAddReq.TabIndex = 10;
            this.tabAddReq.Text = "Added Requirements";
            this.tabAddReq.UseVisualStyleBackColor = true;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(264, 515);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(131, 30);
            this.button1.TabIndex = 4;
            this.button1.Text = "button1";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // cmdEditManu
            // 
            this.cmdEditManu.Location = new System.Drawing.Point(12, 515);
            this.cmdEditManu.Name = "cmdEditManu";
            this.cmdEditManu.Size = new System.Drawing.Size(120, 35);
            this.cmdEditManu.TabIndex = 2;
            this.cmdEditManu.Text = "Edit Manufacturers";
            this.cmdEditManu.UseVisualStyleBackColor = true;
            // 
            // cmdEditEffects
            // 
            this.cmdEditEffects.Location = new System.Drawing.Point(138, 515);
            this.cmdEditEffects.Name = "cmdEditEffects";
            this.cmdEditEffects.Size = new System.Drawing.Size(120, 35);
            this.cmdEditEffects.TabIndex = 3;
            this.cmdEditEffects.Text = "Edit Item Effects";
            this.cmdEditEffects.UseVisualStyleBackColor = true;
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menuFile,
            this.menuEdit,
            this.menuDatabase});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(818, 24);
            this.menuStrip1.TabIndex = 5;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // menuFile
            // 
            this.menuFile.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menuExit});
            this.menuFile.Name = "menuFile";
            this.menuFile.Size = new System.Drawing.Size(35, 20);
            this.menuFile.Text = "File";
            // 
            // menuExit
            // 
            this.menuExit.Name = "menuExit";
            this.menuExit.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.F4)));
            this.menuExit.Size = new System.Drawing.Size(143, 22);
            this.menuExit.Text = "Exit";
            // 
            // menuEdit
            // 
            this.menuEdit.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menuAdd,
            this.menuDelete,
            this.toolStripSeparator1,
            this.menuSave,
            this.menuCancel,
            this.toolStripSeparator5,
            this.menuSearch});
            this.menuEdit.Name = "menuEdit";
            this.menuEdit.Size = new System.Drawing.Size(37, 20);
            this.menuEdit.Text = "Edit";
            // 
            // menuAdd
            // 
            this.menuAdd.Name = "menuAdd";
            this.menuAdd.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.N)));
            this.menuAdd.Size = new System.Drawing.Size(156, 22);
            this.menuAdd.Text = "Add";
            this.menuAdd.Click += new System.EventHandler(this.btnNew_Click);
            // 
            // menuDelete
            // 
            this.menuDelete.Name = "menuDelete";
            this.menuDelete.Size = new System.Drawing.Size(156, 22);
            this.menuDelete.Text = "Delete";
            this.menuDelete.Click += new System.EventHandler(this.btnDelete_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(153, 6);
            // 
            // menuSave
            // 
            this.menuSave.Name = "menuSave";
            this.menuSave.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
            this.menuSave.Size = new System.Drawing.Size(156, 22);
            this.menuSave.Text = "Save";
            this.menuSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // menuCancel
            // 
            this.menuCancel.Name = "menuCancel";
            this.menuCancel.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Z)));
            this.menuCancel.Size = new System.Drawing.Size(156, 22);
            this.menuCancel.Text = "Cancel";
            this.menuCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // toolStripSeparator5
            // 
            this.toolStripSeparator5.Name = "toolStripSeparator5";
            this.toolStripSeparator5.Size = new System.Drawing.Size(153, 6);
            // 
            // menuSearch
            // 
            this.menuSearch.Name = "menuSearch";
            this.menuSearch.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.F)));
            this.menuSearch.Size = new System.Drawing.Size(156, 22);
            this.menuSearch.Text = "Search";
            this.menuSearch.Click += new System.EventHandler(this.btnSearch_Click);
            // 
            // menuDatabase
            // 
            this.menuDatabase.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.itemsToolStripMenuItem1,
            this.menuFactions,
            this.menuMissions});
            this.menuDatabase.Name = "menuDatabase";
            this.menuDatabase.Size = new System.Drawing.Size(65, 20);
            this.menuDatabase.Text = "Database";
            // 
            // itemsToolStripMenuItem1
            // 
            this.itemsToolStripMenuItem1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menuManufacturers,
            this.menuCategories,
            this.menuSubCategories,
            this.menuItems});
            this.itemsToolStripMenuItem1.Name = "itemsToolStripMenuItem1";
            this.itemsToolStripMenuItem1.Size = new System.Drawing.Size(125, 22);
            this.itemsToolStripMenuItem1.Text = "Items";
            // 
            // menuManufacturers
            // 
            this.menuManufacturers.Name = "menuManufacturers";
            this.menuManufacturers.Size = new System.Drawing.Size(159, 22);
            this.menuManufacturers.Text = "Manufacturers";
            // 
            // menuCategories
            // 
            this.menuCategories.Name = "menuCategories";
            this.menuCategories.Size = new System.Drawing.Size(159, 22);
            this.menuCategories.Text = "Categories";
            // 
            // menuSubCategories
            // 
            this.menuSubCategories.Name = "menuSubCategories";
            this.menuSubCategories.Size = new System.Drawing.Size(159, 22);
            this.menuSubCategories.Text = "Sub-Categories";
            // 
            // menuItems
            // 
            this.menuItems.Name = "menuItems";
            this.menuItems.Size = new System.Drawing.Size(159, 22);
            this.menuItems.Text = "Items";
            // 
            // menuFactions
            // 
            this.menuFactions.Name = "menuFactions";
            this.menuFactions.Size = new System.Drawing.Size(125, 22);
            this.menuFactions.Text = "Factions";
            this.menuFactions.Click += new System.EventHandler(this.setEditor);
            // 
            // menuMissions
            // 
            this.menuMissions.Name = "menuMissions";
            this.menuMissions.Size = new System.Drawing.Size(125, 22);
            this.menuMissions.Text = "Missions";
            // 
            // toolbar
            // 
            this.toolbar.ImageScalingSize = new System.Drawing.Size(32, 32);
            this.toolbar.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btnNew,
            this.btnDelete,
            this.toolStripSeparator,
            this.btnSave,
            this.toolStripSeparator3,
            this.btnCancel,
            this.toolStripSeparator2,
            this.btnSearch,
            this.toolStripSeparator4,
            this.lblSearch,
            this.flnSearch});
            this.toolbar.Location = new System.Drawing.Point(0, 24);
            this.toolbar.Name = "toolbar";
            this.toolbar.Size = new System.Drawing.Size(818, 39);
            this.toolbar.TabIndex = 6;
            this.toolbar.Text = "toolStrip1";
            // 
            // btnNew
            // 
            this.btnNew.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnNew.Image = ((System.Drawing.Image)(resources.GetObject("btnNew.Image")));
            this.btnNew.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnNew.Name = "btnNew";
            this.btnNew.Size = new System.Drawing.Size(36, 36);
            this.btnNew.Text = "&New";
            this.btnNew.ToolTipText = "Create a new record";
            this.btnNew.Click += new System.EventHandler(this.btnNew_Click);
            // 
            // btnDelete
            // 
            this.btnDelete.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnDelete.Image = ((System.Drawing.Image)(resources.GetObject("btnDelete.Image")));
            this.btnDelete.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnDelete.Name = "btnDelete";
            this.btnDelete.Size = new System.Drawing.Size(36, 36);
            this.btnDelete.Text = "&Delete";
            this.btnDelete.ToolTipText = "Delete the current record";
            this.btnDelete.Click += new System.EventHandler(this.btnDelete_Click);
            // 
            // toolStripSeparator
            // 
            this.toolStripSeparator.Name = "toolStripSeparator";
            this.toolStripSeparator.Size = new System.Drawing.Size(6, 39);
            // 
            // btnSave
            // 
            this.btnSave.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnSave.Image = ((System.Drawing.Image)(resources.GetObject("btnSave.Image")));
            this.btnSave.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(36, 36);
            this.btnSave.Text = "&Save";
            this.btnSave.ToolTipText = "Save the modifications";
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(6, 39);
            // 
            // btnCancel
            // 
            this.btnCancel.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnCancel.Image = ((System.Drawing.Image)(resources.GetObject("btnCancel.Image")));
            this.btnCancel.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(36, 36);
            this.btnCancel.Text = "&Cancel";
            this.btnCancel.ToolTipText = "Cancel the modifications";
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 39);
            // 
            // btnSearch
            // 
            this.btnSearch.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnSearch.Image = ((System.Drawing.Image)(resources.GetObject("btnSearch.Image")));
            this.btnSearch.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnSearch.Name = "btnSearch";
            this.btnSearch.Size = new System.Drawing.Size(36, 36);
            this.btnSearch.Text = "toolStripButton1";
            this.btnSearch.Click += new System.EventHandler(this.btnSearch_Click);
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(6, 39);
            // 
            // lblSearch
            // 
            this.lblSearch.Name = "lblSearch";
            this.lblSearch.Size = new System.Drawing.Size(73, 36);
            this.lblSearch.Text = "Search by ID:";
            // 
            // flnSearch
            // 
            this.flnSearch.Name = "flnSearch";
            this.flnSearch.Size = new System.Drawing.Size(100, 39);
            this.flnSearch.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.txtSearch_KeyPress);
            // 
            // frmItems
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(818, 557);
            this.Controls.Add(this.toolbar);
            this.Controls.Add(this.cmdEditEffects);
            this.Controls.Add(this.cmdEditManu);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.tabsMain);
            this.Controls.Add(this.menuStrip1);
            this.DoubleBuffered = true;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "frmItems";
            this.Text = "DASE 2.1";
            this.tabsMain.ResumeLayout(false);
            this.tabBase.ResumeLayout(false);
            this.tabBase.PerformLayout();
            this.grpRestictions.ResumeLayout(false);
            this.grpRestictions.PerformLayout();
            this.grpFlags.ResumeLayout(false);
            this.grpFlags.PerformLayout();
            this.tabBeam.ResumeLayout(false);
            this.tabBeam.PerformLayout();
            this.tabProjectile.ResumeLayout(false);
            this.tabProjectile.PerformLayout();
            this.tabMissile.ResumeLayout(false);
            this.tabMissile.PerformLayout();
            this.tabAmmo.ResumeLayout(false);
            this.tabAmmo.PerformLayout();
            this.tabDevice.ResumeLayout(false);
            this.tabDevice.PerformLayout();
            this.tabRS.ResumeLayout(false);
            this.tabRS.PerformLayout();
            this.tabEngine.ResumeLayout(false);
            this.tabEngine.PerformLayout();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.toolbar.ResumeLayout(false);
            this.toolbar.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

        private System.Windows.Forms.TextBox txtId;
		private System.Windows.Forms.TabControl tabsMain;
		private System.Windows.Forms.TabPage tabBase;
		private System.Windows.Forms.TextBox txtDescription;
		private System.Windows.Forms.ComboBox cboCategory;
		private System.Windows.Forms.TabPage tabBeam;
		private System.Windows.Forms.TextBox txtName;
		private System.Windows.Forms.ComboBox cboSubCategory;
		private System.Windows.Forms.ComboBox cboLevel;
		private System.Windows.Forms.TextBox txtStackSize;
        private System.Windows.Forms.TextBox txtPrice;
		private System.Windows.Forms.ComboBox cboManufacturer;
		private System.Windows.Forms.TextBox txt2DAsset;
		private System.Windows.Forms.TextBox txt3DAsset;
		private System.Windows.Forms.GroupBox grpFlags;
		private System.Windows.Forms.CheckBox chkNoStore;
		private System.Windows.Forms.CheckBox chkNoDestroy;
		private System.Windows.Forms.CheckBox chkNoManu;
		private System.Windows.Forms.CheckBox chkNoTrade;
		private System.Windows.Forms.CheckBox chkUnique;
		private System.Windows.Forms.GroupBox grpRestictions;
		private System.Windows.Forms.CheckBox chkJenquaiRest;
		private System.Windows.Forms.CheckBox chkTerranRest;
		private System.Windows.Forms.CheckBox chkProjenRest;
		private System.Windows.Forms.Label lblProfRestResult;
		private System.Windows.Forms.CheckBox chkExplorerRest;
		private System.Windows.Forms.CheckBox chkWarriorRest;
		private System.Windows.Forms.CheckBox chkTraderRest;
		private System.Windows.Forms.Label lblRaceRestResult;
		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.TabPage tabMissile;
		private System.Windows.Forms.TabPage tabAmmo;
		private System.Windows.Forms.TabPage tabDevice;
		private System.Windows.Forms.TabPage tabRS;
		private System.Windows.Forms.TabPage tabEngine;
		private System.Windows.Forms.TabPage tabRefine;
		private System.Windows.Forms.TabPage tabManufacture;
		private System.Windows.Forms.TabPage tabEffects;
		private System.Windows.Forms.TabPage tabAddReq;
		private System.Windows.Forms.Button cmdEditManu;
		private System.Windows.Forms.Button cmdEditEffects;
		private System.Windows.Forms.ComboBox cboBeamDamageType;
		private System.Windows.Forms.TextBox txtBeamFireEffect;
		private System.Windows.Forms.Label label14;
		private System.Windows.Forms.TextBox txtBeamDamage200;
		private System.Windows.Forms.Label label16;
		private System.Windows.Forms.TextBox txtBeamDamage100;
		private System.Windows.Forms.Label label15;
		private System.Windows.Forms.TextBox txtBeamRange;
		private System.Windows.Forms.Label label17;
		private System.Windows.Forms.TextBox txtBeamEnergy;
		private System.Windows.Forms.Label label20;
		private System.Windows.Forms.TextBox txtBeamReload;
		private System.Windows.Forms.Label label22;
		private System.Windows.Forms.TabPage tabProjectile;
		private System.Windows.Forms.Label label55;
		private System.Windows.Forms.Button cmdAmmoLauncherRefresh;
		private System.Windows.Forms.ComboBox cboAmmoLauncher;
		private System.Windows.Forms.TextBox txtProjectileAmmoName;
		private System.Windows.Forms.TextBox txtProjectileEnergy;
		private System.Windows.Forms.Label label19;
		private System.Windows.Forms.TextBox txtProjectileRange;
		private System.Windows.Forms.Label label21;
		private System.Windows.Forms.TextBox txtProjectileReload200;
		private System.Windows.Forms.Label label28;
		private System.Windows.Forms.TextBox txtProjectileReload100;
		private System.Windows.Forms.Label label29;
		private System.Windows.Forms.Label label30;
		private System.Windows.Forms.TextBox txtProjectileRoundsFired;
		private System.Windows.Forms.TextBox txtMissleAmmoName;
		private System.Windows.Forms.TextBox txtMissleEnergy;
		private System.Windows.Forms.Label label18;
		private System.Windows.Forms.TextBox txtMissleReload200;
		private System.Windows.Forms.Label label24;
		private System.Windows.Forms.TextBox txtMissleReload100;
		private System.Windows.Forms.Label label25;
		private System.Windows.Forms.Label label26;
		private System.Windows.Forms.TextBox txtMissleRoundsFired;
		private System.Windows.Forms.TextBox txtAmmoRange;
		private System.Windows.Forms.Label label46;
		private System.Windows.Forms.ComboBox cboAmmoDamageType;
		private System.Windows.Forms.TextBox txtAmmoManouv;
		private System.Windows.Forms.Label label23;
		private System.Windows.Forms.TextBox txtAmmoDamage200;
		private System.Windows.Forms.Label label41;
		private System.Windows.Forms.TextBox txtAmmoDamage100;
		private System.Windows.Forms.Label label43;
		private System.Windows.Forms.Label label44;
		private System.Windows.Forms.TextBox txtAmmoVisual;
		private System.Windows.Forms.Label label47;
		private System.Windows.Forms.TextBox txtDeviceCooldown;
		private System.Windows.Forms.Label label51;
		private System.Windows.Forms.TextBox txtDeviceRange;
		private System.Windows.Forms.Label label48;
		private System.Windows.Forms.TextBox txtDeviceEnergy200;
		private System.Windows.Forms.Label label49;
		private System.Windows.Forms.TextBox txtDeviceEnergy100;
		private System.Windows.Forms.Label label50;
		private System.Windows.Forms.TextBox txtRSCapacity;
		private System.Windows.Forms.Label label32;
		private System.Windows.Forms.TextBox txtRSRecharge200;
		private System.Windows.Forms.Label label33;
		private System.Windows.Forms.TextBox txtRSRecharge100;
		private System.Windows.Forms.Label label34;
		private System.Windows.Forms.Label label35;
		private System.Windows.Forms.TextBox txtEngineSignature200;
		private System.Windows.Forms.Label label31;
		private System.Windows.Forms.TextBox txtEngineSignature100;
		private System.Windows.Forms.Label label36;
		private System.Windows.Forms.TextBox txtEngineThrust200;
		private System.Windows.Forms.Label label37;
		private System.Windows.Forms.TextBox txtEngineWarpCharge;
		private System.Windows.Forms.Label label38;
		private System.Windows.Forms.TextBox txtEngineWarpSpeed;
		private System.Windows.Forms.Label label39;
		private System.Windows.Forms.Label label45;
		private System.Windows.Forms.TextBox txtEngineThrust100;
		private System.Windows.Forms.TextBox txtEngineFreeWarp200;
		private System.Windows.Forms.Label label52;
		private System.Windows.Forms.TextBox txtEngineFreeWarp100;
        private System.Windows.Forms.Label label53;
        private System.Windows.Forms.TextBox txtType;
        private System.Windows.Forms.Label lblType;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem menuFile;
        private System.Windows.Forms.ToolStripMenuItem menuExit;
        private System.Windows.Forms.ToolStripMenuItem menuDatabase;
        private System.Windows.Forms.ToolStripMenuItem itemsToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem menuCategories;
        private System.Windows.Forms.ToolStripMenuItem menuSubCategories;
        private System.Windows.Forms.ToolStripMenuItem menuItems;
        private System.Windows.Forms.ToolStripMenuItem menuMissions;
        private System.Windows.Forms.ToolStripMenuItem menuManufacturers;
        private System.Windows.Forms.ToolStripMenuItem menuFactions;
        private System.Windows.Forms.ToolStrip toolbar;
        private System.Windows.Forms.ToolStripButton btnNew;
        private System.Windows.Forms.ToolStripButton btnSave;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripButton btnDelete;
        private System.Windows.Forms.ToolStripButton btnCancel;
        private System.Windows.Forms.ToolStripMenuItem menuEdit;
        private System.Windows.Forms.ToolStripMenuItem menuAdd;
        private System.Windows.Forms.ToolStripMenuItem menuDelete;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem menuSave;
        private System.Windows.Forms.ToolStripMenuItem menuCancel;
        private System.Windows.Forms.ToolStripButton btnSearch;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.ToolStripLabel lblSearch;
        private System.Windows.Forms.ToolStripTextBox flnSearch;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator5;
        private System.Windows.Forms.ToolStripMenuItem menuSearch;


	}
}

