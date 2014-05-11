using System;
using System.Collections.Generic;
using System.Text;

namespace LaunchNet7.Patching
{
    public class AuthPatcherInfo
    {
        public bool UseHttps
        {
            get { return m_UseHttps; }
            set { m_UseHttps = value; }
        }
        private bool m_UseHttps;

        public UInt16 Port
        {
            get { return m_Port; }
            set { m_Port = value; }
        }
        private UInt16 m_Port;

        public UInt16 TimeOut
        {
            get { return m_TimeOut; }
            set { m_TimeOut = value; }
        }
        private UInt16 m_TimeOut;
    }
}
