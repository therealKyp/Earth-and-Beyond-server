using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Net;
using System.IO;

namespace LaunchNet7
{
    public static class LauncherUtility
    {
        [DllImport("kernel32.dll")]
        static extern int GetShortPathName(string longPath, StringBuilder buffer, int bufferSize);

        public static string GetShortPathName(string path)
        {
            StringBuilder buffer = new StringBuilder(256);
            GetShortPathName(path, buffer, buffer.Capacity);
            return buffer.ToString();
        }

        public static string DownloadString(string url)
        {
            return DownloadString(url, (int)Program.LauncherConfiguration.AutoUpdate.Timeout.TotalMilliseconds);
        }
        public static string DownloadString(string url, int timeoutInMilliseconds)
        {
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create(url);
            request.Timeout = timeoutInMilliseconds;
            using (HttpWebResponse response = (HttpWebResponse)request.GetResponse())
            {
                StreamReader reader = new StreamReader(response.GetResponseStream());
                return reader.ReadToEnd();
            }
        }
    }
}
