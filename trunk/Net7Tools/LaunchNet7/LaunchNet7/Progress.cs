using System;
using System.Collections.Generic;
using System.Text;

namespace LaunchNet7
{
    public class Progress
    {
        public Progress()
            : this(0, 100)
        {
        }
        public Progress(int minimumValue, int maximumValue)
        {
            Reset(minimumValue, maximumValue);
        }

        public int MinimumValue
        {
            get { return m_MinimumValue; }
            set { m_MinimumValue = value; }
        }
        private int m_MinimumValue;

        public int MaximumValue
        {
            get { return m_MaximumValue; }
            set { m_MaximumValue = value; }
        }
        private int m_MaximumValue;

        public int CurrentValue
        {
            get { return m_CurrentValue; }
            set { m_CurrentValue = value; }
        }
        private int m_CurrentValue;

        public void Increment()
        {
            m_CurrentValue++;
        }

        public void Reset()
        {
            m_CurrentValue = m_MinimumValue;
        }
        public void Reset(int maximumValue)
        {
            m_MaximumValue = maximumValue;
            m_CurrentValue = m_MinimumValue;
        }
        public void Reset(int minimumValue, int maximumValue)
        {
            m_MinimumValue = minimumValue;
            m_MaximumValue = maximumValue;
            m_CurrentValue = m_MinimumValue;
        }

        public int ToProgressPercentage()
        {
            if (m_CurrentValue == 0 || m_CurrentValue < m_MinimumValue) return 0;
            else if (m_CurrentValue > m_MaximumValue) return 100;

            float range = (float)(m_MaximumValue - m_MinimumValue);
            if (range == 0f) return 0;
            return (int)(((float)m_CurrentValue / range) * 100);
        }
    }
}
