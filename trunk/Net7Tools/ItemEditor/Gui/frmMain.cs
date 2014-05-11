using System;
using System.Windows.Forms;
using MySql.Data.MySqlClient;
using System.Data;
using Net7_Tools.Database;
using System.Collections.Generic;
using Net7_Tools.Editor;
using Net7_Tools.Search;
using Net7_Tools.Widgets;
using Net7_Tools.Editors;
using CommonTools.Database;

namespace Net7_Tools
{
    public partial class frmItems : Form, IGui
	{
        public enum EditorType
        {
            Categories, SubCategories, Manufacturers, Factions, Missions,
            Items, Items_Ammo, Items_Beam, Items_Projectile, Items_Missile,
            Items_Device, Items_Engine,Items_Reactor, Items_Shield
        };
        public enum SingleTableEditor
        {
            Manufacturing
        };
        public enum SearchDialog
        {
            Item
        };

        private Gui.FrmItemValidationReport m_itemValidationReport = null;
        private IEditor m_currentEditor;
        private EditorType m_editorType;
        private System.Collections.Generic.Dictionary<EditorType, IEditor> listEditors;
        private System.Collections.Generic.Dictionary<SearchDialog, ISearchDialog> listSearchDialogs;
        private FrmItemTree m_itemSearch = new FrmItemTree();
        // Ungrouped tables
        private TableEditor m_tableEditor = new TableEditor();

		public frmItems()
		{
			InitializeComponent();

            this.Text = this.Text + CommonTools.Gui.LoginData.ApplicationVersion;

            listEditors = new System.Collections.Generic.Dictionary<EditorType, IEditor>();
            listSearchDialogs = new Dictionary<SearchDialog, ISearchDialog>();
            listSearchDialogs.Add(SearchDialog.Item, m_itemSearch);

            Restrictions.setRestrictions(chkTerranRest, chkJenquaiRest, chkProjenRest, raceRestrictionsLbl,
                                         chkWarriorRest, chkTraderRest, chkExplorerRest, professionRestrictionsLbl);

            TableIO itemsTableIO = initializeItems();
            TableIO ammoTableIO = initializeItemAmmo();
            TableIO beamTableIO = initializeItemBeam();
            TableIO projectileTableIO = initializeProjectile();
            TableIO missileTableIO = initializeMissile();
            TableIO deviceTableIO = initializeDevice();
            TableIO engineTableIO = initializeEngine();
            TableIO reactorTableIO = initializeReactor();
            TableIO shieldTableIO = initializeShield();
            TableIO manufacturingTableIO = initializeItemManufacturing(m_itemSearch);
            manufacturingTableIO.setRecordOptional();

            // Ungrouped tables
            m_tableEditor.add(SingleTableEditor.Manufacturing, manufacturingTableIO);

            // Ammo
            ItemEditor itemEditor = new ItemEditor(m_itemSearch);
            itemEditor.addTableHandler(tabBase, itemsTableIO);
            itemEditor.addTableHandler(tabAmmo, ammoTableIO);
            itemEditor.addTableHandler(null, manufacturingTableIO);
            listEditors.Add(EditorType.Items_Ammo, itemEditor);

            // Beam
            itemEditor = new ItemEditor(m_itemSearch);
            itemEditor.addTableHandler(tabBase, itemsTableIO);
            itemEditor.addTableHandler(tabBeam, beamTableIO);
            itemEditor.addTableHandler(null, manufacturingTableIO);
            listEditors.Add(EditorType.Items_Beam, itemEditor);

            // Projectile
            itemEditor = new ItemEditor(m_itemSearch);
            itemEditor.addTableHandler(tabBase, itemsTableIO);
            itemEditor.addTableHandler(tabProjectile, projectileTableIO);
            itemEditor.addTableHandler(null, manufacturingTableIO);
            listEditors.Add(EditorType.Items_Projectile, itemEditor);

            // Missile
            itemEditor = new ItemEditor(m_itemSearch);
            itemEditor.addTableHandler(tabBase, itemsTableIO);
            itemEditor.addTableHandler(tabMissile, missileTableIO);
            itemEditor.addTableHandler(null, manufacturingTableIO);
            listEditors.Add(EditorType.Items_Missile, itemEditor);

            // Device
            itemEditor = new ItemEditor(m_itemSearch);
            itemEditor.addTableHandler(tabBase, itemsTableIO);
            itemEditor.addTableHandler(tabDevice, deviceTableIO);
            itemEditor.addTableHandler(null, manufacturingTableIO);
            listEditors.Add(EditorType.Items_Device, itemEditor);

            // Engine
            itemEditor = new ItemEditor(m_itemSearch);
            itemEditor.addTableHandler(tabBase, itemsTableIO);
            itemEditor.addTableHandler(tabEngine, engineTableIO);
            itemEditor.addTableHandler(null, manufacturingTableIO);
            listEditors.Add(EditorType.Items_Engine, itemEditor);

            // Reactor
            itemEditor = new ItemEditor(m_itemSearch);
            itemEditor.addTableHandler(tabBase, itemsTableIO);
            itemEditor.addTableHandler(tabRS, reactorTableIO);
            itemEditor.addTableHandler(null, manufacturingTableIO);
            listEditors.Add(EditorType.Items_Reactor, itemEditor);

            // Shield
            itemEditor = new ItemEditor(m_itemSearch);
            itemEditor.addTableHandler(tabBase, itemsTableIO);
            itemEditor.addTableHandler(tabRS, shieldTableIO);
            itemEditor.addTableHandler(null, manufacturingTableIO);
            listEditors.Add(EditorType.Items_Shield, itemEditor);

            // Component
            itemEditor = new ItemEditor(m_itemSearch);
            itemEditor.addTableHandler(tabBase, itemsTableIO);
            itemEditor.addTableHandler(null, manufacturingTableIO);
            listEditors.Add(EditorType.Items, itemEditor);

            // Select the default editor
            setEditor(EditorType.Items, true);
		}

        public TableInfo getTableInfo(SingleTableEditor singleTableEditor)
        {
            return m_tableEditor.getTableInfo(singleTableEditor);
        }

        /// <summary>
        ///   <para>Set the tab pages to display</para>
        /// </summary>
        public void setTabPages(TabPage[] listTabPages)
        {
            foreach (TabPage tabPage in tabsMain.TabPages)
            {
                tabsMain.TabPages.Remove(tabPage);
            }
            tabsMain.TabPages.AddRange(listTabPages);
        }

        /// <summary>
        ///   <para>Initialize the item page</para>
        /// </summary>
        /// <remarks>There's a lot of cross-reference among the classes</remarks>
        private TableIO initializeItems()
        {
            ItemRecordManager itemRecordManager = new ItemRecordManager(this,
                                                        cboCategory,
                                                        cboSubCategory,
                                                        baseTypeCbo,
                                                        base2DAssetTxt,
                                                        base3DAssetTxt,
                                                        baseBaseItemTxt,
                                                        baseBaseItemBtn,
                                                        baseManufacturingBtn,
                                                        guiEquipVisualEffectTxt);
            TableIO tableIO = new TableIO(this, itemRecordManager);
            DataBinding dataBinding;

            dataBinding = tableIO.bind(txtId, Net7.Table_item_base._id, "ID");
            tableIO.addFieldNeverEnabled(txtId);

            dataBinding = tableIO.bind(baseStatusCbo, Net7.Table_item_base._status, "Status");

            dataBinding = tableIO.bind(txtDescription, Net7.Table_item_base._description, "Description");
            //dataBinding.addValidation(new ValidateNonEmpty());

            tableIO.bind(cboCategory, Net7.Table_item_base._category, "Category");
            //tableIO.addFieldEnabledOnlyOnAdd(cboCategory);

            tableIO.bind(cboSubCategory, Net7.Table_item_base._sub_category, "Sub-Category");
            //tableIO.addFieldEnabledOnlyOnAdd(cboSubCategory);

            dataBinding = tableIO.bind(baseTypeCbo, Net7.Table_item_base._type, "Type");
            baseTypeCbo.Items.Clear();
            baseTypeCbo.Items.AddRange(Database.Database.getItemTypes());

            tableIO.bind(cboManufacturer, Net7.Table_item_base._manufacturer, "Manufacturer");

            dataBinding = tableIO.bind(txtPrice, Net7.Table_item_base._price, "Price");
            dataBinding.addValidation(new ValidateNumericGreaterEqual(0));

            tableIO.bind(cboLevel, Net7.Table_item_base._level, "Level");
            // This value is self-validated

            dataBinding = tableIO.bind(txtStackSize, Net7.Table_item_base._max_stack, "Stack Size");
            dataBinding.addValidation(new ValidateInteger());
            dataBinding.addValidation(new ValidateNumericGreater(0));
            dataBinding.addValidation(new ValidateNumericLesserEqual(60000));

            dataBinding = tableIO.bind(txtName, Net7.Table_item_base._name, "Name");
            dataBinding.addValidation(new ValidateNonEmpty());

            dataBinding = tableIO.bind(base2DAssetTxt, Net7.Table_item_base._2d_asset, "2D Asset");
            dataBinding.addValidation(new ValidateInteger());
            dataBinding.addValidation(new ValidateNumericGreaterEqual(0));
            base2DAssetBtn.Click += new EventHandler(itemRecordManager.base2DAssetBtn_Click);
            new AssetToolip(base2DAssetBtn, base2DAssetTxt);

            dataBinding = tableIO.bind(base3DAssetTxt, Net7.Table_item_base._3d_asset, "3D Asset");
            dataBinding.addValidation(new ValidateInteger());
            dataBinding.addValidation(new ValidateNumericGreaterEqual(0));
            base3DAssetBtn.Click += new EventHandler(itemRecordManager.base3DAssetBtn_Click);
            new AssetToolip(base3DAssetBtn, base3DAssetTxt);

            tableIO.addFieldEnabledOnlyOnView(baseManufacturingBtn);
            tableIO.bind(baseManufacturingBtn, null, "");
            baseManufacturingBtn.Click += new EventHandler(itemRecordManager.baseManufacturingBtn_Click);

            dataBinding = tableIO.bind(baseCustom, Net7.Table_item_base._custom_flag, "Custom item");
            baseCustom.CheckedChanged += new System.EventHandler(itemRecordManager.onCustomItem);

            tableIO.bind(baseBaseItemTxt, Net7.Table_item_base._item_base_id, "Item base ID");
            baseBaseItemTxt.EnabledChanged += new EventHandler(itemRecordManager.onBaseItemEnabledChanged);
            new AssetToolip(baseBaseItemBtn, baseBaseItemTxt);
            baseBaseItemBtn.Click += new EventHandler(itemRecordManager.baseItemBtn_Click);

            dataBinding = tableIO.bind(guiEquipVisualEffectTxt, Net7.Table_item_base._effect_id, "Effect ID");
            dataBinding.addValidation(new ValidateInteger());
            dataBinding.addValidation(new ValidateNumericGreaterEqual(0));
            guiEquipVisualEffectBtn.Click += new EventHandler(itemRecordManager.equipVisualEffectBtn_Click);

            // These values are self-validated
            tableIO.bind(chkNoTrade, Net7.Table_item_base._no_trade, "No Tradable");
            tableIO.bind(chkNoStore, Net7.Table_item_base._no_store, "No Storable");
            tableIO.bind(chkNoDestroy, Net7.Table_item_base._no_destroy, "No Destroyable");
            tableIO.bind(chkNoManu, Net7.Table_item_base._no_manu, "Not manufacturable");
            tableIO.bind(chkUnique, Net7.Table_item_base._unique, "Unique");

            // Initialize the item status
            baseStatusCbo.Items.Clear();
            baseStatusCbo.Items.AddRange(Database.Database.getItemStatus());

            // Initialize the item levels
            cboLevel.Items.Clear();
            cboLevel.Items.AddRange(ItemRecordManager.getLevels());

            // Initialize the list of manufacturers
            cboManufacturer.Items.Clear();
            cboManufacturer.Items.AddRange(Database.Database.getManufacturers());

            // Initialize the list of categories
            cboCategory.Items.Clear();
            cboCategory.Items.AddRange(Database.Database.getItemCategories());
            cboCategory.SelectionChangeCommitted += new EventHandler(itemRecordManager.onCategorySelectionChanged);

            // Initialize the sub-categories
            cboSubCategory.SelectionChangeCommitted += new EventHandler(itemRecordManager.onSubCategorySelectionChanged);

            tableIO.initialize(Net7.Tables.item_base.ToString(), Net7.Table_item_base._id, Database.Database.getQueryItem());

            itemRecordManager.setTabPages(Items.ItemSubCategory.Ammo, new TabPage[] { tabBase, tabAmmo });
            itemRecordManager.setTabPages(Items.ItemSubCategory.Beam_Weapon, new TabPage[] { tabBase, tabBeam });
            itemRecordManager.setTabPages(Items.ItemSubCategory.Projectile_Launcher, new TabPage[] { tabBase, tabProjectile });
            itemRecordManager.setTabPages(Items.ItemSubCategory.Missile_Launcher, new TabPage[] { tabBase, tabMissile });
            itemRecordManager.setTabPages(Items.ItemSubCategory.Device, new TabPage[] { tabBase, tabDevice });
            itemRecordManager.setTabPages(Items.ItemSubCategory.Engine, new TabPage[] { tabBase, tabEngine });
            itemRecordManager.setTabPages(Items.ItemSubCategory.Reactor, new TabPage[] { tabBase, tabRS });
            itemRecordManager.setTabPages(Items.ItemSubCategory.Shield, new TabPage[] { tabBase, tabRS });
            itemRecordManager.setTabPages(Items.ItemSubCategory.Other, new TabPage[] { tabBase });

            /*itemRecordManager.setTabPages(Items.ItemCategory.Consumable, new TabPage[] { tabBase });
            itemRecordManager.setTabPages(Items.ItemCategory.Electronic_Item, new TabPage[] { tabBase });
            itemRecordManager.setTabPages(Items.ItemCategory.Reactor_Component, new TabPage[] { tabBase });
            itemRecordManager.setTabPages(Items.ItemCategory.Fabricated_Item, new TabPage[] { tabBase });
            itemRecordManager.setTabPages(Items.ItemCategory.Weapon_Component, new TabPage[] { tabBase });
            itemRecordManager.setTabPages(Items.ItemCategory.Ammo_Component, new TabPage[] { tabBase });
            itemRecordManager.setTabPages(Items.ItemCategory.Refined_Resource, new TabPage[] { tabBase });
            itemRecordManager.setTabPages(Items.ItemCategory.Raw_Resource, new TabPage[] { tabBase });
            itemRecordManager.setTabPages(Items.ItemCategory.Trade_Good, new TabPage[] { tabBase });*/

            return tableIO;
        }

        private TableIO initializeItemAmmo()
        {
            ItemAmmoRecordManager itemRecordManager = new ItemAmmoRecordManager(this);
            TableIO tableIO = new TableIO(this, itemRecordManager);
            DataBinding dataBinding;

            dataBinding = tableIO.bind(ammoLauncherTypeCbo, Net7.Table_item_ammo._ammo_type_id, "Ammo Type");

            dataBinding = tableIO.bind(ammoDamageTypeCbo, Net7.Table_item_ammo._damage_type, "Damage Type");

            dataBinding = tableIO.bind(ammoVisualEffectTxt, Net7.Table_item_ammo._fire_effect, "Visual Effect");
            dataBinding.addValidation(new ValidateNumericGreaterEqual(0));

            dataBinding = tableIO.bind(ammoRange100Txt, Net7.Table_item_ammo._range_100, "Range 100%");
            dataBinding.addValidation(new ValidateInteger());
            dataBinding.addValidation(new ValidateNumericGreaterEqual(0));

            dataBinding = tableIO.bind(ammoDamage100Txt, Net7.Table_item_ammo._damage_100, "Damage 100%");
            dataBinding.addValidation(new ValidateNumericGreaterEqual(0));

            tableIO.bind(null, Net7.Table_item_ammo._maneuv_100, "");

            tableIO.initialize(Net7.Tables.item_ammo.ToString(), Net7.Table_item_ammo._item_id, Database.Database.getQueryAmmo());

            return tableIO;
        }

        private TableIO initializeItemBeam()
        {
            ItemBeamRecordManager itemRecordManager = new ItemBeamRecordManager(this);
            TableIO tableIO = new TableIO(this, itemRecordManager);
            DataBinding dataBinding;

            tableIO.bind(beamTypeCbo, Net7.Table_item_beam._damage_type, "Beam Type");
            beamTypeCbo.Items.Clear();
            beamTypeCbo.Items.AddRange(Database.Database.getItemAmmoTypes(100));

            dataBinding = tableIO.bind(beamFireEffectTxt, Net7.Table_item_beam._fire_effect, "Visual Effect");
            dataBinding.addValidation(new ValidateNumericGreaterEqual(0));

            dataBinding = tableIO.bind(beamRange100Txt, Net7.Table_item_beam._range_100, "Range 100%");
            dataBinding.addValidation(new ValidateInteger());
            dataBinding.addValidation(new ValidateNumericGreaterEqual(0));

            dataBinding = tableIO.bind(beamEnergy100Txt, Net7.Table_item_beam._energy_100, "Energy 100%");
            dataBinding.addValidation(new ValidateInteger());
            dataBinding.addValidation(new ValidateNumericGreaterEqual(0));

            dataBinding = tableIO.bind(beamReload100Txt, Net7.Table_item_beam._reload_100, "Reload 100%");
            dataBinding.addValidation(new ValidateNumericGreaterEqual(0));

            dataBinding = tableIO.bind(beamDamage100Txt, Net7.Table_item_beam._damage_100, "Damage 100%");
            dataBinding.addValidation(new ValidateNumericGreaterEqual(0));

            // The actual field is unimportant.  All that matters is that we get a call to RecordManager
            tableIO.bind(raceRestrictionsLbl, Net7.Table_item_beam._rest_race, "");
            tableIO.bind(professionRestrictionsLbl, Net7.Table_item_beam._rest_prof, "");

            tableIO.initialize(Net7.Tables.item_beam.ToString(), Net7.Table_item_beam._item_id, Database.Database.getQueryBeam());

            return tableIO;
        }

        private TableIO initializeProjectile()
        {
            ItemProjectileRecordManager itemRecordManager = new ItemProjectileRecordManager(this);
            TableIO tableIO = new TableIO(this, itemRecordManager);
            DataBinding dataBinding;

            dataBinding = tableIO.bind(null, Net7.Table_item_projectile._ammo, "");

            dataBinding = tableIO.bind(projectileTypeCbo, Net7.Table_item_projectile._ammo_type_id, "Ammo Type");
            projectileTypeCbo.Items.Clear();
            projectileTypeCbo.Items.AddRange(Database.Database.getItemAmmoTypes(101));

            dataBinding = tableIO.bind(projectileRoundsFiredTxt, Net7.Table_item_projectile._ammo_per_shot, "Rounds fired");
            dataBinding.addValidation(new ValidateInteger());
            dataBinding.addValidation(new ValidateNumericGreaterEqual(0));

            dataBinding = tableIO.bind(projectileRangeTxt, Net7.Table_item_projectile._range_100, "Range 100%");
            dataBinding.addValidation(new ValidateInteger());
            dataBinding.addValidation(new ValidateNumericGreaterEqual(0));

            dataBinding = tableIO.bind(projectileEnergyTxt, Net7.Table_item_projectile._energy_100, "Energy 100%");
            dataBinding.addValidation(new ValidateInteger());
            dataBinding.addValidation(new ValidateNumericGreaterEqual(0));

            dataBinding = tableIO.bind(projectileReload100Txt, Net7.Table_item_projectile._reload_100, "Reload 100%");
            dataBinding.addValidation(new ValidateNumericGreaterEqual(0));

            // The actual field is unimportant.  All that matters is that we get a call to RecordManager
            dataBinding = tableIO.bind(raceRestrictionsLbl, Net7.Table_item_projectile._rest_race, "");
            dataBinding = tableIO.bind(professionRestrictionsLbl, Net7.Table_item_projectile._rest_prof, "");

            tableIO.initialize(Net7.Tables.item_projectile.ToString(), Net7.Table_item_projectile._item_id, Database.Database.getQueryProjectile());

            return tableIO;
        }

        private TableIO initializeMissile()
        {
            ItemMissileRecordManager itemRecordManager = new ItemMissileRecordManager(this);
            TableIO tableIO = new TableIO(this, itemRecordManager);
            DataBinding dataBinding;

            dataBinding = tableIO.bind(missileTypeCbo, Net7.Table_item_missile._ammo_type_id, "Ammo Type");
            missileTypeCbo.Items.Clear();
            missileTypeCbo.Items.AddRange(Database.Database.getItemAmmoTypes(102));

            dataBinding = tableIO.bind(missileRoundsFiredTxt, Net7.Table_item_missile._ammo_per_shot, "Rounds fired");
            dataBinding.addValidation(new ValidateNumericGreaterEqual(0));

            dataBinding = tableIO.bind(missileEnergyTxt, Net7.Table_item_missile._energy_100, "Energy");
            dataBinding.addValidation(new ValidateInteger());
            dataBinding.addValidation(new ValidateNumericGreaterEqual(0));

            dataBinding = tableIO.bind(missileReload100Txt, Net7.Table_item_missile._reload_100, "Reload 100%");
            dataBinding.addValidation(new ValidateNumericGreaterEqual(0));

            // The actual field is unimportant.  All that matters is that we get a call to RecordManager
            tableIO.bind(raceRestrictionsLbl, Net7.Table_item_missile._rest_race, "");
            tableIO.bind(professionRestrictionsLbl, Net7.Table_item_missile._rest_prof, "");

            tableIO.initialize(Net7.Tables.item_missile.ToString(), Net7.Table_item_missile._item_id, Database.Database.getQueryMissile());

            return tableIO;
        }

        private TableIO initializeDevice()
        {
            ItemDeviceRecordManager itemRecordManager = new ItemDeviceRecordManager(this);
            TableIO tableIO = new TableIO(this, itemRecordManager);
            DataBinding dataBinding;

            dataBinding = tableIO.bind(deviceRangeTxt, Net7.Table_item_device._range_100, "Range 100%");
            dataBinding.addValidation(new ValidateInteger());
            dataBinding.addValidation(new ValidateNumericGreaterEqual(0));

            dataBinding = tableIO.bind(deviceEnergy100Txt, Net7.Table_item_device._energy_100, "Energy 100%");
            dataBinding.addValidation(new ValidateInteger());
            dataBinding.addValidation(new ValidateNumericGreaterEqual(0));

            // The actual field is unimportant.  All that matters is that we get a call to RecordManager
            dataBinding = tableIO.bind(raceRestrictionsLbl, Net7.Table_item_device._rest_race, "");
            dataBinding = tableIO.bind(professionRestrictionsLbl, Net7.Table_item_device._rest_prof, "");

            tableIO.initialize(Net7.Tables.item_device.ToString(), Net7.Table_item_device._item_id, Database.Database.getQueryDevice());

            return tableIO;
        }

        private TableIO initializeEngine()
        {
            ItemEngineRecordManager itemRecordManager = new ItemEngineRecordManager(this);
            TableIO tableIO = new TableIO(this, itemRecordManager);
            DataBinding dataBinding;

            dataBinding = tableIO.bind(engineWarpSpeedTxt, Net7.Table_item_engine._warp, "Warp speed");
            dataBinding.addValidation(new ValidateInteger());
            dataBinding.addValidation(new ValidateNumericGreaterEqual(0));

            dataBinding = tableIO.bind(engineSignature100Txt, Net7.Table_item_engine._signature_100, "Signature 100%");
            dataBinding.addValidation(new ValidateInteger());
            dataBinding.addValidation(new ValidateNumericGreaterEqual(0));

            dataBinding = tableIO.bind(engineThrust100Txt, Net7.Table_item_engine._thrust_100, "Thrust 100%");
            dataBinding.addValidation(new ValidateInteger());
            dataBinding.addValidation(new ValidateNumericGreaterEqual(0));

            dataBinding = tableIO.bind(engineWarpDrain100Txt, Net7.Table_item_engine._warp_drain_100, "Warp drain 100%");
            dataBinding.addValidation(new ValidateNumericGreaterEqual(0));

            tableIO.bind(null, Net7.Table_item_engine._energy_100, "");
            tableIO.bind(null, Net7.Table_item_engine._range_100, "");

            // The actual field is unimportant.  All that matters is that we get a call to RecordManager
            tableIO.bind(raceRestrictionsLbl, Net7.Table_item_engine._rest_race, "");
            tableIO.bind(professionRestrictionsLbl, Net7.Table_item_engine._rest_prof, "");

            tableIO.initialize(Net7.Tables.item_engine.ToString(), Net7.Table_item_engine._item_id, Database.Database.getQueryEngine());

            return tableIO;
        }

        private TableIO initializeReactor()
        {
            ItemReactorRecordManager itemRecordManager = new ItemReactorRecordManager(this);
            TableIO tableIO = new TableIO(this, itemRecordManager);
            DataBinding dataBinding;

            dataBinding = tableIO.bind(reactorCapacityTxt, Net7.Table_item_reactor._cap_100, "Capacity");
            dataBinding.addValidation(new ValidateInteger());
            dataBinding.addValidation(new ValidateNumericGreaterEqual(0));

            dataBinding = tableIO.bind(reactorRecharge100Txt, Net7.Table_item_reactor._recharge_100, "Recharge 100%");
            dataBinding.addValidation(new ValidateNumericGreaterEqual(0));

            dataBinding = tableIO.bind(null, Net7.Table_item_reactor._energy_100, "");
            dataBinding = tableIO.bind(null, Net7.Table_item_reactor._range_100, "");

            // The actual field is unimportant.  All that matters is that we get a call to RecordManager
            tableIO.bind(raceRestrictionsLbl, Net7.Table_item_reactor._rest_race, "");
            tableIO.bind(professionRestrictionsLbl, Net7.Table_item_reactor._rest_prof, "");

            tableIO.initialize(Net7.Tables.item_reactor.ToString(), Net7.Table_item_reactor._item_id, Database.Database.getQueryReactor());

            return tableIO;
        }

        private TableIO initializeShield()
        {
            ItemShieldRecordManager itemRecordManager = new ItemShieldRecordManager(this);
            TableIO tableIO = new TableIO(this, itemRecordManager);
            DataBinding dataBinding;

            dataBinding = tableIO.bind(reactorCapacityTxt, Net7.Table_item_shield._cap_100, "Capacity");
            dataBinding.addValidation(new ValidateInteger());
            dataBinding.addValidation(new ValidateNumericGreaterEqual(0));

            dataBinding = tableIO.bind(reactorRecharge100Txt, Net7.Table_item_shield._recharge_100, "Recharge 100%");
            dataBinding.addValidation(new ValidateNumericGreaterEqual(0));

            dataBinding = tableIO.bind(null, Net7.Table_item_shield._energy_100, "");
            dataBinding = tableIO.bind(null, Net7.Table_item_shield._range_100, "");

            // The actual field is unimportant.  All that matters is that we get a call to RecordManager
            tableIO.bind(raceRestrictionsLbl, Net7.Table_item_shield._rest_race, "");
            tableIO.bind(professionRestrictionsLbl, Net7.Table_item_shield._rest_prof, "");

            tableIO.initialize(Net7.Tables.item_shield.ToString(), Net7.Table_item_shield._item_id, Database.Database.getQueryShield());

            return tableIO;
        }

        private TableIO initializeItemManufacturing(ISearchDialog itemSearchDialog)
        {
            Net7_Tools.Gui.frmManufacturing manufacturingDlg = new Net7_Tools.Gui.frmManufacturing();
            ItemManufacturingRecordManager itemRecordManager = new ItemManufacturingRecordManager(manufacturingDlg, this);
            TableIO tableIO = new TableIO(this, itemRecordManager);
            manufacturingDlg.setTableIO(tableIO);
            DataBinding dataBinding;

            dataBinding = tableIO.bind(manufacturingDlg.manufacturingCboDifficulty, Net7.Table_item_manufacture._difficulty, "Difficulty Level");
            dataBinding.addValidation(new ValidateNumericInsideRange(1, 9));

            //dataBinding.addValidation(new ValidateInTable(Net7_db.Tables.item_base.ToString(), DB.getQueryItem(itemId));
            dataBinding = tableIO.bind(manufacturingDlg.manufacturingTxtComponent1, Net7.Table_item_manufacture._comp_1, "Component 1", false);
            dataBinding = tableIO.bind(manufacturingDlg.manufacturingTxtComponent2, Net7.Table_item_manufacture._comp_2, "Component 2", false);
            dataBinding = tableIO.bind(manufacturingDlg.manufacturingTxtComponent3, Net7.Table_item_manufacture._comp_3, "Component 3", false);
            dataBinding = tableIO.bind(manufacturingDlg.manufacturingTxtComponent4, Net7.Table_item_manufacture._comp_4, "Component 4", false);
            dataBinding = tableIO.bind(manufacturingDlg.manufacturingTxtComponent5, Net7.Table_item_manufacture._comp_5, "Component 5", false);
            dataBinding = tableIO.bind(manufacturingDlg.manufacturingTxtComponent6, Net7.Table_item_manufacture._comp_6, "Component 6", false);

            // Initialize the manufacturing difficulty levels
            manufacturingDlg.manufacturingCboDifficulty.Items.Clear();
            manufacturingDlg.manufacturingCboDifficulty.Items.AddRange(Database.Database.getItemManufacturingLevels());

            // Configure the 6 component fields, search buttons, and descriptions
            FieldSearchDescription.Data fieldSearchDescriptionData
                    = new FieldSearchDescription.Data(
                            itemRecordManager,
                            itemSearchDialog,
                            Database.Database.getQueryItem_param(),
                            DB.QueryParameterCharacter + ColumnData.GetName(Net7.Table_item_base._id),
                            Net7.Table_item_base._name);
            itemRecordManager.setFieldSearchDescriptionData(fieldSearchDescriptionData);
            itemRecordManager.addComponentFields(new FieldSearchDescription.Fields(manufacturingDlg.manufacturingTxtComponent1, manufacturingDlg.manufacturingBtnComponent1, manufacturingDlg.manufacturingTxtComponentDesc1));
            itemRecordManager.addComponentFields(new FieldSearchDescription.Fields(manufacturingDlg.manufacturingTxtComponent2, manufacturingDlg.manufacturingBtnComponent2, manufacturingDlg.manufacturingTxtComponentDesc2));
            itemRecordManager.addComponentFields(new FieldSearchDescription.Fields(manufacturingDlg.manufacturingTxtComponent3, manufacturingDlg.manufacturingBtnComponent3, manufacturingDlg.manufacturingTxtComponentDesc3));
            itemRecordManager.addComponentFields(new FieldSearchDescription.Fields(manufacturingDlg.manufacturingTxtComponent4, manufacturingDlg.manufacturingBtnComponent4, manufacturingDlg.manufacturingTxtComponentDesc4));
            itemRecordManager.addComponentFields(new FieldSearchDescription.Fields(manufacturingDlg.manufacturingTxtComponent5, manufacturingDlg.manufacturingBtnComponent5, manufacturingDlg.manufacturingTxtComponentDesc5));
            itemRecordManager.addComponentFields(new FieldSearchDescription.Fields(manufacturingDlg.manufacturingTxtComponent6, manufacturingDlg.manufacturingBtnComponent6, manufacturingDlg.manufacturingTxtComponentDesc6));

            tableIO.initialize(Net7.Tables.item_manufacture.ToString(), Net7.Table_item_manufacture._item_id, Database.Database.getQueryManufacturing());

            return tableIO;
        }

        public ToolStripTextBox getSearchField()
        {
            return flnSearch;
        }

        public TabControl getTabControl()
        {
            return tabsMain;
        }

        /// <summary>
        ///   <para>Enable various GUI items based on the state of the add, delete, and saveCancel parameters</para>
        /// </summary>
        public void setGuiEnabled(Boolean add, Boolean delete, Boolean saveCancel)
        {
            btnNew.Enabled = add;
            menuAdd.Enabled = add;

            btnDelete.Enabled = delete;
            menuDelete.Enabled = delete;

            btnSave.Enabled = saveCancel;
            menuSave.Enabled = saveCancel;
            btnCancel.Enabled = saveCancel;
            menuCancel.Enabled = saveCancel;

            // Can search and switch editor when not adding nor editing
            btnSearch.Enabled = !saveCancel;
            flnSearch.Enabled = !saveCancel;
            menuDatabase.Enabled = !saveCancel;
        }

        /// <summary>
        ///   <para>Callback from the menu to activate a particular editor</para>
        /// </summary>
        public void setEditor(object sender, EventArgs e)
        {
            EditorType currentEditorType = m_editorType;
            if (sender.ToString().Equals(menuCategories.Text))
            {
                m_editorType = EditorType.Categories;
            }
            else if (sender.ToString().Equals(menuSubCategories.Text))
            {
                m_editorType = EditorType.SubCategories;
            }
            else if (sender.ToString().Equals(menuManufacturers.Text))
            {
                m_editorType = EditorType.Manufacturers;
            }
            else if (sender.ToString().Equals(menuItems.Text))
            {
                m_editorType = EditorType.Items_Ammo;
            }
            else if (sender.ToString().Equals(menuFactions.Text))
            {
                m_editorType = EditorType.Factions;
            }
            else if (sender.ToString().Equals(menuMissions.Text))
            {
                m_editorType = EditorType.Missions;
            }
            else 
            {
                MessageBox.Show("Internal error within frmMain.cs:setEditor()\rsender: '" + sender.ToString());
            }

            if (!m_editorType.Equals(currentEditorType))
            {
                setEditor(m_editorType, true);
                // TODO: Initialize the new editor
            }
        }

        /// <summary>
        ///   <para>Activate the specified editor</para>
        /// </summary>
        public void setEditor(EditorType editorType, Boolean activate)
        {
            if (listEditors.TryGetValue(editorType, out m_currentEditor))
            {
                m_editorType = editorType;
                TableIO.State state = m_currentEditor.getTableIO(tabBase).getState();
                m_currentEditor.ensureState(state);
                if (activate)
                {
                    String currentId = m_currentEditor.activate();
                    flnSearch.Text = currentId == null ? "" : currentId;
                }
            }
            else
            {
                // TODO: User feedback
                MessageBox.Show("Unable to activate the editor " + editorType.ToString());
            }
        }

        public EditorType getCurrentEditorType()
        {
            return m_editorType;
        }

        public IEditor getEditor(TabPage tabPage)
        {
            TabPage TableHandlerTabPage;
            foreach (IEditor editor in listEditors.Values)
            {
                foreach (TableHandler tableHandler in editor.getTableHandlers())
                {
                    TableHandlerTabPage = tableHandler.getTabPage();
                    if (TableHandlerTabPage != null && TableHandlerTabPage.Equals(tabPage))
                    {
                        return editor;
                    }
                }
            }
            return null;
        }

        public IEditor getEditor(EditorType editorType)
        {
            IEditor editor;
            if (!listEditors.TryGetValue(editorType, out editor))
            {
                editor = null;
            }
            return editor;
        }

        /// <summary>
        ///   <para>Retrieve the current editor</para>
        /// </summary>
        public IEditor getCurrentEditor()
        {
            return m_currentEditor;
        }

        /// <summary>
        ///   <para>The _new_ action was triggered by the GUI</para>
        /// </summary>
        private void btnNew_Click(object sender, EventArgs e)
        {
            m_currentEditor.addRecord();
        }

        /// <summary>
        ///   <para>The _delete_ action was triggered by the GUI</para>
        /// </summary>
        private void btnDelete_Click(object sender, EventArgs e)
        {
            DialogResult dialogResult = MessageBox.Show("Do you really want to delete this record?", "Deletion Confirmation", MessageBoxButtons.YesNo);
            if(dialogResult.Equals(DialogResult.Yes))
            {
                m_currentEditor.deleteRecord();
            }
        }

        /// <summary>
        ///   <para>The _save_ action was triggered by the GUI</para>
        /// </summary>
        private void btnSave_Click(object sender, EventArgs e)
        {
            m_currentEditor.saveRecord();
        }

        /// <summary>
        ///   <para>The _cancel_ action was triggered by the GUI</para>
        /// </summary>
        private void btnCancel_Click(object sender, EventArgs e)
        {
            m_currentEditor.cancelRecord();
        }

        /// <summary>
        ///   <para>A key was pressed within the search field.  Only
        ///         the &lt;enter> key is responded to.</para>
        /// </summary>
        private void txtSearch_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar.Equals('\r'))
            {
                if(!m_currentEditor.selectRecord(flnSearch.Text))
                {
                    MessageBox.Show("This record does not exist");
                }
            }
        }

        /// <summary>
        ///   <para>The search button was pressed, display the appropriate search dialog.</para>
        /// </summary>
        private void btnSearch_Click(object sender, EventArgs e)
        {
            if (flnSearch.Text.Length == 0)
            {
                // TODO: Query the current record ID and put it into the search field
            }

            String foundItemId = m_currentEditor.getIdFromSearchDialog(flnSearch.Text);
            if (foundItemId != null && foundItemId.Length != 0)
            {
                flnSearch.Text = foundItemId;
                if (!m_currentEditor.selectRecord(flnSearch.Text))
                {
                    MessageBox.Show("This record does not exist");
                }
            }
        }

        public ISearchDialog getSearchDialog(SearchDialog searchDialog)
        {
            ISearchDialog iSearchDialog;
            if (!listSearchDialogs.TryGetValue(searchDialog, out iSearchDialog))
            {
                iSearchDialog = null;
            }
            return iSearchDialog;
        }

        private void onItemValidationReport(object sender, EventArgs e)
        {
            if (m_itemValidationReport == null)
            {
                IEditor itemEditor = (IEditor)getEditor(EditorType.Items);
                ItemRecordManager itemRecordManager = (ItemRecordManager)itemEditor.getTableIO(tabBase).getRecordManager();
                m_itemValidationReport = new Gui.FrmItemValidationReport(itemRecordManager, this);
            }
            m_itemValidationReport.ShowDialog();

            // TODO: Item Validation Report should not change the TabControl
            getTabControl().SelectedTab = getTabControl().TabPages[0]; // Temporary fix
            
            // TODO: ItemValidationReport as a search dialog
            // Call an internal show and pass whether the Editor is Items
            // If it is then allow a Select button such that the report acts as a Search dialog
        }

        private void onExit(object sender, EventArgs e)
        {
            Application.Exit();
        }


    }
}
