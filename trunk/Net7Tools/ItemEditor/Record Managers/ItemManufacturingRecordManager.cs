using System;
using System.Collections.Generic;
using System.Text;
using System.Data;
using Net7_Tools.Widgets;
using System.Windows.Forms;
using System.Drawing;
using CommonTools.Database;

namespace Net7_Tools.Database
{
    class ItemManufacturingRecordManager : IRecordManager, IWidgetModified
    {
        public const String UnspecifiedComponentId = "-1";
        private FieldSearchDescription.Data m_fieldSearchDescriptionData;
        private List<FieldSearchDescription> m_listComponentFields;
        private Net7_Tools.Gui.frmManufacturing m_manufacturing;
        private Form m_parent;

        public ItemManufacturingRecordManager(Net7_Tools.Gui.frmManufacturing manufacturing, Form parent)
        {
            m_manufacturing = manufacturing;
            m_fieldSearchDescriptionData = null;
            m_listComponentFields = new List<FieldSearchDescription>();
            m_parent = parent;
            m_manufacturing.Shown += new EventHandler(manufacturing_Shown);
        }

        void manufacturing_Shown(object sender, EventArgs e)
        {
            m_manufacturing.Location = new Point(m_parent.Location.X + m_parent.Width - 3 *
            m_manufacturing.Width / 2, m_parent.Location.Y + m_parent.Height - 3 * m_manufacturing.Height / 2);
            m_manufacturing.StartPosition = FormStartPosition.Manual;
            m_manufacturing.Show();
        }

        public Gui.frmManufacturing getManufacturingDlg()
        {
            return m_manufacturing;
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
            Net7.Table_item_manufacture manuField = (Net7.Table_item_manufacture)dataBinding.enumeration;
            switch (manuField)
            {
                case Net7.Table_item_manufacture._difficulty:
                    if(dataRow == null)
                    {
                        System.Windows.Forms.ComboBox difficulty = (System.Windows.Forms.ComboBox) dataBinding.control;
                        difficulty.SelectedItem = difficulty.Items[0];
                        return true;
                    }
                    break;
                case Net7.Table_item_manufacture._comp_1:
                case Net7.Table_item_manufacture._comp_2:
                case Net7.Table_item_manufacture._comp_3:
                case Net7.Table_item_manufacture._comp_4:
                case Net7.Table_item_manufacture._comp_5:
                case Net7.Table_item_manufacture._comp_6:
                {
                    int componentIndex = getComponentIndexFromEnum(manuField);
                    if (componentIndex == -1)
                    {
                        // TODO: Handle data error
                        throw (new Exception("Invalid component index"));
                    }
                    else
                    {
                        dataBinding.control.Text = (dataRow == null)
                                                    ? UnspecifiedComponentId
                                                    : ColumnData.GetString(dataRow, dataBinding.enumeration);
                        if (dataBinding.control.Text.Length == 0)
                        {
                            dataBinding.control.Text = UnspecifiedComponentId;
                        }
                        m_listComponentFields[componentIndex].setDescription();
                        return true;
                    }
                }
            }

            return false;
        }

        /// <summary>
        /// Callback from TableIO
        /// </summary>
        /// <param name="dataBinding"></param>
        /// <param name="dataRow"></param>
        /// <param name="enabled"></param>
        /// <returns></returns>
        public Boolean enableField(DataBinding dataBinding, DataRow dataRow, Boolean enabled)
        {
            int componentIndex = getComponentIndexFromEnum((Net7.Table_item_manufacture)dataBinding.enumeration);
            enableField(componentIndex, enabled);
            return true;
        }

        private void enableField(int componentIndex, Boolean enabled)
        {
            if (componentIndex == -1)
            {
                // TODO: Handle data error
                throw (new Exception("Invalid component index"));
            }
            else
            {
                // The fields of a component are only enabled when the previous component is specified
                if (enabled && componentIndex > 0)
                {
                    enabled = !m_listComponentFields[componentIndex - 1].getFields().m_field.Text.Equals(UnspecifiedComponentId);
                }
                m_listComponentFields[componentIndex].getFields().m_field.Enabled = enabled;
                m_listComponentFields[componentIndex].getFields().m_search.Enabled = enabled;
            }
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
            switch ((Net7.Table_item_manufacture)dataBinding.enumeration)
            {
                case Net7.Table_item_manufacture._item_id:
                    {
                        if (newRecord)
                        {
                            // Store the item ID
                            String itemIdColumnName = ColumnData.GetName(Net7.Table_item_base._id);
                            String itemId = dataRow[itemIdColumnName].ToString();
                            String manufacturingIdColumnName = ColumnData.GetName(Net7.Table_item_manufacture._item_id);
                            dataRow[manufacturingIdColumnName] = itemId;
                        }
                    }
                    break;
                case Net7.Table_item_manufacture._comp_1:
                case Net7.Table_item_manufacture._comp_2:
                case Net7.Table_item_manufacture._comp_3:
                case Net7.Table_item_manufacture._comp_4:
                case Net7.Table_item_manufacture._comp_5:
                case Net7.Table_item_manufacture._comp_6:
                    {
                        int componentIndex = getComponentIndexFromEnum((Net7.Table_item_manufacture)dataBinding.enumeration);
                        String componentId = m_listComponentFields[componentIndex].getFields().m_field.Text;
                        if (componentIndex == 0)
                        {
                            dataValidationResult = componentId.Equals(UnspecifiedComponentId)
                                                   ? null
                                                   : DataValidation.isInTable(componentId,
                                                                              Net7.Tables.item_manufacture.ToString(),
                                                                              Database.getQueryItem(componentId));
                        }
                        else if (componentIndex > 0)
                        {
                            // Valid to be unspecified otherwise must be in the item table
                            dataValidationResult = componentId.Equals(UnspecifiedComponentId)
                                                   ? null
                                                   : DataValidation.isInTable(componentId,
                                                                              "Items",
                                                                              Database.getQueryItem(componentId));
                            if (dataValidationResult == null || dataValidationResult.isValid())
                            {
                                // Previous component must be specified
                                String previousComponentId = m_listComponentFields[componentIndex - 1].getFields().m_field.Text;
                                if (!componentId.Equals(UnspecifiedComponentId) && previousComponentId.Equals(UnspecifiedComponentId))
                                {
                                    if (dataValidationResult == null)
                                    {
                                        dataValidationResult = new DataValidation.Result();
                                    }
                                    dataValidationResult.setMessage("You must define the previous component before specifying this one");
                                }
                            }
                        }
                    }
                    break;
            }
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
                 + Net7.Tables.item_manufacture.ToString()
                 + " WHERE "
                 + ColumnData.GetName(Net7.Table_item_manufacture._item_id)
                 + " = "
                 + currentId;
        }

        public void setFieldSearchDescriptionData(FieldSearchDescription.Data fieldSearchDescriptionData)
        {
            m_fieldSearchDescriptionData = fieldSearchDescriptionData;
        }

        public void addComponentFields(FieldSearchDescription.Fields fields)
        {
            FieldSearchDescription fieldSearchDescription = new FieldSearchDescription(fields, m_fieldSearchDescriptionData);
            m_listComponentFields.Add(fieldSearchDescription);
        }

        private int getComponentIndexFromEnum(Net7.Table_item_manufacture component)
        {
            switch (component)
            {
                case Net7.Table_item_manufacture._comp_1:
                    return 0;
                case Net7.Table_item_manufacture._comp_2:
                    return 1;
                case Net7.Table_item_manufacture._comp_3:
                    return 2;
                case Net7.Table_item_manufacture._comp_4:
                    return 3;
                case Net7.Table_item_manufacture._comp_5:
                    return 4;
                case Net7.Table_item_manufacture._comp_6:
                    return 5;
            }
            return 0;
        }

        private int getComponentIndexFromControl(System.Windows.Forms.Control control)
        {
            for(int componentIndex = 0; componentIndex < m_listComponentFields.Count; componentIndex++)
            {
                if (m_listComponentFields[componentIndex].getFields().m_field.Name.Equals(control.Name))
                {
                    return componentIndex;
                }
            }
            return 0;
        }

        public void widgetModified(System.Windows.Forms.Control control)
        {
            Boolean enablingFields = false;
            String componentId;
            for (int componentIndex = m_listComponentFields.Count - 1; componentIndex >= 0;  componentIndex--)
            {
                componentId = componentIndex == 0
                            ? ""
                            : m_listComponentFields[componentIndex - 1].getFields().m_field.Text;
                if(!enablingFields && !componentId.Equals(UnspecifiedComponentId))
                {
                    enablingFields = true;
                }
                if (!enablingFields && !m_listComponentFields[componentIndex].getFields().m_field.Text.Equals(UnspecifiedComponentId))
                {
                    // TODO: Isn't this triggering a cascade of change events?
                    m_listComponentFields[componentIndex].getFields().m_field.Text = UnspecifiedComponentId;
                }
                m_listComponentFields[componentIndex].getFields().m_field.Enabled = enablingFields;
                m_listComponentFields[componentIndex].getFields().m_search.Enabled = enablingFields;
            }
        }
    }
}
