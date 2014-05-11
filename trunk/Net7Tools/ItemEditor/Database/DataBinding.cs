using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace Net7_Tools.Database
{
    /// <summary>
    ///   <para>Associate a control (text, combobox, checkbox) with a database
    ///         field enumeration (see the contents of Database\net7_db.cs).
    ///         This is used to automate the transfer of data between the control
    ///         and the database.</para>
    /// </summary>
    public class DataBinding
    {
        public Control control;
        public Enum enumeration;
        public String controlName;
        public List<DataValidation> m_listDataValidation;

        public DataBinding()
        {
            control = null;
            enumeration = null;
            m_listDataValidation = new List<DataValidation>();
        }

        public DataBinding(Control control, Enum enumeration, String controlName)
        {
            this.control = control;
            this.enumeration = enumeration;
            this.controlName = controlName;
            m_listDataValidation = new List<DataValidation>();
        }

        public void addValidation(DataValidation dataValidation)
        {
            m_listDataValidation.Add(dataValidation);
        }
    }
}
