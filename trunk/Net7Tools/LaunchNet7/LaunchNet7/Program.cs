using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Configuration;
using System.Net;
using System.Text;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Threading;
using LaunchNet7.Configuration;
using LaunchNet7.Cryptography;

namespace LaunchNet7
{
    static class Program
    {
        [STAThread]
        static void Main(string[] args)
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            m_WorkingDirectory = Environment.CurrentDirectory;

            bool runSelfUpdateTest = false;

            string exeUpdateFileName = null;
            CommandLineReader reader = new CommandLineReader(args);
            while (reader.Read())
            {
                if (String.Equals(reader.CurrentArgument, "-delete", StringComparison.InvariantCultureIgnoreCase))
                {
                    reader.TryReadNextAsString(out exeUpdateFileName);
                }
                else if (String.Equals(reader.CurrentArgument, "-selfupdatetest", StringComparison.InvariantCultureIgnoreCase))
                {
                    runSelfUpdateTest = true;
                }
            }

            if (runSelfUpdateTest)
            {
                RunSelfUpdateTest();
                return;
            }

            try
            {
                // Delete Self-Update Exe
                if (String.IsNullOrEmpty(exeUpdateFileName) == false &&
                    File.Exists(exeUpdateFileName))
                    // && String.Equals(Path.GetFullPath(Path.GetTempPath()), Path.GetFullPath(Path.GetDirectoryName(exeUpdateFileName)), StringComparison.InvariantCultureIgnoreCase))
                {
                    int counter = 0;
                    while (true)
                    {
                        counter++;
                        if (counter > 30) break;

                        try
                        {
                            File.Delete(exeUpdateFileName);
                            break;
                        }
                        catch { }
                        Thread.Sleep(100);
                    }
                }
            }
            catch (Exception e)
            {
                // Just logging, no messages here
                LogException(e);
            }

            try
            {
                InitializeConfiguration();
            }
            catch (Exception e)
            {
                MessageBox.Show("Error on reading configuration.\nDetails: " + e.Message, Title + " - Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            string CurrentDir = Directory.GetCurrentDirectory();
            // Go down a dir when looking for the files

            string[] Dirs = CurrentDir.Split('\\');

            // If we are in bin do this
            if (Dirs[Dirs.Length - 1] == "bin")
            {
                Directory.SetCurrentDirectory(Directory.GetParent(CurrentDir).FullName);
            }

            Application.Run(new FormMain());

            Directory.SetCurrentDirectory(CurrentDir);
        }

        private static void RunSelfUpdateTest()
        {
            string tempString = Path.GetTempFileName();
            string temporaryFileName = Path.ChangeExtension(tempString, ".exe");
            if (File.Exists(temporaryFileName)) File.Delete(temporaryFileName);
            File.Move(tempString, temporaryFileName);
            File.WriteAllBytes(temporaryFileName, LauncherResources.ExeUpdater);

            string exeFileName = Application.ExecutablePath;
            string replaceFileName = exeFileName + ".tmp";
            File.Copy(Application.ExecutablePath, replaceFileName, true);
            Process.Start(temporaryFileName, String.Format("-exeFileName \"{0}\" -replaceFileName \"{1}\" -waitForPid {2}",
                Path.GetFullPath(exeFileName),
                Path.GetFullPath(replaceFileName),
            Process.GetCurrentProcess().Id));

            Thread.Sleep(1000);
        }

        private static void InitializeConfiguration()
        {
            ExeConfigurationFileMap map = new ExeConfigurationFileMap();
            map.ExeConfigFilename = "LaunchNet7.cfg";

            m_Configuration = ConfigurationManager.OpenMappedExeConfiguration(map, ConfigurationUserLevel.None);
            m_LauncherConfiguration = (LauncherConfigurationSection)m_Configuration.Sections[LauncherConfigurationSection.DefaultSectionName];
            if (m_LauncherConfiguration == null)
            {
                m_LauncherConfiguration = new LauncherConfigurationSection();
                m_LauncherConfiguration.SectionInformation.Type = "LaunchNet7.Configuration.LauncherConfigurationSection, LaunchNet7";
                m_LauncherConfiguration.SectionInformation.ForceSave = true;
                m_Configuration.Sections.Add(LauncherConfigurationSection.DefaultSectionName, m_LauncherConfiguration);
                m_Configuration.Save(ConfigurationSaveMode.Minimal);
            }
        }

        internal static string WorkingDirectory
        {
            get { return m_WorkingDirectory; }
        }
        private static string m_WorkingDirectory;

        public static System.Configuration.Configuration Configuration
        {
            get { return m_Configuration; }
            set { m_Configuration = value; }
        }
        private static System.Configuration.Configuration m_Configuration;

        public static LauncherConfigurationSection LauncherConfiguration
        {
            get { return m_LauncherConfiguration; }
        }
        private static LauncherConfigurationSection m_LauncherConfiguration;

        public static string Title
        {
            get { return "LaunchNet7"; }
        }

        public static string TitleForErrorMessages
        {
            get { return Title + " - Error"; }
        }

        public static string TitleForWarningMessages
        {
            get { return Title + " - Warning"; }
        }

        public static string TitleForInformationMessages
        {
            get { return Title + " - Information"; }
        }

        internal static void LogException(Exception e)
        {
#if DEBUG
            Debug.WriteLine(e.ToString());
#else
            try
            {
                File.AppendAllText("LaunchNet7.Error.log", String.Format("\n\nLaunchNet7 - Exception, Version: {0}, Time: {1}\n{2}",
                    AssemblyFileInfo.Current.FileVersion.ToString(),
                    DateTime.UtcNow.ToString("R"),
                    e.ToString()));
            }
            catch { }
#endif
        }
    }
}