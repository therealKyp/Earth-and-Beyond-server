using System;
using System.Collections.Generic;
using System.Text;

namespace CommonTools.Database
{
    /// <summary>
    ///   <para>An attribute to store the data type of the column.</para>
    /// </summary>
    class DataType : Attribute
    {
	    public readonly string Text;
        public DataType(string text)
	    {
		    Text = text;
	    }
    }
}
