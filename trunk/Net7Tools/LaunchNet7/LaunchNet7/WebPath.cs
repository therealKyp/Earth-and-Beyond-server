using System;
using System.Collections.Generic;
using System.Text;

namespace LaunchNet7
{
    public static class WebPath
    {
        public static string Combine(string url1, string url2)
        {
            if (url1.EndsWith("/") == false) url1 += '/';
            return url1 + url2;
        }
    }
}
