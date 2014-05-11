using System;
using System.Collections.Generic;
using System.Text;

namespace LaunchNet7.Updateing
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
