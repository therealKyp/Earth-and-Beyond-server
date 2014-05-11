using System;
using System.Collections.Generic;
using System.Text;
using System.Configuration;
using LaunchNet7.Updateing;

namespace LaunchNet7.Configuration
{
    public class AutoUpdateTaskConfigurationElement: ConfigurationElement
    {
        [ConfigurationProperty("name", IsRequired = true)]
        public String Name
        {
            get { return (String)base["name"]; }
            set { base["name"] = value; }
        }

        [ConfigurationProperty("restartOnUpdate", DefaultValue = false)]
        public bool RestartOnUpdate
        {
            get { return (bool)base["restartOnUpdate"]; }
            set { base["restartOnUpdate"] = value; }
        }

        [ConfigurationProperty("baseUrl", IsRequired = true)]
        public String BaseUrl
        {
            get { return (String)base["baseUrl"]; }
            set { base["baseUrl"] = value; }
        }

        [ConfigurationProperty("fileListName", IsRequired = true)]
        public String FileListName
        {
            get { return (String)base["fileListName"]; }
            set { base["fileListName"] = value; }
        }

        [ConfigurationProperty("versionFileName", IsRequired = true)]
        public String VersionFileName
        {
            get { return (String)base["versionFileName"]; }
            set { base["versionFileName"] = value; }
        }

        [ConfigurationProperty("changelogFileName")]
        public String ChangelogFileName
        {
            get { return (String)base["changelogFileName"]; }
            set { base["changelogFileName"] = value; }
        }

        [ConfigurationProperty("versionCompareMode", DefaultValue = VersionCompareMode.Version)]
        public VersionCompareMode VersionCompareMode
        {
            get { return (VersionCompareMode)base["versionCompareMode"]; }
            set { base["versionCompareMode"] = value; }
        }	
    }
}
