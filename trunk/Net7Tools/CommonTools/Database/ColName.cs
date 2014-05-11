using System;
using System.Collections.Generic;
using System.Text;

namespace CommonTools.Database
{
    /// <summary>
    ///   <para>An attribute to store the "real" database column name rather
    ///         than the textual conversion of an enum value.</para>
    /// </summary>
    class ColName : Attribute
    {
	    public readonly string Text;
        public ColName(string text)
	    {
		    Text = text;
	    }
    }
}
