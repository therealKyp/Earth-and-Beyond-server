using System;
using System.Collections.Generic;
using System.Text;
using System.Data;
using System.Windows.Forms;
using System.Drawing;
using Net7_Tools.Widgets;
using CommonTools.Database;
using CommonTools.Gui;

namespace Net7_Tools.Database
{
    public class ItemRecordManager : IRecordManager
    {
        private IGui m_gui;
        private Dictionary<Items.ItemCategory, TabPage[]> m_categoryTabPages;
        private Dictionary<Items.ItemSubCategory, TabPage[]> m_subCategoryTabPages;
        private ComboBox m_categoryControl;
        private ComboBox m_subCategoryControl;
        private ComboBox m_typeControl;
        private Search.FrmAsset m_assetSearch = new Net7_Tools.Search.FrmAsset();
        private DlgSearch m_equipVisualEffectSearch = null;
        private TextBox m_asset2D;
        private TextBox m_asset3D;
        private TextBox m_baseBaseItem;
        private Button m_baseBaseItemBtn;
        private Button m_baseManufacturingBtn;
        private TextBox m_equipVisualEffect;

        public ItemRecordManager(IGui gui,
                                 ComboBox categoryControl, 
                                 ComboBox subCategoryControl,
                                 ComboBox typeControl,
                                 TextBox asset2D,
                                 TextBox asset3D,
                                 TextBox baseBaseItem,
                                 Button baseBaseItemBtn,
                                 Button baseManufacturingBtn,
                                 TextBox equipVisualEffect)
        {
            m_gui = gui;
            m_categoryControl = categoryControl;
            m_subCategoryControl = subCategoryControl;
            m_typeControl = typeControl;
            m_categoryTabPages = new Dictionary<Items.ItemCategory, TabPage[]>();
            m_subCategoryTabPages = new Dictionary<Items.ItemSubCategory, TabPage[]>();
            m_asset2D = asset2D;
            m_asset3D = asset3D;
            m_baseBaseItem = baseBaseItem;
            m_baseBaseItemBtn = baseBaseItemBtn;
            m_baseManufacturingBtn = baseManufacturingBtn;
            m_equipVisualEffect = equipVisualEffect;
        }

        public IGui getGui()
        {
            return m_gui;
        }

        public Boolean displayField(DataBinding dataBinding, DataRow dataRow)
        {
            if (dataBinding.enumeration.Equals(Net7.Table_item_base._id))
            {
                String code = (dataRow == null) ? "" : ColumnData.GetString(dataRow, dataBinding.enumeration);
                m_baseManufacturingBtn.ForeColor = (code.Length == 0 || Database.hasManufacturing(code)) ? Color.Black : Color.Red;
            }
            else if (dataBinding.enumeration.Equals(Net7.Table_item_base._category))
            {
                // Show the relevant tab pages given the category
                Int32 code = (dataRow == null) ? 50 : ColumnData.GetInt32(dataRow, dataBinding.enumeration);
                Items.ItemCategory category = (Items.ItemCategory)code;
                TabPage[] listTabPages;
                if (m_categoryTabPages.TryGetValue(category, out listTabPages))
                {
                    m_gui.setTabPages(listTabPages);
                }
            }
            else if (dataBinding.enumeration.Equals(Net7.Table_item_base._sub_category))
            {
                // Show the relevant tab pages given the sub-category
                Int32 code = (dataRow == null) ? 0 : ColumnData.GetInt32(dataRow, dataBinding.enumeration);
                Items.ItemSubCategory subCategory = (Items.ItemSubCategory)code;
                TabPage[] listTabPages;
                if (m_subCategoryTabPages.TryGetValue(subCategory, out listTabPages))
                {
                    m_gui.setTabPages(listTabPages);
                }
                else if (m_subCategoryTabPages.TryGetValue(Items.ItemSubCategory.Other, out listTabPages))
                {
                    // Assume that the sub-category refers to a component
                    m_gui.setTabPages(listTabPages);
                }
                Int32 category = (dataRow == null) ? 0 : ColumnData.GetInt32(dataRow, Net7.Table_item_base._category);
                CodeValue[] listSubCategory = Database.getItemSubCategories((int)category);
                ComboBox subCategories = (ComboBox)dataBinding.control;
                subCategories.Items.Clear();
                subCategories.Items.AddRange(listSubCategory);

                // Rely on the child tables (i.e. Devices, Beams, Missile/Projectile Launcher)
                // to activate the restrictions when needed
                Restrictions.displayField(null, null, null);
                Restrictions.enableField(false);

                Int32 subCategoryId = (dataRow == null) ? 0 : ColumnData.GetInt32(dataRow, dataBinding.enumeration);
                m_gui.setEditor(Items.getEditorType(subCategory), false);
            }
            else if (dataBinding.enumeration.Equals(Net7.Table_item_base._description))
            {
                // Convert from E&B style
                String description = (dataRow == null) ? "" : ColumnData.GetString(dataRow, dataBinding.enumeration);
                dataBinding.control.Text = Utility.ParseDescription(description);
                return true;
            }
            else if (dataBinding.enumeration.Equals(Net7.Table_item_base._custom_flag))
            {
                ((CheckBox)dataBinding.control).Checked = (dataRow == null) ? false : ColumnData.GetBoolean(dataRow, dataBinding.enumeration);
                return true;
            }
            else if (dataBinding.enumeration.Equals(Net7.Table_item_base._2d_asset))
            {
                dataBinding.control.Text = (dataRow == null) ? "744" : ColumnData.GetString(dataRow, dataBinding.enumeration);
                return true;
            }
            else if (dataBinding.enumeration.Equals(Net7.Table_item_base._max_stack))
            {
                dataBinding.control.Text = (dataRow == null) ? "1" : ColumnData.GetString(dataRow, dataBinding.enumeration);
                return true;
            }
            else if (dataBinding.enumeration.Equals(Net7.Table_item_base._3d_asset))
            {
                dataBinding.control.Text = (dataRow == null) ? "69" : ColumnData.GetString(dataRow, dataBinding.enumeration);
                return true;
            }
            else if (dataBinding.enumeration.Equals(Net7.Table_item_base._price))
            {
                dataBinding.control.Text = (dataRow == null) ? "0" : ColumnData.GetString(dataRow, dataBinding.enumeration);
                return true;
            }
            return false;
        }

        public Boolean enableField(DataBinding dataBinding, DataRow dataRow, Boolean enabled)
        {
            switch ((Net7.Table_item_base)dataBinding.enumeration)
            {
            case Net7.Table_item_base._item_base_id:
                {
                    Boolean customItem = dataRow != null
                                        && ColumnData.GetBoolean(dataRow, Net7.Table_item_base._custom_flag);
                    if (customItem == false && enabled == true)
                    {
                        enabled = false;
                    }
                    dataBinding.control.Enabled = enabled;
                    return true;
                }
            case Net7.Table_item_base._category:
                {
                    if (enabled == true)
                    {
                        if (m_gui.getCurrentEditor().getTableHandlers()[0].getTableIO().getState().Equals(TableIO.State.Add))
                        {
                            // Always enable in Add mode
                        }
                        else
                        {
                            Int32 code = (dataRow == null) ? 0 : ColumnData.GetInt32(dataRow, dataBinding.enumeration);
                            Items.ItemCategory category = (Items.ItemCategory) code;
                            enabled = Items.isComponent(category);
                        }
                    }
                }
                dataBinding.control.Enabled = enabled;
                return true;
            case Net7.Table_item_base._sub_category:
                {
                    if (enabled == true)
                    {
                        if (m_gui.getCurrentEditor().getTableHandlers()[0].getTableIO().getState().Equals(TableIO.State.Add))
                        {
                            // Always enable in Add mode
                        }
                        else
                        {
                            Int32 code = (dataRow == null) ? 0 : ColumnData.GetInt32(dataRow, dataBinding.enumeration);
                            Items.ItemSubCategory subCategory = (Items.ItemSubCategory)code;
                            TabPage[] listTabPages = getTabPages(subCategory);
                            enabled = listTabPages.Length == 1;
                        }
                    }
                }
                dataBinding.control.Enabled = enabled;
                return true;
            } // switch

            return false;
        }

        public DataValidation.Result validField(Boolean newRecord, DataBinding dataBinding, DataRow dataRow)
        {
            DataValidation.Result dataValidationResult = null;
            switch ((Net7.Table_item_base)dataBinding.enumeration)
            {
                case Net7.Table_item_base._id:
                    {
                        if (newRecord)
                        {
                            dataBinding.control.Text = Database.getNextItemId();
                        }
                    }
                    break;

                case Net7.Table_item_base._category:
                    {
                        ComboBox comboBox = (ComboBox)dataBinding.control;
                        CodeValue codeValue = (CodeValue)comboBox.SelectedItem;
                        //dataValidationResult = DataValidation.isNumericGreaterEqual(codeValue.code.ToString(), 1);
                        dataValidationResult = DataValidation.isNonEmpty(codeValue);
                        if (dataValidationResult.isValid()
                            && !Enum.IsDefined(typeof(Items.ItemCategory), codeValue.code))
                        {
                            dataValidationResult = new DataValidation.Result("The category '" + codeValue.code + "' is invalid");
                        }
                        // TODO: Validate through the item_category table
                        /*
                        dataValidationResult = DataValidation.isInTable(codeValue.code.ToString(),
                                                                        Net7_db.Tables.item_category.ToString(),
                                                                        DB.getQueryItemCategory());*/
                    }
                    break;

                case Net7.Table_item_base._sub_category:
                    {
                        ComboBox comboBox = (ComboBox)dataBinding.control;
                        CodeValue codeValue = (CodeValue)comboBox.SelectedItem;
                        // TODO: codeValue can be null even though there is something displayed on the screen
                        //dataValidationResult = DataValidation.isNumericGreaterEqual(codeValue.code.ToString(), 1);
                        dataValidationResult = DataValidation.isNonEmpty(codeValue);
                        if (dataValidationResult.isValid()
                            && !Enum.IsDefined(typeof(Items.ItemSubCategory), codeValue.code))
                        {
                            dataValidationResult = new DataValidation.Result("The sub-category '" + codeValue.code + "' is invalid");
                        }
                        // TODO: Validate through the item_sub_category table
                        /*
                        dataValidationResult = DataValidation.isInTable(codeValue.code.ToString(),
                                                                        Net7_db.Tables.item_sub_category.ToString(),
                                                                        DB.getQueryItemSubCategory());*/
                        /*
                        if(dataValidationResult.isValid())
                        {
                            String query = "SELECT item_id"
                                         + " FROM " + Net7_db.Tables.item_subcategory.ToString()
                                         + " WHERE " + ColumnData.GetName(Net7_db.item_subcategory._item_id)
                                         + " = '" + codeValue.code.ToString() + "'";
                            dataValidationResult = DataValidation.isIdInTable(codeValue.code.ToString(),
                                                                              Net7_db.Tables.item_subcategory.ToString(),
                                                                              query);
                        }
                        */
                    }
                    break;

                case Net7.Table_item_base._manufacturer:
                    {
                        ComboBox comboBox = (ComboBox)dataBinding.control;
                        CodeValue codeValue = (CodeValue)comboBox.SelectedItem;
                        dataValidationResult = DataValidation.isNumeric(codeValue.code.ToString());
                        if(dataValidationResult.isValid())
                        {
                            String query = "SELECT " + ColumnData.GetName(Net7.Table_item_manufacturer_base._id)
                                         + " FROM " + Net7.Tables.item_manufacturer_base.ToString()
                                         + " WHERE " + ColumnData.GetName(Net7.Table_item_manufacturer_base._id)
                                         + " = '" + codeValue.code.ToString() + "'";
                            dataValidationResult = DataValidation.isInTable(codeValue.code.ToString(),
                                                                            "Manufacturers",
                                                                            query);
                        }
                    }
                    break;

                case Net7.Table_item_base._name:
                    dataValidationResult = DataValidation.isNonEmpty(dataBinding.control.Text);
                    if (dataValidationResult.isValid())
                    {
                        String columnName = ColumnData.GetName(Net7.Table_item_base._name);
                        String currentValue = newRecord ? "" : dataRow[columnName].ToString();
                        String newValue = dataBinding.control.Text;
                        if(    currentValue.ToLower().CompareTo(newValue.ToLower()) != 0 // It's not a simple rename
                            && !currentValue.Equals(newValue))
                        {
                            String query = "SELECT " + columnName
                                         + " FROM " + Net7.Tables.item_base.ToString()
                                         + " WHERE " + columnName
                                         + " = '" + dataBinding.control.Text.Replace("'", "''") + "'";
                            dataValidationResult = DataValidation.isNotInTable(dataBinding.control.Text,
                                                                               "Items",
                                                                                query);
                        }
                    }
                    break;

                case Net7.Table_item_base._item_base_id:
                {
                    TextBox baseItemId = (TextBox)dataBinding.control;
                    if (baseItemId.Enabled)
                    {
                        String columnName = ColumnData.GetName(Net7.Table_item_base._id);
                        String query = "SELECT " + columnName
                                     + " FROM " + Net7.Tables.item_base.ToString()
                                     + " WHERE " + columnName
                                     + " = '" + dataBinding.control.Text + "'";
                        dataValidationResult = DataValidation.isInTable(dataBinding.control.Text,
                                                                        "Items",
                                                                        query);
                    }
                }
                break;

                case Net7.Table_item_base._effect_id:
                {
                    TextBox equipVisualEffectId = (TextBox)dataBinding.control;
                    if (equipVisualEffectId.Enabled)
                    {
                        String columnName = ColumnData.GetName(Net7.Table_effects._effect_id);
                        String query = "SELECT " + columnName
                                     + " FROM " + Net7.Tables.effects
                                     + " WHERE " + columnName
                                     + " = '" + dataBinding.control.Text + "'";
                        dataValidationResult = DataValidation.isInTable(dataBinding.control.Text,
                                                                        "Effects",
                                                                        query);
                    }
                }
                break;

            }
            return dataValidationResult;
        }

        public Boolean updateField(Boolean newRecord, DataBinding dataBinding, DataRow dataRow)
        {
            if (dataBinding.enumeration.Equals(Net7.Table_item_base._description))
            {
                // Convert to E&B style
                String columnName = ColumnData.GetName(dataBinding.enumeration);
                dataRow[columnName] = Utility.FormatDescription(dataBinding.control.Text);
                return true;
            }
            else if (dataBinding.enumeration.Equals(Net7.Table_item_base._item_base_id))
            {
                    TextBox baseItemId = (TextBox)dataBinding.control;
                    if (!baseItemId.Enabled)
                    {
                        String columnName = ColumnData.GetName(dataBinding.enumeration);
                        dataRow[columnName] = DBNull.Value;
                        return true;
                    }
            }

            return false;
        }

        public String selectDataRowFromId(String currentId)
        {
            return "SELECT * FROM "
                 + Net7.Tables.item_base.ToString()
                 + " WHERE "
                 + ColumnData.GetName(Net7.Table_item_base._id)
                 + " = "
                 + currentId;
        }

        /// <summary>
        ///   <para>Initialize the possible item levels</para>
        /// </summary>
        /// <returns>The list of possible item levels</returns>
        /// <remarks>The array is indexed from 0 to 8 but the item levels range from 1 to 9</remarks>
        public static CodeValue[] getLevels()
        {
            CodeValue[] levels = new CodeValue[9];
            for (int levelIndex = 0; levelIndex < levels.Length; ++levelIndex)
            {
                levels[levelIndex] = new CodeValue(levelIndex + 1, (levelIndex + 1).ToString());
            }
            return levels;
        }

        public void setTabPages(Items.ItemCategory category, TabPage[] listTabPages)
        {
            m_categoryTabPages.Add(category, listTabPages);
        }

        public TabPage[] getTabPages(Items.ItemCategory category)
        {
            TabPage[] listTabPages;
            if (m_categoryTabPages.TryGetValue(category, out listTabPages))
            {
                return listTabPages;
            }
            return null;
        }

        public void setTabPages(Items.ItemSubCategory subCategory, TabPage[] listTabPages)
        {
            m_subCategoryTabPages.Add(subCategory, listTabPages);
        }

        public TabPage[] getTabPages(Items.ItemSubCategory subCategory)
        {
            TabPage[] listTabPages;
            if (m_subCategoryTabPages.TryGetValue(subCategory, out listTabPages))
            {
                // Found a configured sub-category
                return listTabPages;
            }
            if (m_subCategoryTabPages.TryGetValue(Items.ItemSubCategory.Other, out listTabPages))
            {
                // We are dealing with an "other" sub-category than those configured
                return listTabPages;
            }
            return null;
        }

        public void onCategorySelectionChanged(object sender, EventArgs e)
        {
            ComboBox categoryCbo = (ComboBox)sender;
            CodeValue codeValue = (CodeValue)categoryCbo.SelectedItem;

            // Still in view mode since the change has not yet propagated
            if (m_gui.getCurrentEditor().getTableHandlers()[0].getTableIO().getState().Equals(TableIO.State.View))
            {
                // We are editing.  If the category is enabled then we can only select a component
                Items.ItemCategory category = (Items.ItemCategory)codeValue.code;
                if (!Items.isComponent(category))
                {
                    MessageBox.Show("The category must remain a component");
                    codeValue = new CodeValue((int)Items.ItemCategory.Invalid);
                    categoryCbo.SelectedItem = codeValue;
                    return;
                }
            }


            CodeValue[] listSubCategory = Database.getItemSubCategories(codeValue.code);
            m_subCategoryControl.Items.Clear();
            if (listSubCategory != null && listSubCategory.Length != 0)
            {
                m_subCategoryControl.Items.AddRange(listSubCategory);
                m_subCategoryControl.SelectedItem = listSubCategory.GetValue(0);
                onSubCategorySelectionChanged(null, null); // Hack: Force a selection change event
            }
            else
            {
                m_subCategoryControl.Text = "";
            }

            Items.ItemCategory itemCategory = (Items.ItemCategory)codeValue.code;
            TabPage[] tabPages = getTabPages(itemCategory);
            if (tabPages != null)
            {
                m_gui.setTabPages(tabPages);
                // Rely on the child tables (i.e. Beams) to activate the restrictions when needed
                Restrictions.enableField(false);
                //m_gui.setEditor(Items.getEditorType(itemCategory), false);
                categoryCbo.Focus();
            }
        }

        public void onSubCategorySelectionChanged(object sender, EventArgs e)
        {
            CodeValue codeValue = (CodeValue)m_categoryControl.SelectedItem;
            Items.ItemCategory itemCategory = (Items.ItemCategory)codeValue.code;

            codeValue = (CodeValue)m_subCategoryControl.SelectedItem;
            Items.ItemSubCategory itemSubCategory = (Items.ItemSubCategory)codeValue.code;

            TabPage[] tabPages = getTabPages(itemSubCategory);
            if (tabPages != null)
            {
                m_gui.setTabPages(tabPages);
                // Rely on the child tables (i.e. Beams) to activate the restrictions when needed
                Restrictions.enableField(false);
                m_gui.setEditor(Items.getEditorType(itemSubCategory), false);
                m_subCategoryControl.Focus();
            }

            selectType(itemCategory, itemSubCategory);
        }

        public void selectType(Items.ItemCategory itemCategory, Items.ItemSubCategory itemSubCategory)
        {
            Net7.Enum_item_type type = Net7.Enum_item_type._Base;
                 if(itemSubCategory.Equals(Items.ItemSubCategory.Ammo))                 type = Net7.Enum_item_type._Ammo;
            else if(itemSubCategory.Equals(Items.ItemSubCategory.Beam_Weapon))          type = Net7.Enum_item_type._Beam_Weapon;
            else if(itemSubCategory.Equals(Items.ItemSubCategory.Missile_Launcher))     type = Net7.Enum_item_type._Missile_Launcher;
            else if(itemSubCategory.Equals(Items.ItemSubCategory.Projectile_Launcher))  type = Net7.Enum_item_type._Projectile_Weapon;
            else if(itemSubCategory.Equals(Items.ItemSubCategory.Engine))               type = Net7.Enum_item_type._Engine;
            else if(itemSubCategory.Equals(Items.ItemSubCategory.Reactor))              type = Net7.Enum_item_type._Reactor;
            else if(itemSubCategory.Equals(Items.ItemSubCategory.Shield))               type = Net7.Enum_item_type._Shields;
            else if(itemCategory.Equals(Items.ItemCategory.Device))                     type = Net7.Enum_item_type._Devices;

            m_typeControl.SelectedItem = new CodeValue((int)type);
        }

        public void base2DAssetBtn_Click(object sender, EventArgs e)
        {
            m_assetSearch.setSelectedItemId(m_asset2D.Text);
            List<String> limitedCategory = new List<String>();
            limitedCategory.Add("Icons");
            m_assetSearch.setCategoryLimits(limitedCategory);
            m_assetSearch.ShowDialog();
            if (m_assetSearch.getSelectedItemId().Length != 0)
            {
                m_asset2D.Text = m_assetSearch.getSelectedItemId();
            }
        }

        public void base3DAssetBtn_Click(object sender, EventArgs e)
        {
            m_assetSearch.setSelectedItemId(m_asset3D.Text);
            List<String> limitedCategory = new List<String>();
            limitedCategory.Add("Pickups ( loot )");
            m_assetSearch.setCategoryLimits(limitedCategory);
            m_assetSearch.ShowDialog();
            if (m_assetSearch.getSelectedItemId().Length != 0)
            {
                m_asset3D.Text = m_assetSearch.getSelectedItemId();
            }
        }

        public void baseManufacturingBtn_Click(object sender, EventArgs e)
        {
            ItemManufacturingRecordManager itemManufacturingRecordManager = (ItemManufacturingRecordManager)m_gui.getTableInfo(frmItems.SingleTableEditor.Manufacturing).m_tableIO.getRecordManager();
            Net7_Tools.Gui.frmManufacturing manufacturingDlg = itemManufacturingRecordManager.getManufacturingDlg();
            manufacturingDlg.setItemId(m_gui.getCurrentEditor().getPrimaryId());
            manufacturingDlg.ShowDialog();
        }

        public void onCustomItem(object sender, EventArgs e)
        {
            if (((CheckBox)sender).Checked)
            {
                m_baseBaseItem.Enabled = true;
                m_baseBaseItem.Focus();
            }
            else
            {
                m_baseBaseItem.Enabled = false;
                m_baseBaseItem.Text = "";
            }
        }

        public void baseItemBtn_Click(object sender, EventArgs e)
        {
            Net7_Tools.Search.ISearchDialog searchDialog = m_gui.getSearchDialog(frmItems.SearchDialog.Item);
            searchDialog.setSelectedItemId(m_baseBaseItem.Text);
            searchDialog.getForm().ShowDialog();
            if (searchDialog.getSelectedItemId().Length != 0)
            {
                m_baseBaseItem.Text = searchDialog.getSelectedItemId();
            }
        }

        public void equipVisualEffectBtn_Click(object sender, EventArgs e)
        {
            if (m_equipVisualEffectSearch == null)
            {
                m_equipVisualEffectSearch = new DlgSearch();
                m_equipVisualEffectSearch.configure(Net7.Tables.effects);
            }
            m_equipVisualEffectSearch.ShowDialog();
            String selectedId = m_equipVisualEffectSearch.getSelectedId();
            if (selectedId.Length != 0)
            {
                m_equipVisualEffect.Text = selectedId;
            }
        }

        public void onBaseItemEnabledChanged(object sender, EventArgs e)
        {
            m_baseBaseItemBtn.Enabled = ((TextBox)sender).Enabled;
        }

    }
}
