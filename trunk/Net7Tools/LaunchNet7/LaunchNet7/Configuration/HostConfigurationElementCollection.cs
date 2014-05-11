using System;
using System.Collections.Generic;
using System.Text;
using System.Configuration;

namespace LaunchNet7.Configuration
{
    public class HostConfigurationElementCollection: ConfigurationElementCollection
    {
        protected override ConfigurationElement CreateNewElement()
        {
            return new HostConfigurationElement();
        }

        protected override object GetElementKey(ConfigurationElement element)
        {
            HostConfigurationElement typedElement = element as HostConfigurationElement;
            if (typedElement == null) return null;
            return typedElement.Hostname;
        }

        public override ConfigurationElementCollectionType CollectionType
        {
            get { return ConfigurationElementCollectionType.BasicMap; }
        }

        protected override string ElementName
        {
            get { return "host"; }
        }

        public HostConfigurationElement GetByHostName(string hostName)
        {
            return (HostConfigurationElement)BaseGet(hostName);
        }
    }
}
