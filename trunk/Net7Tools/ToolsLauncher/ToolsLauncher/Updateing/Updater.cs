using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.ComponentModel;
using System.IO;
using System.Windows.Forms;
using ToolsLauncher.Updateing;
using ToolsLauncher.Cryptography;

namespace ToolsLauncher.Updateing
{
    public class Updater
    {
        public const int DisplayProgressText1 = -1;
        public const int DisplayProgressText2 = -2;
        public const int DisplayProgress1 = 1;
        public const int DisplayProgress2 = 2;

        private string m_ServerVersionRawText;

        public string RootPath
        {
            get { return m_RootPath; }
            set { m_RootPath = value; }            
        }
        private string m_RootPath;


        public string Name
        {
            get { return m_Name; }
            set { m_Name = value; }
        }
        private string m_Name;

        public string VersionFileName
	    {
		    get { return m_VersionFileName;}
		    set { m_VersionFileName = value;}
	    }
	    private string m_VersionFileName;

        public string FileListFileName
        {
            get { return m_FileListFileName; }
            set { m_FileListFileName = value; }
        }
        private string m_FileListFileName;

        public string ChangelogFileName
        {
            get { return m_ChangelogFileName; }
            set { m_ChangelogFileName = value; }
        }
        private string m_ChangelogFileName;

        public string ChangelogUrl
        {
            get { return WebPath.Combine(BaseUrl, ChangelogFileName); }
        }

        public string CurrentVersionText
        {
            get { return m_CurrentVersionText; }
            set { m_CurrentVersionText = value; }
        }
        private string m_CurrentVersionText;

        public string NewVersionText
        {
            get { return m_NewVersionText; }
            set { m_NewVersionText = value; }
        }
        private string m_NewVersionText;

        public bool RestartOnUpdate
        {
            get { return m_RestartOnUpdate; }
            set { m_RestartOnUpdate = value; }
        }
        private bool m_RestartOnUpdate;

        public UpdateCheckResult CheckResult
        {
            get { return m_CheckResult; }
            set { m_CheckResult = value; }
        }
        private UpdateCheckResult m_CheckResult;

        public Exception Error
        {
            get { return m_Error; }
            set { m_Error = value; }
        }
        private Exception m_Error;

        public VersionCompareMode VersionCompareMode
        {
            get { return m_VersionCompareMode; }
            set { m_VersionCompareMode = value; }
        }
        private VersionCompareMode m_VersionCompareMode;

        public bool ForceUpdate
        {
            get { return m_ForceUpdate; }
            set { m_ForceUpdate = value; }
        }
        private bool m_ForceUpdate;

        public string BaseUrl
	    {
            get { return m_BaseUrl; }
            set { m_BaseUrl = value; }
	    }
        private string m_BaseUrl;

        public string VersionFileUrl
        {
            get { return WebPath.Combine(BaseUrl, VersionFileName); }
        }

        public string FileListUrl
        {
            get { return WebPath.Combine(BaseUrl, FileListFileName); }
        }

        public string CurrentChangelog
        {
            get { return m_CurrentChangelog; }
            set { m_CurrentChangelog = value; }
        }
        private string m_CurrentChangelog;

        public virtual UpdateCheckResult CheckForUpdates()
        {
            if (String.IsNullOrEmpty(VersionFileName)) throw new InvalidOperationException("VersionFileName is not set to a value.");
            if (String.IsNullOrEmpty(BaseUrl)) throw new InvalidOperationException("BaseUrl is not set to a value.");

            if (ForceUpdate)
            {
                ForceUpdate = true;
                CurrentVersionText = "Update required.";
            }

            // Force Update If File Missing
            Version currentVersion = null;
            int currentVersionNumber = 0;
            string currentVersionText = null;

            if (File.Exists(VersionFileName) == false)
            {
                ForceUpdate = true;
                CurrentVersionText = "Version cannot be determined.";
            }
            else
            {
                currentVersionText = File.ReadAllText(VersionFileName);

                // Reading Current Version
                switch (VersionCompareMode)
                {
                    case VersionCompareMode.Version:
                        currentVersion = new Version(currentVersionText);
                        CurrentVersionText = currentVersion.ToString();
                        break;

                    case VersionCompareMode.Number:
                        currentVersionNumber = Int32.Parse(currentVersionText);
                        CurrentVersionText = currentVersionNumber.ToString();
                        break;

                    case VersionCompareMode.SameText:
                        CurrentVersionText = currentVersionText;
                        break;
                }
            }

            // Getting Server Version
            string serverVersionText;
            try
            {
                //using (WebClient client = new WebClient())
                //{
                //    serverVersionText = client.DownloadString(VersionFileUrl);
                //    m_ServerVersionRawText = serverVersionText;
                //}
                serverVersionText = LauncherUtility.DownloadString(VersionFileUrl);
                m_ServerVersionRawText = serverVersionText;
            }
            catch (WebException e)
            {
                Error = e;
                Program.LogException(e);
                if (e.Status == WebExceptionStatus.Timeout)
                {
                    CheckResult = UpdateCheckResult.Timeout;
                }
                else
                {
                    CheckResult = UpdateCheckResult.Error;
                }
                return CheckResult;
            }
            catch (Exception e)
            {
                Error = e;
                Program.LogException(e);
                CheckResult = UpdateCheckResult.Error;
                return CheckResult;
            }

            Version serverVersion = null;
            int serverVersionNumber = 0;

            switch (VersionCompareMode)
            {
                case VersionCompareMode.Version:
                    serverVersion = new Version(serverVersionText);
                    NewVersionText = serverVersion.ToString();
                    break;

                case VersionCompareMode.Number:
                    serverVersionNumber = Int32.Parse(serverVersionText);
                    NewVersionText = serverVersionNumber.ToString();
                    break;

                case VersionCompareMode.SameText:
                    NewVersionText = serverVersionText;
                    break;
            }

            if (ForceUpdate)
            {
                CheckResult = UpdateCheckResult.DownloadRequired;
                return CheckResult;
            }

            // Comparing Versions
            switch (VersionCompareMode)
            {
                case VersionCompareMode.Version:
                    CheckResult = (serverVersion > currentVersion) ? UpdateCheckResult.UpdateAvailable : UpdateCheckResult.NoUpdatesAvailable;
                    break;

                case VersionCompareMode.Number:
                    CheckResult = serverVersionNumber > currentVersionNumber ? UpdateCheckResult.UpdateAvailable : UpdateCheckResult.NoUpdatesAvailable;
                    break;

                case VersionCompareMode.SameText:
                    CheckResult = String.Equals(serverVersionText, currentVersionText, StringComparison.InvariantCultureIgnoreCase) == false ? UpdateCheckResult.UpdateAvailable : UpdateCheckResult.NoUpdatesAvailable;
                    break;

                default:
                    CheckResult = UpdateCheckResult.UpdateAvailable;
                    break;
            }
            return CheckResult;
        }

        public void UpdateWithWorker(BackgroundWorker worker)
        {
            Progress progress1 = new Progress(0, 3);
            Progress progress2 = new Progress();

            int bytesRead;
            byte[] buffer = new byte[4096];
            
            worker.ReportProgress(DisplayProgress1, progress1);

            // Download File List
            using (WebClient client = new WebClient())
            {
                worker.ReportProgress(DisplayProgressText1, "Downloading File List ...");
                worker.ReportProgress(DisplayProgressText2, String.Empty);
                string files;
                try
                {
                    files = client.DownloadString(FileListUrl);
                }
                catch (Exception e)
                {
                    throw new ApplicationException("Could not download file-list.\nDetails: " + e.Message, e);
                }
                progress1.Increment();
                worker.ReportProgress(DisplayProgress1, progress1);

                worker.ReportProgress(DisplayProgressText1, "Validating File List ...");
                worker.ReportProgress(DisplayProgressText2, String.Empty);
                Items.Clear();
                StringReader reader = new StringReader(files);
                string line = reader.ReadLine();
                while (line != null)
                {
                    string[] fragments = line.Split('\t');
                    UpdateItem item = new UpdateItem(m_RootPath);
                    if (fragments.Length >= 1)
                    {
                        item.FileName = fragments[0];
                        if (String.Equals(Path.GetFullPath(item.FileName), Path.GetFullPath(Application.ExecutablePath), StringComparison.InvariantCultureIgnoreCase))
                        {
                            item.TargetFileName = item.FileName + ".tmp";
                            SelfUpdateDetected = true;
                            SelfUpdateItem = item;
                        }
                        if (String.IsNullOrEmpty(item.FileName)) continue;
                        m_Items.Add(item);
                    }
                    if (fragments.Length >= 2)
                    {
                        item.Hash = fragments[1];
                    }
                    line = reader.ReadLine();
                }
                progress1.Increment();
                worker.ReportProgress(DisplayProgress1, progress1);

                if (worker.CancellationPending) return;

                progress2.Reset(0, Items.Count);
                worker.ReportProgress(DisplayProgress2, progress2);
                worker.ReportProgress(DisplayProgressText1, "Validating Files ...");
                Crc32Processor processor = new Crc32Processor();
                foreach (UpdateItem item in Items)
                {
                    if (worker.CancellationPending) return;
                    worker.ReportProgress(DisplayProgressText2, item.FullName);

                    if (File.Exists(item.FullName) == false)
                    {
                        item.CheckStatus = UpdateCheckStatus.NotExisiting;
                        item.IsUpdateRequired = true;
                    }
                    else
                    {
                        using (FileStream fs = File.OpenRead(item.FullName))
                        {
                            while (true)
                            {
                                bytesRead = fs.Read(buffer, 0, buffer.Length);
                                processor.Process(buffer, 0, bytesRead);
                                if (bytesRead != buffer.Length) break;
                            }
                        }
                        if (item.Hash != processor.Current.ToString("X8"))
                        {
                            item.CheckStatus = UpdateCheckStatus.HashMismatch;
                            item.IsUpdateRequired = true;
                        }
                        else
                        {
                            item.CheckStatus = UpdateCheckStatus.Ok;
                        }
                        processor.Reset();
                    }

                    progress2.Increment();
                    worker.ReportProgress(DisplayProgress2, progress2);
                }
                progress1.Increment();
                worker.ReportProgress(DisplayProgress1, progress1);


                worker.ReportProgress(DisplayProgressText1, "Downloading Files ...");
                foreach (UpdateItem item in Items)
                {
                    if (worker.CancellationPending) return;
                    
                    if (item.IsUpdateRequired == false) continue;

                    string fileName = item.FileName;
                    if (fileName.StartsWith("./")) fileName = fileName.Substring(2);
                    
                    using (Stream stream = client.OpenRead(WebPath.Combine(BaseUrl, fileName)))
                    {
                        // Create Directory If Missing
                        string directoryName = Path.GetDirectoryName(item.FullName);
                        if (Directory.Exists(directoryName) == false) Directory.CreateDirectory(directoryName);

                        using (FileStream outStream = File.Open(m_RootPath + item.TargetFileName, File.Exists(m_RootPath + item.TargetFileName) ? FileMode.Truncate : FileMode.Create, FileAccess.Write, FileShare.None))
                        {
                            int contentLength = Int32.Parse(client.ResponseHeaders[HttpResponseHeader.ContentLength]);
                            progress2.Reset(0, contentLength);
                            worker.ReportProgress(DisplayProgress2, progress2);

                            int totalBytesRead = 0;
                            while (true)
                            {
                                worker.ReportProgress(DisplayProgressText2, String.Format("Downloading: {0} - {1:N0}/{2:N0}", fileName, totalBytesRead, contentLength));

                                bytesRead = stream.Read(buffer, 0, buffer.Length);
                                outStream.Write(buffer, 0, bytesRead);
                                totalBytesRead += bytesRead;

                                progress2.CurrentValue = totalBytesRead;
                                worker.ReportProgress(DisplayProgress2, progress2);

                                if (totalBytesRead >= contentLength) break;
                            }
                        }
                    }

                    // Validating File
                    if (String.IsNullOrEmpty(item.Hash) == false)
                    {
                        progress2.Reset();
                        worker.ReportProgress(DisplayProgressText2, String.Format("Validating: {0}", fileName));
                        using (FileStream fs = File.OpenRead(m_RootPath + item.TargetFileName))
                        {
                            while (true)
                            {
                                bytesRead = fs.Read(buffer, 0, buffer.Length);
                                processor.Process(buffer, 0, bytesRead);
                                if (bytesRead == 0) break;
                            }
                        }
                        if (item.Hash != processor.Current.ToString("X8"))
                        {
                            throw new ApplicationException("Downloaded file is invalid. Please restart launcher and repatch files.\nFile: " + m_RootPath + item.TargetFileName);
                        }
                        processor.Reset();
                    }
                }

                // Write Version File
                worker.ReportProgress(DisplayProgressText1, "Writing Version File ...");
                worker.ReportProgress(DisplayProgressText2, String.Empty);
                File.WriteAllText(VersionFileName, m_ServerVersionRawText, Encoding.Default);

                m_SuccessfulUpdated = true;
                m_CurrentVersionText = m_NewVersionText;
            }
        }

        public bool SuccessfulUpdated
        {
            get { return m_SuccessfulUpdated; }
        }
        private bool m_SuccessfulUpdated;

        public UpdateItemCollection Items
        {
            get { return m_Items; }
        }
        private UpdateItemCollection m_Items = new UpdateItemCollection();

        public bool SelfUpdateDetected
        {
            get { return m_SelfUpdateDetected; }
            set { m_SelfUpdateDetected = value; }
        }
        private bool m_SelfUpdateDetected;

        public UpdateItem SelfUpdateItem
        {
            get { return m_SelfUpdateItem; }
            set { m_SelfUpdateItem = value; }
        }
        private UpdateItem m_SelfUpdateItem;

        public void DownloadPatchlog()
        {
            if (String.IsNullOrEmpty(ChangelogFileName) == false)
            {
                try
                {
                    CurrentChangelog = LauncherUtility.DownloadString(ChangelogUrl);
                }
                catch (Exception e)
                {
                    Program.LogException(e);
                }
            }
        }
    }
}
