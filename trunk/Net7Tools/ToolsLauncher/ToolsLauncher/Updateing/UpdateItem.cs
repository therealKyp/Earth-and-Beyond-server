using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ToolsLauncher.Updateing;

namespace ToolsLauncher.Updateing
{
    public class UpdateItem
    {
        private string m_RootPath;

        public UpdateItem(string RootPath)
        {
            m_RootPath = RootPath;
        }

        public UpdateItem()
        {
            m_RootPath = "";
        }

        public string FileName
        {
            get { return m_FileName; }
            set
            {
                m_FileName = value;
                m_FullName = Path.GetFullPath(m_RootPath + value);
            }
        }
        private string m_FileName;

        public string TargetFileName
        {
            get
            {
                if (m_TargetFileName == null)
                {
                    return m_FileName;
                }
                return m_TargetFileName;
            }
            set { m_TargetFileName = value; }
        }
        private string m_TargetFileName;
	

        public string FullName
        {
            get { return m_FullName;  }
        }
        private string m_FullName;

        public string Hash
        {
            get { return m_Hash; }
            set { m_Hash = value; }
        }
        private string m_Hash;

        public bool IsUpdateRequired
        {
            get { return m_IsUpdateRequired; }
            set { m_IsUpdateRequired = value; }
        }
        private bool m_IsUpdateRequired;

        public UpdateCheckStatus CheckStatus
        {
            get { return m_CheckStatus; }
            set { m_CheckStatus = value; }
        }
        private UpdateCheckStatus m_CheckStatus;
    }
}
