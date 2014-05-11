using System;
using System.Collections.Generic;
using System.Text;
using System.Reflection;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace ToolsLauncher
{
    public class AssemblyFileInfo
    {
        public static AssemblyFileInfo FromFile(string assemblyFileName)
        {
            Assembly assembly = Assembly.LoadFile(assemblyFileName); 
            return AssemblyFileInfo.FromAssembly(assembly);
        }

        public static AssemblyFileInfo FromAssembly(Assembly assembly)
        {
            AssemblyFileInfo info = new AssemblyFileInfo();
            info.m_AssemblyVersion = assembly.GetName().Version;
            foreach (object attribute in assembly.GetCustomAttributes(true))
            {
                if (attribute is AssemblyFileVersionAttribute)
                {
                    info.m_FileVersion = new Version(((AssemblyFileVersionAttribute)attribute).Version);
                }
                else if (attribute is AssemblyTitleAttribute)
                {
                    info.m_Title = ((AssemblyTitleAttribute)attribute).Title;
                }
                else if (attribute is AssemblyCompanyAttribute)
                {
                    info.m_Company = ((AssemblyCompanyAttribute)attribute).Company;
                }
                else if (attribute is AssemblyDescriptionAttribute)
                {
                    info.m_Description = ((AssemblyDescriptionAttribute)attribute).Description;
                }
                else if (attribute is AssemblyConfigurationAttribute)
                {
                    info.m_Configuration = ((AssemblyConfigurationAttribute)attribute).Configuration;
                }
                else if (attribute is AssemblyCopyrightAttribute)
                {
                    info.m_Copyright = ((AssemblyCopyrightAttribute)attribute).Copyright;
                }
                else if (attribute is AssemblyCultureAttribute)
                {
                    info.m_Culture = ((AssemblyCultureAttribute)attribute).Culture;
                }
                else if (attribute is GuidAttribute)
                {
                    info.m_Guid = new Guid(((GuidAttribute)attribute).Value);
                }
                else if (attribute is ComVisibleAttribute)
                {
                    info.m_IsComVisible = ((ComVisibleAttribute)attribute).Value;
                }
                else if (attribute is AssemblyProductAttribute)
                {
                    info.m_Product = ((AssemblyProductAttribute)attribute).Product;
                }
                else if (attribute is AssemblyTrademarkAttribute)
                {
                    info.m_Trademark = ((AssemblyTrademarkAttribute)attribute).Trademark;
                }
                else if (attribute is AssemblyInformationalVersionAttribute)
                {
                    info.m_InformalVersion = new Version(((AssemblyInformationalVersionAttribute)attribute).InformationalVersion);
                }
            }

            return info;
        }

        public static AssemblyFileInfo Current
        {
            get
            {
                if (m_Current == null)
                {
                    m_Current = FromAssembly(Assembly.GetExecutingAssembly());
                }
                return m_Current;
            }
        }
        private static AssemblyFileInfo m_Current;

        public Version FileVersion
        {
            get { return m_FileVersion; }
        }
        private Version m_FileVersion;

        public Version AssemblyVersion
        {
            get { return m_AssemblyVersion; }
        }
        private Version m_AssemblyVersion;

        public Version InformalVersion
        {
            get { return m_InformalVersion; }
        }
        private Version m_InformalVersion;

        public String Title
        {
            get { return m_Title; }
        }
        private String m_Title;

        public String Company
        {
            get { return m_Company; }
        }
        private String m_Company;

        public String Description
        {
            get { return m_Description; }
        }
        private String m_Description;

        public String Configuration
        {
            get { return m_Configuration; }
            set { m_Configuration = value; }
        }
        private String m_Configuration;

        public String Culture
        {
            get { return m_Culture; }
        }
        private String m_Culture;

        public String Product
        {
            get { return m_Product; }
        }
        private String m_Product;

        public String Trademark
        {
            get { return m_Trademark; }
        }
        private String m_Trademark;

        public string Copyright
        {
            get { return m_Copyright; }
        }
        private string m_Copyright;

        public Guid Guid
        {
            get { return m_Guid; }
            set { m_Guid = value; }
        }
        private Guid m_Guid;

        public bool IsComVisible
        {
            get { return m_IsComVisible; }
        }
        private bool m_IsComVisible;
    }
}
