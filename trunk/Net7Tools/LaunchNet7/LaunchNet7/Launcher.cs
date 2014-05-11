using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Diagnostics;
using System.Threading;
using System.Net;
using Microsoft.Win32;
using LaunchNet7.Patching;

namespace LaunchNet7
{
    public class Launcher
    {
        public Launcher(LaunchSetting setting)
        {
            if (setting == null) throw new ArgumentNullException("setting");
            m_Setting = setting;
        }

        public LaunchSetting Setting
        {
            get { return m_Setting; }
            set { m_Setting = value; }
        }
        private LaunchSetting m_Setting;

        public void Launch()
        {
            PatchRegistry();
            PatchAuthLoginFile();
            PatchRegDataFileNames();
            PatchRegDataFile();
            PatchAuthIniFile();
            PatchNetworkIniFile();

            switch (Setting.LaunchName.ToUpperInvariant())
            {
                case "NET7SP":
                    {
                        LaunchNet7Server();
                        Thread.Sleep(25000); //allow 25 seconds for server startup
                        
                        LaunchNet7Proxy();
                    }
                    break;

                case "NET7MP":
                    {
                        LaunchNet7Proxy();
                    }
                    break;

                default:
                    {
                        LaunchClient();
                    }
                    break;
            }
        }

        private void LaunchClient()
        {
            IPAddress[] addresses = Dns.GetHostAddresses(Setting.Hostname);
            if (addresses.Length == 0) throw new InvalidOperationException(String.Format("Could not resolve hostname '{0}'.", Setting.Hostname));

            ProcessStartInfo info = new ProcessStartInfo();
            if (Setting.UseClientDetours)
            {
                info.WorkingDirectory = Path.Combine(Directory.GetCurrentDirectory(), "bin");
                info.FileName = Path.Combine(info.WorkingDirectory, "Detours.exe");
                info.Arguments = String.Format
                (
                    "/ADDR:{0} /CLIENT:{1}",
                    addresses[0].ToString(),
                    LauncherUtility.GetShortPathName(Setting.ClientPath)
                );
            }
            else
            {
                info.WorkingDirectory = Path.GetDirectoryName(Setting.ClientPath);
                info.FileName = Setting.ClientPath;
                info.Arguments = String.Format
                (
                    "-SERVER_ADDR {0} -PROTOCOL {1}",
                    addresses[0].ToString(),
                    "TCP"
                );
            }
            try
            {
                Process.Start(info);
            }
            catch (Exception e)
            {
                throw new ApplicationException(String.Format("Could not launch client.\nWorking Directory: {0}\nFileName: {1}\nArguments: {2}\nDetails: {3}", info.WorkingDirectory, info.FileName, info.Arguments, e.Message), e);
            }
        }

        private void LaunchNet7Proxy()
        {
            IPAddress[] addresses = Dns.GetHostAddresses(Setting.Hostname);
            if (addresses.Length == 0) throw new InvalidOperationException(String.Format("Could not resolve hostname '{0}'.", Setting.Hostname));

            ProcessStartInfo info = new ProcessStartInfo();

            info.WorkingDirectory = Path.Combine(Directory.GetCurrentDirectory(), "bin");
            info.FileName = Path.Combine(info.WorkingDirectory, "Net7Proxy.exe");
            info.Arguments = String.Format
            (
                "/ADDRESS:{0} /CLIENT:{1}",
                addresses[0].ToString(),
                LauncherUtility.GetShortPathName(Setting.ClientPath)
            );

            if (Setting.UseClientDetours)
            {
                info.Arguments += " /L";
            }

            if (Setting.UseLocalCert)
            {
                info.Arguments += " /LC";
                info.Arguments += String.Format
                (
                    " /SSL:{0}",
                    Setting.AuthenticationPort.ToString()
                );
            }

            try
            {
                Process.Start(info);
            }
            catch (Exception e)
            {
                Program.LogException(e);
                throw new ApplicationException(String.Format("Could not launch Net7Proxy.\nWorking Directory: {0}\nFileName: {1}\nArguments: {2}\nDetails: {3}", info.WorkingDirectory, info.FileName, info.Arguments, e.Message), e);
            }
        }

        private void LaunchNet7Server()
        {
            ProcessStartInfo info = new ProcessStartInfo();

            info.WorkingDirectory = Path.Combine(Directory.GetCurrentDirectory(), "bin");
            info.FileName = Path.Combine(info.WorkingDirectory, "Net7.exe");
            try
            {
                Process.Start(info);
            }
            catch (Exception e)
            {
                throw new ApplicationException("Could not launch Net7 Server. Details: " + e.Message, e);
            }
        }

        private void PatchNetworkIniFile()
        {
            string file = Path.Combine(Setting.CommonDirectoryName, "Network.ini");
            string fileBackup = Path.Combine(Setting.CommonDirectoryName, "Network.ini.orig");
            bool patchRequired = false;

            if (File.Exists(file) == false && File.Exists(fileBackup))
            {
                // Restore Backup
                File.Copy(fileBackup, file);
            }

            string hostName = Setting.Hostname;

            if (Setting.UseLocalCert)
            {
                hostName = "local.net-7.org";
            }

            if (String.Equals(IniUtility.GetValue(file, "MasterServer", "Name"), hostName, StringComparison.InvariantCultureIgnoreCase) == false)
            {
                patchRequired = true;
            }
            else if (String.Equals(IniUtility.GetValue(file, "RegisterServer", "Name"), hostName, StringComparison.InvariantCultureIgnoreCase) == false)
            {
                patchRequired = true;
            }
            else if (String.Equals(IniUtility.GetValue(file, "ReporterServer", "Name"), hostName, StringComparison.InvariantCultureIgnoreCase) == false)
            {
                patchRequired = true;
            }
            else if (String.Equals(IniUtility.GetValue(file, "GlobalServer_Directory", "Name"), hostName, StringComparison.InvariantCultureIgnoreCase) == false)
            {
                patchRequired = true;
            }
            else if (String.Equals(IniUtility.GetValue(file, "GlobalServer_Client", "Name"), hostName, StringComparison.InvariantCultureIgnoreCase) == false)
            {
                patchRequired = true;
            }
            else if (String.Equals(IniUtility.GetValue(file, "GlobalServer_Register", "Name"), hostName, StringComparison.InvariantCultureIgnoreCase) == false)
            {
                patchRequired = true;
            }
            else if (String.Equals(IniUtility.GetValue(file, "GlobalServer_Parent", "Name"), hostName, StringComparison.InvariantCultureIgnoreCase) == false)
            {
                patchRequired = true;
            }
            else if (String.Equals(IniUtility.GetValue(file, "ChatServer", "Name"), hostName, StringComparison.InvariantCultureIgnoreCase) == false)
            {
                patchRequired = true;
            }
            else if (String.Equals(IniUtility.GetValue(file, "ChatServer_Basic", "Name"), hostName, StringComparison.InvariantCultureIgnoreCase) == false)
            {
                patchRequired = true;
            }
            else if (String.Equals(IniUtility.GetValue(file, "GroupServer", "Name"), hostName, StringComparison.InvariantCultureIgnoreCase) == false)
            {
                patchRequired = true;
            }
            else if (String.Equals(IniUtility.GetValue(file, "GuildServer", "Name"), hostName, StringComparison.InvariantCultureIgnoreCase) == false)
            {
                patchRequired = true;
            }

            if (patchRequired == false) return;

            // Create Backup
            File.Copy(file, fileBackup, true);

            IniUtility.SetValue(file, "MasterServer", "Name", hostName);
            IniUtility.SetValue(file, "RegisterServer", "Name", hostName);
            IniUtility.SetValue(file, "ReporterServer", "Name", hostName);
            IniUtility.SetValue(file, "GlobalServer_Directory", "Name", hostName);
            IniUtility.SetValue(file, "GlobalServer_Client", "Name", hostName);
            IniUtility.SetValue(file, "GlobalServer_Register", "Name", hostName);
            IniUtility.SetValue(file, "GlobalServer_Parent", "Name", hostName);
            IniUtility.SetValue(file, "ChatServer", "Name", hostName);
            IniUtility.SetValue(file, "ChatServer_Basic", "Name", hostName);
            IniUtility.SetValue(file, "GroupServer", "Name", hostName);
            IniUtility.SetValue(file, "GuildServer", "Name", hostName);
        }

        private void PatchAuthIniFile()
        {
            string file = Path.Combine(Setting.IniDirectoryName, "Auth.ini");
            string fileBackup = Path.Combine(Setting.IniDirectoryName, "Auth.ini.orig");
            bool patchRequired = false;

            if (File.Exists(file) == false && File.Exists(fileBackup))
            {
                // Restore Backup
                File.Copy(fileBackup, file);
            }

            string RegHostName = Setting.RegistrationHostname;
            if (Setting.UseLocalCert)
            {
                RegHostName = "local.net-7.org";
            }

            UriBuilder builder = new UriBuilder();
            builder.Scheme = Setting.UseSecureAuthentication ? "https" : "http";
            builder.Host = RegHostName;
            builder.Path = "misc/touchsession.jsp";
            builder.Query = "lkey=%s";

            string url = builder.ToString();

            if (String.Equals(IniUtility.GetValue(file, "General", "AAIUrl"), RegHostName, StringComparison.InvariantCultureIgnoreCase) == false)
            {
                patchRequired = true;
            }
            else if (String.Equals(IniUtility.GetValue(file, "General", "LKeyUrl"), url, StringComparison.InvariantCultureIgnoreCase) == false)
            {
                patchRequired = true;
            }

            if (patchRequired == false) return;

            // Create Backup
            File.Copy(file, fileBackup, true);

            IniUtility.SetValue(file, "General", "AAIUrl", RegHostName);
            IniUtility.SetValue(file, "General", "LKeyUrl", url);
        }

        private void PatchRegDataFile()
        {
            string file = Path.Combine(Setting.IniDirectoryName, "rg_regdata.ini");
            string fileBackup = Path.Combine(Setting.IniDirectoryName, "rg_regdata.ini.orig");
            bool patchRequired = false;

            string RegHostName = Setting.RegistrationHostname;
            if (Setting.UseLocalCert)
            {
                RegHostName = "local.net-7.org";
            }

            if (File.Exists(file) == false && File.Exists(fileBackup))
            {
                // Restore Backup
                File.Copy(fileBackup, file);
            }

            UriBuilder builder = new UriBuilder();
            builder.Scheme = Setting.UseSecureAuthentication ? "https" : "http";
            builder.Host = RegHostName;
            builder.Path = "subsxml";

            string registrationUrl = builder.ToString();

            if (String.Equals(IniUtility.GetValue(file, "Connection", "regserverurl"), registrationUrl, StringComparison.InvariantCultureIgnoreCase) == false)
            {
                patchRequired = true;
            }

            if (patchRequired == false) return;

            // Create Backup
            File.Copy(file, fileBackup, true);

            IniUtility.SetValue(file, "Connection", "regserverurl", registrationUrl);
        }

        private void PatchRegDataFileNames()
        {
            string file1 = Path.Combine(Setting.IniDirectoryName, "rg_regdata_org");
            string file2 = Path.Combine(Setting.IniDirectoryName, "rg_regdata.ini");

            if (File.Exists(file1) && File.Exists(file2) == false)
            {
                try
                {
                    File.Move(file1, file2);
                }
                catch (Exception e)
                {
                    throw new ApplicationException("Could not repair rg_regdata.ini filename.", e);
                }
            }
        }

        private void PatchAuthLoginFile()
        {
            try
            {
                AuthPatcherInfo info = AuthLoginPatcher.ReadInformation(Setting.AuthLoginFileName);
                if (info.Port != Setting.AuthenticationPort || info.UseHttps != Setting.UseSecureAuthentication)
                {
                    info.Port = (ushort)Setting.AuthenticationPort;
                    info.UseHttps = Setting.UseSecureAuthentication;
                    AuthLoginPatcher.WriteInformation(Setting.AuthLoginFileName, info);
                }
            }
            catch (Exception e)
            {
                throw new ApplicationException("Could not patch AuthLogin.dll.", e);
            }
        }

        private void PatchRegistry()
        {
            string keyName = "HKEY_LOCAL_MACHINE\\Software\\Westwood Studios\\Earth and Beyond\\Registration";
            if (((int)Registry.GetValue(keyName, "Registered", 0)) != 1)
            {
                try
                {
                    Registry.SetValue(keyName, "Registered", 1, RegistryValueKind.DWord);
                }
                catch (Exception e)
                {
                    throw new ApplicationException("Could not patch registry-settings.", e);
                }
            }
        }
    }
}
