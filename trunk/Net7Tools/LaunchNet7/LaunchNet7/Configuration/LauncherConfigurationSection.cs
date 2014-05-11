using System;
using System.Collections.Generic;
using System.Text;
using System.Configuration;

namespace LaunchNet7.Configuration
{
    public class LauncherConfigurationSection: ConfigurationSection
    {
        public static string DefaultSectionName
        {
            get { return "launchNet7"; }
        }

        [ConfigurationProperty("defaultWebsite")]
        public String DefaultWebsite
        {
            get { return (String)base["defaultWebsite"]; }
            set { base["defaultWebsite"] = value; }
        }

        [ConfigurationProperty("servers")]
        public ServerConfigurationElementCollection Servers
        {
            get { return (ServerConfigurationElementCollection)base["servers"]; }
        }

        [ConfigurationProperty("autoUpdate")]
        public AutoUpdateConfigurationElement AutoUpdate
        {
            get { return (AutoUpdateConfigurationElement)base["autoUpdate"]; }
        }
    }
}
