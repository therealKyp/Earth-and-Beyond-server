using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MySql.Data.MySqlClient;

namespace SQLBind
{
    static class DB
    {
        private static MySqlConnection m_conn;
        private static String m_LoginStr;

        public static void SetLogin(String Host, String Port, String DataBase, String User, String Pass)
        {
            String LoginStr = "server="+Host+";database="+DataBase+";port="+Port+";uid="+User+";password="+Pass;
            m_LoginStr = LoginStr;
            m_conn = new MySqlConnection(LoginStr);
        }

        public static string GetLoginStr()
        {
            return m_LoginStr;
        }

        public static void Connect()
        {
            m_conn.Open();
        }

        public static MySqlConnection GetConnection()
        {
            return m_conn;
        }
    }
}
