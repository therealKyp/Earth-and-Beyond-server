using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace MissionEditor.Database
{
    class CodeDescSearch
    {
        private DataConfiguration.DataType m_dataType;
        private TextBox m_code;
        private TextBox m_description;
        private Button m_search;

        public CodeDescSearch(TextBox code, TextBox description, Button search)
        {
            m_code = code;
            m_code.Leave += new EventHandler(onLeaveId);

            m_description = description;

            m_search = search;
            m_search.Click += new EventHandler(onSearch);
        }

        public void setDataType(DataConfiguration.DataType dataType)
        {
            m_dataType = dataType;
        }

        void onLeaveId(object sender, EventArgs e)
        {
            setDescription();
        }

        private void onSearch(object sender, EventArgs e)
        {
            String id = DataConfiguration.search(m_dataType);
            if (id.Length != 0)
            {
                m_code.Text = id;
                setDescription();
            }
        }

        private void setDescription()
        {
            m_description.Text = DataConfiguration.getDescription(m_dataType, m_code.Text);
        }

    }
}
