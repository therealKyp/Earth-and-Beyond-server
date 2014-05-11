using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using FileListCreator.Cryptography;

namespace FileListCreator
{
    class Program
    {
        static void Main(string[] args)
        {
            string fileListFileName = "Files.txt";

            string baseDirectory = Environment.CurrentDirectory;
            if (args.Length > 0)
            {
                if (Directory.Exists(args[0]))
                {
                    baseDirectory = args[0];
                }
                if (args.Length >= 2)
                {
                    fileListFileName = args[1];
                }   
            }

            using (FileStream fs = File.Open(fileListFileName, File.Exists(fileListFileName) ? FileMode.Truncate : FileMode.Create, FileAccess.Write, FileShare.None))
            {
                StreamWriter writer = new StreamWriter(fs, Encoding.Default);
                Crc32Processor processor = new Crc32Processor();
                byte[] buffer = new byte[4096];
                foreach (string fileName in Directory.GetFiles(baseDirectory, "*.*", SearchOption.AllDirectories))
                {
                    if (String.Equals(Path.GetFullPath(fileListFileName), Path.GetFullPath(fileName), StringComparison.InvariantCultureIgnoreCase)) continue;

                    using (FileStream inStream = File.OpenRead(fileName))
                    {
                        int bytesRead = -1;
                        while (bytesRead != 0)
                        {
                            bytesRead = inStream.Read(buffer, 0, buffer.Length);
                            processor.Process(buffer, 0, bytesRead);
                        }
                    }

                    writer.Write(fileName.Substring(baseDirectory.Length + 1));
                    writer.Write('\t');
                    writer.Write(processor.Current.ToString("X8"));
                    writer.WriteLine();

                    processor.Reset();
                }
                writer.Flush();
            }
        }
    }
}
