using System;
using System.Collections.Generic;
using System.Text;
using System.Data;
using System.Windows.Forms;
using CommonTools.Database;

namespace Net7_Tools.Database
{
    class ItemMissileRecordManager : IRecordManager
    {
        private IGui m_gui;

        public ItemMissileRecordManager(IGui gui)
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
            switch ((Net7.Table_item_missile)dataBinding.enumeration)
            {
                case Net7.Table_item_missile._rest_race:
                case Net7.Table_item_missile._rest_prof:
                    // Handles both race and profession restrictions
                    return Restrictions.displayField(dataRow, Net7.Table_item_missile._rest_race, Net7.Table_item_missile._rest_prof);
                case Net7.Table_item_missile._ammo_per_shot:
                    dataBinding.control.Text = (dataRow == null) ? "1" : ColumnData.GetString(dataRow, dataBinding.enumeration);
                    return true;
                case Net7.Table_item_missile._energy_100:
                    dataBinding.control.Text = (dataRow == null) ? "50" : ColumnData.GetString(dataRow, dataBinding.enumeration);
                    return true;
                case Net7.Table_item_missile._reload_100:
                    dataBinding.control.Text = (dataRow == null) ? "10" : ColumnData.GetString(dataRow, dataBinding.enumeration);
                    return true;
            }
            return false;
        }

        public Boolean enableField(DataBinding dataBinding, DataRow dataRow, Boolean enabled)
        {
            switch ((Net7.Table_item_missile)dataBinding.enumeration)
            {
                case Net7.Table_item_missile._rest_race:
                case Net7.Table_item_missile._rest_prof:
                    // Handles both race and profession restrictions
                    return Restrictions.enableField(enabled);
            }
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
            /*switch ((Net7_db.Table_item_missile)dataBinding.enumeration)
            {
            }*/
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
            Net7.Table_item_missile missileField = (Net7.Table_item_missile)dataBinding.enumeration;
            switch (missileField)
            {
                case Net7.Table_item_missile._rest_race:
                case Net7.Table_item_missile._rest_prof:
                    // Handles both race and profession restrictions
                    return Restrictions.updateField(newRecord, dataRow, Net7.Table_item_missile._rest_race, Net7.Table_item_missile._rest_prof);
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
                 + Net7.Tables.item_missile.ToString()
                 + " WHERE "
                 + ColumnData.GetName(Net7.Table_item_missile._item_id)
                 + " = "
                 + currentId;
        }


    }
}
