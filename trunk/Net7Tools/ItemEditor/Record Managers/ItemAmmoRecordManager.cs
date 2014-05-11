using System;
using System.Collections.Generic;
using System.Text;
using System.Data;
using System.Windows.Forms;
using CommonTools.Database;

namespace Net7_Tools.Database
{
    class ItemAmmoRecordManager : IRecordManager
    {
        // Hard coded values from the item_type table
        const int ProjectileLauncherSubCategory = 101;
        const int MissileLauncherSubCategory = 102;
        const int ProjectileLauncherType = 143;
        const int MissileLauncherType = 144;

        private IGui m_gui;

        public ItemAmmoRecordManager(IGui gui)
        {
            m_gui = gui;
        }

        /// <summary>
        ///   <para>
        ///     Display the value within the appropriate DataRow column into
        ///     the field contained within DataBinding</para>
        /// </summary>
        /// <returns>
        ///     <para>True: the field has been displayed.</para>
        ///     <para>False: the field has not been displayed, use the default code </para>
        /// </returns>
        public Boolean displayField(DataBinding dataBinding, DataRow dataRow)
        {
            switch ((Net7.Table_item_ammo)dataBinding.enumeration)
            {
                case Net7.Table_item_ammo._ammo_type_id:
                    {
                        ComboBox ammoType = (ComboBox)dataBinding.control;
                        ammoType.Items.Clear();
                        ammoType.Items.AddRange(Database.getItemAmmoTypes(102));
                        ammoType.Items.AddRange(Database.getItemAmmoTypes(101));
                    }
                    return false;
                case Net7.Table_item_ammo._damage_type:
            {
                ComboBox damageType = (ComboBox)dataBinding.control;
                damageType.Items.Clear();
                damageType.Items.AddRange(Database.getItemDamageTypes());
                return false;
            }
            case Net7.Table_item_ammo._fire_effect:
                dataBinding.control.Text = (dataRow == null) ? "0" : ColumnData.GetString(dataRow, dataBinding.enumeration);
                return true;
            case Net7.Table_item_ammo._range_100:
                    dataBinding.control.Text = (dataRow == null) ? "0" : ColumnData.GetString(dataRow, dataBinding.enumeration);
                    return true;
                case Net7.Table_item_ammo._damage_100:
                    dataBinding.control.Text = (dataRow == null) ? "10" : ColumnData.GetString(dataRow, dataBinding.enumeration);
                    return true;
            }
            return false;
        }

        public Boolean enableField(DataBinding dataBinding, DataRow dataRow, Boolean enabled)
        {
            return false;
        }

        /// <summary>
        ///   <para>
        ///     Validate the display the value of the field contained within DataBinding</para>
        /// </summary>
        /// <returns>
        ///     <para>The data validation result.</para>
        /// </returns>
        public DataValidation.Result validField(Boolean newRecord, DataBinding dataBinding, DataRow dataRow)
        {
            DataValidation.Result dataValidationResult = null;
            return dataValidationResult;
        }

        /// <summary>
        ///   <para>
        ///     Update the value within the appropriate DataRow column from
        ///     the contents of the field within DataBinding</para>
        /// </summary>
        /// <returns>
        ///     <para>True: the data has been updated.</para>
        ///     <para>False: the data has not been updated, use the default code </para>
        /// </returns>
        public Boolean updateField(Boolean newRecord, DataBinding dataBinding, DataRow dataRow)
        {
            Net7.Table_item_ammo ammoField = (Net7.Table_item_ammo)dataBinding.enumeration;
            switch (ammoField)
            {
                case Net7.Table_item_ammo._maneuv_100:
                    dataRow[ColumnData.GetName(ammoField)] = 0;
                    return true;
            }
            return false;
        }

        /// <summary>
        ///   <para>
        ///     Compose a select criteria on the specified id.  This criteria
        ///     will be used to locate the relevant DataRow within the DataSet.
        /// </para>
        /// </summary>
        /// <returns>
        ///     <para>The appropriate select critera.</para>
        /// </returns>
        /// <example>return "id = " + currentId;</example>
        public String selectDataRowFromId(String currentId)
        {
            return " SELECT * FROM "
                 + Net7.Tables.item_ammo.ToString()
                 + " WHERE "
                 + ColumnData.GetName(Net7.Table_item_ammo._item_id)
                 + " = "
                 + currentId;
        }

    }
}
