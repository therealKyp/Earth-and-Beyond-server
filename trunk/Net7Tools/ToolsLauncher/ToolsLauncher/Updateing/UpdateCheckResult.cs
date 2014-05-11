using System;
using System.Collections.Generic;
using System.Text;

namespace ToolsLauncher.Updateing
{
    public enum UpdateCheckResult
    {
        None = 0,
        NoUpdatesAvailable,
        UpdateAvailable,
        DownloadRequired,
        Timeout,
        Error
    }
}
