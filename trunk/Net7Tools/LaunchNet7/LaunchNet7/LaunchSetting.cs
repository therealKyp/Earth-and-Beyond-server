using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using LaunchNet7.Configuration;

namespace LaunchNet7
{
    public class LaunchSetting
    {
        public string ClientPath
        {
            get { return m_ClientPath; }
            set
            {
                m_ClientPath = value;
                if (String.IsNullOrEmpty(value) == false)
                {
                    m_BaseFolder = Directory.GetParent(Path.GetDirectoryName(value)).FullName;
                }
            }
        }
        private string m_ClientPath;

        public string BaseFolder
        {
            get { return m_BaseFolder; }
        }
        private string m_BaseFolder;

        public string AuthLoginFileName
        {
            get { return Path.Combine(BaseFolder, @"release\authlogin.dll"); }
        }

        public string IniDirectoryName
        {
            get { return Path.Combine(BaseFolder, @"Data\client\ini"); }
        }

        public string CommonDirectoryName
        {
            get { return Path.Combine(BaseFolder, @"Data\common"); }
        }

        public int AuthenticationPort
        {
            get { return m_AuthenticationPort; }
            set { m_AuthenticationPort = value; }
        }
        private int m_AuthenticationPort = 443;

        public bool UseSecureAuthentication
        {
            get { return m_UseSecureAuthentication; }
            set { m_UseSecureAuthentication = value; }
        }
        private bool m_UseSecureAuthentication = true;

        public string Hostname
        {
            get { return m_Hostname; }
            set { m_Hostname = value; }
        }
        private string m_Hostname;

        public string RegistrationHostname
        {
            get
            {
                if (String.IsNullOrEmpty(m_RegistrationHostname))
                {
                    return m_Hostname;
                }
                else
                {
                    return m_RegistrationHostname;
                }
            }
            set { m_RegistrationHostname = value; }
        }
        private string m_RegistrationHostname;

        public string LaunchName
        {
            get { return m_LaunchName; }
            set { m_LaunchName = value; }
        }
        private string m_LaunchName;

        public bool UseClientDetours
        {
            get { return m_UseClientDetours; }
            set { m_UseClientDetours = value; }
        }
        public bool UseLocalCert
        {
            get { return m_UseLocalCert; }
            set { m_UseLocalCert = value; }
        }
        private bool m_UseClientDetours;
        private bool m_UseLocalCert;
    }
}
