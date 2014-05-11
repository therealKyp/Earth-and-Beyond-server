using System;
using System.Collections.Generic;
using System.Text;

namespace ExeUpdater
{
    public enum WaitMode
    {
        WaitByProcessId = 0,
        WaitByProcessName,
        WaitForFile
    }
}
