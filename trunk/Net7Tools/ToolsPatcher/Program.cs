using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace ToolsPatcher
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            string exefile = "";
            if (args.Length != 0 && args[0] != "")
            {
                exefile = args[0];
            }
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1(exefile));
        }
    }
}