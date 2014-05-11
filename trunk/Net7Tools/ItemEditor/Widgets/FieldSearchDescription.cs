using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using Net7_Tools.Search;
using System.Data;
using Net7_Tools.Database;
using CommonTools.Database;

namespace Net7_Tools.Widgets
{
    class FieldSearchDescription
    {
        public class Fields
        {
            public TextBox m_field;
            public Button m_search;
            public TextBox m_description;

            public Fields(TextBox field,
                          Button search,
                          TextBox description)
            {
                m_field = field;
                m_search = search;
                m_description = description;
            }
        }

        public class Data
        {
            public IWidgetModified m_widgetModified;
            public ISearchDialog m_searchDialog;
            public String m_descriptionQuery;
            public String m_descriptionParameter;
            public Enum m_descriptionColumn;

            public Data(IWidgetModified widgetModified,
                        ISearchDialog searchDialog,
                        String descriptionQuery,
                        String descriptionParameter,
                        Enum descriptionColumn)
            {
                m_widgetModified = widgetModified;
                m_searchDialog = searchDialog;
                m_descriptionQuery = descriptionQuery;
                m_descriptionParameter = descriptionParameter;
                m_descriptionColumn = descriptionColumn;
            }
        }

        private Fields m_fields;
        private Data m_data;

        public Fields getFields()
        {
            return m_fields;
        }

        public Data getData()
        {
            return m_data;
        }

        public FieldSearchDescription(Fields fields,
                                      Data data)
        {
            m_fields = fields;
            m_data = data;

            // Configure the leave event
            m_fields.m_field.Leave += new EventHandler(field_Leave);

            // Configure the search click
            m_fields.m_search.Click += new EventHandler(search_Click);
        }

        /// <summary>
        ///   <para>Left the field, update the description if valid</para>
        /// </summary>
        private void field_Leave(object sender, EventArgs e)
        {
            setDescription();
        }

        /// <summary>
        ///   <para>The search button was clicked, display the search dialog, set the field and description.</para>
        /// </summary>
        private void search_Click(object sender, EventArgs e)
        {
            m_data.m_searchDialog.setSelectedItemId("");
            m_data.m_searchDialog.getForm().ShowDialog();
            if (m_data.m_searchDialog.getSelectedItemId().Length != 0)
            {
                m_fields.m_field.Text = m_data.m_searchDialog.getSelectedItemId();
                setDescription();
            }
        }

        public void setDescription()
        {
            DataTable dataTable = DB.Instance.executeQuery(m_data.m_descriptionQuery,
                                                  new String[] { m_data.m_descriptionParameter },
                                                  new String[] { m_fields.m_field.Text });
            String description = "";
            if (dataTable != null && dataTable.Rows.Count == 1)
            {
                DataRow dataRow = dataTable.Rows[0];
                description = ColumnData.GetString(dataRow, m_data.m_descriptionColumn);
            }
            m_fields.m_description.Text = description;
            m_data.m_widgetModified.widgetModified(m_fields.m_field);
        }

    }
}
