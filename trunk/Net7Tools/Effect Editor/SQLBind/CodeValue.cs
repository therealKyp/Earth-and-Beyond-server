using System;
using System.Collections.Generic;
using System.Text;

namespace SQLBind.Tools
{
    /// <summary>
    ///   <para>Store a code/id and a value/description such that the code/id
    ///         is easily retrievable and the value/description is displayed
    ///         in a control such as a combobox</para>
    /// </summary>
    public class CodeValue
    {
        public Int32 code;
        public String value;

        public CodeValue()
        {
            code = 0;
            value = "";
        }

        public CodeValue(Int32 code)
        {
            this.code = code;
            this.value = "";
        }

        public CodeValue(Int32 code, String value)
        {
            this.code = code;
            this.value = value;
        }

        public static CodeValue Formatted(int code, String value)
        {
            String formattedValue = value
                                  + " ("
                                  + code.ToString()
                                  + ")";
            return new CodeValue(code, formattedValue);
        }

        /// <summary>
        ///   <para>Text displayed within controls such as comboboxes</para>
        /// </summary>
        public override string ToString()
        {
            return value;
        }

        /// <summary>
        ///   <para>Equality is based only on the value/id rather than the entire class</para>
        /// </summary>
        public override bool Equals(object obj)
        {
            if (obj.GetType() != typeof(CodeValue))
                return false;
            return code.Equals(((CodeValue)obj).code);
        }

        public override int GetHashCode()
        {
            return base.GetHashCode();
        }
    }
}
