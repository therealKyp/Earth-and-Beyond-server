using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Data;
using MySql.Data.MySqlClient;
using CommonTools.Database;

namespace Net7_Tools.Database
{
    public class TableIO
    {
        /// <summary>The possible data states</summary>
        public enum State { Unknown, NoRecordAvailable, View, Add, Edit };

        private List<DataBinding> listDataBinding;
        private State m_state;
        private List<Control> m_listControlsEnabledOnlyOnAdd;
        private List<Control> m_listControlsEnabledOnlyOnView;
        private List<Control> m_listControlsNeverEnabled;
        private DataRow m_dataRow;
        private MySqlConnection m_connection;
        private MySqlDataAdapter m_sqlDataAdapter;
        private DataSet m_dataSet;
        private String m_tableName;
        private Enum m_idField;
        private Boolean m_suspendValueChanged;
        private Boolean m_suspendGuiChanged;
        private IRecordManager m_recordManager;
        private IGui m_gui;
        private Boolean m_recordIsOptional;

        public TableIO(IGui gui, IRecordManager recordManager)
        {
            m_gui = gui;
            m_recordManager = recordManager;
            m_state = State.Unknown;
            m_listControlsEnabledOnlyOnAdd = new List<Control>();
            m_listControlsEnabledOnlyOnView = new List<Control>();
            m_listControlsNeverEnabled = new List<Control>();
            m_dataRow = null;
            m_connection = null;
            m_sqlDataAdapter = null;
            m_dataSet = null;
            m_tableName = "";
            m_suspendValueChanged = false;
            m_suspendGuiChanged = false;
            listDataBinding = new List<DataBinding>();
            m_recordIsOptional = false;
        }

        public void setRecordOptional()
        {
            m_recordIsOptional = true;
        }

        public Boolean isRecordOptional()
        {
            return m_recordIsOptional;
        }

        public DataRow getCurrentDataRow()
        {
            return m_dataRow;
        }

        public DataRow activate()
        {
            // This will ensure a state change to View or NoRecordAvailable
            m_state = State.Unknown;

            // Retrieve the first row and trigger the appropriate View or NoRecordAvailable event
            //m_dataRow = m_dataSet.Tables[m_tableName].Rows.Count == 0 ? null : m_dataSet.Tables[m_tableName].Rows[0];

            String query = "SELECT * FROM "
                         + m_tableName
                         + " LIMIT 1";
            DataTable dataTable = DB.Instance.executeQuery(query, null, null);
            m_dataRow = dataTable.Rows.Count == 0 ? null : dataTable.Rows[0];

            setState(m_dataRow == null ? State.NoRecordAvailable : State.View);
            return m_dataRow;
        }

        public void initialize(String tableName, Enum idField, String selectQuery)
        {
            m_tableName = tableName;
            m_idField = idField;

            String query = "SELECT * FROM "
                         + tableName
                         + " LIMIT 1";
            m_connection = DB.Instance.openConnection();
            m_sqlDataAdapter = new MySqlDataAdapter(query, m_connection);
            m_sqlDataAdapter.MissingSchemaAction = MissingSchemaAction.AddWithKey;

            m_dataSet = new DataSet();
            m_sqlDataAdapter.Fill(m_dataSet, m_tableName);

//dataSet.Tables["tablename"].PrimaryKey = primaryKey;
/*
if(m_tableName.Equals("item_base"))
{
DataColumn[] primaryKey = new DataColumn[1];
primaryKey[0] = m_dataSet.Tables[m_tableName].Columns[ColumnData.GetName(m_idField)];
m_dataSet.Tables[m_tableName].PrimaryKey = primaryKey;

MySqlCommandBuilder cmdBldr = new MySqlCommandBuilder(m_sqlDataAdapter);
cmdBldr.GetDeleteCommand().CommandText = "DELETE FROM `net7`.`item_base` WHERE `id` <=> ?Original_id";
Console.WriteLine("Delete: " + cmdBldr.GetDeleteCommand().CommandText);
Console.WriteLine("Insert: " + cmdBldr.GetInsertCommand().CommandText);
Console.WriteLine("Update: " + cmdBldr.GetUpdateCommand().CommandText);
}
*/


/*
            // TODO: Ensure that this is called only once
            // DataSet can contain more than one table so it may need to be moved out of here
            // or at least no longer disposing it.
            m_tableName = tableName;
            m_idField = idField;

            if (m_connection == null)
            {
                String connectionString = SQLData.ConnStr(DB.DatabaseName.net7.ToString());
                m_connection = new MySqlConnection(connectionString);
            }
            if (m_dataSet != null)
            {
                m_dataSet.Dispose();
            }
            if (m_sqlDataAdapter != null)
            {
                m_sqlDataAdapter.Dispose();
            }

            m_dataSet = new DataSet();
            m_sqlDataAdapter = new MySqlDataAdapter(selectQuery, m_connection);

            // Fill-in the insert, update, and delete commands
            // TODO: Check that without calling this the commands really are not created
            MySqlCommandBuilder cmdBldr = new MySqlCommandBuilder(m_sqlDataAdapter);
            //Console.WriteLine("Delete: " + cmdBldr.GetDeleteCommand().CommandText);
            //Console.WriteLine("Insert: " + cmdBldr.GetInsertCommand().CommandText);
            //Console.WriteLine("Update: " + cmdBldr.GetUpdateCommand().CommandText);

            // Copy the data into a local table (of the same name)
//            m_sqlDataAdapter.Fill(m_dataSet, m_tableName);

            //Console.WriteLine("Tables.Count: " + dataSet.Tables.Count);
            //Console.WriteLine("Rows.Count: " + dataSet.Tables[0].Rows.Count);
            //Console.WriteLine("[4][1]: " + dataSet.Tables[0].Rows[4][1].ToString());
            //dataSet.Tables[0].Rows[4][1] = "Sparta";
            //sqlDataAdapter.Update(dataSet, "item_manufacturer_base");

            //activate();
*/
        }

        public void setGuiChangeSuspended(Boolean suspendGuiChanged)
        {
            m_suspendGuiChanged = suspendGuiChanged;
        }

        public void setState(State state)
        {
            if (!m_state.Equals(state))
            {
                m_state = state;
                switch (state)
                {
                    case State.NoRecordAvailable:
                        noRecordAvailable();
                        break;
                    case State.View:
                        viewRecord();
                        break;
                    case State.Add:
                        addRecord();
                        break;
                    case State.Edit:
                        editRecord();
                        break;
                }
            }
        }

        public void selectState()
        {
            setState(m_dataRow == null ? State.NoRecordAvailable : State.View);
        }

        public State getState()
        {
            return m_state;
        }

        public void addFieldEnabledOnlyOnAdd(Control control)
        {
            m_listControlsEnabledOnlyOnAdd.Add(control);
            control.Enabled = false; // TODO: Need to recurse the children?
        }

        public void addFieldEnabledOnlyOnView(Control control)
        {
            m_listControlsEnabledOnlyOnView.Add(control);
            control.Enabled = false; // TODO: Need to recurse the children?
        }

        public void addFieldNeverEnabled(Control control)
        {
            m_listControlsNeverEnabled.Add(control);
            control.Enabled = false; // TODO: Need to recurse the children?
        }

        private void noRecordAvailable()
        {
            enableFields(false);
            m_gui.setGuiEnabled(true, false, false);
        }

        public void viewRecord()
        {
            // Suspend the value changed event notification since the user is
            // not actually changing the values
            m_suspendValueChanged = true;

            // Process each widget bound to a database field
            foreach (DataBinding dataBinding in listDataBinding)
            {
                // Is there an override within the record manager?
                if (dataBinding.enumeration != null && !m_recordManager.displayField(dataBinding, m_dataRow))
                {
                    // Use the standard data manager
                    if (dataBinding.control != null)
                    {
                        if (dataBinding.control.GetType() == typeof(CheckBox))
                        {
                            ((CheckBox)dataBinding.control).Checked = (m_dataRow == null)
                                                                    ? false
                                                                    : ColumnData.GetBoolean(m_dataRow, dataBinding.enumeration);
                        }
                        else if (dataBinding.control.GetType() == typeof(ListBox))
                        {
                            // TODO: If the DB value is not available in the list then select first entry and enter edit mode
                            ListBox listBox = (ListBox)dataBinding.control;
                            CodeValue codeValue = new CodeValue();
                            codeValue.code = (m_dataRow == null) ? 0 : ColumnData.GetInt32(m_dataRow, dataBinding.enumeration);
                            if (m_dataRow == null)
                            {
                                codeValue = listBox.Items.Count == 0 ? null : (CodeValue)listBox.Items[0];
                            }
                            else
                            {
                                codeValue.code = ColumnData.GetInt32(m_dataRow, dataBinding.enumeration);
                            }
                            listBox.SelectedItem = codeValue;
                        }
                        else if (dataBinding.control.GetType() == typeof(ComboBox))
                        {
                            // TODO: If the DB value is not available in the list then select first entry and enter edit mode
                            ComboBox comboBox = (ComboBox)dataBinding.control;
                            CodeValue codeValue = new CodeValue();
                            codeValue.code = (m_dataRow == null) ? 0 : ColumnData.GetInt32(m_dataRow, dataBinding.enumeration);
                            if (m_dataRow == null)
                            {
                                codeValue = comboBox.Items.Count == 0 ? null : (CodeValue)comboBox.Items[0];
                            }
                            else
                            {
                                codeValue.code = ColumnData.GetInt32(m_dataRow, dataBinding.enumeration);
                            }
                            comboBox.SelectedItem = codeValue;
                        }
                        else
                        {
                            dataBinding.control.Text = (m_dataRow == null)
                                                     ? ""
                                                     : ColumnData.GetString(m_dataRow, dataBinding.enumeration);
                        }
                    }
                }
            }
            m_suspendValueChanged = false;

            // There's a record available so enable the fields
            enableFields(true);

            // There's a record available so we can add a new one, delete the current
            // one, but cannot save nor cancel it (it's not being added nor modified)
            if (!m_suspendGuiChanged)
            {
                m_gui.setGuiEnabled(true, true, false);
            }
        }

        private void addRecord()
        {
            // Clear the fields by sending null but do not lose the current m_dataRow
            DataRow currentDataRow = m_dataRow;
            m_dataRow = null;
            viewRecord();
            m_dataRow = currentDataRow;
            if (!m_suspendGuiChanged)
            {
                m_gui.setGuiEnabled(false, false, true);
            }
        }

        public void showDataRow(DataRow dataRow)
        {
            DataRow currentDataRow = m_dataRow;
            m_dataRow = dataRow;
            viewRecord();
            m_dataRow = currentDataRow;
        }

        private void editRecord()
        {
            m_gui.setGuiEnabled(false, false, true);
            enableFields(true);
            //TODO: Place the cursor within the first enabled field
        }

        public void recordAdd()
        {
            setState(TableIO.State.Add);
            enableFields(true);
        }

        static public Boolean recordDelete(List<TableHandler> listTableHandler)
        {
            DB.Instance.startTransaction();

            // Reverse the order of delete such that the "main" table is deleted last
            TableHandler tableHandler;
            String errorMessage;
            for (int tableHandlerIndex = listTableHandler.Count - 1; tableHandlerIndex >= 0; --tableHandlerIndex)
            {
                tableHandler = listTableHandler[tableHandlerIndex];
                errorMessage = tableHandler.getTableIO().recordDelete();
                if (errorMessage.Length != 0)
                {
                    DB.Instance.rollbackTransaction();
                    MessageBox.Show(errorMessage);
                    return false;
                }
            }

            DB.Instance.commitTransaction();
            return true;
        }

        public String recordDelete()
        {
            String id = m_gui.getCurrentEditor().getPrimaryId();
            DataRow[] dataRows = m_dataSet.Tables[m_tableName].Select(ColumnData.GetName(m_idField) + " = " + id);
            if (dataRows.Length != 0)
            {
                try
                {
                    DataRow m_dataRow = dataRows[0];
                    m_dataRow.Delete();
                    MySqlCommandBuilder mySqlCommandBuilder = new MySqlCommandBuilder(m_sqlDataAdapter);
                    m_sqlDataAdapter.Update(m_dataSet, m_tableName);
                    m_dataSet.AcceptChanges();
                    mySqlCommandBuilder.Dispose();
                    m_dataRow = null;
                }
                catch(MySqlException e)
                {
                    switch (e.Number)
                    {
                        case 1451:
                            return "This record cannot be deleted because it is used in other tables\n\n"
                                 + e.GetBaseException().Message;
                        default:
                            return "MySqlException: " + e.Number.ToString();
                    }
                }
            }
            return "";


/*
            String query = "DELETE FROM "
                         + m_tableName
                         + " WHERE "
                         + ColumnData.GetName(m_idField)
                         + " = "
                         + m_dataRow[0].ToString();
            m_dataRow.Delete();
            m_sqlDataAdapter.Update(m_dataset);
            m_dataSet.AcceptChanges();
            m_dataRow = null;
            setState(TableIO.State.View);
            viewRecord();
*/
        }

        private void updateFields(DataRow dataRow)
        {
            Boolean newRecord = m_state.Equals(State.Add);
            foreach (DataBinding dataBinding in listDataBinding)
            {
                if (dataBinding.enumeration != null
                    && !m_recordManager.updateField(newRecord, dataBinding, dataRow)
                    && dataBinding.control != null)
                {
                    if (dataBinding.control.GetType() == typeof(CheckBox))
                    {
                        dataRow[ColumnData.GetName(dataBinding.enumeration)] = ((CheckBox)dataBinding.control).Checked ? 1 : 0;
                    }
                    else if (dataBinding.control.GetType() == typeof(ComboBox))
                    {
                        ComboBox comboBox = (ComboBox)dataBinding.control;
                        CodeValue codeValue = (CodeValue)comboBox.SelectedItem;
                        dataRow[ColumnData.GetName(dataBinding.enumeration)] = codeValue.code;
                    }
                    else if (dataBinding.control.GetType() == typeof(ListBox))
                    {
                        ListBox listBox = (ListBox)dataBinding.control;
                        CodeValue codeValue = (CodeValue)listBox.SelectedItem;
                        dataRow[ColumnData.GetName(dataBinding.enumeration)] = codeValue.code;
                    }
                    else
                    {
                        dataRow[ColumnData.GetName(dataBinding.enumeration)] = dataBinding.control.Text;
                    }
                }
            }
        }

        public DataValidation.Result recordValid()
        {
            Boolean newRecord = m_state.Equals(State.Add);
            DataValidation.Result dataValidationResult = new DataValidation.Result();
            String value;
            foreach (DataBinding dataBinding in listDataBinding)
            {
                dataValidationResult = null;
                // Process the list of validations
                foreach (DataValidation dataValidation in dataBinding.m_listDataValidation)
                {
                    if (dataBinding.control.GetType() == typeof(ComboBox))
                    {
                        ComboBox comboBox = (ComboBox)dataBinding.control;
                        CodeValue codeValue = (CodeValue)comboBox.SelectedItem;
                        if (codeValue == null)
                        {
                            value = null;
                        }
                        else
                        {
                            value = codeValue.code.ToString();
                        }
                    }
                    else
                    {
                        value = dataBinding.control.Text;
                    }

                    dataValidationResult = dataValidation.validate(value);
                    if (dataValidationResult != null && !dataValidationResult.isValid())
                    {
                        break;
                    }
                }

                if ((dataValidationResult == null || dataValidationResult.isValid())
                    && dataBinding.enumeration != null)
                {
                    // Process any special validations specified within the record manager.
                    dataValidationResult = m_recordManager.validField(newRecord, dataBinding, m_dataRow);
                }

                if (dataValidationResult != null && !dataValidationResult.isValid())
                {
                    dataValidationResult.setControlName(dataBinding.controlName);
                    setFocusOnError(dataBinding.control);
                    break;
                }
            }
            return dataValidationResult;
        }

        private void setFocusOnError(Control controlInError)
        {
            // 1. Ensure the proper page is shown
            Control parentTabPage = controlInError.Parent;
            while (parentTabPage != null && parentTabPage.GetType() != typeof(TabPage))
            {
                parentTabPage = parentTabPage.Parent;
            }
            if (parentTabPage != null && parentTabPage.GetType() == typeof(TabPage))
            {
                m_gui.getTabControl().SelectedTab = (TabPage)parentTabPage;
            }

            // 2. Put the keyboard focus within the control in error
            controlInError.Select();
        }

        public Boolean recordSave(String primaryId)
        {
            Boolean newRecord = m_state.Equals(State.Add);
            if (newRecord)
            {
                DataTable dataTable = m_dataSet.Tables[m_tableName];
                DataRow newDataRow = dataTable.NewRow();
                newDataRow[ColumnData.GetName(m_idField)] = primaryId;
                updateFields(newDataRow);
                Object[] columns = newDataRow.ItemArray;
                dataTable.Rows.Add(newDataRow);
                MySqlCommandBuilder mySqlCommandBuilder = new MySqlCommandBuilder(m_sqlDataAdapter);
                m_sqlDataAdapter.Update(m_dataSet, m_tableName);
                m_dataSet.AcceptChanges();
                mySqlCommandBuilder.Dispose();
                m_dataRow = newDataRow;
            }
            else // Edit
            {
                try
                {
                    // http://www.codersource.net/csharp_adonet_tutorial_ed.html
                    m_dataRow.BeginEdit();
                    updateFields(m_dataRow);
                    m_dataRow.EndEdit();
                    if (m_dataRow.HasErrors)
                    {
                        DataColumn[] errorColumns = m_dataRow.GetColumnsInError();
                        for (int error = 0; error < errorColumns.Length; error++)

                        {
                            // TODO: Handle column validation errors
                            MessageBox.Show("Validation error with column " + errorColumns[error].ColumnName);
                        }
                        if (errorColumns.Length != 0)
                        {
                            return false;
                        }
                    }
                    else
                    {

                        /*DataRow[] dataRows = new DataRow[]{m_dataRow};
                        m_dataSet.Merge(dataRows);
                        DataSet dsChanges = m_dataSet.GetChanges(DataRowState.Modified);
                        if (dsChanges.HasErrors)
                        {
                           m_dataSet.RejectChanges();
                        }
                        else
                        {
                            MySqlCommandBuilder mySqlCommandBuilder = new MySqlCommandBuilder(m_sqlDataAdapter);
                            m_sqlDataAdapter.Update(dsChanges, m_tableName);
                            m_dataSet.AcceptChanges();
                        }*/


                        MySqlCommandBuilder mySqlCommandBuilder = new MySqlCommandBuilder(m_sqlDataAdapter); // Last one wins
                        DataRow[] dataRows = new DataRow[]{m_dataRow};
                        m_sqlDataAdapter.Update(dataRows);
                        mySqlCommandBuilder.Dispose();
                    }
                }
                catch (System.Data.DBConcurrencyException e)
                {
//DB.printDataSetContents(m_dataSet, "TableIO.recordSave()");
                    // http://msdn.microsoft.com/en-us/library/tf579hcz(VS.80).aspx
                    MessageBox.Show("Another user has modified the current record.\n"
                                   + "To perform your modifications:\n"
                                   + "\t1. Jot down your changes\n"
                                   + "\t2. Cancel your changes\n"
                                   + "\t3. Modify the record again"
                                   + "\n\n" + e.Message
                                   + "\n\n" + e.StackTrace,
                                   "Changes Cannot be Saved");
                    return false;
                }
            }
            return true;
        }

        public void recordCancel()
        {
            if (m_state.Equals(TableIO.State.Edit))
            {
                String currentId = ColumnData.GetString(m_dataRow, m_idField);
                setState(TableIO.State.View);
                if (recordSearch("SELECT * FROM "
                                + m_tableName
                                + " WHERE "
                                + ColumnData.GetName(m_idField)
                                + " = "
                                + currentId))
                {
                    viewRecord();
                }
            }
            else
            {
                setState(TableIO.State.View);
            }
        }

        private void enableFields(Boolean enabled)
        {
            Boolean fieldEnabled;
            foreach (DataBinding dataBinding in listDataBinding)
            {
                // Determine whether this field should be enabled
                if (enabled == false)
                {
                    // Everyone gets disabled, without exceptions
                    fieldEnabled = false;
                }
                else
                {
                    // Enabling a field is tricky since there are exceptions
                    if (m_listControlsNeverEnabled.Contains(dataBinding.control))
                    {
                        // The control is in the list of fields to never enable
                        fieldEnabled = false;
                    }
                    else if (!m_state.Equals(State.Add)
                             && m_listControlsEnabledOnlyOnAdd.Contains(dataBinding.control))
                    {
                        // This is not the add state and the control is in the list of
                        // controls that are only enabled on add
                        fieldEnabled = false;
                    }
                    else if (!m_state.Equals(State.View)
                        && m_listControlsEnabledOnlyOnView.Contains(dataBinding.control))
                    {
                        // This is not the view state and the control is in the list of
                        // controls that are only enabled on view
                        fieldEnabled = false;
                    }
                    else
                    {
                        // Looks like it's ok to enable the field
                        fieldEnabled = enabled;
                    }
                }

                // Is there an override within the record manager?
                if (dataBinding.enumeration == null
                    || (dataBinding.enumeration != null
                        && !m_recordManager.enableField(dataBinding, m_dataRow, fieldEnabled)))
                {
                    if (dataBinding.control != null)
                    {
                        dataBinding.control.Enabled = fieldEnabled;
                    }
                }
            }
        }

        public DataBinding bind(Control control, Enum enumeration, String controlName)
        {
            return bind(control, enumeration, controlName, true);
        }

        /// <summary>
        ///   <para>Associate a control/widget with a database field.</para>
        ///   <para>This list is used to automate the display, validation, and update of data.</para>
        /// </summary>
        /// <returns>The data binding instance created for the control and field association</returns>
        /// <param name="control">The control/widget on the form</param>
        /// <param name="enumeration">The field within the database</param>
        /// <param name="controlName">The name of the control/widget, to be displayed within validation errors</param>
        /// <param name="autoEditMode">Whether changes within that field trigger a state change to edit mode</param>
        /// <remarks>The returned DataBinding can be used to specify data validations</remarks>
        public DataBinding bind(Control control, Enum enumeration, String controlName, Boolean autoEditMode)
        {
            if (control != null && enumeration != null)
            {
                // Specify the database table name and column name as the tooltip.  This first needs to
                // be cleaned up as the table information initially results in:
                // Net7_Tools.Net7+Table_item_reactor
                String databaseTableAndField = enumeration.GetType().ToString() + "." + ColumnData.GetName(enumeration);
                int index = databaseTableAndField.Contains("+") ? databaseTableAndField.IndexOf('+') : 0;
                if (index != 0)
                {
                    databaseTableAndField = databaseTableAndField.Substring(index + 1);
                }
                if (databaseTableAndField.StartsWith("Table_"))
                {
                    databaseTableAndField = databaseTableAndField.Substring(6);
                }
                ToolTip toolTip = new ToolTip();
                String database = enumeration.GetType().ToString();
                toolTip.SetToolTip(control, databaseTableAndField);
            }

            DataBinding dataBinding = new DataBinding(control, enumeration, controlName);
            listDataBinding.Add(dataBinding);
            if (autoEditMode && control != null)
            {
                if (control.GetType() == typeof(CheckBox))
                {
                    ((CheckBox)control).CheckedChanged += new EventHandler(valueChanged);
                }
                else
                {
                    control.TextChanged += new EventHandler(valueChanged);
                }
            }

            if (control != null && control.GetType() == typeof(TextBox))
            {
                control.Enter += new EventHandler(onTextBoxEnter);
            }
            return dataBinding;
        }

        void onTextBoxEnter(object sender, EventArgs e)
        {
            TextBox textBox = (TextBox)sender;
            textBox.SelectAll();
        }

        /// <summary>
        ///   <para>
        ///     A generic event handler to detect a change in a field
        ///     and trigger a state change
        ///   </para>
        /// </summary>
        private void valueChanged(object sender, EventArgs e)
        {
            if(!m_suspendValueChanged && m_state.Equals(State.View))
            {
                setState(State.Edit);
            }
        }

        /// <summary>
        ///   <para>Select a record based on the specified select criteria.</para>
        /// </summary>
        /// <example>String selectCommand = "id = " + currentId;</example>
        public Boolean recordSearch(String selectCommand)
        {
            int idPosition = selectCommand.IndexOf('=');
            if (idPosition != -1)
            {
                String id = selectCommand.Substring(idPosition + 2);
                String subSelectCommand = ColumnData.GetName(m_idField) + " = " + id;

                // Always reset the data set and retrieve data from the server
                try
                {
                    m_dataSet.Clear();
                    m_sqlDataAdapter.Dispose();
                    m_sqlDataAdapter = new MySqlDataAdapter(selectCommand, DB.Instance.openConnection());
                    m_sqlDataAdapter.Fill(m_dataSet, m_tableName);
                    m_dataSet.AcceptChanges();
                    DataRow[] dataRows = m_dataSet.Tables[m_tableName].Select(subSelectCommand);
                    if (dataRows.Length == 0 && isRecordOptional())
                    {
                        m_dataRow = null;
                    }
                    else if (dataRows.Length != 0)
                    {
                        // Display the first/only record matching the select command
                        m_dataRow = dataRows[0];
                        return true;
                    }
                }
                catch (MySqlException)
                {
                    return false;
                }
            }
            return false;
        }

        public IRecordManager getRecordManager()
        {
            return m_recordManager;
        }

        public Control findControl(Enum enumeration)
        {
            foreach (DataBinding dataBinding in listDataBinding)
            {
                if (dataBinding.enumeration.Equals(enumeration))
                {
                    return dataBinding.control;
                }
            }
            return null;
        }

    }
}
