using System;
using System.Collections.Generic;
using System.Text;
using System.Reflection;

namespace CommonTools.Database
{
    public class ColumnData
    {
        public class ColumnDataInfo
        {
            public String DataType;
            public Boolean TypeIsString;
            public String Name;

            public override string ToString()
            {
                return Name;
            }
        }

        /// <summary>
        ///   <para>Retrieve the "ColName" attribute from an attribute.</para>
        /// </summary>
        /// <returns>The "real" column name.</returns>
        /// <param name="tableEnumValue">The table enumeration value</param>
        /// <param name="columnEnumValue">The column enumeration value</param>
        public static String GetName(Enum tableEnumValue, Enum columnEnumValue)
        {
            return tableEnumValue.ToString() + "." + GetName(columnEnumValue);
        }

        /// <summary>
        ///   <para>Retrieve the "ColName" attribute from an attribute.</para>
        /// </summary>
        /// <returns>The "real" column name.</returns>
        /// <param name="enumValue">The enumeration value</param>
        public static String GetName(Enum enumValue)
        {
            string columnName = "";
            Type type = enumValue.GetType();
            MemberInfo[] memberInfo = type.GetMember(enumValue.ToString());
            if (memberInfo != null && memberInfo.Length == 1)
            {
                object[] attributes = memberInfo[0].GetCustomAttributes(typeof(ColName), false);
                if (attributes.Length == 1)
                {
                    columnName = ((ColName)attributes[0]).Text;
                }
            }
            return columnName;
        }

        /// <summary>
        ///   <para>Retrieve the DataType attribute from an attribute.</para>
        /// </summary>
        /// <returns>The DataType of the column.</returns>
        /// <param name="enumValue">The enumeration value</param>
        public static String GetDataType(Enum enumValue)
        {
            string dataType = "";
            Type type = enumValue.GetType();
            MemberInfo[] memberInfo = type.GetMember(enumValue.ToString());
            if (memberInfo != null && memberInfo.Length == 1)
            {
                object[] attributes = memberInfo[0].GetCustomAttributes(typeof(DataType), false);
                if (attributes.Length == 1)
                {
                    dataType = ((DataType)attributes[0]).Text;
                }
            }
            return dataType;
        }

        /// <summary>
        ///   Whether the DataType attribute is a String: char, longtext, text, varchar.
        /// </summary>
        /// <returns>True when the DataType is a String, false otherwhise</returns>
        /// <param name="enumValue">The enumeration value</param>
        public static Boolean IsDataTypeString(Enum enumValue)
        {
            String dataType = GetDataType(enumValue);
            return dataType.CompareTo("char") == 0
                || dataType.CompareTo("longtext") == 0
                || dataType.CompareTo("text") == 0
                || dataType.CompareTo("varchar") == 0;

        }

        /// <summary>
        ///   <para>Retrieve the String value of the "ColName" attribute from an attribute.</para>
        /// </summary>
        /// <returns>The String value of the column name.</returns>
        /// <param name="dataRow">The data row</param>
        /// <param name="enumValue">The enumeration value</param>
        public static String GetString(System.Data.DataRow dataRow, Enum enumValue)
        {
            string columnName = GetName(enumValue);
            return dataRow[columnName].ToString();
        }

        /// <summary>
        ///   <para>Retrieve the Int32 value of the "ColName" attribute from an attribute.</para>
        /// </summary>
        /// <returns>The Int32 value of the column name.</returns>
        /// <param name="dataRow">The data row</param>
        /// <param name="enumValue">The enumeration value</param>
        public static Int32 GetInt32(System.Data.DataRow dataRow, Enum enumValue)
        {
            string columnName = GetName(enumValue);
            String tableName = dataRow.Table.TableName;
            return System.Int32.Parse(dataRow[columnName].ToString());
        }

        /// <summary>
        ///   <para>Retrieve the Int32 value of the "ColName" attribute from an attribute.</para>
        /// </summary>
        /// <returns>The Int32 value of the column name.</returns>
        /// <param name="dataRow">The data row</param>
        /// <param name="enumValue">The enumeration value</param>
        public static Boolean GetBoolean(System.Data.DataRow dataRow, Enum enumValue)
        {
            string columnName = GetName(enumValue);
            string Data = dataRow[columnName].ToString();

            return (Data == "true" || Data == "1");
        }
    
    }
}
