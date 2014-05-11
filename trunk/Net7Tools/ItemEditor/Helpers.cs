using System;
using System.Collections.Generic;
using System.Text;

namespace Net7_Tools
{
	static class Utility
	{
		public static string FormatDescription(string Src)
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


        /// <summary>
        /// Retrieve the ID within parenthesis from a string formatted as "(123) Description"
        /// </summary>
        /// <param name="Src"></param>
        /// <returns></returns>
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

}
