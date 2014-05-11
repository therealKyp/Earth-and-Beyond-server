using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Diagnostics;
using System.Threading;
using System.Reflection;
using System.Windows.Forms;

namespace ExeUpdater
{
    class Program
    {
        [STAThread]
        static int Main(string[] args)
        {
                string exeFileName = null;
                string waitForProcessName = null;
                int waitForPid = 0;
                int waitTimeOut = 10000;
                string replaceFileName = null;
                bool noArgs = false;
                WaitMode waitMode = WaitMode.WaitByProcessName;

                CommandLineReader reader = new CommandLineReader(args);
                if (reader.Count == 0)
                {
                    DisplayUsage();
                    return 1;
                }

                while (reader.Read())
                {
                    switch (reader.CurrentArgument.ToLowerInvariant())
                    {
                        case "-exefilename":
                            if (reader.TryReadNextAsString(out exeFileName) == false) return 2;
                            break;

                        case "-timeout":
                            if (reader.TryReadNextAsInt32(out waitTimeOut) == false)
                            {
                                waitTimeOut = 10000;
                            }
                            break;

                        case "-waitforpid":
                            if (reader.TryReadNextAsInt32(out waitForPid) == false) return 2;
                            waitMode = WaitMode.WaitByProcessId;
                            break;

                        case "-waitforprocessname":
                            if (reader.TryReadNextAsString(out waitForProcessName) == false) return 2;
                            waitMode = WaitMode.WaitByProcessName;
                            break;

                        case "-replacefilename":
                            if (reader.TryReadNextAsString(out replaceFileName) == false) return 2;
                            break;

                        case "-noargs":
                            noArgs = true;
                            break;

                        case "-waitforexefile":
                            waitMode = WaitMode.WaitForFile;
                            break;
                    }
                }

                if (String.IsNullOrEmpty(exeFileName) == false && File.Exists(exeFileName))
                {
                    switch (waitMode)
                    {
                        case WaitMode.WaitByProcessId:
                            {
                                Process process = null;
                                foreach (Process p in Process.GetProcesses())
                                {
                                    if (p.Id == waitForPid)
                                    {
                                        process = p;
                                        break;
                                    }
                                }
                                if (process != null)
                                {
                                    if (process.WaitForExit(waitTimeOut) == false)
                                    {
                                        return 3;
                                    }
                                }
                            }
                            break;

                        case WaitMode.WaitByProcessName:
                            {
                                if (String.IsNullOrEmpty(waitForProcessName))
                                {
                                    waitForProcessName = Path.GetFileNameWithoutExtension(exeFileName);
                                }
                                int time = Environment.TickCount;
                                while (true)
                                {
                                    Process[] processes = Process.GetProcessesByName(waitForProcessName);
                                    if (processes.Length > 0)
                                    {
                                        Process process = processes[0];
                                        if (process.WaitForExit(waitTimeOut) == false)
                                        {
                                            return 3;
                                        }
                                    }
                                    else
                                    {
                                        break;
                                    }

                                    if (Environment.TickCount - time > waitTimeOut) return 3;
                                }
                            }
                            break;

                        case WaitMode.WaitForFile:
                            {
                                int time = Environment.TickCount;
                                while (true)
                                {
                                    try
                                    {
                                        using (FileStream fs = File.Open(exeFileName, FileMode.Open, FileAccess.Write, FileShare.Delete))
                                        {
                                            // Only Open Check
                                        }
                                        break;
                                    }
                                    catch (IOException)
                                    {
                                        // Ignore
                                    }

                                    if (Environment.TickCount - time > waitTimeOut) return 3;

                                    Thread.Sleep(1000);
                                }
                            }
                            break;
                    }

                    Thread.Sleep(100);

                    if (String.IsNullOrEmpty(replaceFileName) == false && File.Exists(replaceFileName))
                    {
                        try
                        {
                            File.Delete(exeFileName);
                            File.Move(replaceFileName, exeFileName);
                        }
                        catch (Exception e)
                        {
                            Debug.WriteLine(e.ToString());
                        }
                    }

                    ProcessStartInfo info = new ProcessStartInfo();

                    try
                    {
                        if (noArgs)
                        {
                            info.WorkingDirectory = Path.GetDirectoryName(exeFileName);
                            info.FileName = exeFileName;
                            Process.Start(info);
                        }
                        else
                        {
                            info.Arguments = String.Format("-delete \"{0}\" -pid {1}", Assembly.GetExecutingAssembly().Location, Process.GetCurrentProcess().Id);
                            info.WorkingDirectory = Path.GetDirectoryName(exeFileName);
                            info.FileName = exeFileName;
                            Process.Start(info);
                        }
                    }
                    catch
                    {
                        // No Error Handling On This One
                    }
                }
          
            return 0;
        }

        private static void DisplayUsage()
        {
            string text = @"ExeUpdater

Description: A lightweight executable for replaceing and restarting other exe-files. Usefull especially for self-updating scenarios.

Usage: ExeUpdater.exe [-exeFileName <fileName>] [-waitForPid <pid>] [-waitforProcessName [<name>]] [-replaceFileName <fileName>] [-waitforexefile] [-noargs]

- exeFileName <fileName>: Filename of the exe which should be restartet/replaced.
- replaceFileName <fileName>: Filename which should replace the exe.
- waitForPid <pid>: Process-Id which should be monitored to exit before starting replace operation. If none is given the ending is monitored by the exe filename.
- waitForProcessName [<name>]: Waiting for the given process-name to exit. If not name is given it wait for the exefile. Warning: If more than once process with the same name are running, the program waiting for all processes to exit or until timeout.
- timeout <ms>: Timeout in milliseconds the updater should wait for the restartet/replaced exe to exit before terminating itself. Defaul is: 10000 (10 secs).
- waitforfile: Wait for the process by trying to get an exclusive handle on the exe file.
- noargs: When calling the replaced/restarted exe two arguments are normally given -delete <fileName> -pid <pid> of this exe. This information can be used to delete this exe if temporary used.
";
            MessageBox.Show(text, "ExeUpdater - Information", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }
    }
}
