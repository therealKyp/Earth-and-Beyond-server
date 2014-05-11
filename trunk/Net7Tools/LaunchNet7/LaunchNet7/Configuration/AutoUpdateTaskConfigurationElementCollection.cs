using System;
using System.Collections.Generic;
using System.Text;
using System.Configuration;

namespace LaunchNet7.Configuration
{
    public class AutoUpdateTaskConfigurationElementCollection: ConfigurationElementCollection
    {
        protected override ConfigurationElement CreateNewElement()
        {
            return new AutoUpdateTaskConfigurationElement();
        }

        protected override object GetElementKey(ConfigurationElement element)
        {
            return ((AutoUpdateTaskConfigurationElement)element).Name;
        }

        public override ConfigurationElementCollectionType CollectionType
        {
            get { return ConfigurationElementCollectionType.BasicMap; }
        }

        protected override string ElementName
        {
            get { return "autoUpdateTask"; }
        }
    }
}
