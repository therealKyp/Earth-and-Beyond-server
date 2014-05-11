using System;
using System.Collections.Generic;
using System.Text;

namespace N7.Utilities
{
	static class SQLData
	{
		private static string m_Host;
		private static int m_Port;

		private static string m_User;
		private static string m_Pass;

		public static string Host
		{
			get { return m_Host; }
			set { m_Host = value; }
		}

		public static int Port
		{
			get { return m_Port; }
			set { m_Port = value; }
		}

		public static string User
		{
			get { return m_User; }
			set { m_User = value; }
		}

		public static string Pass
		{
			get { return m_Pass; }
			set { m_Pass = value; }
		}

		public static string ConnStr(string DB)
		{
			return "Connect Timeout=30;Persist Security Info=False;Database=" + DB + ";Host=" + m_Host + ";Port=" + m_Port.ToString() + ";Username=" + m_User + ";Password=" + m_Pass;
		}
	}
}
