using System;
using System.Collections.Generic;
using System.Text;

namespace LaunchNet7
{
    public class CommandLineReader
    {
        public CommandLineReader()
            : this(Environment.GetCommandLineArgs())
        {
        }
        public CommandLineReader(string[] args)
        {
            if (args == null) throw new ArgumentNullException("args");
            m_Args = args;
        }

        public int Count
        {
            get { return m_Args.Length; }
        }

        private string[] m_Args;

        public int CurrentIndex
        {
            get { return m_CurrentIndex; }
            set
            {
                if (value < 0) throw new ArgumentOutOfRangeException();
                m_CurrentIndex = value;
            }
        }
        private int m_CurrentIndex = -1;

        public bool Read()
        {
            if (m_CurrentIndex == -1)
            {
                m_CurrentIndex = 0;
                return true;
            }
            m_CurrentIndex++;
            return !IsOutOfRange;
        }

        private bool IsOutOfRange
        {
            get { return (m_CurrentIndex < 0) || (m_CurrentIndex >= m_Args.Length); }
        }

        public string CurrentArgument
        {
            get
            {
                return (IsOutOfRange) ? null : m_Args[CurrentIndex];
            }
        }

        public bool TryGetString(out string value)
        {
            if (IsOutOfRange)
            {
                value = null;
                return false;
            }
            value = CurrentArgument;
            return true;
        }

        public bool TryReadNextAsString(out string value)
        {
            if (IsOutOfRange)
            {
                value = null;
                return false;
            }
            else if (Read() == false)
            {
                value = null;
                return false;
            }
            value = CurrentArgument;
            return true;
        }

        public bool TryGetInt32(out Int32 value)
        {
            if (IsOutOfRange)
            {
                value = 0;
                return false;
            }
            return Int32.TryParse(CurrentArgument, out value);
        }

        public bool TryReadNextAsInt32(out Int32 value)
        {
            if (IsOutOfRange || Read() == false)
            {
                value = 0;
                return false;
            }
            return Int32.TryParse(CurrentArgument, out value);
        }

        public int RemainingCount
        {
            get { return (m_Args.Length - m_CurrentIndex); }
        }
    }
}
