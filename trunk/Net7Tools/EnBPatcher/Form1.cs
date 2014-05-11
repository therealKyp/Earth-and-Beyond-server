using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.IO;
using System.Threading;


namespace EnBPatcher
{
    public partial class Form1 : Form
    {
        // Global Static Strings
        static string NET7_PATCH_URL = "http://patch.net-7.org/";
        static string Me = "./EnBPatcher.exe";
        static string MeAlt = "./EnBPatcher1.exe";
        static string LauncherExe = "./LaunchNet7.exe";

        string CurrentVer;
        string ServerVer;
        string FileList;
        string FilesToPatch;

        Thread UpdateThread;

        public Form1()
        {
            InitializeComponent();
        }

        private void PullFileList()
        {
            // Grab File List
            System.Net.WebClient Client = new WebClient();
            Stream strm = Client.OpenRead(NET7_PATCH_URL + "Files.txt");
            StreamReader sr = new StreamReader(strm);
            FileList = sr.ReadToEnd();
            strm.Close();
        }

        private void GetClientVersion()
        {
            StreamReader SR;
            try
            {
                SR = File.OpenText("Version.txt");
                CurrentVer = SR.ReadLine();
                SR.Close();
            }
            catch
            {
                CurrentVer = "No Version";
            }
            //ClientVer.Text = "Client Version: " + CurrentVer;
        }


        // Download a file from the Update Server
        private bool DownloadFile(string FileName, string CopyName)
        {
            // Grab File List
            System.Net.WebClient Client = new WebClient();
            // The stream of data retrieved from the web server
            Stream strResponse;
            // The stream of data that we write to the harddrive
            Stream strLocal;
            // The request to the web server for file information
            HttpWebRequest webRequest;
            // The response from the web server containing information about the file
            HttpWebResponse webResponse; 


            try
            {
                // Create a request to the file we are downloading
                webRequest = (HttpWebRequest)WebRequest.Create(NET7_PATCH_URL + FileName);
                // Set default authentication for retrieving the file
                webRequest.Credentials = CredentialCache.DefaultCredentials;
                // Retrieve the response from the server
                webResponse = (HttpWebResponse)webRequest.GetResponse();
                // Ask the server for the file size and store it
                Int64 fileSize = webResponse.ContentLength;

                // Open the URL for download 
                strResponse = Client.OpenRead(NET7_PATCH_URL + FileName);

                // Split into directorys
                string[] Directorys = FileName.Split('/');

                // Create Directorys if needed
                if (Directorys.Length != 1)
                {
                    string CPath = "./";
                    int Loops = 0;
                    // We need to make sure all of the directorys are created
                    foreach (string Dir in Directorys)
                    {
                        Loops++;
                        // Create directory if it does not exsist
                        if (!Directory.Exists(CPath + "/" + Dir))
                        {
                            Directory.CreateDirectory(CPath + "/" + Dir);
                        }
                        // Add to current path
                        CPath = CPath + "/" + Dir;
                        // If we are down to the file name drop out of the loop
                        if (Loops == Directorys.Length - 1)
                            break;
                    }
                }

                // Create a new file stream where we will be saving the data (local drive)
                strLocal = new FileStream(CopyName, FileMode.Create, FileAccess.Write, FileShare.None);

                // It will store the current number of bytes we retrieved from the server
                int bytesSize = 0;
                // A buffer for storing and writing the data retrieved from the server
                byte[] downBuffer = new byte[2048];

                // Loop through the buffer until the buffer is empty
                while ((bytesSize = strResponse.Read(downBuffer, 0, downBuffer.Length)) > 0)
                {
                    // Write the data from the buffer to the local hard drive
                    strLocal.Write(downBuffer, 0, bytesSize);
                    // Update Progress Bar
                    FileProgress.Invoke(new UpdateProgressCallback(UpdateProgress),
                        new object[] { strLocal.Length, fileSize }
                    );
                }
                strResponse.Close();
                strLocal.Close();
                webResponse.Close();
            }
            catch
            {
                MessageBox.Show("Error Patching File \"" + FileName + "\"\nMake sure application is shut down!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            return true;
        }

        private void GetServerVersion()
        {
            // Grab Server Version
            System.Net.WebClient Client = new WebClient();
            Stream strm = Client.OpenRead(NET7_PATCH_URL + "Version.txt");
            StreamReader sr = new StreamReader(strm);
            ServerVer = sr.ReadLine();
            strm.Close();
            // Print out server Versions
            //VerLabel.Text = "Server Version: " + ServerVer;
        }

        private bool NeedUpdate()
        {
            bool Update = false;


            // Grab Server Version
            GetServerVersion();
            // Get Our Version #
            GetClientVersion();

            // Compair versions
            if (ServerVer.CompareTo(CurrentVer) != 0)
            {
                // If Versions don't match pull files
                PullFileList();
                Update = true;
            }

            return Update;
        }

        // Delegates 
        public delegate void ExitCallBack();
        public delegate void UpdateFileNameCallback(string Name);
        public delegate void UpdateProgressCallback(Int64 BytesRead, Int64 TotalBytes);
        public delegate void UpdateTotalProgressCallback(int LoopCount, int Total);

        private void Start_Update()
        {
            bool Update = false;
            int Status = 1;

            Update = NeedUpdate();

            if (!Update)
            {
                //MessageBox.Show("Nothing to be patched", "Patch", MessageBoxButtons.OK, MessageBoxIcon.Information);
                UpdateCompleate(Status);
                return;
            }

            string[] Files = FileList.Split('\n');
            string CRC32;

            FilesToPatch = "";

            // Generate a List of Files that need patched
            foreach(string Lines in Files)
            {
                string[] Data = Lines.Split('\t');

                // Check CRC if file exists
                try
                {
                    uint crc = Crc32.GetFileCRC32(Data[0]);
                    CRC32 = String.Format("{0:X8}", crc);
                }
                catch
                {
                    CRC32 = "";
                    // if its our file name read from seperate file
                    if (Data[0] == Me)
                    {
                        try
                        {
                            StreamReader readcrc = File.OpenText(Me + ".crc");
                            CRC32 = readcrc.ReadLine();
                            readcrc.Close();
                        }
                        catch
                        {
                            CRC32 = "";
                        }
                    }


                }

                // If the CRC's match don't download the file
                if (Data[0] != "" && CRC32.CompareTo(Data[1]) != 0)
                {
                    FilesToPatch += Data[0] + "\t" + Data[1] + "\n";
                }
            }

            // Download/Patch the files
            string[] PatchFiles = FilesToPatch.Split('\n');
            int LoopCount = 0;

            foreach (string File in PatchFiles)
            {
                // We are done
                if (File == "")
                    break;

                string[] Data = File.Split('\t');

                // Update File count
                LoopCount++;

                // Display file downloading
                FileProgress.Invoke(new UpdateFileNameCallback(UpdateFileName),
                    new object[] { Data[0] }
                );


                // Download File
                if (!DownloadFile(Data[0], Data[0] == Me ? MeAlt : Data[0]))
                {
                    // if an error exit out of this
                    return;
                }

                // Let the patcher know to restart
                if (Data[0] == Me)
                    Status = 2;

                // Check CRC32 after download
                uint crc = Crc32.GetFileCRC32(Data[0] == Me? MeAlt : Data[0]);
                CRC32 = String.Format("{0:X8}", crc);

                if (Data[0] != "" && CRC32.CompareTo(Data[1]) != 0)
                {
                    MessageBox.Show("CRC32 Error in File " + Data[0] + "\nPlease run the patcher Again", "CRC32 Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }

                // Save our CRC in a file
                if (Data[0] == Me)
                {
                    FileInfo crcfile = new FileInfo(Me + ".crc");
                    StreamWriter writecrc = crcfile.CreateText();
                    writecrc.WriteLine(Data[1]);
                    writecrc.Close();
                }

                // Update total progress bar
                FileProgress.Invoke(new UpdateTotalProgressCallback(UpdateTotalProgress),
                    new object[] { LoopCount, PatchFiles.Length - 1 }
                );

            }
            // Download the newest version info
            DownloadFile("Version.txt", "Version.txt");
            MessageBox.Show("All Files are Updated", "Patching Done", MessageBoxButtons.OK, MessageBoxIcon.Information);
            UpdateCompleate(Status);
            return;
        }

        private void UpdateFileName(string Name)
        {
            CFileName.Text = "File Downloading: " + Name;
            CFileName.Refresh();
        }

        private void UpdateProgress(Int64 BytesRead, Int64 TotalBytes)
        {
            // Calculate the download progress in percentages
            FileProgress.Value = Convert.ToInt32((BytesRead * 100) / TotalBytes);
            ProgressValue.Text = FileProgress.Value + "%";
            ProgressValue.Refresh();
        }

        private void UpdateTotalProgress(int LoopCount, int Total)
        {
            // Update Total Progress
            TotalProgress.Value = Convert.ToInt32(((float)LoopCount / (float)Total) * 100.0);
            TotalProgressValue.Text = TotalProgress.Value + "%";
            TotalProgressValue.Refresh();
        }

        private void ExitApp()
        {
            // Close update thread/Exit
            UpdateThread.Abort();
            this.Close();
        }

        private void ForceRepatch()
        {
            // Remove version to repatch missing files
            File.Delete("./Version.txt");
            // Repatch
            StartUpdate_Tick(null, null);
        }
      
        private void UpdateCompleate(int PatchStatus)
        {
            if (PatchStatus == 2)
            {
                // Restart as alt name so we can patch ourself
                if (File.Exists(MeAlt))
                {
                    System.Diagnostics.Process.Start(Directory.GetCurrentDirectory() + "\\" + MeAlt);
                }
                else
                {
                    MessageBox.Show("Error: Can't Find file " + MeAlt, "Can't find file", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    ForceRepatch();
                    return;
                }
            }
            if (PatchStatus == 1)
            {
                // Change dir's if we can
                if (Directory.Exists("c:\\net7\\bin"))
                    Directory.SetCurrentDirectory("c:\\net7\\bin");

                // Execute Launcher
                if (File.Exists(LauncherExe))
                {
                    // Launch launcher
                    System.Diagnostics.Process.Start(Directory.GetCurrentDirectory() + "\\" + LauncherExe, "patcher");
                } 
                else
                {
                    MessageBox.Show("Error: Can't Find file " + LauncherExe, "Can't find file", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    // See if we changed directorys
                    if (Directory.Exists("c:\\net7\\bin"))
                        Directory.SetCurrentDirectory("c:\\net7");
                    ForceRepatch();
                    return;
                }
            }
            FileProgress.Invoke(new ExitCallBack(ExitApp), null);
        }

        private void StartUpdate_Tick(object sender, EventArgs e)
        {
            // Disable Timer
            StartUpdate.Enabled = false;

            // If I am running as an alt name download the newest copy of me!
            if (Path.GetFileName(Application.ExecutablePath) == Path.GetFileName(MeAlt))
            {
                // Copy file
                try
                {
                    File.Copy(MeAlt, Me, true);
                }
                catch
                {
                    // Remove version to repatch missing files
                    File.Delete("./Version.txt");
                    // remove the patcher crc
                    File.Delete(Me + ".crc");

                    MessageBox.Show("Error: Can't copy patcher!\nRestart patcher", "Error Copying", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    this.Close();
                    return;
                }

                /*

                // Download new patcher
                //DownloadFile(Me, Me);

                // Execute Normal Process
                if (File.Exists(Me))
                {
                    System.Diagnostics.Process.Start(Directory.GetCurrentDirectory() + "\\" + Me);
                }
                else
                {
                    MessageBox.Show("Error: Can't Find file " + Me + "\nPlease restart patcher", "Can't find file", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    this.Close();
                    return;
                }

                // Exit if we are runnig as a differnt process
                this.Close();
                */
            }
            else
            {
                // Delete our patch if its here
                try
                {
                    if (File.Exists(MeAlt))
                        File.Delete(MeAlt);
                }
                catch
                {
                    // We cant delete this file when we can
                }
            }

            // Start update thread
            UpdateThread = new Thread(Start_Update);
            UpdateThread.Start();
        }

    }
}