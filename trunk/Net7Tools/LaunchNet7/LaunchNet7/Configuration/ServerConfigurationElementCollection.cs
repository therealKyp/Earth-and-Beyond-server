using System;
using System.Collections.Generic;
using System.Text;
using System.Configuration;

namespace LaunchNet7.Configuration
{
    public class ServerConfigurationElementCollection: ConfigurationElementCollection
    {
        protected override ConfigurationElement CreateNewElement()
        {
 	        return new ServerConfigurationElement();
        }

        protected override object GetElementKey(ConfigurationElement element)
        {
            ServerConfigurationElement typedElement = element as ServerConfigurationElement;
            if (typedElement == null) return null;
            return typedElement.Name;
        }

        public override ConfigurationElementCollectionType CollectionType
        {
            get { return ConfigurationElementCollectionType.BasicMap; }
        }

        protected override string ElementName
        {
            get { return "server"; }
        }

        public new ServerConfigurationElement this[string name]
        {
            get { return (ServerConfigurationElement)BaseGet(name); }
        }
    }
}
