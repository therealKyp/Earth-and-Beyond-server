using System;
using System.Collections.Generic;
using System.Text;
using System.Configuration;

namespace LaunchNet7.Configuration
{
    public class ServerConfigurationElement: ConfigurationElement
    {
        [ConfigurationProperty("name", IsRequired = true)]
        public String Name
        {
            get { return (String)base["name"]; }
            set { base["name"] = value; }
        }

        [ConfigurationProperty("displayName")]
        public String DisplayName
        {
            get { return (String)base["displayName"]; }
            set { base["displayName"] = value; }
        }

        [ConfigurationProperty("launchName")]
        public String LaunchName
        {
            get { return (String)base["launchName"]; }
            set { base["launchName"] = value; }
        }
	

        public string GetDisplayName()
        {
            if (String.IsNullOrEmpty(DisplayName))
            {
                return Name;
            }
            else
            {
                return DisplayName;
            }
        }

        public string GetLaunchName()
        {
            if (String.IsNullOrEmpty(LaunchName))
            {
                return Name;
            }
            else
            {
                return LaunchName;
            }
        }

        [ConfigurationProperty("", IsDefaultCollection = true)]
        public HostConfigurationElementCollection Hosts
        {
            get { return (HostConfigurationElementCollection)base[""]; }
        }

        [ConfigurationProperty("allowProtocolSelection")]
        public bool AllowProtocolSelection
        {
            get { return (bool)base["allowProtocolSelection"]; }
            set { base["allowProtocolSelection"] = value; }
        }

        [ConfigurationProperty("defaultProtocol", DefaultValue = NetworkProtocol.Auto)]
        public NetworkProtocol DefaultProtocol
        {
            get { return (NetworkProtocol)base["defaultProtocol"]; }
            set { base["defaultProtocol"] = value; }
        }

        [ConfigurationProperty("isSinglePlayer")]
        public bool IsSinglePlayer
        {
            get { return (bool)base["isSinglePlayer"]; }
            set { base["isSinglePlayer"] = value; }
        }

        [ConfigurationProperty("enableAdvancedSettings", DefaultValue = false)]
        public bool EnableAdvancedSettings
        {
            get { return (bool)base["enableAdvancedSettings"]; }
            set { base["enableAdvancedSettings"] = value; }
        }

        public override string ToString()
        {
            return GetDisplayName();
        }
    }
}
