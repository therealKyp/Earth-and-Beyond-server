using System;
using System.Collections.Generic;
using System.Text;
using System.Configuration;

namespace LaunchNet7.Configuration
{
    public class AutoUpdateConfigurationElement: ConfigurationElement
    {
        [ConfigurationProperty("", IsDefaultCollection = true)]
        public AutoUpdateTaskConfigurationElementCollection AutoUpdateTasks
        {
            get { return (AutoUpdateTaskConfigurationElementCollection)base[""]; }
        }

        [ConfigurationProperty("timeout", DefaultValue = "0:00:04")]
        public TimeSpan Timeout
        {
            get { return (TimeSpan)base["timeout"]; }
            set { base["timeout"] = value; }
        }
	
    }
}
