using System;
using System.Collections.Generic;
using System.Text;
using System.Configuration;

namespace LaunchNet7.Configuration
{
    public class HostConfigurationElement: ConfigurationElement
    {
        [ConfigurationProperty("hostname")]
        public String Hostname
        {
            get { return (String)base["hostname"]; }
            set { base["hostname"] = value; }
        }

        [ConfigurationProperty("supportSecureAuthentication", DefaultValue = true)]
        public bool SupportsSecureAuthentication
        {
            get { return (bool)base["supportSecureAuthentication"]; }
            set { base["supportSecureAuthentication"] = value; }
        }

        [ConfigurationProperty("secureAuthenticationPort", DefaultValue = 443)]
        [IntegerValidator(MinValue = 1, MaxValue = 65536, ExcludeRange = false)]
        public int SecureAuthenticationPort
        {
            get { return (int)base["secureAuthenticationPort"]; }
            set { base["secureAuthenticationPort"] = value; }
        }

        [ConfigurationProperty("authenticationPort", DefaultValue = 80)]
        [IntegerValidator(MinValue = 1, MaxValue = 65536, ExcludeRange = false)]
        public int AuthenticationPort
        {
            get { return (int)base["authenticationPort"]; }
            set { base["authenticationPort"] = value; }
        }

        [ConfigurationProperty("globalServerPort", DefaultValue=3805)]
        [IntegerValidator(MinValue = 1, MaxValue = 65536, ExcludeRange = false)]
        public int GlobalServerPort
        {
            get { return (int)base["globalServerPort"]; }
            set { base["globalServerPort"] = value; }
        }
    }
}
