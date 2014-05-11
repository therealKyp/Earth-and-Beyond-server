using System;
using System.Collections.Generic;
using System.Text;

namespace ToolsLauncher
{
	static class Utility
	{
		public static string FormDescription(string Src)
		{
			return Src.Replace("\r", "").Replace("\n", "\\n");
		}

		public static string ParseDescription(string Src)
		{
			return Src.Replace("\\n", "\r\n");
		}

		public static string FormParen(int Data, int Width)
		{
			return FormParen(Data.ToString(), Width);
		}

		public static string FormParen(string Data, int Width)
		{
			if (Data.Length > Width)
			{
				return "Invalid";
			}

			string result = "(";

			for (int i = 0; i < Width - Data.ToString().Length; i++)
				result += "0";

			result += Data + ")";

			return result;
		}

		public static int ParseParen(string Src)
		{
			int Begin = Src.IndexOf('(');
			if (Begin == -1)
				return -1;

			int End = Src.IndexOf(')', Begin);
			if (End == -1)
				return -1;

			string result = Src.Substring(Begin + 1, End - Begin - 1);

			try
			{
				return Int32.Parse(result);
			}
			catch
			{
				return -1;
			}
		}
	}

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
