using System;
using System.Collections.Generic;
using System.Text;
using System.Collections.Specialized;
using System.Runtime.InteropServices;

namespace LaunchNet7
{
    public static class IniUtility
    {
        #region DllImports

        [DllImport("KERNEL32.DLL", EntryPoint = "GetPrivateProfileString")]
        private static extern int GetPrivateProfileString(string lpAppName,
            string lpKeyName, string lpDefault,
            StringBuilder lpReturnedString, int nSize,
            string lpFileName);

        [DllImport("KERNEL32.DLL")]
        private static extern int GetPrivateProfileInt(string lpAppName,
            string lpKeyName, int iDefault,
            string lpFileName);

        [DllImport("KERNEL32.DLL", EntryPoint = "WritePrivateProfileString")]
        private static extern bool WritePrivateProfileString(string lpAppName,
            string lpKeyName, string lpString,
            string lpFileName);

        [DllImport("KERNEL32.DLL", EntryPoint = "GetPrivateProfileSection")]
        private static extern int GetPrivateProfileSection(string lpAppName,
            byte[] lpReturnedString, int nSize,
            string lpFileName);

        [DllImport("KERNEL32.DLL", EntryPoint = "WritePrivateProfileSection")]
        private static extern bool WritePrivateProfileSection(string lpAppName,
            byte[] data, string lpFileName);

        [DllImport("KERNEL32.DLL", EntryPoint = "GetPrivateProfileSectionNames")]
        private static extern int GetPrivateProfileSectionNames(
            byte[] lpReturnedString,
            int nSize, string lpFileName);

        #endregion

        /// <summary>
        /// Reading a value from an INI-File.
        /// </summary>
        /// <param name="filename">INI-filename to read from.</param>
        /// <param name="section">Section within the INI-File.</param>
        /// <param name="key">The INI-Key to read.</param>
        /// <returns>A string representing the value read from the INI-file. Returns null if value was not found or another problem occured.</returns>
        public static string GetValue(string filename, string section, string key)
        {
            StringBuilder buffer = new StringBuilder(256);
            string sDefault = "";
            if (GetPrivateProfileString(section, key, sDefault,
                buffer, buffer.Capacity, filename) != 0)
            {
                return buffer.ToString();
            }
            else
            {
                return null;
            }
        }

        /// <summary>
        /// Writes a value into an INI-File.
        /// </summary>
        /// <param name="filename">The filename of the INI-File.</param>
        /// <param name="section">The section to write to.</param>
        /// <param name="key">The name of the key, which the values belongs to.</param>
        /// <param name="value">The value, which have to be written.</param>
        /// <returns>Returns true, if the value have been written sucessfully, otherwise returns false.</returns>
        public static bool SetValue(string filename, string section, string key, string value)
        {
            return WritePrivateProfileString(section, key, value, filename);
        }

        public static int GetINIInt(String filename,
            String section, String key)
        {
            int iDefault = -1;
            return GetPrivateProfileInt(section, key,
                iDefault, filename);
        }

        public static StringCollection GetSection(
            String filename, String section)
        {
            StringCollection items = new StringCollection();
            byte[] buffer = new byte[32768];
            int bufLen = 0;
            bufLen = GetPrivateProfileSection(section, buffer,
                buffer.GetUpperBound(0), filename);
            if (bufLen > 0)
            {
                StringBuilder sb = new StringBuilder();
                for (int i = 0; i < bufLen; i++)
                {
                    if (buffer[i] != 0)
                    {
                        sb.Append((char)buffer[i]);
                    }
                    else
                    {
                        if (sb.Length > 0)
                        {
                            items.Add(sb.ToString());
                            sb = new StringBuilder();
                        }
                    }
                }
            }
            return items;
        }

        public static bool SetSection(string filename, string
            section, StringCollection items)
        {
            byte[] b = new byte[32768];
            int j = 0;
            foreach (string s in items)
            {
                ASCIIEncoding.ASCII.GetBytes(s, 0, s.Length, b, j);
                j += s.Length;
                b[j] = 0;
                j += 1;
            }
            b[j] = 0;
            return WritePrivateProfileSection(section, b, filename);
        }

        public static StringCollection GetSectionNames(
            String filename)
        {
            StringCollection sections = new StringCollection();
            byte[] buffer = new byte[32768];
            int bufLen = 0;
            bufLen = GetPrivateProfileSectionNames(buffer,
                buffer.GetUpperBound(0), filename);
            if (bufLen > 0)
            {
                StringBuilder sb = new StringBuilder();
                for (int i = 0; i < bufLen; i++)
                {
                    if (buffer[i] != 0)
                    {
                        sb.Append((char)buffer[i]);
                    }
                    else
                    {
                        if (sb.Length > 0)
                        {
                            sections.Add(sb.ToString());
                            sb = new StringBuilder();
                        }
                    }
                }
            }
            return sections;
        }

        public static bool KeyExist(string filename, string section, string key)
        {
            int iDefault = -1;
            return !(GetPrivateProfileInt(section, key, iDefault, filename) == -1);
        }

    }
}
