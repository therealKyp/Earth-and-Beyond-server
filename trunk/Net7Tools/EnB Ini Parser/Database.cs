using System;
using System.Collections.Generic;
using System.Text;
using MySql.Data.MySqlClient;
using System.Data;

namespace EnBIniParser
{
    static class Database
    {

        public enum DatabaseName { net7, net7_db };

        static public DataTable executeQuery(DatabaseName databaseName, String query)
        {
            DataTable dataTable = new DataTable();
            MySqlConnection mySqlConnection = new MySqlConnection(SQLData.ConnStr(databaseName.ToString()));
            mySqlConnection.Open();
            MySqlDataAdapter mySqlDataAdapter = new MySqlDataAdapter(query, mySqlConnection);
            mySqlDataAdapter.Fill(dataTable);
            mySqlConnection.Close();
            return dataTable;
        }
    }
}
