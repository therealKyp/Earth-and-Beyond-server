using System;
using System.Collections.Generic;
using System.Text;
using Net7_Tools.Database;

namespace Net7_Tools.Utilities
{
    public class SearchCriteria
    {
        public Database_net7.Table_item_base column;
        public int comparisonSymbol;
        public string criteria;
        public String subQuery;
        public String sqlParameter;
        public String sqlValue;
        private static String[] m_comparatorSymbol = { "<", "<=", "=", ">=", ">", "like" };

        public SearchCriteria()
        {
            comparisonSymbol = 0;
            criteria = "";
        }

        public void Clear()
        {
            comparisonSymbol = 0;
            criteria = "";
            subQuery = "";
            sqlParameter = "";
            sqlValue = "";
        }

        public Boolean IsEmpty()
        {
            return subQuery == null || subQuery.Length == 0;
        }

        public override String ToString()
        {
            String cleanedQuery = subQuery;
            String parameter = DB.QueryParameterCharacter + sqlParameter;
            if(cleanedQuery.Contains(parameter))
            {
                int position = cleanedQuery.IndexOf(parameter);
                cleanedQuery = cleanedQuery.Substring(0, position)
                             + sqlValue
                             + cleanedQuery.Substring(position + parameter.Length);
            }
            return cleanedQuery;
        }

        public String getQuery()
        {
            return ColumnData.GetName(column) + " "
                 + getComparatorSymbol() + " "
                 + DB.QueryParameterCharacter + sqlParameter;
        }

        public String getComparatorSymbol()
        {
            return m_comparatorSymbol[comparisonSymbol];
        }

        static public String getComparatorSymbol(int symbol)
        {
            return m_comparatorSymbol[symbol];
        }

    }
}
