using System;
using System.Collections.Generic;
using System.Text;
using MySql.Data.MySqlClient;
using System.Data;

using N7.Utilities;

namespace N7.SQL
{
    static class Database
    {
        public enum DatabaseName { net7, net7_db };
        public static MySqlConnection mySqlConnection;

        static public void initiliazeDB(DatabaseName databaseName)
        {
            mySqlConnection = new MySqlConnection(SQLData.ConnStr(databaseName.ToString()));
            mySqlConnection.Open();
        }

        static public DataTable executeQuery(String query)
        {
            DataTable dataTable = new DataTable();
            MySqlDataAdapter mySqlDataAdapter = new MySqlDataAdapter(query, mySqlConnection);
            mySqlDataAdapter.Fill(dataTable);
            
            return dataTable;
        }

        static public void closeDB()
        {
            mySqlConnection.Close();
        }
    }
}
