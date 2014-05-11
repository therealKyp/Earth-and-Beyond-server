using System;
using System.Windows.Forms;
using MySql.Data.MySqlClient;
using System.Data;
using Net7_Tools.Database;

namespace Net7_Tools
{
    public partial class frmItems : Form, IGui
	{
        public enum EditorType { Categories, SubCategories, Manufacturers, Items, Factions, Missions };

        private TableIO m_currentTableIO;
        private EditorType m_editorType;
        private System.Collections.Generic.Dictionary<EditorType, TableIO> listEditors;


		public frmItems()
		{
			InitializeComponent();

            listEditors = new System.Collections.Generic.Dictionary<EditorType, TableIO>();

            listEditors.Add(EditorType.Items, initializeItems());
            // TODO: Add other editors here
            setEditor(EditorType.Items);

            // TODO: Checksum the current DB schema and if need-be call: generateDatabaseEnumerations();
		}

        /// <summary>
        ///   <para>Generate the database enumerations into the
        ///         net7_db.cs and net7.cs files.</para>
        /// </summary>
        /// <remarks>Use with care.</remarks>
        public void generateDatabaseEnumerations()
        {
            DB.makeDatabaseVariables(DB.DatabaseName.net7_db);
            DB.makeDatabaseVariables(DB.DatabaseName.net7);
        }

		public void removeAllTabs()
		{
			tabsMain.TabPages.Remove(tabAddReq);
			tabsMain.TabPages.Remove(tabAmmo);
			tabsMain.TabPages.Remove(tabBeam);
			tabsMain.TabPages.Remove(tabDevice);
			tabsMain.TabPages.Remove(tabEffects);
			tabsMain.TabPages.Remove(tabEngine);
			tabsMain.TabPages.Remove(tabManufacture);
			tabsMain.TabPages.Remove(tabMissile);
			tabsMain.TabPages.Remove(tabProjectile);
			tabsMain.TabPages.Remove(tabRefine);
			tabsMain.TabPages.Remove(tabRS);
		}

        private TableIO initializeItems()
        {
            Item itemRecord = new Item();
            TableIO tableIO = new TableIO(this, itemRecord);

            tableIO.addFieldEnabledOnlyOnAdd(txtId);

            tableIO.bind(txtId, Database_net7_db.Table_item_base._id);
            tableIO.bind(txtDescription, Database_net7_db.Table_item_base._description);
            tableIO.bind(cboCategory, Database_net7_db.Table_item_base._category);
            tableIO.bind(cboSubCategory, Database_net7_db.Table_item_base._sub_category);
            tableIO.bind(cboManufacturer, Database_net7_db.Table_item_base._manufacturer);
            tableIO.bind(txtPrice, Database_net7_db.Table_item_base._price);
            tableIO.bind(cboLevel, Database_net7_db.Table_item_base._level);
            tableIO.bind(txtStackSize, Database_net7_db.Table_item_base._max_stack);
            tableIO.bind(txtName, Database_net7_db.Table_item_base._name);
            tableIO.bind(txt2DAsset, Database_net7_db.Table_item_base._2d_asset);
            tableIO.bind(txt3DAsset, Database_net7_db.Table_item_base._3d_asset);
            tableIO.bind(txtType, Database_net7_db.Table_item_base._type);
            tableIO.bind(chkNoTrade, Database_net7_db.Table_item_base._no_trade);
            tableIO.bind(chkNoStore, Database_net7_db.Table_item_base._no_store);
            tableIO.bind(chkNoDestroy, Database_net7_db.Table_item_base._no_destroy);
            tableIO.bind(chkNoManu, Database_net7_db.Table_item_base._no_manu);
            tableIO.bind(chkUnique, Database_net7_db.Table_item_base._unique);

            //tableIO.bind(chkExplorerRest, Database_net7_db.Table_item_base._explorer_rest);
            //tableIO.bind(chkJenquaiRest, Database_net7_db.Table_item_base._jenquai_rest);
            //tableIO.bind(chkProjenRest, Database_net7_db.Table_item_base._projen_rest);
            //tableIO.bind(chkTerranRest, Database_net7_db.Table_item_base._terran_rest);
            //tableIO.bind(chkTraderRest, Database_net7_db.Table_item_base._trader_rest);
            //tableIO.bind(chkWarriorRest, Database_net7_db.Table_item_base._warrior_rest);

            InitializeLevels();
            InitializeManufacturers();
            InitializeCategories();

            tableIO.initialize(Database_net7_db.Tables.item_base.ToString(), DB.getQueryItem());

            return tableIO;
        }

        private void InitializeLevels()
        {
            cboLevel.Items.Clear();
            for (int level = 1; level < 10; ++level)
            {
                cboLevel.Items.Add(new CodeValue(level, level.ToString()));
            }
        }

        private void InitializeManufacturers()
        {
            cboManufacturer.Items.Clear();
            cboManufacturer.Items.AddRange(DB.getManufacturers());
        }

        private void InitializeCategories()
		{
			cboCategory.Items.Clear();
			AddCategory(Items.ItemCategory.Weapon);
			AddCategory(Items.ItemCategory.Device);
			AddCategory(Items.ItemCategory.Core_Item);
			//AddCategory(Items.ItemCategory.Consumable);
			AddCategory(Items.ItemCategory.Electronic_Item);
			AddCategory(Items.ItemCategory.Reactor_Component);
			AddCategory(Items.ItemCategory.Fabricated_Item);
			AddCategory(Items.ItemCategory.Weapon_Component);
			AddCategory(Items.ItemCategory.Ammo_Component);
			AddCategory(Items.ItemCategory.Refined_Resource);
			AddCategory(Items.ItemCategory.Trade_Good);
			AddCategory(Items.ItemCategory.Looted_Item);
			cboCategory.Text = "(--) Invalid Category";
			DisableSubCategories();
		}

		private void DisableSubCategories()
		{
			SetSubCategory(0);
			cboSubCategory.Enabled = false;
		}

		private void AddCategory(Items.ItemCategory Cat)
		{
			//cboCategory.Items.Add(Utility.FormParen((int)Cat, 2) + " " + Items.CategoryLookup(Cat));
            CodeValue codeValue = new CodeValue();
            codeValue.code  = (Int32) Cat;
            codeValue.value = Cat.ToString() + " (" + (Int32) Cat + ")";
            cboCategory.Items.Add(codeValue);
		}

		private void AddSubCategory(Items.ItemSubCategory SubCat)
		{
			//cboSubCategory.Items.Add(Utility.FormParen((int)SubCat, 3) + " " + Items.SubCategoryLookup(SubCat));
            CodeValue codeValue = new CodeValue();
            codeValue.code = (Int32)SubCat;
            codeValue.value = SubCat.ToString() + " (" + (Int32)SubCat + ")";
            cboSubCategory.Items.Add(codeValue);
        }

		private void SetCategory(int Cat)
		{
			if (Cat == 0)
			{
				cboCategory.SelectedIndex = -1;
				cboCategory.Text = "(---) Invalid Category";

				//Invalidate the subcategory too
				DisableSubCategories();
				return;
			}

			//We need to update the sub categories appropriately
			cboSubCategory.Items.Clear();
			cboSubCategory.Enabled = true;
			SetSubCategory(0);
			removeAllTabs();

			switch ((Items.ItemCategory)Cat)
			{
				case Items.ItemCategory.Weapon:
					cboCategory.SelectedIndex = 0;
					AddSubCategory(Items.ItemSubCategory.Beam_Weapon);
					AddSubCategory(Items.ItemSubCategory.Projectile_Launcher);
					AddSubCategory(Items.ItemSubCategory.Missle_Launcher);
					AddSubCategory(Items.ItemSubCategory.Ammo);
                    tabsMain.TabPages.Add(tabBeam);
                    tabsMain.TabPages.Add(this.tabMissile);
                    tabsMain.TabPages.Add(this.tabProjectile);
					break;

				case Items.ItemCategory.Device:
					cboCategory.SelectedIndex = 1;
					AddSubCategory(Items.ItemSubCategory.Device);
					SetSubCategory(110); //Theres one 1 anyways
                    tabsMain.TabPages.Add(this.tabDevice);
					break;

				case Items.ItemCategory.Core_Item:
					cboCategory.SelectedIndex = 2;
					AddSubCategory(Items.ItemSubCategory.Reactor);
					AddSubCategory(Items.ItemSubCategory.Engine);
					AddSubCategory(Items.ItemSubCategory.Shield);
					break;

				case Items.ItemCategory.Electronic_Item:
					cboCategory.SelectedIndex = 3;
					AddSubCategory(Items.ItemSubCategory.Software);
					AddSubCategory(Items.ItemSubCategory.Electronic_Item);
					AddSubCategory(Items.ItemSubCategory.Computer);
					break;

				case Items.ItemCategory.Reactor_Component:
					cboCategory.SelectedIndex = 4;
					AddSubCategory(Items.ItemSubCategory.Power_Converter);
					AddSubCategory(Items.ItemSubCategory.Power_Coupling);
					AddSubCategory(Items.ItemSubCategory.Power_Core);
					AddSubCategory(Items.ItemSubCategory.Power_Generator);
					break;

				case Items.ItemCategory.Fabricated_Item:
					cboCategory.SelectedIndex = 5;
					AddSubCategory(Items.ItemSubCategory.Casing);
					AddSubCategory(Items.ItemSubCategory.Engine_Frame);
					AddSubCategory(Items.ItemSubCategory.Drone_Frame);
					AddSubCategory(Items.ItemSubCategory.Mount);
					break;

				case Items.ItemCategory.Weapon_Component:
					cboCategory.SelectedIndex = 6;
					AddSubCategory(Items.ItemSubCategory.Firing_Mechanism);
					AddSubCategory(Items.ItemSubCategory.Ammunition_Feeder);
					AddSubCategory(Items.ItemSubCategory.Beam_Optic);
					AddSubCategory(Items.ItemSubCategory.Weapon_Barrel);
					break;

				case Items.ItemCategory.Ammo_Component:
					cboCategory.SelectedIndex = 7;
					AddSubCategory(Items.ItemSubCategory.Ammunition_Slug);
					AddSubCategory(Items.ItemSubCategory.Warhead);
					AddSubCategory(Items.ItemSubCategory.Propellant);
					AddSubCategory(Items.ItemSubCategory.Shell_Casing);
					break;

				case Items.ItemCategory.Refined_Resource:
					cboCategory.SelectedIndex = 8;
					AddSubCategory(Items.ItemSubCategory.Optic_Gem);
					AddSubCategory(Items.ItemSubCategory.Metal);
					AddSubCategory(Items.ItemSubCategory.Conductor);
					AddSubCategory(Items.ItemSubCategory.Alloy);
					AddSubCategory(Items.ItemSubCategory.Radioactive);
					AddSubCategory(Items.ItemSubCategory.Catalyst);
					AddSubCategory(Items.ItemSubCategory.Hydrocarbon);
					AddSubCategory(Items.ItemSubCategory.Silicate);
					AddSubCategory(Items.ItemSubCategory.Magnetic);
					AddSubCategory(Items.ItemSubCategory.Gemstone);
					AddSubCategory(Items.ItemSubCategory.Gas);
					AddSubCategory(Items.ItemSubCategory.Core_Metal);
					break;

				case Items.ItemCategory.Trade_Good:
					cboCategory.SelectedIndex = 9;
					DisableSubCategories();
					break;

				case Items.ItemCategory.Looted_Item:
					cboCategory.SelectedIndex = 10;
					DisableSubCategories();
					tabsMain.TabPages.Add(tabRefine); //Some looted items refine (V'Rix Chitin)
					break;

				default:
					cboCategory.SelectedIndex = -1;
					cboCategory.Text = "(---) Invalid Category";
					DisableSubCategories();
					break;
			}

			cboSubCategory.DropDownHeight = cboSubCategory.ItemHeight * cboSubCategory.Items.Count + 5;
			grpRestictions.Enabled = false;
		}

		private void SetSubCategory(int SubCat)
		{
			//We assume that the correct Category has been set for this
			if (!cboSubCategory.Enabled)
				return;

			if (SubCat == 0)
			{
				cboSubCategory.SelectedIndex = -1;
				cboSubCategory.Text = "(---) Invalid SubCategory";
				return;
			}

			//Make sure that the category is in the list
			bool Found = false;
			for (int i = 0; i < cboSubCategory.Items.Count; i++)
			{
				if (Utility.ParseParen(cboSubCategory.Items[i].ToString()) == SubCat)
				{
					cboSubCategory.SelectedIndex = i;
					Found = true;
					break;
				}
			}

			//If its not in the list, then the subcategory is not valid for the current category
			if (!Found)
			{
				cboSubCategory.SelectedIndex = -1;
				cboSubCategory.Text = "(---) Invalid SubCategory";
				return;
			}

			//Disable all of the optional tabs
			removeAllTabs();

			//Now add tabs based on the subcategory
			switch ((Items.ItemSubCategory)SubCat)
			{
				case Items.ItemSubCategory.Beam_Weapon:
					tabsMain.TabPages.Add(tabBeam);
					tabsMain.TabPages.Add(tabEffects);
					tabsMain.TabPages.Add(tabManufacture);
					tabsMain.TabPages.Add(tabAddReq);
					break;

				case Items.ItemSubCategory.Projectile_Launcher:
					tabsMain.TabPages.Add(tabProjectile);
					tabsMain.TabPages.Add(tabEffects);
					tabsMain.TabPages.Add(tabManufacture);
					tabsMain.TabPages.Add(tabAddReq);
					break;

				case Items.ItemSubCategory.Missle_Launcher:
					tabsMain.TabPages.Add(tabMissile);
					tabsMain.TabPages.Add(tabEffects);
					tabsMain.TabPages.Add(tabManufacture);
					tabsMain.TabPages.Add(tabAddReq);
					break;

				case Items.ItemSubCategory.Ammo:
					tabsMain.TabPages.Add(tabAmmo);
					tabsMain.TabPages.Add(tabManufacture);
					break;

				case Items.ItemSubCategory.Device:
					tabsMain.TabPages.Add(tabDevice);
					tabsMain.TabPages.Add(tabEffects);
					tabsMain.TabPages.Add(tabManufacture);
					tabsMain.TabPages.Add(tabAddReq);
					break;

				case Items.ItemSubCategory.Reactor:
				case Items.ItemSubCategory.Shield:
					tabsMain.TabPages.Add(tabRS);
					tabsMain.TabPages.Add(tabEffects);
					tabsMain.TabPages.Add(tabManufacture);
					tabsMain.TabPages.Add(tabAddReq);
					break;

				case Items.ItemSubCategory.Engine:
					tabsMain.TabPages.Add(tabEngine);
					tabsMain.TabPages.Add(tabEffects);
					tabsMain.TabPages.Add(tabManufacture);
					tabsMain.TabPages.Add(tabAddReq);
					break;

				case Items.ItemSubCategory.Optic_Gem:
				case Items.ItemSubCategory.Metal:
				case Items.ItemSubCategory.Conductor:
				case Items.ItemSubCategory.Alloy:
				case Items.ItemSubCategory.Radioactive:
				case Items.ItemSubCategory.Catalyst:
				case Items.ItemSubCategory.Hydrocarbon:
				case Items.ItemSubCategory.Silicate:
				case Items.ItemSubCategory.Magnetic:
				case Items.ItemSubCategory.Gemstone:
				case Items.ItemSubCategory.Gas:
				case Items.ItemSubCategory.Core_Metal:
					tabsMain.TabPages.Add(tabRefine);
					break;
			}
		}

		private void cboItemCategory_SelectionChangeCommitted(object sender, EventArgs e)
		{
			SetCategory(Utility.ParseParen(cboCategory.Items[cboCategory.SelectedIndex].ToString()));
		}

        private void cboSubCategory_SelectionChangeCommitted(object sender, EventArgs e)
		{
			SetSubCategory(Utility.ParseParen(cboSubCategory.Items[cboSubCategory.SelectedIndex].ToString()));
		}

		private void button1_Click(object sender, EventArgs e)
		{
			string mystr = Utility.FormDescription(txtDescription.Text);
			MessageBox.Show(mystr);
			mystr = Utility.ParseDescription(mystr);
			MessageBox.Show(mystr);
		}

		private void button2_Click(object sender, EventArgs e)
		{
			Items.ItemCategory cat;
			cat = (Items.ItemCategory)(-1);
			MessageBox.Show(cat.ToString());
        }

        public ToolStripTextBox getSearchField()
        {
            return flnSearch;
        }

        public TabControl getTabControl()
        {
            return tabsMain;
        }

        public void displayFields(DataRow dataRow)
        {
            tabsMain.SelectedIndex = 0;

            txtId.Text              = dataRow == null ? "" : ColumnData.GetString(dataRow, Database_net7_db.Table_item_base._id);
            cboLevel.SelectedIndex  = dataRow == null ? 0 : ColumnData.GetInt32(dataRow, Database_net7_db.Table_item_base._level) - 1;
            SetCategory(              dataRow == null ? 0 : ColumnData.GetInt32(dataRow, Database_net7_db.Table_item_base._category));
            SetSubCategory(           dataRow == null ? 0 : ColumnData.GetInt32(dataRow, Database_net7_db.Table_item_base._sub_category));
            // TODO: Handle the type column
            //txtPrice.Text           = dataRow == null ? "" : ColumnData.GetString(dataRow, Database_net7_db.Table_item_base._price);
            //txtStackSize.Text       = dataRow == null ? "" : ColumnData.GetString(dataRow, Database_net7_db.Table_item_base._max_stack);
            //txtName.Text            = dataRow == null ? "" : ColumnData.GetString(dataRow, Database_net7_db.Table_item_base._name);
            txtDescription.Text     = dataRow == null ? "" : Utility.ParseDescription(ColumnData.GetString(dataRow, Database_net7_db.Table_item_base._description));
            //RefreshManufacturers(     dataRow == null ? 0 : ColumnData.GetInt32(dataRow, Database_net7_db.Table_item_base._manufacturer));
            //txt2DAsset.Text         = dataRow == null ? "" : ColumnData.GetString(dataRow, Database_net7_db.Table_item_base._2d_asset);
            //txt3DAsset.Text         = dataRow == null ? "" : ColumnData.GetString(dataRow, Database_net7_db.Table_item_base._3d_asset);

            //chkNoTrade.Checked      = dataRow == null ? false : ColumnData.GetBoolean(dataRow, Database_net7_db.Table_item_base._no_trade);
            //chkNoStore.Checked      = dataRow == null ? false : ColumnData.GetBoolean(dataRow, Database_net7_db.Table_item_base._no_store);
            //chkNoDestroy.Checked    = dataRow == null ? false : ColumnData.GetBoolean(dataRow, Database_net7_db.Table_item_base._no_destroy);
            //chkNoManu.Checked       = dataRow == null ? false : ColumnData.GetBoolean(dataRow, Database_net7_db.Table_item_base._no_manu);
            //chkUnique.Checked       = dataRow == null ? false : ColumnData.GetBoolean(dataRow, Database_net7_db.Table_item_base._unique);


            //Obtain the category (in case data was corrupt)
            int SubCat = Utility.ParseParen(cboSubCategory.Text);
            DataTable dataTable;
            int ItemID = txtId.Text.Length == 0 ? 0 : Int32.Parse(txtId.Text);
            switch ((Items.ItemSubCategory)SubCat)
            {
                case Items.ItemSubCategory.Beam_Weapon:
                    dataTable = DB.executeQuery(DB.DatabaseName.net7_db,
                                                      DB.getQueryBeam(ItemID.ToString()));
                    if (dataTable.Rows.Count == 1)
                    {
                        dataRow = dataTable.Rows[0];
                        cboBeamDamageType.Text = ColumnData.GetString(dataRow, Database_net7_db.Table_item_beam._damage_type);
                        txtBeamFireEffect.Text = ColumnData.GetString(dataRow, Database_net7_db.Table_item_beam._fire_effect);
                        txtBeamRange.Text = ColumnData.GetString(dataRow, Database_net7_db.Table_item_beam._range_100);
                        txtBeamEnergy.Text = ColumnData.GetString(dataRow, Database_net7_db.Table_item_beam._energy_100);
                        txtBeamReload.Text = ColumnData.GetString(dataRow, Database_net7_db.Table_item_beam._reload_100);
                        txtBeamDamage100.Text = ColumnData.GetString(dataRow, Database_net7_db.Table_item_beam._damage_100);
                        txtBeamDamage200.Text = ColumnData.GetString(dataRow, Database_net7_db.Table_item_beam._damage_200);
                    }
                    LoadEffects(ItemID);
                    break;

                case Items.ItemSubCategory.Missle_Launcher:
                    dataTable = DB.executeQuery(DB.DatabaseName.net7_db,
                                                      DB.getQueryMissile(ItemID.ToString()));
                    if (dataTable.Rows.Count == 1)
                    {
                        dataRow = dataTable.Rows[0];
                        txtMissleAmmoName.Text = ColumnData.GetString(dataRow, Database_net7_db.Table_item_missle._ammo);
                        txtMissleRoundsFired.Text = ColumnData.GetString(dataRow, Database_net7_db.Table_item_missle._ammo_per_shot);
                        //TODO: Add field energy_100 in table item_missle
                        //txtMissleEnergy.Text        = ColumnData.GetString(dataRow, Database_net7_db.Table_item_missle._energy_100);
                        txtMissleReload100.Text = ColumnData.GetString(dataRow, Database_net7_db.Table_item_missle._reload_100);
                        txtMissleReload200.Text = ColumnData.GetString(dataRow, Database_net7_db.Table_item_missle._reload_200);
                    }
                    LoadEffects(ItemID);
                    break;

                case Items.ItemSubCategory.Projectile_Launcher:
                    dataTable = DB.executeQuery(DB.DatabaseName.net7_db,
                                                      DB.getQueryProjectile(ItemID.ToString()));
                    if (dataTable.Rows.Count == 1)
                    {
                        dataRow = dataTable.Rows[0];
                        txtProjectileAmmoName.Text = ColumnData.GetString(dataRow, Database_net7_db.Table_item_projectile._ammo);
                        txtProjectileRoundsFired.Text = ColumnData.GetString(dataRow, Database_net7_db.Table_item_projectile._ammo_per_shot);
                        txtProjectileRange.Text = ColumnData.GetString(dataRow, Database_net7_db.Table_item_projectile._range_100);
                        //txtProjectileEnergy.Text        = ColumnData.GetString(dataRow, Database_net7_db.Table_item_projectile._energy_100);
                        txtProjectileReload100.Text = ColumnData.GetString(dataRow, Database_net7_db.Table_item_projectile._reload_100);
                        txtProjectileReload200.Text = ColumnData.GetString(dataRow, Database_net7_db.Table_item_projectile._reload_200);
                    }
                    LoadEffects(ItemID);
                    break;

                case Items.ItemSubCategory.Ammo:
                    dataTable = DB.executeQuery(DB.DatabaseName.net7_db,
                                                      DB.getQueryAmmo(ItemID.ToString()));
                    if (dataTable.Rows.Count == 1)
                    {
                        //TODO: Load the launcher name
                        dataRow = dataTable.Rows[0];
                        cboAmmoLauncher.Text = ColumnData.GetString(dataRow, Database_net7_db.Table_item_ammo._launcher_id);
                        cboAmmoDamageType.Text = ColumnData.GetString(dataRow, Database_net7_db.Table_item_ammo._damage_type);
                        txtAmmoVisual.Text = ColumnData.GetString(dataRow, Database_net7_db.Table_item_ammo._fire_effect);
                        txtAmmoManouv.Text = ColumnData.GetString(dataRow, Database_net7_db.Table_item_ammo._maneuv_100);
                        txtAmmoRange.Text = ColumnData.GetString(dataRow, Database_net7_db.Table_item_ammo._range_100);
                        txtAmmoDamage100.Text = ColumnData.GetString(dataRow, Database_net7_db.Table_item_ammo._damage_100);
                        txtAmmoDamage200.Text = ColumnData.GetString(dataRow, Database_net7_db.Table_item_ammo._damage_200);
                    }
                    break;
            }
        }

        public void LoadEffects(int ItemID)
        {
        }

        public void updateFields(DataRow dataRow)
        {
            //dataRow[ColumnData.GetName(Database_net7_db.Table_item_base._id)] = txtId.Text;
            //dataRow[ColumnData.GetName(Database_net7_db.Table_item_base._category)] = ;
            //dataRow[ColumnData.GetName(Database_net7_db.Table_item_base._sub_category)] = ;
            //dataRow[ColumnData.GetName(Database_net7_db.Table_item_base._price)] = txtPrice.Text;
            /*
            dataRow[ColumnData.GetName(Database_net7_db.Table_item_base._max_stack)] = txtStackSize.Text;
            dataRow[ColumnData.GetName(Database_net7_db.Table_item_base._name)] = txtName.Text;
            */
            //dataRow[ColumnData.GetName(Database_net7_db.Table_item_base._description)] = txtDescription.Text;
            //dataRow[ColumnData.GetName(Database_net7_db.Table_item_base._manufacturer)] = ;
            /*
            dataRow[ColumnData.GetName(Database_net7_db.Table_item_base._2d_asset)] = txt2DAsset.Text;
            dataRow[ColumnData.GetName(Database_net7_db.Table_item_base._3d_asset)] = txt3DAsset.Text;
            dataRow[ColumnData.GetName(Database_net7_db.Table_item_base._no_trade)] = chkNoTrade.Checked;
            dataRow[ColumnData.GetName(Database_net7_db.Table_item_base._no_store)] = chkNoStore.Checked;
            dataRow[ColumnData.GetName(Database_net7_db.Table_item_base._no_destroy)] = chkNoDestroy.Checked;
            dataRow[ColumnData.GetName(Database_net7_db.Table_item_base._no_manu)] = chkNoManu.Checked;
            dataRow[ColumnData.GetName(Database_net7_db.Table_item_base._unique)] = chkUnique.Checked;
             */
        }

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
                m_editorType = EditorType.Items;
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
                removeAllTabs();
                setEditor(m_editorType);
                // TODO: Initialize the new editor
            }
        }

        private void setEditor(EditorType editorType)
        {
            if (listEditors.TryGetValue(editorType, out m_currentTableIO))
            {
                m_editorType = editorType;
                removeAllTabs();
            }
        }

        private void selectingTabPage(object sender, TabControlCancelEventArgs e)
        {
            // Prevent going to another tab during an Add or Edit
            if (m_currentTableIO.getState().Equals(TableIO.State.Add) || m_currentTableIO.getState().Equals(TableIO.State.Edit))
            {
                e.Cancel = true;
            }
        }

        private void btnNew_Click(object sender, EventArgs e)
        {
            m_currentTableIO.recordAdd();
        }

        private void btnDelete_Click(object sender, EventArgs e)
        {
            m_currentTableIO.recordDelete();
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            m_currentTableIO.recordSave();
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            m_currentTableIO.recordCancel();
        }

        private void txtSearch_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar.Equals('\r'))
            {
                m_currentTableIO.recordSearch(flnSearch.Text);
            }
        }

        private void btnSearch_Click(object sender, EventArgs e)
        {
            flnSearch.Text = m_currentTableIO.getRecordManager().getIdFromSearch(flnSearch.Text);
            m_currentTableIO.recordSearch(flnSearch.Text);
        }

    }
}
